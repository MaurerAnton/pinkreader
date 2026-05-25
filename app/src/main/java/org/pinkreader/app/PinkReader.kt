package org.pinkreader.app

import android.app.AlertDialog
import android.content.Intent
import android.graphics.*
import android.os.Bundle
import android.os.Handler
import android.os.Looper
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
    companion object { init { System.loadLibrary("pinkreader_native") } }

    private external fun nativeInit(cd: String); private external fun nativeGetVersion(): String
    private external fun nativeSetAuth(u: String, t: String); private external fun nativeIsAuthenticated(): Boolean
    private external fun nativeGetUsername(): String
    private external fun nativeFetchPosts(sub: String, sort: String, after: String, limit: Int): String
    private external fun nativeFetchPostsFull(sub: String, sort: String, after: String, limit: Int): String
    private external fun nativeFetchComments(sub: String, pid: String, sort: String, limit: Int): String
    private external fun nativeFetchPost(sub: String, pid: String): String
    private external fun nativeFetchInbox(filter: String, limit: Int): String
    private external fun nativeSearchSubreddits(query: String, limit: Int): String
    private external fun nativeVote(id: String, dir: Int): Boolean
    private external fun nativeSave(id: String, s: Boolean): Boolean
    private external fun nativeHide(id: String, h: Boolean): Boolean
    private external fun nativeDelete(id: String): Boolean
    private external fun nativeGetCachedPosts(sub: String, sort: String): String
    private external fun nativeCachePosts(sub: String, sort: String, data: String)
    private external fun nativeClearCache()
    private external fun nativeSetDarkTheme(d: Boolean); private external fun nativeGetDarkTheme(): Boolean
    private external fun nativeSetNsfw(n: Boolean); private external fun nativeGetNsfw(): Boolean
    private external fun nativeMarkRead(id: String); private external fun nativeIsRead(id: String): Boolean

    private var currentSub = "popular"; private var currentSort = "hot"; private var after = ""
    private var lastAfter = ""; private var isLoading = false
    private var isInComments = false; private var isInMenu = true
    private var currentPostId = ""; private var currentPostSub = ""
    private val postIds = HashSet<String>(); private var postRefreshCount = -1
    private val handler = Handler(Looper.getMainLooper())

    private lateinit var root: FrameLayout; private lateinit var toolbar: LinearLayout
    private lateinit var titleText: TextView; private lateinit var sortSpinner: Spinner
    private lateinit var refresh: SwipeRefreshLayout; private lateinit var postList: RecyclerView
    private lateinit var commentList: RecyclerView; private lateinit var menuList: RecyclerView
    private lateinit var loadingBar: ProgressBar; private lateinit var emptyText: TextView
    private lateinit var errorText: TextView; private lateinit var subInput: EditText
    private lateinit var bottomBar: LinearLayout
    private lateinit var postAdapter: PostAdapter; private lateinit var commentAdapter: CommentAdapter
    private lateinit var menuAdapter: MenuAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState); nativeInit(cacheDir.absolutePath)
        Theme.isDark = nativeGetDarkTheme()
        window.statusBarColor = if(Theme.isDark) 0xFF0D0D0D.toInt() else 0xFFE0E0E0.toInt()
        window.navigationBarColor = Theme.toolbarBg(this@PinkReader)

        root = FrameLayout(this).apply { setBackgroundColor(Theme.bg(this@PinkReader)) }
        buildToolbar(); buildContent(); setContentView(root)
        if(savedInstanceState == null) showMainMenu()
    }

    private fun buildToolbar() {
        toolbar = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL; setBackgroundColor(Theme.toolbarBg(this@PinkReader))
            setPadding(12.dp, 8.dp, 12.dp, 8.dp); visibility = View.GONE
        }
        val top = LinearLayout(this).apply { orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER_VERTICAL }
        top.addView(TextView(this).apply { text="←"; setTextColor(Theme.accent()); textSize=22f; setPadding(0,0,12.dp,0)
            setOnClickListener { if(isInComments) hideComments() else if(!isInMenu) showMainMenu() } })
        titleText = TextView(this).apply {
            setTextColor(Theme.txt(this@PinkReader)); textSize=18f; setTypeface(typeface,Typeface.BOLD)
            layoutParams = LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
        }
        sortSpinner = Spinner(this).apply {
            adapter = ArrayAdapter(this@PinkReader,android.R.layout.simple_spinner_dropdown_item, arrayOf("Hot","New","Top","Rising","Controversial","Best"))
            onItemSelectedListener = object:AdapterView.OnItemSelectedListener{
                override fun onItemSelected(p0:AdapterView<*>?,p1:View?,p2:Int,p3:Long){
                    currentSort=arrayOf("hot","new","top","rising","controversial","confidence")[p2]
                    if(!isInComments&&!isInMenu){after="";lastAfter="";postIds.clear();postRefreshCount=-1;loadPosts()}}
                override fun onNothingSelected(p0:AdapterView<*>?){}}}
        top.addView(titleText); top.addView(sortSpinner)
        top.addView(TextView(this).apply { text="☰";setTextColor(Theme.accent());textSize=22f;setPadding(12.dp,0,0,0);setOnClickListener{showMainMenu()} })
        toolbar.addView(top)
        val ir = LinearLayout(this).apply{orientation=LinearLayout.HORIZONTAL;gravity=Gravity.CENTER_VERTICAL}
        subInput = EditText(this).apply { hint="Go to subreddit...";setHintTextColor(Theme.sec(this@PinkReader));setTextColor(Theme.txt(this@PinkReader));textSize=14f
            setBackgroundColor(Theme.card(this@PinkReader));setPadding(8.dp,4.dp,8.dp,4.dp);setSingleLine()
            imeOptions=EditorInfo.IME_ACTION_GO;layoutParams=LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)
            setOnEditorActionListener{_,a,_->if(a==EditorInfo.IME_ACTION_GO){navigate(text.toString().trim());true}else false}}
        ir.addView(subInput)
        ir.addView(TextView(this).apply{text="GO";setTextColor(Theme.accent());setTypeface(typeface,Typeface.BOLD);setPadding(12.dp,8.dp,0,8.dp);setOnClickListener{navigate(subInput.text.toString().trim())}})
        toolbar.addView(ir)
        root.addView(toolbar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT).apply{gravity=Gravity.TOP})
    }

    private fun buildContent() {
        loadingBar = ProgressBar(this).apply{isIndeterminate=true;visibility=View.GONE}
        refresh = SwipeRefreshLayout(this).apply{setColorSchemeColors(Theme.accent());setOnRefreshListener{after="";lastAfter="";postIds.clear();loadPosts()}}

        postAdapter = PostAdapter({p->nativeMarkRead(p.id);openComments(p)},{p->thread{nativeVote("t3_"+p.id,1)};toast("↑Upvoted")},{p->thread{nativeVote("t3_"+p.id,-1)};toast("↓Downvoted")})
        postList = RecyclerView(this).apply{
            layoutManager=LinearLayoutManager(this@PinkReader);adapter=postAdapter;setBackgroundColor(Theme.bg(this@PinkReader))
            addItemDecoration(object:RecyclerView.ItemDecoration(){override fun getItemOffsets(o:Rect,v:View,p:RecyclerView,s:RecyclerView.State){o.bottom=2.dp}})
            addOnScrollListener(object:RecyclerView.OnScrollListener(){override fun onScrolled(rv:RecyclerView,dx:Int,dy:Int){
                val lm=rv.layoutManager as LinearLayoutManager
                if(!isLoading&&after.isNotEmpty()&&after!=lastAfter&&lm.findLastVisibleItemPosition()>=postAdapter.itemCount-5)loadMore()}})}
        refresh.addView(postList)

        commentAdapter = CommentAdapter{c->c.collapsed=!c.collapsed;commentAdapter.notifyDataSetChanged()}
        commentList = RecyclerView(this).apply{layoutManager=LinearLayoutManager(this@PinkReader);adapter=commentAdapter;setBackgroundColor(Theme.bg(this@PinkReader));visibility=View.GONE}

        menuAdapter = MenuAdapter{i->onMenuSelected(i)}
        menuList = RecyclerView(this).apply{layoutManager=LinearLayoutManager(this@PinkReader);adapter=menuAdapter;setBackgroundColor(Theme.bg(this@PinkReader))}

        emptyText = TextView(this).apply{setTextColor(Theme.sec(this@PinkReader));textSize=14f;gravity=Gravity.CENTER;visibility=View.GONE;setPadding(32.dp,64.dp,32.dp,32.dp)}
        errorText = TextView(this).apply{setTextColor(0xFFFF5252.toInt());textSize=14f;gravity=Gravity.CENTER;visibility=View.GONE;setPadding(32.dp,32.dp,32.dp,32.dp)}

        root.addView(menuList, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(refresh, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(commentList, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(loadingBar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,4.dp).apply{gravity=Gravity.TOP})
        root.addView(emptyText, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))
        root.addView(errorText, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.MATCH_PARENT))

        bottomBar = LinearLayout(this).apply{orientation=LinearLayout.HORIZONTAL;gravity=Gravity.CENTER;setBackgroundColor(Theme.toolbarBg(this@PinkReader));setPadding(0,6.dp,0,10.dp)
            fun add(icon:String,label:String,a:()->Unit){val b=LinearLayout(this@PinkReader).apply{orientation=LinearLayout.VERTICAL;gravity=Gravity.CENTER;layoutParams=LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f);setOnClickListener{a()}}
                b.addView(TextView(this@PinkReader).apply{text=icon;textSize=18f;gravity=Gravity.CENTER;setTextColor(Theme.sec(this@PinkReader))})
                b.addView(TextView(this@PinkReader).apply{text=label;textSize=10f;gravity=Gravity.CENTER;setTextColor(Theme.sec(this@PinkReader))});addView(b)}
            add("🔥","Popular"){navigate("popular")};add("🌐","All"){navigate("all")}
            add("🏠","Front"){navigate("")};add("📬","Inbox"){showInbox()}
            add("🎨","Theme"){Theme.isDark=!Theme.isDark;nativeSetDarkTheme(Theme.isDark);recreate()};add("⚙","Settings"){startActivity(Intent(this@PinkReader,PinkReaderSettings::class.java))}}
        root.addView(bottomBar, FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,ViewGroup.LayoutParams.WRAP_CONTENT).apply{gravity=Gravity.BOTTOM})
    }

    private fun showMainMenu() { isInMenu=true;isInComments=false;toolbar.visibility=View.GONE;menuList.visibility=View.VISIBLE;postList.visibility=View.GONE;commentList.visibility=View.GONE;errorText.visibility=View.GONE;emptyText.visibility=View.GONE }
    private fun navigate(sub:String) { currentSub=sub.ifEmpty{"frontpage"};currentSort="hot";after="";lastAfter="";postIds.clear();postRefreshCount=-1;isInMenu=false;isInComments=false;sortSpinner.setSelection(0);titleText.text=if(sub.isEmpty())"front page" else "r/$sub";subInput.setText("");toolbar.visibility=View.VISIBLE;menuList.visibility=View.GONE;commentList.visibility=View.GONE;postList.visibility=View.VISIBLE;errorText.visibility=View.GONE;emptyText.visibility=View.GONE;loadPosts() }
    private fun openComments(p:PostData){currentPostId=p.id;currentPostSub=p.subreddit;isInComments=true;isInMenu=false;titleText.text=p.title.take(40);toolbar.visibility=View.VISIBLE;postList.visibility=View.GONE;commentList.visibility=View.VISIBLE;menuList.visibility=View.GONE;errorText.visibility=View.GONE;emptyText.visibility=View.GONE;loadComments()}
    private fun hideComments(){isInComments=false;titleText.text="r/$currentSub";commentList.visibility=View.GONE;postList.visibility=View.VISIBLE}
    private fun showInbox(){toolbar.visibility=View.VISIBLE;titleText.text="Inbox";postList.visibility=View.GONE;commentList.visibility=View.GONE;menuList.visibility=View.GONE
        thread{try{val msgs=JSONArray(nativeFetchInbox("inbox",25));val sb=StringBuilder("=== Inbox ===\n\n")
            for(i in 0 until msgs.length()){val m=msgs.getJSONObject(i);sb.append("From: u/${m.optString("author","?")}\nSubject: ${m.optString("subject","")}\n${m.optString("body","").take(300)}\n\n---\n\n")}
            handler.post{errorText.text=sb.toString();errorText.visibility=View.VISIBLE}}catch(ex:Exception){handler.post{showError("Inbox: ${ex.message}")}}}}

    private fun onMenuSelected(item:MenuItem){when(item.type){
        MenuItemType.SHORTCUT->when(item.id){"frontpage"->navigate("");"popular"->navigate("popular");"all"->navigate("all");"search"->showSearchDialog()}
        MenuItemType.USER->when(item.id){"inbox"->showInbox()}
        else->{}}}

    private fun showSearchDialog(){val input=EditText(this).apply{hint="Search subreddits...";setSingleLine()}
        AlertDialog.Builder(this).setTitle("Find Subreddit").setView(input).setPositiveButton("Search"){_,_->thread{try{val subs=JSONArray(nativeSearchSubreddits(input.text.toString(),25));val sb=StringBuilder("Results:\n\n")
            for(i in 0 until subs.length()){val s=subs.getJSONObject(i);sb.append("r/${s.optString("name")} - ${s.optString("title","")} (${s.optInt("subscribers",0)} subs)\n\n")}
            handler.post{errorText.text=sb.toString();errorText.visibility=View.VISIBLE;menuList.visibility=View.GONE}}catch(ex:Exception){handler.post{toast(ex.message?:"Search failed")}}}}.setNegativeButton("Cancel",null).show()}

    private fun loadPosts(){if(isLoading)return;isLoading=true;loadingBar.visibility=View.VISIBLE;errorText.visibility=View.GONE;emptyText.visibility=View.GONE
        thread{try{var json=nativeGetCachedPosts(currentSub,currentSort)
            if(json=="[]"){json=nativeFetchPosts(currentSub,currentSort,after,25);nativeCachePosts(currentSub,currentSort,json)}
            val arr=JSONArray(json);val posts=mutableListOf<PostData>()
            for(i in 0 until arr.length()){val o=arr.getJSONObject(i);val id=o.optString("id");if(postIds.contains(id))continue;postIds.add(id)
                posts.add(parsePost(o));if(postRefreshCount>0)postRefreshCount--;if(posts.isNotEmpty())after="t3_"+posts.last().id}
            handler.post{postAdapter.setData(posts);loadingBar.visibility=View.GONE;refresh.isRefreshing=false;isLoading=false
                if(posts.isEmpty()&&postAdapter.itemCount==0){emptyText.text="No posts\nPull to refresh";emptyText.visibility=View.VISIBLE}}}
        catch(ex:Exception){handler.post{showError("Load failed: ${ex.message}");loadingBar.visibility=View.GONE;refresh.isRefreshing=false;isLoading=false}}}}

    private fun loadMore(){if(isLoading||after.isEmpty()||after==lastAfter)return;lastAfter=after;isLoading=true
        thread{try{val json=nativeFetchPosts(currentSub,currentSort,after,25);val arr=JSONArray(json);val posts=mutableListOf<PostData>()
            for(i in 0 until arr.length()){val o=arr.getJSONObject(i);val id=o.optString("id");if(postIds.contains(id))continue;postIds.add(id)
                posts.add(parsePost(o));if(postRefreshCount>0)postRefreshCount--;if(posts.isNotEmpty())after="t3_"+posts.last().id}
            handler.post{postAdapter.addData(posts);isLoading=false}}catch(ex:Exception){handler.post{isLoading=false}}}}

    private fun loadComments(){isLoading=true;loadingBar.visibility=View.VISIBLE
        thread{try{val postJson=JSONObject(nativeFetchPost(currentPostSub,currentPostId));val ph=parsePost(postJson)
            val commentJson=nativeFetchComments(currentPostSub,currentPostId,"confidence",300);val arr=JSONArray(commentJson);val comments=flattenComments(arr,0)
            handler.post{commentAdapter.setData(ph,comments);loadingBar.visibility=View.GONE;isLoading=false
                if(comments.isEmpty()){emptyText.text="No comments yet";emptyText.visibility=View.VISIBLE}}}
        catch(ex:Exception){handler.post{showError("Comments failed: ${ex.message}");loadingBar.visibility=View.GONE;isLoading=false}}}}

    private fun parsePost(o:JSONObject)=PostData(id=o.optString("id"),name=o.optString("name"),title=o.optString("title"),author=o.optString("author"),subreddit=o.optString("subreddit"),score=o.optInt("score"),numComments=o.optInt("num_comments"),created=o.optLong("created"),url=o.optString("url"),thumbnail=o.optString("thumbnail"),isSelf=o.optBoolean("is_self"),over18=o.optBoolean("over18"),spoiler=o.optBoolean("spoiler"),stickied=o.optBoolean("stickied"),flair=o.optString("flair"),selftext=o.optString("selftext"),domain=o.optString("domain"),permalink=o.optString("permalink"),likes=o.optInt("likes"),saved=o.optBoolean("saved"),distinguished=o.optString("distinguished"),postHint=o.optString("post_hint"),previewUrl=o.optString("preview_url"),upvoteRatio=o.optDouble("upvote_ratio"),gilded=o.optInt("gilded"),locked=o.optBoolean("locked"),archived=o.optBoolean("archived"))
    private fun flattenComments(arr:JSONArray,depth:Int):List<CommentData>{val r=mutableListOf<CommentData>()
        for(i in 0 until arr.length()){val o=arr.getJSONObject(i);val repliesArr=o.optJSONArray("replies")
            r.add(CommentData(id=o.optString("id"),author=o.optString("author"),body=o.optString("body"),bodyHtml=o.optString("body_html"),score=o.optInt("score"),created=o.optLong("created"),depth=depth,stickied=o.optBoolean("stickied"),flair=o.optString("flair"),distinguished=o.optString("distinguished"),isSubmitter=o.optBoolean("is_submitter"),likes=o.optInt("likes"),gilded=o.optInt("gilded"),collapsed=false))
            if(repliesArr!=null)r.addAll(flattenComments(repliesArr,depth+1))}
        return r}
    private fun showError(msg:String){errorText.text=msg;errorText.visibility=View.VISIBLE}
    private fun toast(msg:String){Toast.makeText(this,msg,Toast.LENGTH_SHORT).show()}
    private val Int.dp get()=(this*resources.displayMetrics.density).toInt()
    override fun onBackPressed(){if(isInComments)hideComments()else if(!isInMenu)showMainMenu()else super.onBackPressed()}
}

// Data classes
data class PostData(val id:String,val name:String,val title:String,val author:String,val subreddit:String,val score:Int,val numComments:Int,val created:Long,val url:String,val thumbnail:String,val isSelf:Boolean,val over18:Boolean,val spoiler:Boolean,val stickied:Boolean,val flair:String,val selftext:String,val domain:String,val permalink:String,val likes:Int,val saved:Boolean,val distinguished:String,val postHint:String,val previewUrl:String,val upvoteRatio:Double,val gilded:Int,val locked:Boolean,val archived:Boolean)
data class CommentData(val id:String,val author:String,val body:String,val bodyHtml:String,val score:Int,val created:Long,val depth:Int,val stickied:Boolean,val flair:String,val distinguished:String,val isSubmitter:Boolean,val likes:Int,val gilded:Int,var collapsed:Boolean=false)
enum class MenuItemType { SHORTCUT, USER, PINNED, SUBREDDIT }
data class MenuItem(val type:MenuItemType,val id:String,val label:String)

// PostAdapter
class PostAdapter(private val onClick:(PostData)->Unit,private val onSwipeR:(PostData)->Unit,private val onSwipeL:(PostData)->Unit):RecyclerView.Adapter<PostAdapter.VH>(){
    private val posts=mutableListOf<PostData>()
    class VH(val v:View):RecyclerView.ViewHolder(v){var sx=0f;var sd=false}
    override fun onCreateViewHolder(p:ViewGroup,t:Int):VH{val ctx=p.context
        val card=FrameLayout(ctx).apply{setBackgroundColor(Theme.card(ctx));setPadding(12.dp,10.dp,12.dp,10.dp)}
        val inner=LinearLayout(ctx).apply{orientation=LinearLayout.VERTICAL}
        val tags=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val st=TextView(ctx).apply{text="📌";setTextColor(0xFF2E7D32.toInt());textSize=12f;visibility=View.GONE}
        val nf=TextView(ctx).apply{text="NSFW";setTextColor(Color.WHITE);textSize=10f;setPadding(4.dp,1,4.dp,1);setBackgroundColor(0xFFFF5252.toInt());visibility=View.GONE}
        val fl=TextView(ctx).apply{setTextColor(Theme.accent());textSize=11f;setPadding(4.dp,2,4.dp,2);visibility=View.GONE}
        tags.addView(st);tags.addView(nf);tags.addView(fl)
        val tr=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val ti=TextView(ctx).apply{setTextColor(Theme.txt(ctx));textSize=15f;setLineSpacing(2f,1f);maxLines=4;setTypeface(null,Typeface.BOLD);layoutParams=LinearLayout.LayoutParams(0,ViewGroup.LayoutParams.WRAP_CONTENT,1f)}
        val th=ImageView(ctx).apply{layoutParams=FrameLayout.LayoutParams(64.dp,64.dp).apply{gravity=Gravity.END or Gravity.TOP};scaleType=ImageView.ScaleType.CENTER_CROP;visibility=View.GONE}
        tr.addView(ti);tr.addView(th)
        val se=TextView(ctx).apply{setTextColor(Theme.sec(ctx));textSize=13f;maxLines=3;visibility=View.GONE}
        val me=TextView(ctx).apply{setTextColor(Theme.sec(ctx));textSize=12f}
        val co=TextView(ctx).apply{setTextColor(Theme.accent());textSize=12f}
        inner.addView(tags);inner.addView(tr);inner.addView(se);inner.addView(me);inner.addView(co);card.addView(inner)
        card.setOnTouchListener{v,e->val vh=v.tag as? VH?:return@setOnTouchListener false
            when(e.action){MotionEvent.ACTION_DOWN->{vh.sx=e.x;vh.sd=false;false}MotionEvent.ACTION_UP->{val dx=e.x-vh.sx;if(abs(dx)>100*ctx.resources.displayMetrics.density&&!vh.sd){vh.sd=true;if(dx>0)onSwipeR(posts[vh.adapterPosition])else onSwipeL(posts[vh.adapterPosition]);true}else false}else->false}}
        val vh=VH(card);card.tag=vh;return vh}
    override fun onBindViewHolder(h:VH,p:Int){val post=posts[p];val card=h.v as FrameLayout;val inner=card.getChildAt(0)as LinearLayout
        val tags=inner.getChildAt(0)as LinearLayout;(tags.getChildAt(0)as TextView).visibility=if(post.stickied)View.VISIBLE else View.GONE
        (tags.getChildAt(1)as TextView).visibility=if(post.over18)View.VISIBLE else View.GONE
        (tags.getChildAt(2)as TextView).apply{text=post.flair;visibility=if(post.flair.isNotEmpty())View.VISIBLE else View.GONE}
        val tr=inner.getChildAt(1)as LinearLayout;(tr.getChildAt(0)as TextView).text=post.title
        (tr.getChildAt(1)as ImageView).visibility=if(post.thumbnail.isNotEmpty()&&post.thumbnail!="self"&&post.thumbnail!="nsfw"&&post.thumbnail!="default")View.VISIBLE else View.GONE
        (inner.getChildAt(2)as TextView).apply{text=post.selftext.take(200);visibility=if(post.selftext.isNotEmpty())View.VISIBLE else View.GONE}
        val now=System.currentTimeMillis()/1000;val diff=now-post.created;val age=when{diff<60->"now";diff<3600->"${diff/60}m";diff<86400->"${diff/3600}h";diff<2592000->"${diff/86400}d";diff<31536000->"${diff/2592000}mo";else->"${diff/31536000}y"}
        (inner.getChildAt(3)as TextView).text="r/${post.subreddit} • u/${post.author} • $age"+(if(post.distinguished.isNotEmpty())" • ${post.distinguished.uppercase()}" else "")
        (inner.getChildAt(4)as TextView).text=if(post.numComments>0)"💬 ${post.numComments} comments" else "💬 discuss";card.setOnClickListener{onClick(post)}}
    override fun getItemCount()=posts.size
    fun setData(d:List<PostData>){posts.clear();posts.addAll(d);notifyDataSetChanged()}
    fun addData(d:List<PostData>){val s=posts.size;posts.addAll(d);notifyItemRangeInserted(s,d.size)}
    private val Int.dp get()=(this*2.5f).toInt()
}

// CommentAdapter
class CommentAdapter(private val onToggle:(CommentData)->Unit):RecyclerView.Adapter<RecyclerView.ViewHolder>(){
    private var postHeader:PostData?=null;private val items=mutableListOf<CommentData>()
    override fun getItemViewType(p:Int)=if(p==0&&postHeader!=null)0 else 1
    override fun onCreateViewHolder(p:ViewGroup,t:Int):RecyclerView.ViewHolder{val ctx=p.context
        if(t==0){val v=TextView(ctx).apply{setTextColor(Theme.txt(ctx));textSize=16f;setTypeface(null,Typeface.BOLD);setPadding(16.dp,16.dp,16.dp,8.dp);setBackgroundColor(Theme.card(ctx))};return object:RecyclerView.ViewHolder(v){}}
        val card=LinearLayout(ctx).apply{orientation=LinearLayout.VERTICAL;setBackgroundColor(Theme.card(ctx));setPadding(8.dp,6,8.dp,6)}
        val top=LinearLayout(ctx).apply{orientation=LinearLayout.HORIZONTAL}
        val au=TextView(ctx).apply{setTextColor(Theme.accent());textSize=13f;setTypeface(null,Typeface.BOLD)}
        val fl=TextView(ctx).apply{setTextColor(Theme.sec(ctx));textSize=10f;setPadding(6.dp,0,0,0);visibility=View.GONE}
        top.addView(au);top.addView(fl)
        val bo=TextView(ctx).apply{setTextColor(Theme.txt(ctx));textSize=14f;setLineSpacing(2f,1f)}
        val bh=TextView(ctx).apply{setTextColor(Theme.sec(ctx));textSize=14f;text="[collapsed]";visibility=View.GONE}
        val me=TextView(ctx).apply{setTextColor(Theme.sec(ctx));textSize=11f}
        val sc=TextView(ctx).apply{setTextColor(Theme.sec(ctx));textSize=11f}
        card.addView(top);card.addView(bo);card.addView(bh);card.addView(me);card.addView(sc)
        card.setOnClickListener{val c=it.tag as? CommentData?;if(c!=null)onToggle(c)}
        return object:RecyclerView.ViewHolder(card){}}
    override fun onBindViewHolder(h:RecyclerView.ViewHolder,p:Int){
        if(getItemViewType(p)==0){val v=h.itemView as TextView;val ph=postHeader!!;v.text="${ph.title}\n\nr/${ph.subreddit} • u/${ph.author} • ${ph.score} pts • ${ph.numComments} comments\n${ph.selftext.take(500)}";return}
        var idx=p;if(postHeader!=null)idx--;val c=items[idx];val card=h.itemView as LinearLayout;card.tag=c
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
    fun setData(ph:PostData,comments:List<CommentData>){postHeader=ph;items.clear();items.addAll(comments);notifyDataSetChanged()}
    private val Int.dp get()=(this*2.5f).toInt()
}

// MenuAdapter
class MenuAdapter(private val onSelect:(MenuItem)->Unit):RecyclerView.Adapter<RecyclerView.ViewHolder>(){
    private val items=mutableListOf<MenuItem>()
    init{items.add(MenuItem(MenuItemType.SHORTCUT,"frontpage","📋  Front Page"))
        items.add(MenuItem(MenuItemType.SHORTCUT,"popular","🔥  Popular"))
        items.add(MenuItem(MenuItemType.SHORTCUT,"all","🌐  All"))
        items.add(MenuItem(MenuItemType.SHORTCUT,"search","🔍  Find Subreddit"))
        items.add(MenuItem(MenuItemType.USER,"inbox","📬  Inbox"))
        items.add(MenuItem(MenuItemType.USER,"saved","⭐  Saved"))
        items.add(MenuItem(MenuItemType.USER,"hidden","🙈  Hidden"))
        items.add(MenuItem(MenuItemType.USER,"upvoted","👍  Upvoted"))
        items.add(MenuItem(MenuItemType.USER,"downvoted","👎  Downvoted"))}
    override fun onCreateViewHolder(p:ViewGroup,t:Int):RecyclerView.ViewHolder{val ctx=p.context;val v=TextView(ctx).apply{setPadding(16.dp,14.dp,16.dp,14.dp);textSize=15f;setTextColor(Theme.txt(ctx));setBackgroundColor(Theme.card(ctx))};return object:RecyclerView.ViewHolder(v){}}
    override fun onBindViewHolder(h:RecyclerView.ViewHolder,p:Int){(h.itemView as TextView).text=items[p].label;h.itemView.setOnClickListener{onSelect(items[p])}}
    override fun getItemCount()=items.size
    private val Int.dp get()=(this*2.5f).toInt()
}
