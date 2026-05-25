// Origin: org.quantumbadger.redreader.fragments.WebViewFragment.java
// Ported from RedReader (GPLv3)

#include "fragments/web_view_fragment_v2.h"

#include <atomic>
#include <cstdint>
#include <string>
#include <memory>

namespace PinkReader {

// --- Static factory methods ---

std::unique_ptr<WebViewFragment> WebViewFragment::newInstance(const UriString* url, const RedditPost* post) {
    auto f = std::make_unique<WebViewFragment>();
    // In original:
    // final Bundle bundle = new Bundle(1);
    // bundle.putParcelable("url", url);
    // if(post != null) bundle.putParcelable("post", post);
    // f.setArguments(bundle);
    (void)url;
    (void)post;
    return f;
}

std::unique_ptr<WebViewFragment> WebViewFragment::newInstanceHtml(const std::string& html) {
    auto f = std::make_unique<WebViewFragment>();
    // final Bundle bundle = new Bundle(1);
    // bundle.putString("html", html);
    // f.setArguments(bundle);
    f->m_html = html;
    return f;
}

// --- Constructor ---

WebViewFragment::WebViewFragment()
    : m_goingBack(false)
    , m_lastBackDepthAttempt(0) {}

// --- Lifecycle ---

void WebViewFragment::onCreate(void* savedInstanceState) {
    // super.onCreate(savedInstanceState);
    // mUrl = BundleCompat.getParcelable(requireArguments(), "url", UriString.class);
    // html = requireArguments().getString("html");
    (void)savedInstanceState;
}

void* WebViewFragment::onCreateView(void* inflater, void* container, void* savedInstanceState) {
    // This is the largest method in the class (~440 lines in original).
    // Key operations:
    //
    // 1. mActivity = (BaseActivity)getActivity();
    // 2. outer = (FrameLayout)inflater.inflate(R.layout.web_view_fragment, null);
    // 3. Parse srcPost from bundle, create RedditParsedPost -> RedditPreparedPost
    // 4. Find webView and loadingViewFrame from outer
    // 5. Create ProgressBar, set up in loadingViewFrame
    // 6. Find fullscreenViewFrame
    // 7. Create VideoEnabledWebChromeClient with loadingViewFrame, fullscreenViewFrame:
    //    - Override onProgressChanged to update progress bar
    //    - Fullscreen toggle callback manages FLAG_FULLSCREEN, action bar, system UI
    // 8. Set download listener (MaterialAlertDialogBuilder for external downloads)
    // 9. Configure WebSettings: zoom, JS, DOM storage, wide view port
    // 10. Set chrome client, load URL or HTML
    // 11. Set WebViewClient with shouldOverrideUrlLoading:
    //     - Handles data: URIs (imgur bug)
    //     - Redirect loop detection (goingBack + currentUrl check)
    //     - Reddit URL parsing -> LinkHandler.onLinkClicked
    //     - intent:// URLs: parse fallback URL, load in webView
    //     - Custom scheme URLs (twitter://, market://): try native app
    //     - Fallback: open in external browser, custom tabs, or internal
    //     - onPageStarted: set activity title
    //     - onPageFinished: 1s delayed redirect loop check via Timer
    //     - doUpdateVisitedHistory: pass through
    // 12. If post != null: create SideToolbarOverlay + BezelSwipeOverlay:
    //     - BezelSwipeListener.onSwipe: generates toolbar via RedditPostActions,
    //       shows toolbar on left or right side
    //     - BezelSwipeListener.onTap: hides toolbar or returns false
    //     - Add bezel + toolbar overlays, set match parent layout
    // 13. Return outerFrame

    (void)inflater;
    (void)container;
    (void)savedInstanceState;
    return m_outer;
}

void WebViewFragment::onDestroyView() {
    // In original:
    // webView.stopLoading();
    // webView.loadData("<html></html>", "text/plain", "UTF-8");
    // webView.reload();
    // webView.loadUrl("about:blank");
    // outer.removeAllViews();
    // webView.destroy();
    // CookieManager.getInstance().removeAllCookies(null);
    // super.onDestroyView();
}

void WebViewFragment::onPause() {
    // super.onPause();
    // webView.onPause();
    // webView.pauseTimers();
}

void WebViewFragment::onResume() {
    // super.onResume();
    // webView.resumeTimers();
    // webView.onResume();
}

// --- Navigation ---

bool WebViewFragment::onBackButtonPressed() {
    // In original:
    // if(webView.canGoBack()) {
    //     goingBack = true;
    //     lastBackDepthAttempt = -1;
    //     webView.goBack();
    //     return true;
    // }
    // return false;
    return false;
}

UriString* WebViewFragment::getCurrentUrl() {
    // return (currentUrl != null) ? currentUrl : mUrl;
    return m_currentUrl ? m_currentUrl : m_url;
}

// --- PostSelectionListener ---

void WebViewFragment::onPostSelected(RedditPreparedPost* post) {
    // ((RedditPostView.PostSelectionListener)mActivity).onPostSelected(post);
    (void)post;
}

void WebViewFragment::onPostCommentsSelected(RedditPreparedPost* post) {
    // ((RedditPostView.PostSelectionListener)mActivity).onPostCommentsSelected(post);
    (void)post;
}

// --- Cache ---

void WebViewFragment::clearCache() {
    // webView.clearBrowser();
}

// --- Inner WebViewClient helpers ---

bool WebViewFragment::onEncounteredIntentUrl(void* url) {
    // In original (~20 lines):
    // try { nativeAppIntent = Intent.parseUri(url.toString(), Intent.URI_INTENT_SCHEME); }
    // catch(URISyntaxException e) { return false; }
    // if(nativeAppIntent == null) return false;
    // final String fallbackUrl = nativeAppIntent.getStringExtra("browser_fallback_url");
    // if(fallbackUrl == null) return false;
    // webView.loadUrl(fallbackUrl);
    // currentUrl = new UriString(fallbackUrl);
    // return true;
    (void)url;
    return false;
}

bool WebViewFragment::onEncounteredCustomSchemeUrl(void* url) {
    // In original (~10 lines):
    // final Intent nativeAppIntent = new Intent(Intent.ACTION_VIEW, url);
    // try { startActivity(nativeAppIntent); return true; }
    // catch(ActivityNotFoundException e) { return false; }
    (void)url;
    return false;
}

} // namespace PinkReader
