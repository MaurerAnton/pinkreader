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

    // JNI-safe wrappers
    private fun jniCall(b: ()->String) = if(nativeLoaded) try{b()}catch(e:Exception){Log.w(TAG,"JNI",e);""} else ""
    private fun jniVoid(b: ()->Unit) { if(nativeLoaded) try{b()}catch(e:Exception){Log.w(TAG,"JNI",e)} }
    private fun jniBool(b: ()->Boolean) = if(nativeLoaded) try{b()}catch(e:Exception){false} else false

    // ── State ────────────────────────────────────────────────────────
    private var currentSub = "popular"; private var currentSort = "hot"; private var after = ""
    private var lastAfter = ""; @Volatile private var isLoading = false; private var isInComments = false
    private var isInMenu = true; private var currentPostId = ""; private var currentPostSub = ""
    private val postIds = HashSet<String>(); private var accessToken = ""
    private val handler = Handler(Looper.getMainLooper()); @Volatile private var destroyed = false

    // UI
    private lateinit var root: FrameLayout; private lateinit var toolbar: LinearLayout
    private lateinit var titleText: TextView; private lateinit var sortSpinner: Spinner
    private lateinit var refresh: SwipeRefreshLayout; private lateinit var postList: RecyclerView
    private lateinit var commentList: RecyclerView; private lateinit var menuList: RecyclerView
    private lateinit var loadingBar: ProgressBar; private lateinit var emptyText: TextView
    private lateinit var errorText: TextView; private lateinit var subInput: EditText
    private lateinit var bottomBar: LinearLayout
    private lateinit var postAdapter: PostAdapter; private lateinit var commentAdapter: CommentAdapter
    private lateinit var menuAdapter: MenuAdapter

    // ====================================================================
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        try {
            jniVoid { nativeInit(cacheDir.absolutePath) }
            Theme.isDark = jniBool { nativeGetDarkTheme() }
            accessToken = jniCall { nativeGetToken() }
        } catch(e: Exception) { Log.e(TAG,"JNI init failed",e) }
        catch(e: Error) { Log.e(TAG,"JNI init fatal error",e) }

        window.statusBarColor = if(Theme.isDark) 0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt()
        window.navigationBarColor = if(Theme.isDark) 0xFF1E1E1E.toInt() else 0xFFF5F5F5.toInt()

        root = FrameLayout(this).apply { setBackgroundColor(Theme.bg()) }
        try { buildToolbar(); buildContent() } catch(e: Exception) { Log.e(TAG,"UI build failed",e); finish(); return }
        setContentView(root)
        if(savedInstanceState == null) showMainMenu()
    }

    override fun onDestroy() { super.onDestroy(); destroyed = true; handler.removeCallbacksAndMessages(null) }

    // ════════════════════════════════════════════════════════════════════
    // TOOLBAR — RedReader style: dark background, pink accent
    // ════════════════════════════════════════════════════════════════════
    private fun buildToolbar() {
        toolbar = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setBackgroundColor(if(Theme.isDark) 0xFF1A1A1A.toInt() else 0xFFF5F5F5.toInt())
            setPadding(12.dp, 8.dp, 12.dp, 4.dp); visibility = View.GONE
        }
        val top = LinearLayout(this).apply { orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER_VERTICAL }
        top.addView(TextView(this).apply { text="←"; setTextColor(0xFFFF7597.toInt()); textSize=22f; setPadding(0,0,12.dp,0)
            setOnClickListener { try { if(isInComments) hideComments() else if(!isInMenu) showMainMenu() } catch(e:Exception){} } })
        titleText = TextView(this).apply {
            setTextColor(if(Theme.isDark) 0xFFE0E0E0.toInt() else 0xFF212121.toInt()); textSize=18f
            setTypeface(typeface, Typeface.BOLD); layoutParams = LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
        }
        sortSpinner = Spinner(this).apply {
            adapter = ArrayAdapter(this@PinkReader, android.R.layout.simple_spinner_dropdown_item, arrayOf("Hot","New","Top","Rising","Controversial"))
            setPopupBackgroundDrawable(ColorDrawable(if(Theme.isDark)0xFF2A2A2A.toInt() else 0xFFFFFFFF.toInt()))
            onItemSelectedListener = object:AdapterView.OnItemSelectedListener{
                override fun onItemSelected(p0:AdapterView<*>?,p1:View?,p2:Int,p3:Long){
                    currentSort = arrayOf("hot","new","top","rising","controversial")[p2]
                    if(!isInComments&&!isInMenu){ after=""; lastAfter=""; postIds.clear(); loadPosts() }}
                override fun onNothingSelected(p0:AdapterView<*>?){}}
        }
        top.addView(titleText); top.addView(sortSpinner)
        top.addView(TextView(this).apply { text="☰"; setTextColor(0xFFFF7597.toInt()); textSize=22f; setPadding(12.dp,0,0,0); setOnClickListener{showMainMenu()} })
        toolbar.addView(top)

        val ir = LinearLayout(this).apply { orientation=LinearLayout.HORIZONTAL; gravity=Gravity.CENTER_VERTICAL; setPadding(0,4.dp,0,0) }
        subInput = EditText(this).apply {
            hint="Go to subreddit..."; setHintTextColor(0xFF888888.toInt())
            setTextColor(if(Theme.isDark)0xFFE0E0E0.toInt() else 0xFF212121.toInt()); textSize=14f
            setBackgroundColor(if(Theme.isDark)0xFF2A2A2A.toInt() else 0xFFFFFFFF.toInt())
            setPadding(8.dp,4.dp,8.dp,4.dp); setSingleLine(); imeOptions=EditorInfo.IME_ACTION_GO
            layoutParams = LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
            setOnEditorActionListener{_,a,_->if(a==EditorInfo.IME_ACTION_GO){navigate(text.toString().trim());true}else false}
        }
        ir.addView(subInput)
        ir.addView(TextView(this).apply { text="GO"; setTextColor(0xFFFF7597.toInt()); setTypeface(typeface,Typeface.BOLD); setPadding(12.dp,8.dp,0,8.dp); setOnClickListener{navigate(subInput.text.toString().trim())} })
        toolbar.addView(ir)
        root.addView(toolbar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT).apply{gravity=Gravity.TOP})
    }

    // ════════════════════════════════════════════════════════════════════
    // CONTENT — RedReader colors
    // ════════════════════════════════════════════════════════════════════
    private fun buildContent() {
        loadingBar = ProgressBar(this).apply { isIndeterminate=true; visibility=View.GONE
            (layoutParams as? FrameLayout.LayoutParams)?.gravity = Gravity.TOP }

        refresh = SwipeRefreshLayout(this).apply {
            setColorSchemeColors(0xFFFF7597.toInt())
            setProgressBackgroundColorSchemeColor(if(Theme.isDark)0xFF2A2A2A.toInt() else 0xFFFFFFFF.toInt())
            setOnRefreshListener { after=""; lastAfter=""; postIds.clear(); loadPosts() }
        }

        postAdapter = PostAdapter(
            { p -> nativeMarkRead(p.id); openComments(p) },
            { p -> thread { try { val url=nativeBuildActionUrl("vote","t3_"+p.id); val body=nativeBuildActionBody("t3_"+p.id,"dir=1&rank=2"); NetworkManager.post(url,body,accessToken); handler.post{toast("Upvoted")} } catch(e:Exception){} } },
            { p -> thread { try { val url=nativeBuildActionUrl("vote","t3_"+p.id); val body=nativeBuildActionBody("t3_"+p.id,"dir=-1&rank=2"); NetworkManager.post(url,body,accessToken); handler.post{toast("Downvoted")} } catch(e:Exception){} } }
        )
        postList = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader); adapter = postAdapter
            setBackgroundColor(if(Theme.isDark)0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt())
            addItemDecoration(object:RecyclerView.ItemDecoration(){override fun getItemOffsets(o:Rect,v:View,p:RecyclerView,s:RecyclerView.State){o.bottom=1.dp}})
            addOnScrollListener(object:RecyclerView.OnScrollListener(){override fun onScrolled(rv:RecyclerView,dx:Int,dy:Int){
                try { val lm=rv.layoutManager as LinearLayoutManager
                if(!isLoading&&after.isNotEmpty()&&after!=lastAfter&&lm.findLastVisibleItemPosition()>=postAdapter.itemCount-3)loadMore() } catch(e:Exception){} }})
        }
        refresh.addView(postList)

        commentAdapter = CommentAdapter { c -> c.collapsed=!c.collapsed; commentAdapter.notifyDataSetChanged() }
        commentList = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader); adapter = commentAdapter
            setBackgroundColor(if(Theme.isDark)0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt()); visibility=View.GONE
        }

        menuAdapter = MenuAdapter { i -> onMenuSelected(i) }
        menuList = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader); adapter = menuAdapter
            setBackgroundColor(if(Theme.isDark)0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt())
        }

        emptyText = TextView(this).apply {
            setTextColor(0xFF888888.toInt()); textSize=14f; gravity=Gravity.CENTER
            visibility=View.GONE; setPadding(32.dp,64.dp,32.dp,32.dp)
        }
        errorText = TextView(this).apply {
            setTextColor(0xFFFF5252.toInt()); textSize=14f; gravity=Gravity.CENTER
            visibility=View.GONE; setPadding(32.dp,32.dp,32.dp,32.dp)
        }

        root.addView(menuList, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(refresh, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(commentList, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(loadingBar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,3.dp).apply{gravity=Gravity.TOP})
        root.addView(emptyText, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(errorText, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))

        // Bottom bar
        bottomBar = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER
            setBackgroundColor(if(Theme.isDark)0xFF1A1A1A.toInt() else 0xFFF5F5F5.toInt())
            setPadding(0,6.dp,0,10.dp)
            fun add(icon:String,label:String,a:()->Unit) {
                val b = LinearLayout(this@PinkReader).apply {
                    orientation=LinearLayout.VERTICAL; gravity=Gravity.CENTER
                    layoutParams = LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
                    setOnClickListener { try { a() } catch(e:Exception){} }
                }
                b.addView(TextView(this@PinkReader).apply{text=icon;textSize=18f;gravity=Gravity.CENTER;setTextColor(0xFF999999.toInt())})
                b.addView(TextView(this@PinkReader).apply{text=label;textSize=10f;gravity=Gravity.CENTER;setTextColor(0xFF999999.toInt())})
                addView(b)
            }
            add("🔥","Popular"){navigate("popular")}; add("🌐","All"){navigate("all")}
            add("🏠","Front"){navigate("")}; add("📬","Inbox"){showInbox()}
            add("🧪","Test"){runNetworkTest()}
            add("🎨","Theme"){Theme.isDark=!Theme.isDark;nativeSetDarkTheme(Theme.isDark);recreate()}
        }
        root.addView(bottomBar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT).apply{gravity=Gravity.BOTTOM})
    }

    // ── Navigation ──────────────────────────────────────────────────
    private fun showMainMenu(){ isInMenu=true; isInComments=false; toolbar.visibility=View.GONE; menuList.visibility=View.VISIBLE; postList.visibility=View.GONE; commentList.visibility=View.GONE; errorText.visibility=View.GONE; emptyText.visibility=View.GONE }
    private fun navigate(sub:String){ currentSub=sub.ifEmpty{"frontpage"}; currentSort="hot"; after=""; lastAfter=""; postIds.clear(); isInMenu=false; isInComments=false; sortSpinner.setSelection(0); titleText.text=if(sub.isEmpty())"front page" else "r/$sub"; subInput.setText(""); toolbar.visibility=View.VISIBLE; menuList.visibility=View.GONE; commentList.visibility=View.GONE; postList.visibility=View.VISIBLE; errorText.visibility=View.GONE; emptyText.visibility=View.GONE; loadPosts() }
    private fun openComments(p:PostData){ currentPostId=p.id; currentPostSub=p.subreddit; isInComments=true; isInMenu=false; titleText.text=p.title.take(40); toolbar.visibility=View.VISIBLE; postList.visibility=View.GONE; commentList.visibility=View.VISIBLE; menuList.visibility=View.GONE; errorText.visibility=View.GONE; emptyText.visibility=View.GONE; loadComments() }
    private fun hideComments(){ isInComments=false; titleText.text="r/$currentSub"; commentList.visibility=View.GONE; postList.visibility=View.VISIBLE }
    private fun showInbox(){ toolbar.visibility=View.VISIBLE; titleText.text="Inbox"; postList.visibility=View.GONE; commentList.visibility=View.GONE; menuList.visibility=View.GONE
        thread{try{val url=nativeBuildInboxUrl("inbox",25);val json=NetworkManager.get(url,accessToken);if(!json.isOk){handler.post{showError("Inbox requires OAuth login")};return@thread}
            val parsed=nativeParseInbox(json.body);val arr=JSONArray(parsed);val sb=StringBuilder("=== Inbox ===\n\n")
            for(i in 0 until arr.length()){val m=arr.getJSONObject(i);sb.append("From: u/${m.optString("author","?")}\nSubject: ${m.optString("subject","")}\n${m.optString("body","").take(300)}\n\n---\n\n")}
            handler.post{errorText.text=sb.toString();errorText.visibility=View.VISIBLE}}catch(ex:Exception){handler.post{showError("Inbox: ${ex.message}")}}}}

    private fun onMenuSelected(item:MenuItem){when(item.type){MenuItemType.SHORTCUT->when(item.id){"frontpage"->navigate("");"popular"->navigate("popular");"all"->navigate("all");"search"->showSearchDialog()};MenuItemType.USER->when(item.id){"inbox"->showInbox()};else->{}}}
    private fun showSearchDialog(){val input=EditText(this).apply{hint="Search subreddits...";setSingleLine()}
        AlertDialog.Builder(this).setTitle("Find Subreddit").setView(input).setPositiveButton("Search"){_,_->thread{try{val url=nativeBuildSubredditSearchUrl(input.text.toString(),25);val resp=NetworkManager.get(url,accessToken);if(!resp.isOk){handler.post{toast("Search failed")};return@thread}
            val parsed=nativeParseSubredditSearch(resp.body);val arr=JSONArray(parsed);val sb=StringBuilder("Results:\n\n")
            for(i in 0 until arr.length()){val s=arr.getJSONObject(i);sb.append("r/${s.optString("name")} - ${s.optString("title","")}\n")}
            handler.post{errorText.text=sb.toString();errorText.visibility=View.VISIBLE;menuList.visibility=View.GONE}}catch(ex:Exception){handler.post{toast(ex.message?:"Search failed")}}}}.setNegativeButton("Cancel",null).show()}

    // ── Data loading via HTTPS ──────────────────────────────────────
    private fun loadPosts(){
        if(isLoading)return; isLoading=true; loadingBar.visibility=View.VISIBLE; errorText.visibility=View.GONE; emptyText.visibility=View.GONE
        thread{try{var json=nativeGetCached(currentSub+"_"+currentSort)
            if(json.isEmpty()){val url=nativeBuildPostsUrl(currentSub,currentSort,after,25);val resp=NetworkManager.get(url,accessToken);if(resp.isOk)json=resp.body;if(json.isNotEmpty())nativePutCache(currentSub+"_"+currentSort,json)}
            if(json.isEmpty()){handler.post{showError("No response from Reddit.\nReddit may block anonymous requests.\nTry again or use OAuth.\n\nPull to refresh");loadingBar.visibility=View.GONE;refresh.isRefreshing=false;isLoading=false};return@thread}
            val parsed=nativeParsePostListing(json);val arr=JSONArray(parsed);val posts=mutableListOf<PostData>()
            for(i in 0 until arr.length()){val o=arr.getJSONObject(i);val id=o.optString("id");if(postIds.contains(id))continue;postIds.add(id);posts.add(parsePost(o))}
            if(posts.isNotEmpty())after="t3_"+posts.last().id
            handler.post{postAdapter.setData(posts);loadingBar.visibility=View.GONE;refresh.isRefreshing=false;isLoading=false
                if(posts.isEmpty()&&postAdapter.itemCount==0){emptyText.text="No posts loaded\nPull to refresh";emptyText.visibility=View.VISIBLE}
                else if(posts.isNotEmpty())toast("${posts.size} posts loaded")}}
        catch(ex:Exception){handler.post{showError("Error: ${ex.message}");loadingBar.visibility=View.GONE;refresh.isRefreshing=false;isLoading=false}}}}

    private fun loadMore(){if(isLoading||after.isEmpty()||after==lastAfter)return;lastAfter=after;isLoading=true
        thread{try{val url=nativeBuildPostsUrl(currentSub,currentSort,after,25);val resp=NetworkManager.get(url,accessToken);if(!resp.isOk){handler.post{isLoading=false};return@thread}
            val parsed=nativeParsePostListing(resp.body);val arr=JSONArray(parsed);val posts=mutableListOf<PostData>()
            for(i in 0 until arr.length()){val o=arr.getJSONObject(i);val id=o.optString("id");if(postIds.contains(id))continue;postIds.add(id);posts.add(parsePost(o))}
            if(posts.isNotEmpty())after="t3_"+posts.last().id;handler.post{postAdapter.addData(posts);isLoading=false}}catch(ex:Exception){handler.post{isLoading=false}}}}

    private fun loadComments(){isLoading=true;loadingBar.visibility=View.VISIBLE
        thread{try{val url=nativeBuildCommentsUrl(currentPostSub,currentPostId,"confidence",300);val resp=NetworkManager.get(url,accessToken);if(!resp.isOk){handler.post{showError("Failed to load comments");loadingBar.visibility=View.GONE;isLoading=false};return@thread}
            val postJson=nativeParsePost(resp.body);val ph=if(postJson.isNotEmpty())try{parsePost(JSONObject(postJson))}catch(e:Exception){null}else null;val arr=try{JSONArray(nativeParseComments(resp.body))}catch(e:Exception){JSONArray()};val comments=flattenComments(arr,0)
            handler.post{commentAdapter.setData(ph,comments);loadingBar.visibility=View.GONE;isLoading=false;if(comments.isEmpty()){emptyText.text="No comments yet";emptyText.visibility=View.VISIBLE}}}
        catch(ex:Exception){handler.post{showError("Comments: ${ex.message}");loadingBar.visibility=View.GONE;isLoading=false}}}}

    // ── Network test ─────────────────────────────────────────────────
    private fun runNetworkTest(){errorText.visibility=View.VISIBLE;errorText.text="Running tests..."
        thread{val sb=StringBuilder("=== PinkReader Tests ===\n\n")
            try{sb.append("Native: ${if(nativeRunTests())"✓" else "✗"}\n")}catch(e:Exception){sb.append("Native: ✗ ${e.message}\n")}
            try{val url=nativeBuildPostsUrl("popular","hot","",3);sb.append("URL: $url\n")
                val resp=NetworkManager.get(url,accessToken)
                if(resp.isOk&&resp.body.startsWith("{")){val arr=JSONArray(nativeParsePostListing(resp.body));sb.append("Posts: ✓ ${arr.length()} loaded\n")
                    for(i in 0 until minOf(arr.length(),3)){val p=arr.getJSONObject(i);sb.append("  r/${p.optString("subreddit")}: ${p.optString("title","").take(50)}\n")}}
                else sb.append("HTTP: ✗ code=${resp.code} (${resp.body.take(100)})\n")}
            catch(e:Exception){sb.append("Network: ✗ ${e.message}\n")}
            try{nativePutCache("test","ok");sb.append("Cache: ${if(nativeGetCached("test")=="ok")"✓" else "✗"}\n")}catch(e:Exception){sb.append("Cache: ✗\n")}
            sb.append("Engine: ${nativeGetVersion()}\n")
            handler.post{errorText.text=sb.toString()}}}

    // ── Helpers ─────────────────────────────────────────────────────
    private fun parsePost(o:JSONObject) = PostData(id=o.optString("id"),name=o.optString("name"),title=o.optString("title"),author=o.optString("author"),subreddit=o.optString("subreddit"),score=o.optInt("score"),numComments=o.optInt("num_comments"),created=o.optLong("created"),url=o.optString("url"),thumbnail=o.optString("thumbnail"),isSelf=o.optBoolean("is_self"),over18=o.optBoolean("over18"),spoiler=o.optBoolean("spoiler"),stickied=o.optBoolean("stickied"),flair=o.optString("flair"),selftext=o.optString("selftext"),domain=o.optString("domain"),permalink=o.optString("permalink"),likes=o.optInt("likes"),saved=o.optBoolean("saved"),distinguished=o.optString("distinguished"),postHint=o.optString("post_hint"),previewUrl="",upvoteRatio=o.optDouble("upvote_ratio"),gilded=o.optInt("gilded"),locked=o.optBoolean("locked"),archived=o.optBoolean("archived"))

    private fun flattenComments(arr:JSONArray,depth:Int):List<CommentData>{val r=mutableListOf<CommentData>()
        for(i in 0 until arr.length()){val o=arr.getJSONObject(i);val repliesArr=o.optJSONArray("replies")
            r.add(CommentData(id=o.optString("id"),author=o.optString("author"),body=o.optString("body"),bodyHtml=o.optString("body_html"),score=o.optInt("score"),created=o.optLong("created"),depth=depth,stickied=o.optBoolean("stickied"),flair=o.optString("flair"),distinguished=o.optString("distinguished"),isSubmitter=o.optBoolean("is_submitter"),likes=o.optInt("likes"),gilded=o.optInt("gilded"),collapsed=false))
            if(repliesArr!=null)r.addAll(flattenComments(repliesArr,depth+1))}
        return r}

    private fun showError(msg:String){if(!destroyed){errorText.text=msg;errorText.visibility=View.VISIBLE}}
    private fun toast(msg:String){if(!destroyed) Toast.makeText(this,msg,Toast.LENGTH_SHORT).show()}
    private val Int.dp get()=(this*resources.displayMetrics.density).toInt()
    override fun onBackPressed(){if(isInComments)hideComments()else if(!isInMenu)showMainMenu()else super.onBackPressed()}
} // end PinkReader

// ═══════════════════════════════════════════════════════════════════════════
// Data classes
// ═══════════════════════════════════════════════════════════════════════════
data class PostData(val id:String,val name:String,val title:String,val author:String,val subreddit:String,val score:Int,val numComments:Int,val created:Long,val url:String,val thumbnail:String,val isSelf:Boolean,val over18:Boolean,val spoiler:Boolean,val stickied:Boolean,val flair:String,val selftext:String,val domain:String,val permalink:String,val likes:Int,val saved:Boolean,val distinguished:String,val postHint:String,val previewUrl:String,val upvoteRatio:Double,val gilded:Int,val locked:Boolean,val archived:Boolean)
data class CommentData(val id:String,val author:String,val body:String,val bodyHtml:String,val score:Int,val created:Long,val depth:Int,val stickied:Boolean,val flair:String,val distinguished:String,val isSubmitter:Boolean,val likes:Int,val gilded:Int,var collapsed:Boolean=false)
enum class MenuItemType { SHORTCUT, USER, PINNED, SUBREDDIT }
data class MenuItem(val type:MenuItemType,val id:String,val label:String)

// ═══════════════════════════════════════════════════════════════════════════
// PostAdapter — RedReader post cards
// ═══════════════════════════════════════════════════════════════════════════
class PostAdapter(private val onClick:(PostData)->Unit,private val onSwipeR:(PostData)->Unit,private val onSwipeL:(PostData)->Unit):RecyclerView.Adapter<PostAdapter.VH>(){
    private val posts=mutableListOf<PostData>()
    class VH(val v:View):RecyclerView.ViewHolder(v){var sx=0f;var sd=false}
    override fun onCreateViewHolder(p:ViewGroup,t:Int):VH{val ctx=p.context;val dark=Theme.isDark
        val card=LinearLayout(ctx).apply{orientation=LinearLayout.VERTICAL;setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt());setPadding(14.dp,12.dp,14.dp,12.dp)}
        val tags=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val st=TextView(ctx).apply{text="📌";setTextColor(0xFF4CAF50.toInt());textSize=11f;visibility=View.GONE}
        val nf=TextView(ctx).apply{text="NSFW";setTextColor(Color.WHITE);textSize=10f;setPadding(4.dp,1,4.dp,1);setBackgroundColor(0xFFFF5252.toInt());visibility=View.GONE}
        val fl=TextView(ctx).apply{setTextColor(0xFFFF7597.toInt());textSize=11f;setPadding(4.dp,1,4.dp,1);visibility=View.GONE}
        tags.addView(st);(tags.getChildAt(0)as TextView).apply{(layoutParams as ViewGroup.MarginLayoutParams).rightMargin=6.dp}
        tags.addView(nf);(tags.getChildAt(1)as TextView).apply{(layoutParams as ViewGroup.MarginLayoutParams).rightMargin=6.dp}
        tags.addView(fl)
        val tr=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val ti=TextView(ctx).apply{setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt());textSize=15f;setLineSpacing(2f,1f);maxLines=4;layoutParams=LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)}
        val th=ImageView(ctx).apply{layoutParams=LinearLayout.LayoutParams(64.dp,64.dp).apply{gravity=Gravity.CENTER_VERTICAL};scaleType=ImageView.ScaleType.CENTER_CROP;visibility=View.GONE}
        tr.addView(ti);tr.addView(th)
        val se=TextView(ctx).apply{setTextColor(if(dark)0xFF999999.toInt() else 0xFF757575.toInt());textSize=13f;maxLines=3;visibility=View.GONE}
        val me=TextView(ctx).apply{setTextColor(if(dark)0xFF888888.toInt() else 0xFF666666.toInt());textSize=12f;setPadding(0,4.dp,0,0)}
        val co=TextView(ctx).apply{setTextColor(0xFFFF7597.toInt());textSize=12f;setPadding(0,2.dp,0,0)}
        card.addView(tags);card.addView(tr);card.addView(se);card.addView(me);card.addView(co)
        card.setOnTouchListener{v,e->val vh=v.tag as? VH?:return@setOnTouchListener false
            when(e.action){MotionEvent.ACTION_DOWN->{vh.sx=e.x;vh.sd=false;false}
                MotionEvent.ACTION_UP->{val dx=e.x-vh.sx;if(abs(dx)>100*ctx.resources.displayMetrics.density&&!vh.sd){vh.sd=true;if(dx>0)onSwipeR(posts[vh.adapterPosition])else onSwipeL(posts[vh.adapterPosition]);true}else false}
                else->false}}
        val vh=VH(card);card.tag=vh;return vh}
    override fun onBindViewHolder(h:VH,p:Int){val post=posts[p];val card=h.v as LinearLayout;val tags=card.getChildAt(0)as LinearLayout
        (tags.getChildAt(0)as TextView).visibility=if(post.stickied)View.VISIBLE else View.GONE
        (tags.getChildAt(1)as TextView).visibility=if(post.over18)View.VISIBLE else View.GONE
        (tags.getChildAt(2)as TextView).apply{text=post.flair;visibility=if(post.flair.isNotEmpty())View.VISIBLE else View.GONE}
        val tr=card.getChildAt(1)as LinearLayout;(tr.getChildAt(0)as TextView).text=post.title
        (tr.getChildAt(1)as ImageView).visibility=if(post.thumbnail.isNotEmpty()&&post.thumbnail!="self"&&post.thumbnail!="nsfw"&&post.thumbnail!="default")View.VISIBLE else View.GONE
        (card.getChildAt(2)as TextView).apply{text=post.selftext.take(200);visibility=if(post.selftext.isNotEmpty()&&post.isSelf)View.VISIBLE else View.GONE}
        val now=System.currentTimeMillis()/1000;val diff=now-post.created;val age=when{diff<60->"now";diff<3600->"${diff/60}m";diff<86400->"${diff/3600}h";diff<2592000->"${diff/86400}d";diff<31536000->"${diff/2592000}mo";else->"${diff/31536000}y"}
        (card.getChildAt(3)as TextView).text="r/${post.subreddit} • u/${post.author} • $age"+(if(post.distinguished.isNotEmpty())" • ${post.distinguished.uppercase()}" else "")
        (card.getChildAt(4)as TextView).text=if(post.numComments>0)"💬 ${post.numComments} comments" else "💬 discuss"
        card.setOnClickListener{onClick(post)}}
    override fun getItemCount()=posts.size
    fun setData(d:List<PostData>){posts.clear();posts.addAll(d);notifyDataSetChanged()}
    fun addData(d:List<PostData>){val s=posts.size;posts.addAll(d);notifyItemRangeInserted(s,d.size)}
    private val Int.dp get()=(this*2.8f).toInt()}

// ═══════════════════════════════════════════════════════════════════════════
// CommentAdapter
// ═══════════════════════════════════════════════════════════════════════════
class CommentAdapter(private val onToggle:(CommentData)->Unit):RecyclerView.Adapter<RecyclerView.ViewHolder>(){
    private var postHeader:PostData?=null;private val items=mutableListOf<CommentData>()
    override fun getItemViewType(p:Int)=if(p==0&&postHeader!=null)0 else 1
    override fun onCreateViewHolder(p:ViewGroup,t:Int):RecyclerView.ViewHolder{val ctx=p.context;val dark=Theme.isDark
        if(t==0){val v=TextView(ctx).apply{setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt());textSize=16f;setPadding(16.dp,16.dp,16.dp,12.dp);setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt())};return object:RecyclerView.ViewHolder(v){}}
        val card=LinearLayout(ctx).apply{orientation=LinearLayout.VERTICAL;setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt());setPadding(10.dp,8.dp,10.dp,8.dp)}
        val top=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val au=TextView(ctx).apply{setTextColor(0xFFFF7597.toInt());textSize=13f;setTypeface(null,Typeface.BOLD)}
        val fl=TextView(ctx).apply{setTextColor(if(dark)0xFF888888.toInt() else 0xFF666666.toInt());textSize=10f;setPadding(6.dp,0,0,0);visibility=View.GONE}
        top.addView(au);top.addView(fl)
        val bo=TextView(ctx).apply{setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt());textSize=14f;setLineSpacing(2f,1f)}
        val bh=TextView(ctx).apply{setTextColor(if(dark)0xFF888888.toInt() else 0xFF666666.toInt());textSize=14f;text="[collapsed]";visibility=View.GONE}
        val me=TextView(ctx).apply{setTextColor(if(dark)0xFF777777.toInt() else 0xFF888888.toInt());textSize=11f}
        val sc=TextView(ctx).apply{setTextColor(if(dark)0xFF777777.toInt() else 0xFF888888.toInt());textSize=11f}
        card.addView(top);card.addView(bo);card.addView(bh);card.addView(me);card.addView(sc)
        card.setOnClickListener{val c=it.tag as? CommentData?;if(c!=null)onToggle(c)}
        return object:RecyclerView.ViewHolder(card){}}
    override fun onBindViewHolder(h:RecyclerView.ViewHolder,p:Int){
        if(getItemViewType(p)==0){val v=h.itemView as TextView;val ph=postHeader;if(ph!=null)v.text="${ph.title}\n\nr/${ph.subreddit} • u/${ph.author} • ${ph.score} pts • ${ph.numComments} comments\n${ph.selftext.take(500)}";return}
        var idx=p;if(postHeader!=null)idx--;if(idx<0||idx>=items.size)return;val c=items[idx];val card=h.itemView as LinearLayout;card.tag=c
        (card.layoutParams as? ViewGroup.MarginLayoutParams)?.leftMargin=c.depth*12.dp
        val top=card.getChildAt(0)as LinearLayout;(top.getChildAt(0)as TextView).text="u/${c.author}"
        (top.getChildAt(1)as TextView).apply{text=c.flair;visibility=if(c.flair.isNotEmpty())View.VISIBLE else View.GONE}
        val bo=card.getChildAt(1)as TextView;val bh=card.getChildAt(2)as TextView
        if(c.collapsed){bo.visibility=View.GONE;bh.visibility=View.VISIBLE;card.getChildAt(3).visibility=View.GONE;card.getChildAt(4).visibility=View.GONE}
        else{bo.text=c.body;bo.visibility=View.VISIBLE;bh.visibility=View.GONE
            val now=System.currentTimeMillis()/1000;val d=now-c.created;val age=when{d<60->"now";d<3600->"${d/60}m";d<86400->"${d/3600}h";else->"${d/86400}d"}
            (card.getChildAt(3)as TextView).text=age+(if(c.isSubmitter)" • OP" else "")+(if(c.distinguished=="moderator")" • MOD" else if(c.distinguished=="admin")" • ADMIN" else "")
            (card.getChildAt(4)as TextView).text="${c.score} pts"+(if(c.gilded>0)" • ⭐${c.gilded}" else "");card.getChildAt(3).visibility=View.VISIBLE;card.getChildAt(4).visibility=View.VISIBLE}}
    override fun getItemCount()=items.size+(if(postHeader!=null)1 else 0)
    fun setData(ph:PostData?,comments:List<CommentData>){postHeader=ph;items.clear();items.addAll(comments);notifyDataSetChanged()}
    private val Int.dp get()=(this*2.8f).toInt()}

// ═══════════════════════════════════════════════════════════════════════════
// MenuAdapter
// ═══════════════════════════════════════════════════════════════════════════
class MenuAdapter(private val onSelect:(MenuItem)->Unit):RecyclerView.Adapter<RecyclerView.ViewHolder>(){
    private val items=mutableListOf<MenuItem>()
    init{items.add(MenuItem(MenuItemType.SHORTCUT,"frontpage","Front Page"));items.add(MenuItem(MenuItemType.SHORTCUT,"popular","Popular"));items.add(MenuItem(MenuItemType.SHORTCUT,"all","All"));items.add(MenuItem(MenuItemType.SHORTCUT,"search","Find Subreddit"));items.add(MenuItem(MenuItemType.USER,"inbox","Inbox"));items.add(MenuItem(MenuItemType.USER,"saved","Saved"));items.add(MenuItem(MenuItemType.USER,"hidden","Hidden"));items.add(MenuItem(MenuItemType.USER,"upvoted","Upvoted"));items.add(MenuItem(MenuItemType.USER,"downvoted","Downvoted"))}
    override fun onCreateViewHolder(p:ViewGroup,t:Int):RecyclerView.ViewHolder{val ctx=p.context;val dark=Theme.isDark;val v=TextView(ctx).apply{setPadding(16.dp,14.dp,16.dp,14.dp);textSize=15f;setTextColor(if(dark)0xFFDDDDDD.toInt() else 0xFF111111.toInt());setBackgroundColor(if(dark)0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt())};return object:RecyclerView.ViewHolder(v){}}
    override fun onBindViewHolder(h:RecyclerView.ViewHolder,p:Int){(h.itemView as TextView).text=items[p].label;h.itemView.setOnClickListener{onSelect(items[p])}}
    override fun getItemCount()=items.size;private val Int.dp get()=(this*2.8f).toInt()}
