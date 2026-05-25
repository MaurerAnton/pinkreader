package org.pinkreader.app

import android.app.AlertDialog
import android.content.Intent
import android.graphics.*
import android.graphics.drawable.ColorDrawable
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.Log
import android.view.*
import android.view.inputmethod.EditorInfo
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import org.json.JSONArray
import org.json.JSONObject
import kotlin.concurrent.thread
import kotlin.math.abs

class PinkReader : AppCompatActivity() {
    companion object {
        const val TAG = "PinkReader"
        val nativeLoaded: Boolean = run {
            try { System.loadLibrary("pinkreader_native"); true }
            catch(e: UnsatisfiedLinkError) { Log.e(TAG,"Failed to load native lib",e); false }
        }
    }

    // ── Native declarations ──────────────────────────────────────────
    private external fun nativeInit(cd: String)
    private external fun nativeGetVersion(): String
    private external fun nativeGetToken(): String
    private external fun nativeGetDarkTheme(): Boolean
    private external fun nativeSetDarkTheme(d: Boolean)
    private external fun nativeIsRead(id: String): Boolean
    private external fun nativeMarkRead(id: String)
    private external fun nativeRunTests(): Boolean
    private external fun nativeBuildPostsUrl(sub: String, sort: String, after: String, limit: Int): String
    private external fun nativeBuildCommentsUrl(sub: String, pid: String, sort: String, limit: Int): String
    private external fun nativeBuildInboxUrl(filter: String, limit: Int): String
    private external fun nativeBuildSubredditSearchUrl(query: String, limit: Int): String
    private external fun nativeBuildActionUrl(action: String, id: String): String
    private external fun nativeBuildActionBody(id: String, extra: String): String
    private external fun nativeParsePostListing(json: String): String
    private external fun nativeParsePost(json: String): String
    private external fun nativeParseComments(json: String): String
    private external fun nativeParseInbox(json: String): String
    private external fun nativeParseSubredditSearch(json: String): String
    private external fun nativeGetCached(key: String): String
    private external fun nativePutCache(key: String, value: String)
    private external fun nativeClearCache()

    // ── Safe JNI wrappers ────────────────────────────────────────────
    private fun safeJniCall(block: () -> String, fallback: String = ""): String =
        if (nativeLoaded) try { block() } catch(e: Exception) { Log.w(TAG,"JNI call failed",e); fallback } else fallback
    private fun safeJniVoid(block: () -> Unit) { if (nativeLoaded) try { block() } catch(e: Exception) { Log.w(TAG,"JNI void failed",e) } }
    private fun safeJniBool(block: () -> Boolean, fallback: Boolean = false): Boolean =
        if (nativeLoaded) try { block() } catch(e: Exception) { fallback } else fallback

    // ── State (thread-safe) ──────────────────────────────────────────
    private var currentSub = "popular"
    private var currentSort = "hot"
    @Volatile private var after = ""
    @Volatile private var lastAfter = ""
    @Volatile private var isLoading = false
    private var isInComments = false
    private var isInMenu = true
    private var currentPostId = ""
    private var currentPostSub = ""
    private val postIds = HashSet<String>()
    private var accessToken = ""
    private val handler = Handler(Looper.getMainLooper())
    @Volatile private var destroyed = false

    // UI (lateinit — initialized in onCreate)
    private lateinit var root: FrameLayout
    private lateinit var toolbar: LinearLayout
    private lateinit var titleText: TextView
    private lateinit var sortSpinner: Spinner
    private lateinit var refresh: SwipeRefreshLayout
    private lateinit var postList: RecyclerView
    private lateinit var commentList: RecyclerView
    private lateinit var menuList: RecyclerView
    private lateinit var loadingBar: ProgressBar
    private lateinit var emptyText: TextView
    private lateinit var errorText: TextView
    private lateinit var subInput: EditText
    private lateinit var bottomBar: LinearLayout
    private lateinit var postAdapter: PostAdapter
    private lateinit var commentAdapter: CommentAdapter
    private lateinit var menuAdapter: MenuAdapter

    // ====================================================================
    // Fix #1: Robust JNI init catching both Exception AND Error
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        try {
            if (nativeLoaded) {
                safeJniVoid { nativeInit(cacheDir.absolutePath) }
                Theme.isDark = safeJniBool({ nativeGetDarkTheme() }, true)
                accessToken = safeJniCall({ nativeGetToken() })
            } else {
                Theme.isDark = true
                accessToken = ""
            }
        } catch(e: Exception) { Log.e(TAG,"JNI init failed",e) }
        catch(e: Error) { Log.e(TAG,"JNI init fatal error",e) }  // catches UnsatisfiedLinkError and friends

        // Fix #2: Guard against accessing window before fully attached
        try {
            window.statusBarColor = if(Theme.isDark) 0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt()
            window.navigationBarColor = if(Theme.isDark) 0xFF1E1E1E.toInt() else 0xFFF5F5F5.toInt()
        } catch(e: Exception) { Log.w(TAG,"Status bar setup failed",e) }

        root = FrameLayout(this).apply { setBackgroundColor(Theme.bg()) }
        try { buildToolbar(); buildContent() }
        catch(e: Exception) { Log.e(TAG,"UI build failed",e); finish(); return }
        setContentView(root)
        if(savedInstanceState == null) showMainMenu()
    }

    override fun onDestroy() { super.onDestroy(); destroyed = true; handler.removeCallbacksAndMessages(null) }

    // ════════════════════════════════════════════════════════════════════
    // TOOLBAR
    // ════════════════════════════════════════════════════════════════════
    private fun buildToolbar() {
        toolbar = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setBackgroundColor(if(Theme.isDark) 0xFF1A1A1A.toInt() else 0xFFF5F5F5.toInt())
            setPadding(dp(12), dp(8), dp(12), dp(4)); visibility = View.GONE
        }
        val top = LinearLayout(this).apply { orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER_VERTICAL }
        top.addView(TextView(this).apply {
            text="←"; setTextColor(0xFFFF7597.toInt()); textSize=22f; setPadding(0,0,dp(12),0)
            setOnClickListener { try { if(!destroyed) { if(isInComments) hideComments() else if(!isInMenu) showMainMenu() } } catch(e:Exception){} } })
        titleText = TextView(this).apply {
            setTextColor(if(Theme.isDark) 0xFFE0E0E0.toInt() else 0xFF212121.toInt()); textSize=18f
            setTypeface(typeface, Typeface.BOLD); layoutParams = LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
        }
        sortSpinner = Spinner(this).apply {
            adapter = ArrayAdapter(this@PinkReader, android.R.layout.simple_spinner_dropdown_item, arrayOf("Hot","New","Top","Rising","Controversial"))
            setPopupBackgroundDrawable(ColorDrawable(if(Theme.isDark)0xFF2A2A2A.toInt() else 0xFFFFFFFF.toInt()))
            onItemSelectedListener = object:AdapterView.OnItemSelectedListener{
                override fun onItemSelected(p0:AdapterView<*>?,p1:View?,p2:Int,p3:Long){
                    val sorts = arrayOf("hot","new","top","rising","controversial")
                    if(p2 in sorts.indices) {
                        currentSort = sorts[p2]
                        if(!isInComments&&!isInMenu){ synchronized(this@PinkReader){ after=""; lastAfter=""; postIds.clear() }; loadPosts() }}}
                override fun onNothingSelected(p0:AdapterView<*>?){}}}
        top.addView(titleText); top.addView(sortSpinner)
        top.addView(TextView(this).apply {
            text="☰"; setTextColor(0xFFFF7597.toInt()); textSize=22f; setPadding(dp(12),0,0,0)
            setOnClickListener{ if(!destroyed) showMainMenu() } })
        toolbar.addView(top)

        val ir = LinearLayout(this).apply { orientation=LinearLayout.HORIZONTAL; gravity=Gravity.CENTER_VERTICAL; setPadding(0,dp(4),0,0) }
        subInput = EditText(this).apply {
            hint="Go to subreddit..."; setHintTextColor(0xFF888888.toInt())
            setTextColor(if(Theme.isDark)0xFFE0E0E0.toInt() else 0xFF212121.toInt()); textSize=14f
            setBackgroundColor(if(Theme.isDark)0xFF2A2A2A.toInt() else 0xFFFFFFFF.toInt())
            setPadding(dp(8),dp(4),dp(8),dp(4)); setSingleLine(); imeOptions=EditorInfo.IME_ACTION_GO
            layoutParams = LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
            setOnEditorActionListener{_,a,_->if(a==EditorInfo.IME_ACTION_GO){navigate(text.toString().trim());true}else false}
        }
        ir.addView(subInput)
        ir.addView(TextView(this).apply {
            text="GO"; setTextColor(0xFFFF7597.toInt()); setTypeface(typeface,Typeface.BOLD)
            setPadding(dp(12),dp(8),0,dp(8)); setOnClickListener{navigate(subInput.text.toString().trim())} })
        toolbar.addView(ir)
        root.addView(toolbar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT).apply{gravity=Gravity.TOP})
    }

    // ════════════════════════════════════════════════════════════════════
    // CONTENT
    // ════════════════════════════════════════════════════════════════════
    private fun buildContent() {
        loadingBar = ProgressBar(this).apply { isIndeterminate=true; visibility=View.GONE
            (layoutParams as? FrameLayout.LayoutParams)?.gravity = Gravity.TOP }

        refresh = SwipeRefreshLayout(this).apply {
            setColorSchemeColors(0xFFFF7597.toInt())
            setProgressBackgroundColorSchemeColor(if(Theme.isDark)0xFF2A2A2A.toInt() else 0xFFFFFFFF.toInt())
            setOnRefreshListener {
                synchronized(this@PinkReader){ after=""; lastAfter=""; postIds.clear() }
                loadPosts() }
        }

        // Fix #3: Safe callbacks that check destroyed flag
        postAdapter = PostAdapter(
            { p -> safeJniVoid{ nativeMarkRead(p.id) }; if(!destroyed) openComments(p) },
            { p -> if(!destroyed) thread {
                try { val url=safeJniCall{ nativeBuildActionUrl("vote","t3_"+p.id) }
                      val body=safeJniCall{ nativeBuildActionBody("t3_"+p.id,"dir=1&rank=2") }
                      NetworkManager.post(url,body,accessToken); if(!destroyed) handler.post{toast("Upvoted")} }
                catch(e:Exception){ Log.w(TAG,"Upvote failed",e) } } },
            { p -> if(!destroyed) thread {
                try { val url=safeJniCall{ nativeBuildActionUrl("vote","t3_"+p.id) }
                      val body=safeJniCall{ nativeBuildActionBody("t3_"+p.id,"dir=-1&rank=2") }
                      NetworkManager.post(url,body,accessToken); if(!destroyed) handler.post{toast("Downvoted")} }
                catch(e:Exception){ Log.w(TAG,"Downvote failed",e) } } }
        )
        postList = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader); adapter = postAdapter
            setBackgroundColor(if(Theme.isDark)0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt())
            addItemDecoration(object:RecyclerView.ItemDecoration(){override fun getItemOffsets(o:Rect,v:View,p:RecyclerView,s:RecyclerView.State){o.bottom=dp(1)}})
            addOnScrollListener(object:RecyclerView.OnScrollListener(){override fun onScrolled(rv:RecyclerView,dx:Int,dy:Int){
                try { val lm=rv.layoutManager as? LinearLayoutManager ?: return
                if(!isLoading&&after.isNotEmpty()&&after!=lastAfter&&lm.findLastVisibleItemPosition()>=postAdapter.itemCount-3)loadMore() }
                catch(e:Exception){ Log.w(TAG,"scroll error",e) } }})
        }
        refresh.addView(postList)

        commentAdapter = CommentAdapter { c -> c.collapsed=!c.collapsed; commentAdapter.notifyDataSetChanged() }
        commentList = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader); adapter = commentAdapter
            setBackgroundColor(if(Theme.isDark)0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt()); visibility=View.GONE
        }

        menuAdapter = MenuAdapter { i -> if(!destroyed) onMenuSelected(i) }
        menuList = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader); adapter = menuAdapter
            setBackgroundColor(if(Theme.isDark)0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt())
        }

        emptyText = TextView(this).apply {
            setTextColor(0xFF888888.toInt()); textSize=14f; gravity=Gravity.CENTER
            visibility=View.GONE; setPadding(dp(32),dp(64),dp(32),dp(32))
        }
        errorText = TextView(this).apply {
            setTextColor(0xFFFF5252.toInt()); textSize=14f; gravity=Gravity.CENTER
            visibility=View.GONE; setPadding(dp(32),dp(32),dp(32),dp(32))
        }

        root.addView(menuList, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(refresh, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(commentList, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(loadingBar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,dp(3)).apply{gravity=Gravity.TOP})
        root.addView(emptyText, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(errorText, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))

        // Bottom bar
        bottomBar = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER
            setBackgroundColor(if(Theme.isDark)0xFF1A1A1A.toInt() else 0xFFF5F5F5.toInt())
            setPadding(0,dp(6),0,dp(10))
            fun addBtn(icon:String,label:String,a:()->Unit) {
                val b = LinearLayout(this@PinkReader).apply {
                    orientation=LinearLayout.VERTICAL; gravity=Gravity.CENTER
                    layoutParams = LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
                    setOnClickListener { if(!destroyed) try { a() } catch(e:Exception){} }
                }
                b.addView(TextView(this@PinkReader).apply{text=icon;textSize=18f;gravity=Gravity.CENTER;setTextColor(0xFF999999.toInt())})
                b.addView(TextView(this@PinkReader).apply{text=label;textSize=10f;gravity=Gravity.CENTER;setTextColor(0xFF999999.toInt())})
                addView(b)
            }
            addBtn("\uD83D\uDD25","Popular"){navigate("popular")}
            addBtn("\uD83C\uDF10","All"){navigate("all")}
            addBtn("\uD83C\uDFE0","Front"){navigate("")}
            addBtn("\uD83D\uDCEC","Inbox"){showInbox()}
            addBtn("\uD83E\uDDEA","Test"){runNetworkTest()}
            addBtn("\uD83C\uDFA8","Theme"){
                Theme.isDark=!Theme.isDark
                safeJniVoid{ nativeSetDarkTheme(Theme.isDark) }
                if(!destroyed) recreate() }
        }
        root.addView(bottomBar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT).apply{gravity=Gravity.BOTTOM})
    }

    // ── Navigation ──────────────────────────────────────────────────
    private fun showMainMenu(){
        isInMenu=true; isInComments=false; toolbar.visibility=View.GONE
        menuList.visibility=View.VISIBLE; postList.visibility=View.GONE; commentList.visibility=View.GONE
        errorText.visibility=View.GONE; emptyText.visibility=View.GONE }

    private fun navigate(sub:String){
        currentSub=sub.ifEmpty{"frontpage"}; currentSort="hot"; isInMenu=false; isInComments=false
        synchronized(this){ after=""; lastAfter=""; postIds.clear() }
        sortSpinner.setSelection(0)
        titleText.text=if(sub.isEmpty())"front page" else "r/$sub"; subInput.setText("")
        toolbar.visibility=View.VISIBLE; menuList.visibility=View.GONE; commentList.visibility=View.GONE
        postList.visibility=View.VISIBLE; errorText.visibility=View.GONE; emptyText.visibility=View.GONE
        loadPosts() }

    private fun openComments(p:PostData){
        currentPostId=p.id; currentPostSub=p.subreddit; isInComments=true; isInMenu=false
        titleText.text=p.title.take(40); toolbar.visibility=View.VISIBLE
        postList.visibility=View.GONE; commentList.visibility=View.VISIBLE; menuList.visibility=View.GONE
        errorText.visibility=View.GONE; emptyText.visibility=View.GONE
        loadComments() }

    private fun hideComments(){
        isInComments=false; titleText.text="r/$currentSub"
        commentList.visibility=View.GONE; postList.visibility=View.VISIBLE }

    private fun showInbox(){
        if(destroyed) return
        toolbar.visibility=View.VISIBLE; titleText.text="Inbox"
        postList.visibility=View.GONE; commentList.visibility=View.GONE; menuList.visibility=View.GONE
        thread{try{
            val url=safeJniCall{ nativeBuildInboxUrl("inbox",25) }
            if(url.isEmpty()){ safePost{ showError("Native library not loaded") }; return@thread }
            val json=NetworkManager.get(url,accessToken)
            if(!json.isOk){ safePost{ showError("Inbox requires OAuth login") }; return@thread }
            val parsed=safeJniCall{ nativeParseInbox(json.body) }
            val arr=JSONArray(parsed)
            val sb=StringBuilder("=== Inbox ===\n\n")
            for(i in 0 until arr.length()){
                val m=arr.getJSONObject(i)
                sb.append("From: u/${m.optString("author","?")}\nSubject: ${m.optString("subject","")}\n${m.optString("body","").take(300)}\n\n---\n\n") }
            safePost{ errorText.text=sb.toString(); errorText.visibility=View.VISIBLE }
        }catch(ex:Exception){ safePost{ showError("Inbox: ${ex.message}") } }}}

    private fun onMenuSelected(item:MenuItem){
        if(destroyed) return
        when(item.type){
            MenuItemType.SHORTCUT->when(item.id){
                "frontpage"->navigate(""); "popular"->navigate("popular"); "all"->navigate("all"); "search"->showSearchDialog() }
            MenuItemType.USER->when(item.id){ "inbox"->showInbox() }
            else->{} }}

    private fun showSearchDialog(){
        if(destroyed) return
        val input=EditText(this).apply{hint="Search subreddits...";setSingleLine()}
        AlertDialog.Builder(this).setTitle("Find Subreddit").setView(input)
            .setPositiveButton("Search"){_,_->
                thread{try{
                    val url=safeJniCall{ nativeBuildSubredditSearchUrl(input.text.toString(),25) }
                    if(url.isEmpty()){ safePost{ toast("Native library not loaded") }; return@thread }
                    val resp=NetworkManager.get(url,accessToken)
                    if(!resp.isOk){ safePost{ toast("Search failed") }; return@thread }
                    val parsed=safeJniCall{ nativeParseSubredditSearch(resp.body) }
                    val arr=JSONArray(parsed); val sb=StringBuilder("Results:\n\n")
                    for(i in 0 until arr.length()){
                        val s=arr.getJSONObject(i); sb.append("r/${s.optString("name")} - ${s.optString("title","")}\n") }
                    safePost{ errorText.text=sb.toString(); errorText.visibility=View.VISIBLE; menuList.visibility=View.GONE }
                }catch(ex:Exception){ safePost{ toast(ex.message?:"Search failed") } } }}
            .setNegativeButton("Cancel",null).show() }

    // ── Thread-safe post to main thread ─────────────────────────────
    private fun safePost(block: () -> Unit) {
        if(!destroyed) handler.post { if(!destroyed) try { block() } catch(e:Exception) { Log.w(TAG,"safePost",e) } }
    }

    // ── Data loading ──────────────────────────────────────────────────
    private fun loadPosts(){
        synchronized(this) {
            if(isLoading) return
            isLoading = true
        }
        loadingBar.visibility=View.VISIBLE; errorText.visibility=View.GONE; emptyText.visibility=View.GONE
        thread{try{
            var json=safeJniCall{ nativeGetCached(currentSub+"_"+currentSort) }
            if(json.isEmpty()){
                val url=safeJniCall{ nativeBuildPostsUrl(currentSub,currentSort,after,25) }
                if(url.isEmpty()){ safePost{ showError("Native library not loaded"); loadingBar.visibility=View.GONE; refresh.isRefreshing=false; isLoading=false }; return@thread }
                val resp=NetworkManager.get(url,accessToken)
                if(resp.isOk) json=resp.body
                if(json.isNotEmpty()) safeJniVoid{ nativePutCache(currentSub+"_"+currentSort,json) }
            }
            if(json.isEmpty()){
                safePost{ showError("No response from Reddit.\nReddit may block anonymous requests.\nTry again or use OAuth.\n\nPull to refresh"); loadingBar.visibility=View.GONE; refresh.isRefreshing=false; isLoading=false }
                return@thread }
            val parsed=safeJniCall{ nativeParsePostListing(json) }
            if(parsed.isEmpty()){ safePost{ showError("Parse error"); loadingBar.visibility=View.GONE; refresh.isRefreshing=false; isLoading=false }; return@thread }
            val arr=JSONArray(parsed); val posts=mutableListOf<PostData>()
            for(i in 0 until arr.length()){
                val o=arr.getJSONObject(i)
                val id=o.optString("id")
                synchronized(this){ if(postIds.contains(id)) continue; postIds.add(id) }
                posts.add(parsePost(o)) }
            if(posts.isNotEmpty()) synchronized(this){ after="t3_"+posts.last().id }
            safePost{
                postAdapter.setData(posts); loadingBar.visibility=View.GONE; refresh.isRefreshing=false; isLoading=false
                if(posts.isEmpty() && postAdapter.itemCount==0){ emptyText.text="No posts loaded\nPull to refresh"; emptyText.visibility=View.VISIBLE }
                else if(posts.isNotEmpty()) toast("${posts.size} posts loaded") }}
        catch(ex:Exception){ safePost{ showError("Error: ${ex.message}"); loadingBar.visibility=View.GONE; refresh.isRefreshing=false; isLoading=false } }}}

    private fun loadMore(){
        synchronized(this) {
            if(isLoading||after.isEmpty()||after==lastAfter) return
            lastAfter=after; isLoading=true }
        thread{try{
            val url=safeJniCall{ nativeBuildPostsUrl(currentSub,currentSort,after,25) }
            if(url.isEmpty()){ safePost{ isLoading=false }; return@thread }
            val resp=NetworkManager.get(url,accessToken)
            if(!resp.isOk){ safePost{ isLoading=false }; return@thread }
            val parsed=safeJniCall{ nativeParsePostListing(resp.body) }
            if(parsed.isEmpty()){ safePost{ isLoading=false }; return@thread }
            val arr=JSONArray(parsed); val posts=mutableListOf<PostData>()
            for(i in 0 until arr.length()){
                val o=arr.getJSONObject(i)
                val id=o.optString("id")
                synchronized(this){ if(postIds.contains(id)) continue; postIds.add(id) }
                posts.add(parsePost(o)) }
            if(posts.isNotEmpty()) synchronized(this){ after="t3_"+posts.last().id }
            safePost{ postAdapter.addData(posts); isLoading=false } }
        catch(ex:Exception){ safePost{ isLoading=false } }}}

    private fun loadComments(){
        synchronized(this){ isLoading=true }
        loadingBar.visibility=View.VISIBLE
        thread{try{
            val url=safeJniCall{ nativeBuildCommentsUrl(currentPostSub,currentPostId,"confidence",300) }
            if(url.isEmpty()){ safePost{ showError("Failed to build URL"); loadingBar.visibility=View.GONE; isLoading=false }; return@thread }
            val resp=NetworkManager.get(url,accessToken)
            if(!resp.isOk){ safePost{ showError("Failed to load comments"); loadingBar.visibility=View.GONE; isLoading=false }; return@thread }
            // Fix #4: Guard against empty nativeParsePost response
            val postJson=safeJniCall{ nativeParsePost(resp.body) }
            val ph = try { if(postJson.isNotEmpty()) parsePost(JSONObject(postJson)) else null }
                    catch(e: Exception) { Log.w(TAG,"Post parse failed",e); null }
            val commentsJson=safeJniCall{ nativeParseComments(resp.body) }
            val arr = try { if(commentsJson.isNotEmpty()) JSONArray(commentsJson) else JSONArray() }
                      catch(e: Exception) { Log.w(TAG,"Comments parse failed",e); JSONArray() }
            val comments=flattenComments(arr,0)
            safePost{
                commentAdapter.setData(ph,comments); loadingBar.visibility=View.GONE; isLoading=false
                if(comments.isEmpty()){ emptyText.text="No comments yet"; emptyText.visibility=View.VISIBLE } } }
        catch(ex:Exception){ safePost{ showError("Comments: ${ex.message}"); loadingBar.visibility=View.GONE; isLoading=false } }}}

    // ── Network test ─────────────────────────────────────────────────
    private fun runNetworkTest(){
        if(destroyed) return
        errorText.visibility=View.VISIBLE; errorText.text="Running tests..."
        thread{
            val sb=StringBuilder("=== PinkReader Tests ===\n\n")
            try{ sb.append("Native: ${if(safeJniBool({ nativeRunTests() }))"✓" else "✗"}\n") }
            catch(e:Exception){ sb.append("Native: ✗ ${e.message}\n") }
            try{
                val url=safeJniCall{ nativeBuildPostsUrl("popular","hot","",3) }
                sb.append("URL: $url\n")
                val resp=NetworkManager.get(url,accessToken)
                if(resp.isOk&&resp.body.startsWith("{")){
                    val parsed=safeJniCall{ nativeParsePostListing(resp.body) }
                    val arr=try{ JSONArray(parsed) }catch(e:Exception){ JSONArray() }
                    sb.append("Posts: ✓ ${arr.length()} loaded\n")
                    for(i in 0 until minOf(arr.length(),3)){
                        val p=arr.getJSONObject(i); sb.append("  r/${p.optString("subreddit")}: ${p.optString("title","").take(50)}\n") }}
                else sb.append("HTTP: ✗ code=${resp.code} (${resp.body.take(100)})\n") }
            catch(e:Exception){ sb.append("Network: ✗ ${e.message}\n") }
            try{ safeJniVoid{ nativePutCache("test","ok") }; sb.append("Cache: ${if(safeJniCall{ nativeGetCached("test") }=="ok")"✓" else "✗"}\n") }
            catch(e:Exception){ sb.append("Cache: ✗\n") }
            sb.append("Engine: ${safeJniCall{ nativeGetVersion() }}\n")
            safePost{ errorText.text=sb.toString() } } }

    // ── Helpers ─────────────────────────────────────────────────────
    private fun parsePost(o:JSONObject) = PostData(
        id=o.optString("id"),name=o.optString("name"),title=o.optString("title"),
        author=o.optString("author"),subreddit=o.optString("subreddit"),
        score=o.optInt("score"),numComments=o.optInt("num_comments"),
        created=o.optLong("created"),url=o.optString("url"),
        thumbnail=o.optString("thumbnail"),isSelf=o.optBoolean("is_self"),
        over18=o.optBoolean("over18"),spoiler=o.optBoolean("spoiler"),
        stickied=o.optBoolean("stickied"),flair=o.optString("flair"),
        selftext=o.optString("selftext"),domain=o.optString("domain"),
        permalink=o.optString("permalink"),likes=o.optInt("likes"),
        saved=o.optBoolean("saved"),distinguished=o.optString("distinguished"),
        postHint=o.optString("post_hint"),previewUrl="",
        upvoteRatio=o.optDouble("upvote_ratio"),gilded=o.optInt("gilded"),
        locked=o.optBoolean("locked"),archived=o.optBoolean("archived"))

    private fun flattenComments(arr:JSONArray,depth:Int):List<CommentData>{
        val r=mutableListOf<CommentData>()
        for(i in 0 until arr.length()){
            try {
                val o=arr.getJSONObject(i)
                val repliesArr=o.optJSONArray("replies")
                r.add(CommentData(
                    id=o.optString("id"),author=o.optString("author"),
                    body=o.optString("body"),bodyHtml=o.optString("body_html"),
                    score=o.optInt("score"),created=o.optLong("created"),
                    depth=depth,stickied=o.optBoolean("stickied"),
                    flair=o.optString("flair"),distinguished=o.optString("distinguished"),
                    isSubmitter=o.optBoolean("is_submitter"),likes=o.optInt("likes"),
                    gilded=o.optInt("gilded"),collapsed=false))
                if(repliesArr!=null) r.addAll(flattenComments(repliesArr,depth+1))
            } catch(e: Exception) { Log.w(TAG,"Comment parse error at depth $depth",e) }
        }
        return r }

    private fun showError(msg:String){ if(!destroyed){ errorText.text=msg; errorText.visibility=View.VISIBLE } }
    private fun toast(msg:String){ if(!destroyed) Toast.makeText(this,msg,Toast.LENGTH_SHORT).show() }
    private fun dp(v:Int):Int = try { (v*resources.displayMetrics.density).toInt() } catch(e:Exception){ v*3 }
    override fun onBackPressed(){
        if(isInComments) hideComments()
        else if(!isInMenu) showMainMenu()
        else super.onBackPressed() }
} // end PinkReader

// ═══════════════════════════════════════════════════════════════════════════
// Data classes
// ═══════════════════════════════════════════════════════════════════════════
data class PostData(val id:String,val name:String,val title:String,val author:String,
    val subreddit:String,val score:Int,val numComments:Int,val created:Long,val url:String,
    val thumbnail:String,val isSelf:Boolean,val over18:Boolean,val spoiler:Boolean,
    val stickied:Boolean,val flair:String,val selftext:String,val domain:String,
    val permalink:String,val likes:Int,val saved:Boolean,val distinguished:String,
    val postHint:String,val previewUrl:String,val upvoteRatio:Double,val gilded:Int,
    val locked:Boolean,val archived:Boolean)

data class CommentData(val id:String,val author:String,val body:String,
    val bodyHtml:String,val score:Int,val created:Long,val depth:Int,
    val stickied:Boolean,val flair:String,val distinguished:String,
    val isSubmitter:Boolean,val likes:Int,val gilded:Int,var collapsed:Boolean=false)

enum class MenuItemType { SHORTCUT, USER, PINNED, SUBREDDIT }
data class MenuItem(val type:MenuItemType,val id:String,val label:String)

// ═══════════════════════════════════════════════════════════════════════════
// PostAdapter — Fix #5: Tag-based view access instead of getChildAt(N) casts
// ═══════════════════════════════════════════════════════════════════════════
class PostAdapter(
    private val onClick:(PostData)->Unit,
    private val onSwipeR:(PostData)->Unit,
    private val onSwipeL:(PostData)->Unit
): RecyclerView.Adapter<PostAdapter.VH>() {
    private val posts = mutableListOf<PostData>()

    class VH(val v:View) : RecyclerView.ViewHolder(v) {
        var stickiedTag: TextView? = null
        var nsfwTag: TextView? = null
        var flairTag: TextView? = null
        var titleTv: TextView? = null
        var thumbIv: ImageView? = null
        var selftextTv: TextView? = null
        var metaTv: TextView? = null
        var commentsTv: TextView? = null
        var sx = 0f; var sd = false
    }

    override fun onCreateViewHolder(p:ViewGroup,t:Int):VH {
        val ctx=p.context; val dark=Theme.isDark
        val card=LinearLayout(ctx).apply {
            orientation=LinearLayout.VERTICAL
            setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt())
            setPadding(dp(ctx,14),dp(ctx,12),dp(ctx,14),dp(ctx,12)) }

        val tags=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val st=TextView(ctx).apply{text="📌";setTextColor(0xFF4CAF50.toInt());textSize=11f;visibility=View.GONE;tag="stickied"}
        val nf=TextView(ctx).apply{text="NSFW";setTextColor(Color.WHITE);textSize=10f;setPadding(dp(ctx,4),1,dp(ctx,4),1);setBackgroundColor(0xFFFF5252.toInt());visibility=View.GONE;tag="nsfw"}
        val fl=TextView(ctx).apply{setTextColor(0xFFFF7597.toInt());textSize=11f;setPadding(dp(ctx,4),1,dp(ctx,4),1);visibility=View.GONE;tag="flair"}
        (st.layoutParams as? ViewGroup.MarginLayoutParams)?.rightMargin = dp(ctx,6)
        (nf.layoutParams as? ViewGroup.MarginLayoutParams)?.rightMargin = dp(ctx,6)
        tags.addView(st); tags.addView(nf); tags.addView(fl)

        val tr=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val ti=TextView(ctx).apply{setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt());textSize=15f;setLineSpacing(2f,1f);maxLines=4;layoutParams=LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f);tag="title"}
        val th=ImageView(ctx).apply{layoutParams=LinearLayout.LayoutParams(dp(ctx,64),dp(ctx,64)).apply{gravity=Gravity.CENTER_VERTICAL};scaleType=ImageView.ScaleType.CENTER_CROP;visibility=View.GONE;tag="thumb"}
        tr.addView(ti); tr.addView(th)

        val se=TextView(ctx).apply{setTextColor(if(dark)0xFF999999.toInt() else 0xFF757575.toInt());textSize=13f;maxLines=3;visibility=View.GONE;tag="selftext"}
        val me=TextView(ctx).apply{setTextColor(if(dark)0xFF888888.toInt() else 0xFF666666.toInt());textSize=12f;setPadding(0,dp(ctx,4),0,0);tag="meta"}
        val co=TextView(ctx).apply{setTextColor(0xFFFF7597.toInt());textSize=12f;setPadding(0,dp(ctx,2),0,0);tag="comments"}
        card.addView(tags); card.addView(tr); card.addView(se); card.addView(me); card.addView(co)

        card.setOnTouchListener{v,e->
            val vh=v.tag as? VH ?: return@setOnTouchListener false
            when(e.action){
                MotionEvent.ACTION_DOWN->{vh.sx=e.x;vh.sd=false;false}
                MotionEvent.ACTION_UP->{
                    val dx=e.x-vh.sx
                    val density = try { ctx.resources.displayMetrics.density } catch(_:Exception) { 2.8f }
                    if(abs(dx)>100*density&&!vh.sd){
                        vh.sd=true
                        val pos=vh.adapterPosition
                        if(pos>=0&&pos<posts.size){
                            if(dx>0)onSwipeR(posts[pos])else onSwipeL(posts[pos]) }
                        true
                    } else false }
                else->false}}

        val vh=VH(card)
        vh.stickiedTag=st; vh.nsfwTag=nf; vh.flairTag=fl
        vh.titleTv=ti; vh.thumbIv=th; vh.selftextTv=se; vh.metaTv=me; vh.commentsTv=co
        card.tag=vh
        return vh
    }

    override fun onBindViewHolder(h:VH,p:Int){
        if(p<0||p>=posts.size) return  // Fix: bounds check
        val post=posts[p]
        h.stickiedTag?.visibility=if(post.stickied)View.VISIBLE else View.GONE
        h.nsfwTag?.visibility=if(post.over18)View.VISIBLE else View.GONE
        h.flairTag?.apply{text=post.flair;visibility=if(post.flair.isNotEmpty())View.VISIBLE else View.GONE}
        h.titleTv?.text=post.title
        h.thumbIv?.visibility=if(post.thumbnail.isNotEmpty()&&post.thumbnail!="self"&&post.thumbnail!="nsfw"&&post.thumbnail!="default")View.VISIBLE else View.GONE
        h.selftextTv?.apply{text=post.selftext.take(200);visibility=if(post.selftext.isNotEmpty()&&post.isSelf)View.VISIBLE else View.GONE}
        val now=System.currentTimeMillis()/1000;val diff=now-post.created
        val age=when{diff<60->"now";diff<3600->"${diff/60}m";diff<86400->"${diff/3600}h";diff<2592000->"${diff/86400}d";diff<31536000->"${diff/2592000}mo";else->"${diff/31536000}y"}
        h.metaTv?.text="r/${post.subreddit} • u/${post.author} • $age"+(if(post.distinguished.isNotEmpty())" • ${post.distinguished.uppercase()}" else "")
        h.commentsTv?.text=if(post.numComments>0)"💬 ${post.numComments} comments" else "💬 discuss"
        h.itemView.setOnClickListener{onClick(post)}
    }

    override fun getItemCount()=posts.size
    fun setData(d:List<PostData>){
        synchronized(posts){ posts.clear(); posts.addAll(d) }
        notifyDataSetChanged() }
    fun addData(d:List<PostData>){
        synchronized(posts){
            val s=posts.size; posts.addAll(d)
            notifyItemRangeInserted(s,d.size) } }

    companion object {
        private fun dp(ctx:android.content.Context, v:Int):Int =
            try { (v*ctx.resources.displayMetrics.density).toInt() } catch(e:Exception){ v*3 }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// CommentAdapter — Fix #6: Safe index calculation, no force-unwrap
// ═══════════════════════════════════════════════════════════════════════════
class CommentAdapter(private val onToggle:(CommentData)->Unit):
    RecyclerView.Adapter<RecyclerView.ViewHolder>() {
    private var postHeader:PostData?=null
    private val items=mutableListOf<CommentData>()

    private fun hasHeader() = postHeader != null

    override fun getItemViewType(p:Int):Int =
        if(p==0 && hasHeader()) 0 else 1

    override fun getItemCount():Int {
        val h = if(hasHeader()) 1 else 0
        return items.size + h
    }

    override fun onCreateViewHolder(p:ViewGroup,t:Int):RecyclerView.ViewHolder {
        val ctx=p.context; val dark=Theme.isDark
        if(t==0){
            val v=TextView(ctx).apply{
                setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt());textSize=16f
                setPadding(dp(ctx,16),dp(ctx,16),dp(ctx,16),dp(ctx,12))
                setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt()) }
            return object:RecyclerView.ViewHolder(v){} }
        val card=LinearLayout(ctx).apply{
            orientation=LinearLayout.VERTICAL
            setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt())
            setPadding(dp(ctx,10),dp(ctx,8),dp(ctx,10),dp(ctx,8)) }
        val top=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val au=TextView(ctx).apply{setTextColor(0xFFFF7597.toInt());textSize=13f;setTypeface(null,Typeface.BOLD);tag="author"}
        val fl=TextView(ctx).apply{setTextColor(if(dark)0xFF888888.toInt() else 0xFF666666.toInt());textSize=10f;setPadding(dp(ctx,6),0,0,0);visibility=View.GONE;tag="flair"}
        top.addView(au); top.addView(fl)
        val bo=TextView(ctx).apply{setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt());textSize=14f;setLineSpacing(2f,1f);tag="body"}
        val bh=TextView(ctx).apply{setTextColor(if(dark)0xFF888888.toInt() else 0xFF666666.toInt());textSize=14f;text="[collapsed]";visibility=View.GONE;tag="collapsed"}
        val me=TextView(ctx).apply{setTextColor(if(dark)0xFF777777.toInt() else 0xFF888888.toInt());textSize=11f;tag="age"}
        val sc=TextView(ctx).apply{setTextColor(if(dark)0xFF777777.toInt() else 0xFF888888.toInt());textSize=11f;tag="score"}
        card.addView(top); card.addView(bo); card.addView(bh); card.addView(me); card.addView(sc)
        card.setOnClickListener{val c=it.tag as? CommentData?;if(c!=null)onToggle(c)}
        return object:RecyclerView.ViewHolder(card){} }

    override fun onBindViewHolder(h:RecyclerView.ViewHolder,p:Int){
        if(getItemViewType(p)==0){
            val v=h.itemView as TextView
            val ph = postHeader  // safe local copy
            if(ph!=null) v.text="${ph.title}\n\nr/${ph.subreddit} • u/${ph.author} • ${ph.score} pts • ${ph.numComments} comments\n${ph.selftext.take(500)}"
            return }

        // Fix: Safe index with bounds check
        val idx = if(hasHeader()) p-1 else p
        if(idx<0||idx>=items.size) return

        val c=items[idx]; val card=h.itemView as LinearLayout; card.tag=c
        (card.layoutParams as? ViewGroup.MarginLayoutParams)?.leftMargin=c.depth*dp(card.context,12)

        val top=card.getChildAt(0)as? LinearLayout ?: return
        (top.getChildAt(0)as? TextView)?.text="u/${c.author}"
        (top.getChildAt(1)as? TextView)?.apply{text=c.flair;visibility=if(c.flair.isNotEmpty())View.VISIBLE else View.GONE}

        val bo=card.getChildAt(1)as? TextView ?: return
        val bh=card.getChildAt(2)as? TextView ?: return
        if(c.collapsed){
            bo.visibility=View.GONE; bh.visibility=View.VISIBLE
            card.getChildAt(3).visibility=View.GONE; card.getChildAt(4).visibility=View.GONE }
        else{
            bo.text=c.body; bo.visibility=View.VISIBLE; bh.visibility=View.GONE
            val now=System.currentTimeMillis()/1000;val d=now-c.created
            val age=when{d<60->"now";d<3600->"${d/60}m";d<86400->"${d/3600}h";else->"${d/86400}d"}
            (card.getChildAt(3)as? TextView)?.text=age+(if(c.isSubmitter)" • OP" else "")+(if(c.distinguished=="moderator")" • MOD" else if(c.distinguished=="admin")" • ADMIN" else "")
            (card.getChildAt(4)as? TextView)?.text="${c.score} pts"+(if(c.gilded>0)" • ⭐${c.gilded}" else "")
            card.getChildAt(3).visibility=View.VISIBLE; card.getChildAt(4).visibility=View.VISIBLE } }

    fun setData(ph:PostData?,comments:List<CommentData>){
        postHeader=ph; items.clear(); items.addAll(comments); notifyDataSetChanged() }

    companion object {
        private fun dp(ctx:android.content.Context, v:Int):Int =
            try { (v*ctx.resources.displayMetrics.density).toInt() } catch(e:Exception){ v*3 }
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// MenuAdapter
// ═══════════════════════════════════════════════════════════════════════════
class MenuAdapter(private val onSelect:(MenuItem)->Unit):RecyclerView.Adapter<RecyclerView.ViewHolder>(){
    private val items=mutableListOf<MenuItem>()
    init{
        items.add(MenuItem(MenuItemType.SHORTCUT,"frontpage","Front Page"))
        items.add(MenuItem(MenuItemType.SHORTCUT,"popular","Popular"))
        items.add(MenuItem(MenuItemType.SHORTCUT,"all","All"))
        items.add(MenuItem(MenuItemType.SHORTCUT,"search","Find Subreddit"))
        items.add(MenuItem(MenuItemType.USER,"inbox","Inbox"))
        items.add(MenuItem(MenuItemType.USER,"saved","Saved"))
        items.add(MenuItem(MenuItemType.USER,"hidden","Hidden"))
        items.add(MenuItem(MenuItemType.USER,"upvoted","Upvoted"))
        items.add(MenuItem(MenuItemType.USER,"downvoted","Downvoted")) }
    override fun onCreateViewHolder(p:ViewGroup,t:Int):RecyclerView.ViewHolder{
        val ctx=p.context; val dark=Theme.isDark
        val v=TextView(ctx).apply{
            setPadding(mdp(p,16),mdp(p,14),mdp(p,16),mdp(p,14));textSize=15f
            setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt())
            setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt()) }
        return object:RecyclerView.ViewHolder(v){} }
    override fun onBindViewHolder(h:RecyclerView.ViewHolder,p:Int){
        if(p<0||p>=items.size) return
        (h.itemView as TextView).text=items[p].label
        h.itemView.setOnClickListener{onSelect(items[p])} }
    override fun getItemCount()=items.size

    companion object {
        private fun mdp(p:ViewGroup, v:Int):Int =
            try { (v*p.context.resources.displayMetrics.density).toInt() } catch(e:Exception){ v*3 }
    }
}
