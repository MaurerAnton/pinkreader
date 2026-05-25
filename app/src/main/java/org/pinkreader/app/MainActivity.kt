package org.pinkreader.app

import android.os.Bundle
import android.widget.TextView
import android.widget.LinearLayout
import android.widget.ScrollView
import androidx.appcompat.app.AppCompatActivity
import org.json.JSONArray
import org.json.JSONObject

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("pinkreader_native")
        }
    }

    private lateinit var textView: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Create simple UI
        val layout = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            setPadding(32, 32, 32, 32)
        }

        textView = TextView(this).apply {
            text = "Loading...\n"
            textSize = 14f
        }
        layout.addView(ScrollView(this).apply { addView(textView) })

        setContentView(layout)

        // Initialize native code
        nativeInit(cacheDir.absolutePath)

        // Display version
        val version = nativeGetVersion()
        textView.text = "$version\n\n"

        // Fetch some posts
        Thread {
            try {
                val json = nativeFetchPosts("all")
                val posts = JSONArray(json)
                val sb = StringBuilder()
                sb.append("=== r/all - Hot Posts ===\n\n")
                for (i in 0 until posts.length()) {
                    val post = posts.getJSONObject(i)
                    sb.append("${post.optString("title", "No title")}\n")
                    sb.append("  by u/${post.optString("author", "?")} ")
                    sb.append("in r/${post.optString("subreddit", "?")} ")
                    sb.append("- ${post.optInt("score", 0)} pts ")
                    sb.append("(${post.optInt("num_comments", 0)} comments)\n\n")
                }
                if (posts.length() == 0) {
                    sb.append("(No posts - API may require authentication)\n")
                    sb.append("Set up OAuth to fetch Reddit content.\n")
                }

                runOnUiThread {
                    textView.text = version + "\n\n" + sb.toString()
                }
            } catch (e: Exception) {
                runOnUiThread {
                    textView.text = version + "\n\nError: ${e.message}\n\n"
                    textView.append("PinkReader native library loaded successfully.\n")
                    textView.append("Set up Reddit OAuth credentials to fetch content.\n")
                }
            }
        }.start()
    }

    override fun onResume() { super.onResume(); nativeResume() }
    override fun onPause() { super.onPause(); nativePause() }
    override fun onDestroy() { super.onDestroy(); nativeDestroy() }

    // Native methods
    private external fun nativeInit(cachePath: String)
    private external fun nativeResume()
    private external fun nativePause()
    private external fun nativeDestroy()
    private external fun nativeGetVersion(): String
    private external fun nativeFetchPosts(subreddit: String): String
}
