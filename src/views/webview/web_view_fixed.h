// Origin: org.quantumbadger.redreader.views.webview.WebViewFixed.java
// Ported from RedReader (GPLv3) - Taken from reddit-is-fun and cprcrack/VideoEnabledWebView
#pragma once

#include <functional>
#include <map>
#include <string>

namespace PinkReader {

// Forward declarations
class VideoEnabledWebChromeClient;

class WebViewFixed {
public:
    // --- JavascriptInterface inner class ---
    class JavascriptInterface {
    public:
        // Must match Javascript interface method of VideoEnabledWebChromeClient
        void notifyVideoEnd();
    };

    // --- Constructors ---
    explicit WebViewFixed(void* context);
    WebViewFixed(void* context, void* attrs, int defStyle);
    WebViewFixed(void* context, void* attrs);

    // --- Public API ---
    bool isVideoFullscreen() const;

    void setWebChromeClient(void* client); // WebChromeClient*

    void loadData(const std::string& data, const std::string& mimeType, const std::string& encoding);
    void loadDataWithBaseURL(
            const std::string& baseUrl,
            const std::string& data,
            const std::string& mimeType,
            const std::string& encoding,
            const std::string& historyUrl);

    void loadHtmlUTF8WithBaseURL(const std::string& baseUrl, const std::string& html);

    void loadUrl(const std::string& url);
    void loadUrl(const std::string& url, const std::map<std::string, std::string>& additionalHttpHeaders);

    void onWindowFocusChanged(bool hasWindowFocus);

    void clearBrowser();

private:
    void addJavascriptInterface();
    void setTor(void* context);

    VideoEnabledWebChromeClient* m_videoEnabledWebChromeClient = nullptr;
    bool m_addedJavascriptInterface = false;
};

} // namespace PinkReader
