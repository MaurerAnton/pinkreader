package org.pinkreader.app

import android.animation.ValueAnimator
import android.app.Activity
import android.content.Intent
import android.graphics.Color
import android.graphics.Typeface
import android.graphics.Rect
import android.graphics.drawable.ColorDrawable
import android.net.Uri
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.text.Editable
import android.text.TextWatcher
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

/**
 * PinkReader - Full Reddit Client
 * Matches RedReader UI: post feed, comment threads, swipe actions, themes
 */
class PinkReader : AppCompatActivity() {

    companion object { init { System.loadLibrary("pinkreader_native") } }

    // Native methods
    private external fun nativeGetVersion(): String
    private external fun nativeInit(cacheDir: String)
    private external fun nativeSetAuth(user: String, token: String)
    private external fun nativeIsAuthenticated(): Boolean
    private external fun nativeGetUsername(): String
    private external fun nativeFetchPosts(sub: String, sort: String, after: String, limit: Int): String
    private external fun nativeFetchComments(sub: String, postId: String, sort: String, limit: Int): String
    private external fun nativeFetchInbox(filter: String, limit: Int): String
    private external fun nativeSearchSubreddits(query: String, limit: Int): String
    private external fun nativeFetchSubredditInfo(name: String): String
    private external fun nativeVote(id: String, dir: Int): Boolean
    private external fun nativeSave(id: String, save: Boolean): Boolean
    private external fun nativeGetCachedPosts(sub: String, sort: String): String
    private external fun nativeClearCache()

    // UI
    private lateinit var toolbar: LinearLayout
    private lateinit var titleText: TextView
    private lateinit var sortSpinner: Spinner
    private lateinit var refreshLayout: SwipeRefreshLayout
    private lateinit var postRecycler: RecyclerView
    private lateinit var commentRecycler: RecyclerView
    private lateinit var contentContainer: FrameLayout
    private lateinit var postAdapter: PostAdapter
    private lateinit var commentAdapter: CommentAdapter
    private lateinit var loadingBar: ProgressBar
    private lateinit var emptyText: TextView
    private lateinit var errorText: TextView
    private lateinit var subredditInput: EditText
    private lateinit var bottomBar: LinearLayout
    private lateinit var searchResults: RecyclerView

    // State
    private var currentSub = "popular"
    private var currentSort = "hot"
    private var after = ""
    private var isLoading = false
    private var isInComments = false
    private var currentPostId = ""
    private var currentPostSub = ""
    private var currentPostTitle = ""
    private val handler = Handler(Looper.getMainLooper())

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        nativeInit(cacheDir.absolutePath)

        // Create full RedReader-like UI
        val root = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL }

        // === Status bar color ===
        window.statusBarColor = PRTheme.statusBar(this)
        window.navigationBarColor = PRTheme.toolbar(this)

        // === Toolbar (matching RedReader) ===
        toolbar = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setBackgroundColor(PRTheme.toolbar(this@PinkReader))
            setPadding(12.dp, 8.dp, 12.dp, 8.dp)
        }

        // Top row: back + title + sort
        val topRow = LinearLayout(this).apply { orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER_VERTICAL }
        val backBtn = TextView(this).apply {
            text = "←"; setTextColor(PRTheme.accent(this@PinkReader)); textSize = 22f
            setPadding(0,0,16.dp,0); setOnClickListener { if(isInComments) hideComments() else finish() }
        }
        titleText = TextView(this).apply {
            setTextColor(PRTheme.text(this@PinkReader)); textSize = 18f
            setTypeface(typeface, Typeface.BOLD); text = "r/popular"
            layoutParams = LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f)
        }
        sortSpinner = Spinner(this).apply {
            adapter = ArrayAdapter(this@PinkReader, android.R.layout.simple_spinner_dropdown_item,
                arrayOf("Hot", "New", "Top", "Rising", "Controversial"))
            setSelection(0); onItemSelectedListener = object : AdapterView.OnItemSelectedListener {
                override fun onItemSelected(p0: AdapterView<*>?, p1: View?, p2: Int, p3: Long) {
                    currentSort = arrayOf("hot","new","top","rising","controversial")[p2]
                    if(!isInComments) { after=""; loadPosts() }
                }
                override fun onNothingSelected(p0: AdapterView<*>?) {}
            }
        }
        topRow.addView(backBtn); topRow.addView(titleText); topRow.addView(sortSpinner)

        // Subreddit input row
        val inputRow = LinearLayout(this).apply { orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER_VERTICAL }
        subredditInput = EditText(this).apply {
            hint = "Go to subreddit..."; setHintTextColor(PRTheme.textSec(this@PinkReader))
            setTextColor(PRTheme.text(this@PinkReader)); textSize = 14f
            setBackgroundColor(PRTheme.cardBg(this@PinkReader)); setPadding(8.dp,4.dp,8.dp,4.dp)
            setSingleLine(); imeOptions = EditorInfo.IME_ACTION_GO
            layoutParams = LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f)
            setOnEditorActionListener { _, actionId, _ ->
                if(actionId==EditorInfo.IME_ACTION_GO) { navigateTo(text.toString().trim()); true } else false
            }
        }
        val goBtn = TextView(this).apply {
            text = "GO"; setTextColor(PRTheme.accent(this@PinkReader)); textSize = 14f
            setTypeface(typeface, Typeface.BOLD); setPadding(12.dp,8.dp,0,8.dp)
            setOnClickListener { navigateTo(subredditInput.text.toString().trim()) }
        }
        inputRow.addView(subredditInput); inputRow.addView(goBtn)
        toolbar.addView(topRow); toolbar.addView(inputRow)
        root.addView(toolbar)

        // === Content area ===
        contentContainer = FrameLayout(this).apply {
            layoutParams = LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f)
        }

        // Loading bar
        loadingBar = ProgressBar(this).apply {
            isIndeterminate = true; visibility = View.GONE
            layoutParams = FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 4.dp).apply { gravity = Gravity.TOP }
        }

        // Refresh layout
        refreshLayout = SwipeRefreshLayout(this).apply {
            setColorSchemeColors(PRTheme.accent(this@PinkReader))
            setOnRefreshListener { after=""; loadPosts() }
        }

        // Post list
        postAdapter = PostAdapter(
            onPostClick = { post -> openComments(post) },
            onSwipeLeft = { post -> downvote(post) },
            onSwipeRight = { post -> upvote(post) }
        )
        postRecycler = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader)
            adapter = postAdapter
            setBackgroundColor(PRTheme.bg(this@PinkReader))
            addItemDecoration(object : RecyclerView.ItemDecoration() {
                override fun getItemOffsets(outRect: Rect, view: View, parent: RecyclerView, state: RecyclerView.State) {
                    outRect.bottom = (2 * resources.displayMetrics.density).toInt()
                }
            })
            addOnScrollListener(object : RecyclerView.OnScrollListener() {
                override fun onScrolled(rv: RecyclerView, dx: Int, dy: Int) {
                    val lm = rv.layoutManager as LinearLayoutManager
                    if(!isLoading && lm.findLastVisibleItemPosition() >= postAdapter.itemCount-3 && after.isNotEmpty())
                        loadMore()
                }
            })
        }
        refreshLayout.addView(postRecycler)

        // Comment list
        commentAdapter = CommentAdapter()
        commentRecycler = RecyclerView(this).apply {
            layoutManager = LinearLayoutManager(this@PinkReader)
            adapter = commentAdapter
            setBackgroundColor(PRTheme.bg(this@PinkReader))
            visibility = View.GONE
        }

        // Empty/error states
        emptyText = TextView(this).apply {
            setTextColor(PRTheme.textSec(this@PinkReader)); textSize = 14f
            gravity = Gravity.CENTER; text = "No posts to display"; visibility = View.GONE
        }
        errorText = TextView(this).apply {
            setTextColor(PRTheme.nsfw(this@PinkReader)); textSize = 14f
            gravity = Gravity.CENTER; visibility = View.GONE; setPadding(32.dp,32.dp,32.dp,32.dp)
        }

        contentContainer.addView(refreshLayout)
        contentContainer.addView(commentRecycler)
        contentContainer.addView(loadingBar)
        contentContainer.addView(emptyText)
        contentContainer.addView(errorText)
        root.addView(contentContainer)

        // === Bottom bar (RedReader style) ===
        bottomBar = LinearLayout(this).apply {
            orientation = LinearLayout.HORIZONTAL; gravity = Gravity.CENTER
            setBackgroundColor(PRTheme.toolbar(this@PinkReader))
            setPadding(0,8.dp,0,12.dp)
        }
        fun addBtn(label: String, icon: String, action: () -> Unit) {
            val btn = LinearLayout(this).apply {
                orientation = LinearLayout.VERTICAL; gravity = Gravity.CENTER
                layoutParams = LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f)
                setOnClickListener { action() }
            }
            val ic = TextView(this).apply {
                text = icon; textSize = 20f; gravity = Gravity.CENTER
                setTextColor(PRTheme.textSec(this@PinkReader))
            }
            val lb = TextView(this).apply {
                text = label; textSize = 10f; gravity = Gravity.CENTER
                setTextColor(PRTheme.textSec(this@PinkReader))
            }
            btn.addView(ic); btn.addView(lb); bottomBar.addView(btn)
        }
        addBtn("Popular","🔥") { navigateTo("popular") }
        addBtn("All","🌐") { navigateTo("all") }
        addBtn("Front","🏠") { navigateTo("") }
        addBtn("Inbox","📬") { openInbox() }
        addBtn("Theme","🎨") { toggleTheme() }
        root.addView(bottomBar)

        setContentView(root)
        loadPosts()
    }

    // ========================================================================
    // Navigation
    // ========================================================================

    private fun navigateTo(sub: String) {
        currentSub = sub.ifEmpty { "frontpage" }
        currentSort = "hot"
        after = ""
        isInComments = false
        titleText.text = if(sub.isEmpty()) "front page" else "r/$sub"
        subredditInput.setText("")
        commentRecycler.visibility = View.GONE
        postRecycler.visibility = View.VISIBLE
        sortSpinner.setSelection(0)
        loadPosts()
    }

    private fun openComments(post: PostCardView.PostData) {
        currentPostId = post.id
        currentPostSub = post.subreddit
        currentPostTitle = post.title
        isInComments = true
        titleText.text = post.title.take(40)
        postRecycler.visibility = View.GONE
        commentRecycler.visibility = View.VISIBLE
        loadComments()
    }

    private fun hideComments() {
        isInComments = false
        titleText.text = "r/$currentSub"
        commentRecycler.visibility = View.GONE
        postRecycler.visibility = View.VISIBLE
    }

    private fun openInbox() {
        thread {
            try {
                val json = nativeFetchInbox("inbox", 25)
                val msgs = JSONArray(json)
                val sb = StringBuilder("=== Inbox ===\n\n")
                for(i in 0 until msgs.length()) {
                    val m = msgs.getJSONObject(i)
                    sb.append("From: u/${m.optString("author","?")}\n")
                    sb.append("Subject: ${m.optString("subject","")}\n")
                    sb.append("${m.optString("body","").take(200)}...\n\n---\n\n")
                }
                handler.post {
                    errorText.text = sb.toString(); errorText.visibility = View.VISIBLE
                    postRecycler.visibility = View.GONE; commentRecycler.visibility = View.GONE
                }
            } catch(ex: Exception) {
                handler.post { showError("Inbox: ${ex.message}") }
            }
        }
    }

    // ========================================================================
    // Data Loading
    // ========================================================================

    private fun loadPosts() {
        if(isLoading) return
        isLoading = true
        loadingBar.visibility = View.VISIBLE
        errorText.visibility = View.GONE
        emptyText.visibility = View.GONE

        thread {
            try {
                // Try cache first
                var json = nativeGetCachedPosts(currentSub, currentSort)
                if(json == "[]" || json.isEmpty()) {
                    json = nativeFetchPosts(currentSub, currentSort, after, 25)
                }
                val arr = JSONArray(json)
                val posts = mutableListOf<PostCardView.PostData>()
                for(i in 0 until arr.length()) {
                    val o = arr.getJSONObject(i)
                    posts.add(PostCardView.PostData(
                        id=o.optString("id"), title=o.optString("title"),
                        author=o.optString("author"), subreddit=o.optString("subreddit"),
                        score=o.optInt("score"), numComments=o.optInt("num_comments"),
                        created=o.optLong("created"), url=o.optString("url"),
                        thumbnail=o.optString("thumbnail"), isSelf=o.optBoolean("is_self"),
                        over18=o.optBoolean("over18"), spoiler=o.optBoolean("spoiler"),
                        stickied=o.optBoolean("stickied"), flair=o.optString("flair"),
                        selftext=o.optString("selftext"), domain=o.optString("domain"),
                        permalink=o.optString("permalink")
                    ))
                }
                handler.post {
                    postAdapter.setData(posts)
                    loadingBar.visibility = View.GONE
                    refreshLayout.isRefreshing = false
                    isLoading = false
                    if(posts.isEmpty()) {
                        emptyText.text = "No posts in r/$currentSub\n\nPull to refresh"
                        emptyText.visibility = View.VISIBLE
                    }
                    // Update after for pagination
                    if(posts.isNotEmpty()) after = posts.last().id
                }
            } catch(ex: Exception) {
                handler.post {
                    showError("Failed to load r/$currentSub\n\n${ex.message}\n\nPull to refresh or check connection.")
                    loadingBar.visibility = View.GONE; refreshLayout.isRefreshing = false; isLoading = false
                }
            }
        }
    }

    private fun loadMore() {
        if(isLoading || !isInComments) return
        isLoading = true
        thread {
            try {
                val json = nativeFetchPosts(currentSub, currentSort, after, 25)
                val arr = JSONArray(json)
                val posts = mutableListOf<PostCardView.PostData>()
                for(i in 0 until arr.length()) {
                    val o = arr.getJSONObject(i)
                    posts.add(PostCardView.PostData(
                        id=o.optString("id"), title=o.optString("title"),
                        author=o.optString("author"), subreddit=o.optString("subreddit"),
                        score=o.optInt("score"), numComments=o.optInt("num_comments"),
                        created=o.optLong("created"), url=o.optString("url"),
                        thumbnail=o.optString("thumbnail"), isSelf=o.optBoolean("is_self"),
                        over18=o.optBoolean("over18"), spoiler=o.optBoolean("spoiler"),
                        stickied=o.optBoolean("stickied"), flair=o.optString("flair"),
                        selftext=o.optString("selftext"), domain=o.optString("domain"),
                        permalink=o.optString("permalink")
                    ))
                }
                handler.post {
                    postAdapter.addData(posts)
                    isLoading = false
                    if(posts.isNotEmpty()) after = posts.last().id
                }
            } catch(ex: Exception) { handler.post { isLoading = false } }
        }
    }

    private fun loadComments() {
        isLoading = true
        loadingBar.visibility = View.VISIBLE
        thread {
            try {
                val json = nativeFetchComments(currentPostSub, currentPostId, "confidence", 200)
                val arr = JSONArray(json)
                val comments = flattenComments(arr, 0)
                handler.post {
                    commentAdapter.setData(comments)
                    loadingBar.visibility = View.GONE; isLoading = false
                    if(comments.isEmpty()) {
                        emptyText.text = "No comments yet"
                        emptyText.visibility = View.VISIBLE
                    }
                }
            } catch(ex: Exception) {
                handler.post {
                    showError("Failed to load comments\n\n${ex.message}")
                    loadingBar.visibility = View.GONE; isLoading = false
                }
            }
        }
    }

    private fun flattenComments(arr: JSONArray, depth: Int): List<CommentView.CommentData> {
        val result = mutableListOf<CommentView.CommentData>()
        for(i in 0 until arr.length()) {
            val o = arr.getJSONObject(i)
            val repliesArr = o.optJSONArray("replies")
            val repliesJson = if(repliesArr != null) repliesArr.toString() else "[]"
            result.add(CommentView.CommentData(
                id=o.optString("id"), author=o.optString("author"),
                body=o.optString("body"), score=o.optInt("score"),
                created=o.optLong("created"), depth=depth,
                stickied=o.optBoolean("stickied"), flair=o.optString("flair"),
                distinguished=o.optString("distinguished"),
                isSubmitter=o.optBoolean("is_submitter"),
                likes=o.optInt("likes"), replies=repliesJson
            ))
            if(repliesArr != null) result.addAll(flattenComments(repliesArr, depth+1))
        }
        return result
    }

    // ========================================================================
    // Actions
    // ========================================================================

    private fun upvote(post: PostCardView.PostData) {
        thread {
            val ok = nativeVote(post.id, 1)
            handler.post { if(ok) toast("↑ Upvoted!") }
        }
    }

    private fun downvote(post: PostCardView.PostData) {
        thread {
            val ok = nativeVote(post.id, -1)
            handler.post { if(ok) toast("↓ Downvoted") }
        }
    }

    private fun toggleTheme() {
        PRTheme.isDark = !PRTheme.isDark
        recreate()
    }

    private fun showError(msg: String) {
        errorText.text = msg; errorText.visibility = View.VISIBLE
        postRecycler.visibility = if(isInComments) View.GONE else View.VISIBLE
        commentRecycler.visibility = if(isInComments) View.VISIBLE else View.GONE
        emptyText.visibility = View.GONE
    }

    private fun toast(msg: String) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show()
    }

    private val Int.dp get() = (this * resources.displayMetrics.density).toInt()
}
