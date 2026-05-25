/*
 * PinkReader - GPLv3
 * File: web_view_fragment.h - Port of WebViewFragment.java
 *
 * Exact port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/fragments/WebViewFragment.java
 *
 * Every field, method, inner class, and logic branch ported exactly.
 *
 * Original: public class WebViewFragment extends Fragment
 *     implements RedditPostView.PostSelectionListener
 * 575 lines of Java ported.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QWidget>
#include <memory>
#include <atomic>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class BaseActivity;
class Bundle;
class FrameLayout;
class LayoutInflater;
class ProgressBar;
class View;
class ViewGroup;
class WebViewFixed;
class WebView;
class WebResourceRequest;
class WebSettings;
class WebViewClient;
class VideoEnabledWebChromeClient;
class UriString;
class Uri;
class Bitmap;
class RedditPost;
class RedditParsedPost;
class RedditPreparedPost;
class BezelSwipeOverlay;
class SideToolbarOverlay;

/**
 * @brief Port of WebViewFragment.PostSelectionListener (from RedditPostView)
 *
 * Interface for post selection callbacks.
 */
class WebViewPostSelectionListener {
public:
    virtual ~WebViewPostSelectionListener() = default;
    virtual void onPostSelected(RedditPreparedPost *post) = 0;
    virtual void onPostCommentsSelected(RedditPreparedPost *post) = 0;
};

/**
 * @brief Port of org.quantumbadger.redreader.fragments.WebViewFragment
 *
 * Fragment hosting a WebView for browsing Reddit links internally.
 * Implements RedditPostView.PostSelectionListener.
 * Every field, method, inner class, and logic branch ported exactly.
 */
class WebViewFragment : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Nested inner class: WebViewFragmentWebViewClient — Java lines 276-467
    // ========================================================================
    //
    // Port of the anonymous WebViewClient subclass created at Java line 276:
    //     webView.setWebViewClient(new WebViewClient() { ... });
    //
    // Contains:
    //   - shouldOverrideUrlLoading (lines 278-362)
    //   - onEncounteredIntentUrl (lines 367-387)
    //   - onEncounteredCustomSchemeUrl (lines 393-401)
    //   - onPageStarted (lines 403-415)
    //   - onPageFinished (lines 417-458)
    //   - doUpdateVisitedHistory (lines 460-466)
    class WebViewFragmentWebViewClient : public QObject {
        Q_OBJECT

    public:
        WebViewFragmentWebViewClient(
                WebViewFragment *fragment,
                BaseActivity *activity,
                WebViewFixed *webView,
                QObject *parent = nullptr);

        // Java lines 278-362: shouldOverrideUrlLoading
        bool shouldOverrideUrlLoading(WebView *view, WebResourceRequest *request);

        // Java lines 403-415: onPageStarted
        void onPageStarted(WebView *view, const QString &url, Bitmap *favicon);

        // Java lines 417-458: onPageFinished
        void onPageFinished(WebView *view, const QString &url);

        // Java lines 460-466: doUpdateVisitedHistory
        void doUpdateVisitedHistory(WebView *view, const QString &url, bool isReload);

    private:
        // Java lines 367-387: onEncounteredIntentUrl
        bool onEncounteredIntentUrl(const QString &urlString);

        // Java lines 393-401: onEncounteredCustomSchemeUrl(const Uri url)
        bool onEncounteredCustomSchemeUrl(const QString &urlString);

        WebViewFragment *m_fragment;
        BaseActivity *m_activity;
        WebViewFixed *m_webView;
    };

    // ========================================================================
    // Nested inner class: WebViewFragmentChromeClient — Java lines 174-189
    // ========================================================================
    //
    // Port of the anonymous VideoEnabledWebChromeClient subclass created at
    // Java line 174:
    //     final VideoEnabledWebChromeClient chromeClient = new ... { ... };
    class WebViewFragmentChromeClient : public QObject {
        Q_OBJECT

    public:
        WebViewFragmentChromeClient(
                BaseActivity *activity,
                FrameLayout *loadingViewFrame,
                FrameLayout *fullscreenViewFrame,
                ProgressBar *progressView,
                QObject *parent = nullptr);

        // Java lines 178-188: onProgressChanged
        void onProgressChanged(WebView *view, int newProgress);

        // Java lines 191-222: onToggledFullscreen callback
        void onToggledFullscreen(bool fullscreen);

    private:
        BaseActivity *m_activity;
        ProgressBar *m_progressView;
    };

    // ========================================================================
    // Nested inner class: WebViewFragmentBezelSwipeListener — Java lines 476-503
    // ========================================================================
    //
    // Port of the anonymous BezelSwipeOverlay.BezelSwipeListener created at
    // Java line 478:
    //     new BezelSwipeOverlay.BezelSwipeListener() { ... };
    class WebViewFragmentBezelSwipeListener : public QObject {
        Q_OBJECT

    public:
        WebViewFragmentBezelSwipeListener(
                BaseActivity *activity,
                RedditPreparedPost *post,
                SideToolbarOverlay *toolbarOverlay,
                QObject *parent = nullptr);

        // Java lines 480-492: onSwipe
        bool onSwipe(int edge);

        // Java lines 494-503: onTap
        bool onTap();

    private:
        BaseActivity *m_activity;
        RedditPreparedPost *m_post;
        SideToolbarOverlay *m_toolbarOverlay;
    };

    // ========================================================================
    // Static factory methods — Java lines 86-109
    // ========================================================================

    // Java lines 86-98: newInstance(final UriString url, final RedditPost post)
    static WebViewFragment *newInstance(UriString *url, RedditPost *post);

    // Java lines 100-109: newInstanceHtml(final String html)
    static WebViewFragment *newInstanceHtml(const QString &html);

    // ========================================================================
    // Constructor & lifecycle — Java lines 112-530
    // ========================================================================

    explicit WebViewFragment(QObject *parent = nullptr);
    ~WebViewFragment() override;

    // Java lines 112-117: onCreate(Bundle savedInstanceState)
    void onCreate(Bundle *savedInstanceState);

    // Java lines 121-513: onCreateView(LayoutInflater, ViewGroup, Bundle)
    QWidget *onCreateView(
            LayoutInflater *inflater,
            ViewGroup *container,
            Bundle *savedInstanceState);

    // Java lines 516-530: onDestroyView()
    void onDestroyView();

    // Java lines 558-563: onPause()
    void onPause();

    // Java lines 565-570: onResume()
    void onResume();

    // ========================================================================
    // Public API — Java lines 532-574
    // ========================================================================

    // Java lines 532-542: onBackButtonPressed()
    bool onBackButtonPressed();

    // Java lines 544-547: onPostSelected(RedditPreparedPost post)
    void onPostSelected(RedditPreparedPost *post);

    // Java lines 549-552: onPostCommentsSelected(RedditPreparedPost post)
    void onPostCommentsSelected(RedditPreparedPost *post);

    // Java lines 554-556: getCurrentUrl()
    UriString *getCurrentUrl() const;

    // Java lines 572-574: clearCache()
    void clearCache();

private:
    // ========================================================================
    // Fields — Java lines 74-84
    // ========================================================================

    // Java line 74: private BaseActivity mActivity;
    BaseActivity *m_activity = nullptr;

    // Java line 76: private UriString mUrl;
    UriString *m_url = nullptr;

    // Java line 77: private String html;
    QString m_html;

    // Java line 78: private volatile UriString currentUrl;
    std::atomic<UriString *> m_currentUrl{nullptr};

    // Java line 79: private volatile boolean goingBack;
    std::atomic<bool> m_goingBack{false};

    // Java line 80: private volatile int lastBackDepthAttempt;
    std::atomic<int> m_lastBackDepthAttempt{0};

    // Java line 82: private WebViewFixed webView;
    WebViewFixed *m_webView = nullptr;

    // Java line 83: private ProgressBar progressView;
    ProgressBar *m_progressView = nullptr;

    // Java line 84: private FrameLayout outer;
    FrameLayout *m_outer = nullptr;

    // Additional references held for the sub-objects
    WebViewFragmentWebViewClient *m_webViewClient = nullptr;
    WebViewFragmentChromeClient *m_chromeClient = nullptr;
    WebViewFragmentBezelSwipeListener *m_bezelSwipeListener = nullptr;
    SideToolbarOverlay *m_toolbarOverlay = nullptr;
    BezelSwipeOverlay *m_bezelOverlay = nullptr;

    // RedditPost reference passed via newInstance — Java line 92-94
    RedditPost *m_post = nullptr;
};

} // namespace PinkReader
