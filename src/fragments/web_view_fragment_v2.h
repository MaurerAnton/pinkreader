// Origin: org.quantumbadger.redreader.fragments.WebViewFragment.java
// Ported from RedReader (GPLv3)
#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <memory>

namespace PinkReader {

// Forward declarations
class UriString;
class RedditPost;
class RedditPreparedPost;
class BaseActivity;
class WebViewFixed;
class VideoEnabledWebChromeClient;
class SideToolbarOverlay;
class BezelSwipeOverlay;
class RedditPostView;

class WebViewFragment {
public:
    // --- Static factory methods ---
    static std::unique_ptr<WebViewFragment> newInstance(const UriString* url, const RedditPost* post);
    static std::unique_ptr<WebViewFragment> newInstanceHtml(const std::string& html);

    // --- Constructor ---
    WebViewFragment();

    // --- Lifecycle ---
    void onCreate(void* savedInstanceState); // Bundle*
    void* onCreateView(void* inflater, void* container, void* savedInstanceState); // returns View*
    void onDestroyView();
    void onPause();
    void onResume();

    // --- Navigation ---
    bool onBackButtonPressed();
    UriString* getCurrentUrl();

    // --- PostSelectionListener ---
    void onPostSelected(RedditPreparedPost* post);
    void onPostCommentsSelected(RedditPreparedPost* post);

    // --- Cache ---
    void clearCache();

private:
    BaseActivity* m_activity = nullptr;
    UriString* m_url = nullptr;
    std::string m_html;
    UriString* m_currentUrl = nullptr;
    std::atomic<bool> m_goingBack{false};
    std::atomic<int> m_lastBackDepthAttempt{0};

    WebViewFixed* m_webView = nullptr;
    void* m_progressView = nullptr;    // ProgressBar*
    void* m_outer = nullptr;           // FrameLayout*

    // --- Inner WebViewClient callbacks ---
    bool onEncounteredIntentUrl(void* url); // Uri*
    bool onEncounteredCustomSchemeUrl(void* url); // Uri*
};

} // namespace PinkReader
