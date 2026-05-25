package org.pinkreader.app

import android.content.Context

object Theme {
    var isDark = true

    fun bg(ctx: Context) = if(isDark) 0xFF1A1A1A.toInt() else 0xFFF5F5F5.toInt()
    fun card(ctx: Context) = if(isDark) 0xFF252525.toInt() else 0xFFFFFFFF.toInt()
    fun txt(ctx: Context) = if(isDark) 0xFFD0D0D0.toInt() else 0xFF212121.toInt()
    fun sec(ctx: Context) = if(isDark) 0xFF888888.toInt() else 0xFF757575.toInt()
    fun accent() = 0xFFFF7597.toInt()
    fun toolbarBg(ctx: Context) = if(isDark) 0xFF1E1E1E.toInt() else 0xFFFFFFFF.toInt()
    fun divider(ctx: Context) = if(isDark) 0xFF333333.toInt() else 0xFFE0E0E0.toInt()
}
