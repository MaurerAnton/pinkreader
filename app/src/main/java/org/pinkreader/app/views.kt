package org.pinkreader.app

import android.animation.ValueAnimator
import android.content.Context
import android.content.res.ColorStateList
import android.graphics.*
import android.graphics.drawable.Drawable
import android.text.SpannableStringBuilder
import android.text.style.ForegroundColorSpan
import android.text.style.StyleSpan
import android.util.TypedValue
import android.view.Gravity
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import android.view.animation.DecelerateInterpolator
import android.widget.*
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.RecyclerView
import org.json.JSONObject
import kotlin.math.abs
import kotlin.math.min

// ============================================================================
// Theme colors - matching RedReader
// ============================================================================

object PRTheme {
    var isDark = true

    // Dark theme (default, matching RedReader night mode)
    fun bg(ctx: Context) = if(isDark) 0xFF1A1A1A.toInt() else 0xFFFAFAFA.toInt()
    fun cardBg(ctx: Context) = if(isDark) 0xFF252525.toInt() else 0xFFFFFFFF.toInt()
    fun text(ctx: Context) = if(isDark) 0xFFD0D0D0.toInt() else 0xFF212121.toInt()
    fun textSec(ctx: Context) = if(isDark) 0xFF888888.toInt() else 0xFF757575.toInt()
    fun accent(ctx: Context) = 0xFFFF7597.toInt() // PinkReader pink
    fun upvote(ctx: Context) = 0xFFFF8B60.toInt()
    fun downvote(ctx: Context) = 0xFF9494FF.toInt()
    fun nsfw(ctx: Context) = 0xFFFF5252.toInt()
    fun sticky(ctx: Context) = 0xFF2E7D32.toInt()
    fun divider(ctx: Context) = if(isDark) 0xFF333333.toInt() else 0xFFE0E0E0.toInt()
    fun toolbar(ctx: Context) = if(isDark) 0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt()
    fun statusBar(ctx: Context) = if(isDark) 0xFF0D0D0D.toInt() else 0xFFE0E0E0.toInt()
}

// ============================================================================
// RedReader-style Post Card
// ============================================================================

class PostCardView(ctx: Context) : FrameLayout(ctx) {
    private val title = TextView(ctx).apply {
        setTextColor(PRTheme.text(ctx)); textSize = 15f
        setLineSpacing(2f, 1f); maxLines = 5
        setTypeface(typeface, Typeface.BOLD)
    }
    private val meta = TextView(ctx).apply {
        setTextColor(PRTheme.textSec(ctx)); textSize = 12f
    }
    private val flair = TextView(ctx).apply {
        setTextColor(PRTheme.accent(ctx)); textSize = 11f
        setPadding(4,2,4,2); visibility = GONE
    }
    private val thumbnail = ImageView(ctx).apply {
        layoutParams = LayoutParams(72.dp, 72.dp).apply { gravity = Gravity.END or Gravity.TOP }
        scaleType = ImageView.ScaleType.CENTER_CROP
    }
    private val nsfwTag = TextView(ctx).apply {
        text = "NSFW"; setTextColor(Color.WHITE); textSize = 10f
        setPadding(4,1,4,1); setBackgroundColor(PRTheme.nsfw(ctx))
        visibility = GONE
    }
    private val stickyTag = TextView(ctx).apply {
        text = "📌"; setTextColor(PRTheme.sticky(ctx)); textSize = 12f
        visibility = GONE
    }
    private val selftext = TextView(ctx).apply {
        setTextColor(PRTheme.textSec(ctx)); textSize = 13f
        maxLines = 3; visibility = GONE
    }
    private val comments = TextView(ctx).apply {
        setTextColor(PRTheme.accent(ctx)); textSize = 12f
    }
    data class PostData(
        val id: String, val title: String, val author: String, val subreddit: String,
        val score: Int, val numComments: Int, val created: Long, val url: String,
        val thumbnail: String, val isSelf: Boolean, val over18: Boolean,
        val spoiler: Boolean, val stickied: Boolean, val flair: String,
        val selftext: String, val domain: String, val permalink: String
    )

    init {
        setBackgroundColor(PRTheme.cardBg(ctx))
        setPadding(12.dp, 10.dp, 12.dp, 10.dp)

        val inner = LinearLayout(ctx).apply { orientation = LinearLayout.VERTICAL }
        val topRow = LinearLayout(ctx).apply { orientation = LinearLayout.HORIZONTAL }
        val tags = LinearLayout(ctx).apply {
            orientation = LinearLayout.HORIZONTAL
            (stickyTag.layoutParams as MarginLayoutParams).rightMargin = 4.dp
            (nsfwTag.layoutParams as MarginLayoutParams).rightMargin = 4.dp
            addView(stickyTag); addView(nsfwTag); addView(flair)
        }
        val titleRow = LinearLayout(ctx).apply {
            orientation = LinearLayout.HORIZONTAL
            val lp = LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f)
            title.layoutParams = lp
            addView(title); addView(thumbnail)
        }
        inner.addView(tags)
        inner.addView(titleRow)
        inner.addView(selftext)
        inner.addView(meta)
        inner.addView(comments)
        addView(inner)
    }

    fun bind(post: PostData, position: Int) {
        title.text = post.title
        selftext.text = post.selftext.take(200)
        selftext.visibility = if(post.selftext.isNotEmpty() && post.isSelf) VISIBLE else GONE

        val age = ageString(post.created)
        meta.text = "r/${post.subreddit} • u/${post.author} • $age • ${post.domain}"
        comments.text = if(post.numComments > 0) "💬 ${post.numComments} comments" else "💬 no comments"
        comments.visibility = if(post.numComments > 0) VISIBLE else GONE

        nsfwTag.visibility = if(post.over18) VISIBLE else GONE
        stickyTag.visibility = if(post.stickied) VISIBLE else GONE
        flair.visibility = if(post.flair.isNotEmpty()) VISIBLE else GONE
        flair.text = post.flair

        thumbnail.visibility = if(post.thumbnail.isNotEmpty() && post.thumbnail != "self"
            && post.thumbnail != "nsfw" && post.thumbnail != "default") VISIBLE else GONE
    }

    private fun ageString(utc: Long): String {
        val now = System.currentTimeMillis()/1000
        val diff = now - utc
        return when {
            diff < 60 -> "just now"
            diff < 3600 -> "${diff/60}m"
            diff < 86400 -> "${diff/3600}h"
            diff < 2592000 -> "${diff/86400}d"
            diff < 31536000 -> "${diff/2592000}mo"
            else -> "${diff/31536000}y"
        }
    }

    val Int.dp get() = (this * context.resources.displayMetrics.density).toInt()
}

// ============================================================================
// RedReader-style Comment View
// ============================================================================

class CommentView(ctx: Context) : FrameLayout(ctx) {
    private val indent = View(ctx).apply {
        layoutParams = LayoutParams(4.dp, LayoutParams.MATCH_PARENT).apply { gravity = Gravity.START }
    }
    private val author = TextView(ctx).apply {
        setTextColor(PRTheme.accent(ctx)); textSize = 13f
        setTypeface(typeface, Typeface.BOLD)
    }
    private val body = TextView(ctx).apply {
        setTextColor(PRTheme.text(ctx)); textSize = 14f
        setLineSpacing(2f, 1f)
    }
    private val meta = TextView(ctx).apply {
        setTextColor(PRTheme.textSec(ctx)); textSize = 11f
    }
    private val flairLabel = TextView(ctx).apply {
        setTextColor(PRTheme.textSec(ctx)); textSize = 10f
        visibility = GONE; setPadding(4,1,4,1)
    }
    private val scoreText = TextView(ctx).apply {
        setTextColor(PRTheme.textSec(ctx)); textSize = 12f
    }

    data class CommentData(
        val id: String, val author: String, val body: String, val score: Int,
        val created: Long, val depth: Int, val stickied: Boolean, val flair: String,
        val distinguished: String, val isSubmitter: Boolean, val likes: Int, val replies: String
    )

    init {
        setBackgroundColor(PRTheme.cardBg(ctx))
        setPadding(8.dp, 6.dp, 8.dp, 6.dp)

        val inner = LinearLayout(ctx).apply { orientation = LinearLayout.VERTICAL }
        inner.setPadding(8.dp, 4.dp, 8.dp, 4.dp)

        val topRow = LinearLayout(ctx).apply { orientation = LinearLayout.HORIZONTAL }
        topRow.addView(author)
        topRow.addView(flairLabel.apply {
            (layoutParams as MarginLayoutParams).leftMargin = 6.dp
        })

        inner.addView(topRow)
        inner.addView(body)
        inner.addView(meta)
        inner.addView(scoreText)
        addView(inner)
    }

    fun bind(comment: CommentData) {
        author.text = "u/${comment.author}"
        body.text = comment.body

        val now = System.currentTimeMillis()/1000
        val diff = now - comment.created
        val age = when { diff<60->"now"; diff<3600->"${diff/60}m"; diff<86400->"${diff/3600}h"
            else->"${diff/86400}d" }

        var metaStr = "$age"
        if(comment.isSubmitter) metaStr += " • OP"
        if(comment.distinguished=="moderator") metaStr += " • MOD"
        if(comment.distinguished=="admin") metaStr += " • ADMIN"
        meta.text = metaStr

        scoreText.text = "${comment.score} pts"

        flairLabel.text = comment.flair
        flairLabel.visibility = if(comment.flair.isNotEmpty()) VISIBLE else GONE

        // Depth-based indentation via left margin
        val lp = layoutParams as? MarginLayoutParams
        lp?.leftMargin = min(comment.depth * 12, 60).dp
    }

    val Int.dp get() = (this * context.resources.displayMetrics.density).toInt()
}

// ============================================================================
// Post Adapter for RecyclerView
// ============================================================================

class PostAdapter(
    private val onPostClick: (PostCardView.PostData) -> Unit,
    private val onSwipeLeft: (PostCardView.PostData) -> Unit,
    private val onSwipeRight: (PostCardView.PostData) -> Unit
) : RecyclerView.Adapter<PostAdapter.VH>() {

    private val posts = mutableListOf<PostCardView.PostData>()

    class VH(val card: PostCardView) : RecyclerView.ViewHolder(card) {
        var startX = 0f; var swiped = false
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): VH {
        val card = PostCardView(parent.context)
        card.layoutParams = RecyclerView.LayoutParams(
            RecyclerView.LayoutParams.MATCH_PARENT, RecyclerView.LayoutParams.WRAP_CONTENT
        )
        return VH(card)
    }

    override fun onBindViewHolder(holder: VH, pos: Int) {
        val post = posts[pos]
        holder.card.bind(post, pos)
        holder.card.setOnClickListener { onPostClick(post) }

        // Swipe detection
        holder.card.setOnTouchListener { v, event ->
            when(event.action) {
                MotionEvent.ACTION_DOWN -> { holder.startX = event.x; holder.swiped = false; false }
                MotionEvent.ACTION_UP -> {
                    val dx = event.x - holder.startX
                    if(abs(dx) > 100 * holder.itemView.context.resources.displayMetrics.density && !holder.swiped) {
                        holder.swiped = true
                        if(dx > 0) onSwipeRight(post) else onSwipeLeft(post)
                        true
                    } else false
                }
                else -> false
            }
        }
    }

    override fun getItemCount() = posts.size

    fun setData(newPosts: List<PostCardView.PostData>) {
        posts.clear(); posts.addAll(newPosts); notifyDataSetChanged()
    }

    fun addData(newPosts: List<PostCardView.PostData>) {
        val start = posts.size
        posts.addAll(newPosts)
        notifyItemRangeInserted(start, newPosts.size)
    }

}

// ============================================================================
// Comment Adapter
// ============================================================================

class CommentAdapter : RecyclerView.Adapter<CommentAdapter.VH>() {
    private val comments = mutableListOf<CommentView.CommentData>()

    class VH(val view: CommentView) : RecyclerView.ViewHolder(view)

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int) = VH(CommentView(parent.context))
    override fun onBindViewHolder(holder: VH, pos: Int) = holder.view.bind(comments[pos])
    override fun getItemCount() = comments.size

    fun setData(newComments: List<CommentView.CommentData>) {
        comments.clear(); comments.addAll(newComments); notifyDataSetChanged()
    }
}
