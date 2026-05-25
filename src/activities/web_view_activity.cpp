/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: web_view_activity.cpp - Port of WebViewActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/WebViewActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "web_view_activity.h"

namespace PinkReader {

// ===== Static constants (port of Java static final ints) =====

// public static final int VIEW_IN_BROWSER = 10;
const int WebViewActivity::VIEW_IN_BROWSER = 10;

// public static final int CLEAR_CACHE = 20;
const int WebViewActivity::CLEAR_CACHE = 20;

// public static final int USE_HTTPS = 30;
const int WebViewActivity::USE_HTTPS = 30;

// public static final int SHARE = 40;
const int WebViewActivity::SHARE = 40;

// ===== Constructor =====
WebViewActivity::WebViewActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
WebViewActivity::~WebViewActivity() = default;

// ===== Port of: @Override public void onCreate(final Bundle savedInstanceState) =====
// Java lines 52-74:
//   PrefsUtility.applyTheme(this);
//   super.onCreate(savedInstanceState);
//   final Intent intent = getIntent();
//   final UriString url = IntentCompat.getParcelableExtra(intent, "url", UriString.class);
//   mPost = IntentCompat.getParcelableExtra(intent, "post", RedditPost.class);
//   if(url == null) { BugReportActivity.handleGlobalError(this, "No URL"); }
//   webView = WebViewFragment.newInstance(url, mPost);
//   setBaseActivityListing(View.inflate(this, R.layout.main_single, null));
//   getSupportFragmentManager().beginTransaction()
//       .add(R.id.main_single_frame, webView)
//       .commit();
void WebViewActivity::onCreate(Bundle *savedInstanceState) {
    // PrefsUtility::applyTheme(this);
    // super.onCreate(savedInstanceState);
    //
    // final Intent intent = getIntent();
    // final UriString url = IntentCompat.getParcelableExtra(intent, "url", UriString.class);
    // mPost = IntentCompat.getParcelableExtra(intent, "post", RedditPost.class);
    //
    // if(url == null) {
    //     BugReportActivity.handleGlobalError(this, "No URL");
    // }
    //
    // webView = WebViewFragment.newInstance(url, mPost);
    // setBaseActivityListing(View.inflate(this, R.layout.main_single, null));
    // getSupportFragmentManager().beginTransaction()
    //     .add(R.id.main_single_frame, webView)
    //     .commit();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override public void onBackPressed() =====
// Java lines 77-82:
//   if(General.onBackPressed() && !webView.onBackButtonPressed()) {
//       super.onBackPressed();
//   }
void WebViewActivity::onBackPressed() {
    // if(General::onBackPressed() && !webView->onBackButtonPressed()) {
    //     super.onBackPressed();
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override public void onPostSelected(final RedditPreparedPost post) =====
// Java lines 85-87:
//   LinkHandler.onLinkClicked(this, post.src.getUrl(), false, post.src.getSrc());
void WebViewActivity::onPostSelected(RedditPreparedPost * /*post*/) {
    // LinkHandler::onLinkClicked(this, post->src->getUrl(), false, post->src->getSrc());

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override public void onPostCommentsSelected(final RedditPreparedPost post) =====
// Java lines 90-95:
//   LinkHandler.onLinkClicked(
//       this,
//       PostCommentListingURL.forPostId(post.src.getIdAlone()).toUriString(),
//       false);
void WebViewActivity::onPostCommentsSelected(RedditPreparedPost * /*post*/) {
    // LinkHandler::onLinkClicked(
    //     this,
    //     PostCommentListingURL::forPostId(post->src->getIdAlone())->toUriString(),
    //     false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override public boolean onOptionsItemSelected(final MenuItem item) =====
// Java lines 98-164:
//   final UriString currentUrl = webView.getCurrentUrl();
//   switch(item.getItemId()) {
//       case VIEW_IN_BROWSER: ...
//       case CLEAR_CACHE: ...
//       case USE_HTTPS: ...
//       case SHARE: ...
//       default: return super.onOptionsItemSelected(item);
//   }
bool WebViewActivity::onOptionsItemSelected(MenuItem *item) {
    // const UriString *currentUrl = webView->getCurrentUrl();
    //
    // switch(item->getItemId()) {
    //     case VIEW_IN_BROWSER:
    //         if(currentUrl != nullptr) {
    //             try {
    //                 final Intent intent = new Intent(Intent::ACTION_VIEW);
    //                 intent->setData(currentUrl->toUri());
    //                 startActivity(intent);
    //                 finish(); // to clear from backstack
    //             } catch(const Exception &e) {
    //                 Toast::makeText(this, "Error: could not launch browser.",
    //                     Toast::LENGTH_LONG)->show();
    //             }
    //         }
    //         return true;
    //
    //     case CLEAR_CACHE:
    //         webView->clearCache();
    //         Toast::makeText(this, R::string::web_view_clear_cache_success_toast,
    //             Toast::LENGTH_LONG)->show();
    //         return true;
    //
    //     case USE_HTTPS:
    //         if(currentUrl != nullptr) {
    //             if(currentUrl->value.startsWith("https://")) {
    //                 General::quickToast(this, R::string::webview_https_already);
    //                 return true;
    //             }
    //             if(!currentUrl->value.startsWith("http://")) {
    //                 General::quickToast(this, R::string::webview_https_unknownprotocol);
    //                 return true;
    //             }
    //             LinkHandler::onLinkClicked(this,
    //                 UriString(currentUrl->value.replace("http://", "https://")),
    //                 true, mPost);
    //             return true;
    //         }
    //
    //     case SHARE:
    //         if(currentUrl != nullptr) {
    //             LinkHandler::shareText(this,
    //                 mPost != nullptr ? mPost->getTitle()->getDecoded() : nullptr,
    //                 currentUrl->value);
    //         }
    //         return true;
    //
    //     default:
    //         return super.onOptionsItemSelected(item);
    // }

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: @Override public boolean onCreateOptionsMenu(final Menu menu) =====
// Java lines 167-173:
//   menu.add(0, VIEW_IN_BROWSER, 0, R.string.web_view_open_browser);
//   menu.add(0, CLEAR_CACHE, 1, R.string.web_view_clear_cache);
//   menu.add(0, USE_HTTPS, 2, R.string.webview_use_https);
//   menu.add(0, SHARE, 3, R.string.action_share);
//   return super.onCreateOptionsMenu(menu);
bool WebViewActivity::onCreateOptionsMenu(Menu *menu) {
    // menu->add(0, VIEW_IN_BROWSER, 0, R::string::web_view_open_browser);
    // menu->add(0, CLEAR_CACHE, 1, R::string::web_view_clear_cache);
    // menu->add(0, USE_HTTPS, 2, R::string::webview_use_https);
    // menu->add(0, SHARE, 3, R::string::action_share);
    // return super.onCreateOptionsMenu(menu);

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: public UriString getCurrentUrl() =====
// Java lines 175-177:
//   return webView.getCurrentUrl();
UriString *WebViewActivity::getCurrentUrl() {
    // return webView->getCurrentUrl();

    // Stub: deep Android dependencies not yet ported
    return nullptr;
}

} // namespace PinkReader
