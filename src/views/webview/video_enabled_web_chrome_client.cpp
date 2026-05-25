// Origin: org.quantumbadger.redreader.views.webview.VideoEnabledWebChromeClient.java
// Ported from RedReader (GPLv3) - Taken from cprcrack/VideoEnabledWebView

#include "views/webview/video_enabled_web_chrome_client.h"
#include "views/webview/web_view_fixed.h"

#include <string>

namespace PinkReader {

// --- Constructors ---

VideoEnabledWebChromeClient::VideoEnabledWebChromeClient()
    : m_isVideoFullscreen(false) {}

VideoEnabledWebChromeClient::VideoEnabledWebChromeClient(
        void* activityNonVideoView,
        void* activityVideoView)
    : m_activityNonVideoView(activityNonVideoView)
    , m_activityVideoView(activityVideoView)
    , m_loadingView(nullptr)
    , m_webView(nullptr)
    , m_isVideoFullscreen(false) {}

VideoEnabledWebChromeClient::VideoEnabledWebChromeClient(
        void* activityNonVideoView,
        void* activityVideoView,
        void* loadingView)
    : m_activityNonVideoView(activityNonVideoView)
    , m_activityVideoView(activityVideoView)
    , m_loadingView(loadingView)
    , m_webView(nullptr)
    , m_isVideoFullscreen(false) {}

VideoEnabledWebChromeClient::VideoEnabledWebChromeClient(
        void* activityNonVideoView,
        void* activityVideoView,
        void* loadingView,
        WebViewFixed* webView)
    : m_activityNonVideoView(activityNonVideoView)
    , m_activityVideoView(activityVideoView)
    , m_loadingView(loadingView)
    , m_webView(webView)
    , m_isVideoFullscreen(false) {}

// --- Public API ---

bool VideoEnabledWebChromeClient::isVideoFullscreen() const {
    return m_isVideoFullscreen;
}

void VideoEnabledWebChromeClient::setOnToggledFullscreen(ToggledFullscreenCallback callback) {
    m_toggledFullscreenCallback = std::move(callback);
}

void VideoEnabledWebChromeClient::onShowCustomView(void* view, void* callback) {
    // view is expected to be a FrameLayout*
    // In original: if(view instanceof FrameLayout)
    // For C++ port: we assume it's a FrameLayout wrapper with getFocusedChild()
    void* frameLayout = view;  // FrameLayout* cast
    void* focusedChild = nullptr; // frameLayout->getFocusedChild()

    // Save video related variables
    m_isVideoFullscreen = true;
    m_videoViewContainer = frameLayout;
    m_videoViewCallback = callback;

    // Hide the non-video view, add the video view, and show it
    // activityNonVideoView.setVisibility(View.INVISIBLE);
    // activityVideoView.addView(videoViewContainer, MATCH_PARENT, MATCH_PARENT);
    // activityVideoView.setVisibility(View.VISIBLE);

    // Check if focusedChild is android.widget.VideoView
    bool isVideoView = false; // dynamic_cast or type check
    if (isVideoView) {
        // android.widget.VideoView (typically API level <11)
        // Handle all the required events:
        // videoView.setOnPreparedListener(this);
        // videoView.setOnCompletionListener(this);
        // videoView.setOnErrorListener(this);
    } else {
        // Other classes: SurfaceView, TextureView, VideoSurfaceView
        // Handle HTML5 video ended event only if the class is a SurfaceView
        if (m_webView != nullptr /* && webView.getSettings().getJavaScriptEnabled()
                && focusedChild instanceof SurfaceView */) {
            // Run javascript code that detects the video end
            std::string js = "javascript:";
            js += "var _ytrp_html5_video_last;";
            js += "var _ytrp_html5_video = document.getElementsByTagName('video')[0];";
            js += "if (_ytrp_html5_video != undefined && _ytrp_html5_video != _ytrp_html5_video_last) {";
            js += "_ytrp_html5_video_last = _ytrp_html5_video;";
            js += "function _ytrp_html5_video_ended() {";
            js += "_WebViewFixed.notifyVideoEnd();";  // Must match JS interface name
            js += "}";
            js += "_ytrp_html5_video.addEventListener('ended', _ytrp_html5_video_ended);";
            js += "}";
            m_webView->loadUrl(js);
        }
    }

    // Notify full-screen change
    if (m_toggledFullscreenCallback) {
        m_toggledFullscreenCallback(true);
    }
}

void VideoEnabledWebChromeClient::onShowCustomView(
        void* view, int requestedOrientation, void* callback) {
    // Available in API level 14+, deprecated in API level 18+
    onShowCustomView(view, callback);
}

void VideoEnabledWebChromeClient::onHideCustomView() {
    if (m_isVideoFullscreen) {
        // Hide the video view, remove it, and show the non-video view
        // activityVideoView.setVisibility(View.INVISIBLE);
        // activityVideoView.removeView(videoViewContainer);
        // activityNonVideoView.setVisibility(View.VISIBLE);

        // Call back (only in API level <19, avoids chromium crash)
        if (m_videoViewCallback != nullptr
                /* && !videoViewCallback.getClass().getName().contains(".chromium.") */) {
            // videoViewCallback.onCustomViewHidden();
        }

        // Reset video related variables
        m_isVideoFullscreen = false;
        m_videoViewContainer = nullptr;
        m_videoViewCallback = nullptr;

        // Notify full-screen change
        if (m_toggledFullscreenCallback) {
            m_toggledFullscreenCallback(false);
        }
    }
}

void* VideoEnabledWebChromeClient::getVideoLoadingProgressView() {
    if (m_loadingView != nullptr) {
        // loadingView.setVisibility(View.VISIBLE);
        return m_loadingView;
    } else {
        // return super.getVideoLoadingProgressView();
        return nullptr;
    }
}

void VideoEnabledWebChromeClient::onPrepared(void* mp) {
    // Video will start playing, only called for android.widget.VideoView (API <11)
    if (m_loadingView != nullptr) {
        // loadingView.setVisibility(View.GONE);
    }
}

void VideoEnabledWebChromeClient::onCompletion(void* mp) {
    // Video finished playing, only called for android.widget.VideoView (API <11)
    onHideCustomView();
}

bool VideoEnabledWebChromeClient::onError(void* mp, int what, int extra) {
    // Error while playing video, only called for android.widget.VideoView (API <11)
    return false; // By returning false, onCompletion() will be called
}

bool VideoEnabledWebChromeClient::onBackPressed() {
    if (m_isVideoFullscreen) {
        onHideCustomView();
        return true;
    } else {
        return false;
    }
}

} // namespace PinkReader
