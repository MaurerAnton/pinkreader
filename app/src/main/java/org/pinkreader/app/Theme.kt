package org.pinkreader.app

object Theme {
    var isDark = true
    fun bg() = if(isDark) 0xFF0D0D0D.toInt() else 0xFFF5F5F5.toInt()
    fun card() = if(isDark) 0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt()
    fun txt() = if(isDark) 0xFFDDDDDD.toInt() else 0xFF111111.toInt()
    fun sec() = if(isDark) 0xFF888888.toInt() else 0xFF666666.toInt()
    fun accent() = 0xFFFF7597.toInt()
}
