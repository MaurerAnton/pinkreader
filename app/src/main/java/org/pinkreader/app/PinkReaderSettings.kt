package org.pinkreader.app

import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.appcompat.app.AppCompatActivity

class PinkReaderSettings : AppCompatActivity() {
    companion object {
        val nativeLoaded: Boolean = run {
            try { System.loadLibrary("pinkreader_native"); true }
            catch(e: UnsatisfiedLinkError) { android.util.Log.e("PinkReader","Settings: Failed to load native lib",e); false }
        }
    }
    private external fun nativeGetDarkTheme(): Boolean
    private external fun nativeSetDarkTheme(d: Boolean)
    private external fun nativeClearCache()

    private fun safeJniBool(block: () -> Boolean, fallback: Boolean = true): Boolean =
        if (nativeLoaded) try { block() } catch(e: Exception) { fallback } else fallback
    private fun safeJniVoid(block: () -> Unit) { if (nativeLoaded) try { block() } catch(e: Exception) {} }

    private var isDark = true
    private fun bg() = if(isDark) 0xFF1A1A1A.toInt() else 0xFFF5F5F5.toInt()
    private fun card() = if(isDark) 0xFF252525.toInt() else 0xFFFFFFFF.toInt()
    private fun txt() = if(isDark) 0xFFD0D0D0.toInt() else 0xFF212121.toInt()
    private fun sec() = if(isDark) 0xFF888888.toInt() else 0xFF757575.toInt()
    private fun accent() = 0xFFFF7597.toInt()
    private val Int.dp get() = (this * resources.displayMetrics.density).toInt()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        isDark = safeJniBool({ nativeGetDarkTheme() }, true)
        window.statusBarColor = if(isDark) 0xFF0D0D0D.toInt() else 0xFFE0E0E0.toInt()

        val scroll = ScrollView(this)
        val root = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL; setBackgroundColor(bg()); setPadding(16.dp, 16.dp, 16.dp, 16.dp) }

        fun header(text: String) {
            root.addView(TextView(this).apply {
                this.text = text; setTextColor(accent()); textSize = 14f; setTypeface(null, android.graphics.Typeface.BOLD)
                setPadding(0, 16.dp, 0, 8.dp)
            })
        }

        fun item(label: String, desc: String, hasSwitch: Boolean = false, checked: Boolean = false,
                 onClick: ((Boolean) -> Unit)? = null) {
            val card = LinearLayout(this).apply {
                orientation = LinearLayout.HORIZONTAL; setBackgroundColor(card()); setPadding(12.dp, 10.dp, 12.dp, 10.dp)
                gravity = Gravity.CENTER_VERTICAL; (layoutParams as LinearLayout.LayoutParams).bottomMargin = 2.dp
            }
            val texts = LinearLayout(this).apply { orientation = LinearLayout.VERTICAL; layoutParams = LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f) }
            texts.addView(TextView(this).apply { text = label; setTextColor(txt()); textSize = 15f })
            texts.addView(TextView(this).apply { text = desc; setTextColor(sec()); textSize = 12f })
            card.addView(texts)
            if(hasSwitch) {
                val sw = Switch(this).apply { isChecked = checked; setOnCheckedChangeListener { _, b -> onClick?.invoke(b) } }
                card.addView(sw)
            }
            if(onClick != null && !hasSwitch) {
                card.setOnClickListener { onClick(false) }
                card.addView(TextView(this).apply { text = "›"; setTextColor(sec()); textSize = 22f })
            }
            root.addView(card)
        }

        header("APPEARANCE")
        item("Theme", "Dark / Light mode", true, isDark) { d -> isDark = d; safeJniVoid { nativeSetDarkTheme(d) }; recreate() }
        item("Font scale", "Adjust text size (coming soon)", false)

        header("CONTENT")
        item("NSFW Content", "Show adult content", true, false)
        item("Hide read posts", "Automatically hide viewed posts", true, false)
        item("Inline image previews", "Show images in post list", true, true)

        header("BEHAVIOUR")
        item("Default sort", "Hot / New / Top / Rising", false)
        item("Post tap action", "Open link or comments", false)
        item("Swipe left action", "Downvote, Save, Hide...", false)
        item("Swipe right action", "Upvote, Save, Comments...", false)
        item("Back button behaviour", "Double-tap to exit", true, false)

        header("CACHE")
        item("Clear cache", "Free up storage space", false) { safeJniVoid { nativeClearCache() }; Toast.makeText(this, "Cache cleared", Toast.LENGTH_SHORT).show() }

        header("ABOUT")
        item("PinkReader v2.0", "C++20 native engine + Kotlin UI\nOpen source Reddit client for Android\nGPLv3 License · F-Droid available", false)

        scroll.addView(root); setContentView(scroll)
    }
}
