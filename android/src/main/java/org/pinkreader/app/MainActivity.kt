package org.pinkreader.app

import android.os.Bundle
import android.view.WindowManager
import androidx.appcompat.app.AppCompatActivity

/**
 * Main entry point for PinkReader Android app.
 * This thin wrapper loads the Qt/C++ native library and
 * delegates all rendering to the native layer.
 */
class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("PinkReader")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Enable edge-to-edge display
        window.setFlags(
            WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
            WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS
        )

        // Initialize native PinkReader application
        nativeInit(applicationContext.cacheDir.absolutePath)
    }

    override fun onResume() {
        super.onResume()
        nativeResume()
    }

    override fun onPause() {
        super.onPause()
        nativePause()
    }

    override fun onDestroy() {
        nativeDestroy()
        super.onDestroy()
    }

    // Native methods implemented in C++
    private external fun nativeInit(cachePath: String)
    private external fun nativeResume()
    private external fun nativePause()
    private external fun nativeDestroy()
}
