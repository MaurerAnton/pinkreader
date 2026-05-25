package org.pinkreader.app

import okhttp3.*
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.RequestBody.Companion.toRequestBody
import java.io.IOException
import java.util.concurrent.TimeUnit

/**
 * NetworkManager — handles ALL HTTPS requests using OkHttp.
 * Reddit requires HTTPS; OkHttp provides TLS on Android.
 * All JSON parsing is done in native C++ code.
 */
object NetworkManager {
    private val client = OkHttpClient.Builder()
        .connectTimeout(15, TimeUnit.SECONDS)
        .readTimeout(30, TimeUnit.SECONDS)
        .writeTimeout(30, TimeUnit.SECONDS)
        .build()

    private val JSON = "application/json; charset=utf-8".toMediaType()
    private val FORM = "application/x-www-form-urlencoded".toMediaType()
    private val UA = "PinkReader/3.0 (Android)"

    data class Response(val code: Int, val body: String, val error: String? = null) {
        val isOk get() = code in 200..299
    }

    fun get(url: String, token: String = ""): Response {
        val req = Request.Builder().url(url).header("User-Agent", UA)
        if(token.isNotEmpty()) req.header("Authorization", "Bearer $token")
        return execute(req.build())
    }

    fun post(url: String, body: String, token: String = "", contentType: String = "form"): Response {
        val mediaType = if(contentType == "json") JSON else FORM
        val req = Request.Builder().url(url)
            .header("User-Agent", UA)
            .post(body.toRequestBody(mediaType))
        if(token.isNotEmpty()) req.header("Authorization", "Bearer $token")
        return execute(req.build())
    }

    private fun execute(request: Request): Response {
        return try {
            val resp = client.newCall(request).execute()
            Response(resp.code, resp.body?.string() ?: "", null)
        } catch(e: IOException) {
            Response(-1, "", e.message ?: "Network error")
        }
    }
}
