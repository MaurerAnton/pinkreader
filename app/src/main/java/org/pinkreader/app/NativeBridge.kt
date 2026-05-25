package org.pinkreader.app

object NativeBridge {
    init { System.loadLibrary("pinkreader_native") }
    external fun nativeGetDarkTheme(): Boolean
    external fun nativeSetDarkTheme(d: Boolean)
    external fun nativeClearCache()
    external fun nativeGetVersion(): String
}
