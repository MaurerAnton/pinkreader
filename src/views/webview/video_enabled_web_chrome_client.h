// Origin: org.quantumbadger.redreader.views.webview.VideoEnabledWebChromeClient.java
// Ported from RedReader (GPLv3) - Taken from cprcrack/VideoEnabledWebView
#pragma once

#include <functional>
#include <memory>
#include <string>

namespace PinkReader {

// Forward declarations
class WebViewFixed;

class VideoEnabledWebChromeClient {
public:
    // --- ToggledFullscreenCallback inner interface ---
    using ToggledFullscreenCallback = std::function<void(bool fullscreen)>;

    // --- Constructors ---
    VideoEnabledWebChromeClient();

    VideoEnabledWebChromeClient(
            void* activityNonVideoView,      // View*
            void* activityVideoView);         // ViewGroup*

    VideoEnabledWebChromeClient(
            void* activityNonVideoView,      // View*
            void* activityVideoView,          // ViewGroup*
            void* loadingView);               // View*

    VideoEnabledWebChromeClient(
            void* activityNonVideoView,      // View*
            void* activityVideoView,          // ViewGroup*
            void* loadingView,                // View*
            WebViewFixed* webView);

    // --- Public API ---
    bool isVideoFullscreen() const;

    void setOnToggledFullscreen(ToggledFullscreenCallback callback);

    void onShowCustomView(void* view, void* customViewCallback);

    void onShowCustomView(void* view, int requestedOrientation, void* customViewCallback);

    void onHideCustomView();

    void* getVideoLoadingProgressView();

    // MediaPlayer listener methods (typically API level <11)
    void onPrepared(void* mp);   // MediaPlayer*
    void onCompletion(void* mp); // MediaPlayer*
    bool onError(void* mp, int what, int extra);

    bool onBackPressed();

private:
    void* m_activityNonVideoView = nullptr;   // View*
    void* m_activityVideoView = nullptr;       // ViewGroup*
    void* m_loadingView = nullptr;             // View*
    WebViewFixed* m_webView = nullptr;

    bool m_isVideoFullscreen = false;
    void* m_videoViewContainer = nullptr;      // FrameLayout*
    void* m_videoViewCallback = nullptr;       // CustomViewCallback*

    ToggledFullscreenCallback m_toggledFullscreenCallback;
};

} // namespace PinkReader
