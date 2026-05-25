// Origin: org.quantumbadger.redreader.views.webview.WebViewFixed.java
// Ported from RedReader (GPLv3)

#include "views/webview/web_view_fixed.h"
#include "views/webview/video_enabled_web_chrome_client.h"

#include <string>
#include <map>

namespace PinkReader {

// --- JavascriptInterface ---

void WebViewFixed::JavascriptInterface::notifyVideoEnd() {
    // This code is not executed in the UI thread, so we must post to UI thread
    // In original: AndroidCommon.UI_THREAD_HANDLER.post(() -> { ... });
    // In C++ port: the framework layer handles thread posting
    // Callback would be dispatched to UI thread by the platform layer
}

// --- Constructors ---

WebViewFixed::WebViewFixed(void* context)
    : m_addedJavascriptInterface(false) {
    setTor(context);
}

WebViewFixed::WebViewFixed(void* context, void* attrs, int defStyle)
    : m_addedJavascriptInterface(false) {
    setTor(context);
}

WebViewFixed::WebViewFixed(void* context, void* attrs)
    : m_addedJavascriptInterface(false) {
    setTor(context);
}

// --- Public API ---

bool WebViewFixed::isVideoFullscreen() const {
    return m_videoEnabledWebChromeClient != nullptr
            && m_videoEnabledWebChromeClient->isVideoFullscreen();
}

void WebViewFixed::setWebChromeClient(void* client) {
    // getSettings().setJavaScriptEnabled(true);

    // Check if client is VideoEnabledWebChromeClient
    VideoEnabledWebChromeClient* vec = static_cast<VideoEnabledWebChromeClient*>(client);
    if (vec != nullptr) {
        m_videoEnabledWebChromeClient = vec;
    }

    // super.setWebChromeClient(client);
}

void WebViewFixed::loadData(const std::string& data, const std::string& mimeType, const std::string& encoding) {
    addJavascriptInterface();
    // super.loadData(data, mimeType, encoding);
}

void WebViewFixed::loadDataWithBaseURL(
        const std::string& baseUrl,
        const std::string& data,
        const std::string& mimeType,
        const std::string& encoding,
        const std::string& historyUrl) {
    addJavascriptInterface();
    // super.loadDataWithBaseURL(baseUrl, data, mimeType, encoding, historyUrl);
}

void WebViewFixed::loadHtmlUTF8WithBaseURL(const std::string& baseUrl, const std::string& html) {
    loadDataWithBaseURL(baseUrl, html, "text/html; charset=utf-8", "UTF-8", "");
}

void WebViewFixed::loadUrl(const std::string& url) {
    addJavascriptInterface();
    // super.loadUrl(url);
}

void WebViewFixed::loadUrl(const std::string& url, const std::map<std::string, std::string>& additionalHttpHeaders) {
    addJavascriptInterface();
    // super.loadUrl(url, additionalHttpHeaders);
}

void WebViewFixed::addJavascriptInterface() {
    if (!m_addedJavascriptInterface) {
        // Add javascript interface to be called when the video ends (must be done before page load)
        // Must match Javascript interface name of VideoEnabledWebChromeClient
        // In original: addJavascriptInterface(new JavascriptInterface(), "_VideoEnabledWebView");
        m_addedJavascriptInterface = true;
    }
}

void WebViewFixed::onWindowFocusChanged(bool hasWindowFocus) {
    // Fixes the onWindowFocusChanged bug, by catching NullPointerException.
    // In original:
    // try { super.onWindowFocusChanged(hasWindowFocus); }
    // catch(NullPointerException ex) { Log.e("WebView", "WebView.onWindowFocusChanged", ex); }
}

void WebViewFixed::setTor(void* context) {
    // In original:
    // if(TorCommon.isTorEnabled()) {
    //     try { clearBrowser();
    //         final boolean result = WebkitProxy.setProxy(
    //             RedReader.class.getCanonicalName(),
    //             context.getApplicationContext(), this,
    //             "127.0.0.1", 8118);
    //         if(!result) BugReportActivity.handleGlobalError(context, ...);
    //     } catch(Exception e) { BugReportActivity.handleGlobalError(context, e); }
    // }
}

void WebViewFixed::clearBrowser() {
    // this.clearCache(true);
    // this.clearFormData();
    // this.clearHistory();
    // CookieManager.getInstance().removeAllCookies(null);
}

} // namespace PinkReader
