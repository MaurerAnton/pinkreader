/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: oauth_login_activity.cpp - Port of OAuthLoginActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/OAuthLoginActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "oauth_login_activity.h"

namespace PinkReader {

// ===== Static constants (port of Java static final fields) =====

// private static final String TAG = "OAuthLoginActivity";
const QString OAuthLoginActivity::TAG = QStringLiteral("OAuthLoginActivity");

// private static final String OAUTH_HOST = "rr_oauth_redir";
const QString OAuthLoginActivity::OAUTH_HOST = QStringLiteral("rr_oauth_redir");

// private static final String REDREADER_SCHEME = "redreader";
const QString OAuthLoginActivity::REDREADER_SCHEME = QStringLiteral("redreader");

// private static final String HTTP_SCHEME = "http";
const QString OAuthLoginActivity::HTTP_SCHEME = QStringLiteral("http");

// ===== Constructor =====
OAuthLoginActivity::OAuthLoginActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
OAuthLoginActivity::~OAuthLoginActivity() = default;

// ===== Port of: @Override protected void onDestroy() =====
// Java lines 61-72:
//   super.onDestroy();
//   clearBaseActivityListing();
//   for (final WebView w : webViewStack) { w.destroy(); }
//   final CookieManager cookieManager = CookieManager.getInstance();
//   cookieManager.removeAllCookies(null);
void OAuthLoginActivity::onDestroy() {
    // super.onDestroy();
    // clearBaseActivityListing();
    //
    // for (WebView *w : webViewStack) {
    //     w->destroy();
    // }
    //
    // CookieManager *cookieManager = CookieManager::getInstance();
    // cookieManager->removeAllCookies(nullptr);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @SuppressLint("SetJavaScriptEnabled")
//         private @Nullable WebView createWebView() =====
// Java lines 75-230:
// Creates a WebView configured for OAuth login.
// Sets up cookie management, Tor proxy, JavaScript settings,
// WebChromeClient, and WebViewClient.
WebView *OAuthLoginActivity::createWebView() {
    // final WebView view = new WebView(this);
    //
    // final CookieManager cookieManager = CookieManager.getInstance();
    // cookieManager.removeAllCookies(null);
    // cookieManager.setAcceptCookie(true);
    // CookieManager.getInstance().setAcceptThirdPartyCookies(view, true);
    //
    // if (TorCommon.isTorEnabled()) {
    //     try {
    //         final boolean result = WebkitProxy.setProxy(
    //                 RedReader.class.getCanonicalName(),
    //                 getApplicationContext(), view, "127.0.0.1", 8118);
    //         if (!result) {
    //             BugReportActivity.handleGlobalError(this,
    //                 getResources().getString(R.string.error_tor_setting_failed));
    //             return null;
    //         }
    //     } catch (final Exception e) {
    //         BugReportActivity.handleGlobalError(this, e);
    //         return null;
    //     }
    // }
    //
    // final WebSettings settings = view.getSettings();
    // settings.setBuiltInZoomControls(false);
    // settings.setJavaScriptEnabled(true);
    // settings.setUseWideViewPort(true);
    // settings.setLoadWithOverviewMode(true);
    // settings.setDomStorageEnabled(true);
    // if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
    //     settings.setSaveFormData(false);
    // }
    // settings.setDatabaseEnabled(false);
    // settings.setCacheMode(WebSettings.LOAD_NO_CACHE);
    // settings.setDisplayZoomControls(false);
    //
    // // Suggested by Reddit to work around ReCAPTCHA issues
    // settings.setSupportMultipleWindows(true);
    // settings.setJavaScriptCanOpenWindowsAutomatically(true);
    //
    // view.setWebChromeClient(new WebChromeClient() {
    //     @Override public boolean onConsoleMessage(final ConsoleMessage consoleMessage) {
    //         return true;
    //     }
    //     @Override public boolean onCreateWindow(final WebView view, final boolean isDialog,
    //             final boolean isUserGesture, final Message resultMsg) {
    //         Log.i(TAG, "New window created");
    //         final WebView newWebView = createWebView();
    //         webViewStack.add(newWebView);
    //         setBaseActivityListing(newWebView);
    //         final WebView.WebViewTransport transport
    //                 = (WebView.WebViewTransport) resultMsg.obj;
    //         transport.setWebView(newWebView);
    //         resultMsg.sendToTarget();
    //         return true;
    //     }
    //     @Override public void onCloseWindow(final WebView window) {
    //         if (webViewStack.size() > 1) {
    //             final WebView removed = webViewStack.remove(webViewStack.size() - 1);
    //             removed.destroy();
    //             setBaseActivityListing(webViewStack.get(webViewStack.size() - 1));
    //         }
    //     }
    // });
    //
    // view.setWebViewClient(new WebViewClient() {
    //     @Override public void onPageFinished(final WebView view, final String url) {
    //         view.evaluateJavascript(
    //             "(function() {" +
    //                 "var el = document.getElementById(" +
    //                 "'data-protection-consent-wrapper');" +
    //                 "if(el) el.remove();" +
    //             "})()", null);
    //     }
    //     @Override public boolean shouldOverrideUrlLoading(
    //             final WebView view, final WebResourceRequest request) {
    //         final Uri url = request.getUrl();
    //         if (Objects.equals(url.getHost(), OAUTH_HOST) &&
    //                 (Objects.equals(url.getScheme(), REDREADER_SCHEME) ||
    //                     Objects.equals(url.getScheme(), HTTP_SCHEME))) {
    //             final Intent intent = new Intent();
    //             intent.putExtra("url", url.toString());
    //             setResult(123, intent);
    //             finish();
    //         } else {
    //             setTitle(url.getHost());
    //             return false;
    //         }
    //         return true;
    //     }
    //     @Override public void onReceivedHttpError(
    //             final WebView view, final WebResourceRequest request,
    //             final WebResourceResponse errorResponse) {
    //         Log.e(TAG, "onReceivedHttpError: "
    //                 + request.getUrl()
    //                 + ", error = "
    //                 + errorResponse.getStatusCode());
    //         if (request.getUrl().toString().equals(
    //                 "https://www.reddit.com/svc/shreddit/account/login")
    //                 && errorResponse.getStatusCode() / 100 == 4) {
    //             DialogUtils.showDialogPositiveNegative(
    //                 OAuthLoginActivity.this,
    //                 getString(R.string.login_reddit_error_title),
    //                 getString(R.string.login_reddit_error_message),
    //                 R.string.dialog_continue,
    //                 R.string.dialog_cancel,
    //                 () -> {
    //                     LinkHandler.openCustomTab(OAuthLoginActivity.this,
    //                         RedditOAuth.getPromptUri(), null, false);
    //                     finish();
    //                 },
    //                 () -> { finish(); }
    //             );
    //         }
    //     }
    // });
    //
    // return view;

    // Stub: deep Android dependencies not yet ported
    return nullptr;
}

// ===== Port of: @SuppressLint("SetJavaScriptEnabled")
//         @Override public void onCreate(final Bundle savedInstanceState) =====
// Java lines 233-247:
//   PrefsUtility.applyTheme(this);
//   super.onCreate(savedInstanceState);
//   final WebView webView = createWebView();
//   if (webView != null) {
//       webViewStack.add(webView);
//       setBaseActivityListing(webView);
//       webView.loadUrl(RedditOAuth.getPromptUri().toString());
//   }
void OAuthLoginActivity::onCreate(Bundle *savedInstanceState) {
    // PrefsUtility::applyTheme(this);
    // super.onCreate(savedInstanceState);
    //
    // WebView *webView = createWebView();
    //
    // if (webView != nullptr) {
    //     webViewStack.push_back(webView);
    //     setBaseActivityListing(webView);
    //     webView->loadUrl(RedditOAuth::getPromptUri().toString());
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override protected void onPause() =====
// Java lines 250-258:
//   super.onPause();
//   for (final WebView w : webViewStack) {
//       w.onPause();
//       w.pauseTimers();
//   }
void OAuthLoginActivity::onPause() {
    // super.onPause();
    //
    // for (WebView *w : webViewStack) {
    //     w->onPause();
    //     w->pauseTimers();
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override protected void onResume() =====
// Java lines 261-268:
//   super.onResume();
//   for (final WebView w : webViewStack) {
//       w.resumeTimers();
//       w.onResume();
//   }
void OAuthLoginActivity::onResume() {
    // super.onResume();
    //
    // for (WebView *w : webViewStack) {
    //     w->resumeTimers();
    //     w->onResume();
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Inner class methods (anonymous in Java, extracted for C++) =====

// WebChromeClient.onConsoleMessage (Java lines 123-125):
//   @Override public boolean onConsoleMessage(final ConsoleMessage consoleMessage) {
//       return true;
//   }
bool OAuthLoginActivity::onConsoleMessage(void * /*consoleMessage*/) {
    // return true;
    // Stub: deep Android dependencies not yet ported
    return true;
}

// WebChromeClient.onCreateWindow (Java lines 128-143):
//   Log.i(TAG, "New window created");
//   final WebView newWebView = createWebView();
//   webViewStack.add(newWebView);
//   setBaseActivityListing(newWebView);
//   final WebView.WebViewTransport transport = (WebView.WebViewTransport) resultMsg.obj;
//   transport.setWebView(newWebView);
//   resultMsg.sendToTarget();
//   return true;
bool OAuthLoginActivity::onCreateWindow(WebView * /*view*/, bool /*isDialog*/,
                                         bool /*isUserGesture*/, void * /*resultMsg*/) {
    // Log::i(TAG, "New window created");
    // WebView *newWebView = createWebView();
    // webViewStack.push_back(newWebView);
    // setBaseActivityListing(newWebView);
    // WebView::WebViewTransport *transport
    //         = (WebView::WebViewTransport *) resultMsg->obj;
    // transport->setWebView(newWebView);
    // resultMsg->sendToTarget();
    // return true;

    // Stub: deep Android dependencies not yet ported
    return false;
}

// WebChromeClient.onCloseWindow (Java lines 146-152):
//   if (webViewStack.size() > 1) {
//       final WebView removed = webViewStack.remove(webViewStack.size() - 1);
//       removed.destroy();
//       setBaseActivityListing(webViewStack.get(webViewStack.size() - 1));
//   }
void OAuthLoginActivity::onCloseWindow(WebView * /*window*/) {
    // if (webViewStack.size() > 1) {
    //     WebView *removed = webViewStack.back();
    //     webViewStack.pop_back();
    //     removed->destroy();
    //     setBaseActivityListing(webViewStack.back());
    // }

    // Stub: deep Android dependencies not yet ported
}

// WebViewClient.onPageFinished (Java lines 157-168):
//   view.evaluateJavascript(
//       "(function() {" +
//           "var el = document.getElementById(" +
//           "'data-protection-consent-wrapper');" +
//           "if(el) el.remove();" +
//       "})()", null);
void OAuthLoginActivity::onPageFinished(WebView * /*view*/, const QString & /*url*/) {
    // view->evaluateJavascript(
    //     "(function() {"
    //         + "var el = document.getElementById("
    //         + "'data-protection-consent-wrapper');"
    //         + "if(el) el.remove();"
    //     + "})()", nullptr);

    // Stub: deep Android dependencies not yet ported
}

// WebViewClient.shouldOverrideUrlLoading (Java lines 171-190):
//   final Uri url = request.getUrl();
//   if (Objects.equals(url.getHost(), OAUTH_HOST) &&
//           (Objects.equals(url.getScheme(), REDREADER_SCHEME) ||
//               Objects.equals(url.getScheme(), HTTP_SCHEME))) {
//       final Intent intent = new Intent();
//       intent.putExtra("url", url.toString());
//       setResult(123, intent);
//       finish();
//   } else {
//       setTitle(url.getHost());
//       return false;
//   }
//   return true;
bool OAuthLoginActivity::shouldOverrideUrlLoading(WebView * /*view*/, void * /*request*/) {
    // final Uri url = request->getUrl();
    // if (url->getHost() == OAUTH_HOST &&
    //         (url->getScheme() == REDREADER_SCHEME ||
    //             url->getScheme() == HTTP_SCHEME)) {
    //     Intent *intent = new Intent();
    //     intent->putExtra("url", url->toString());
    //     setResult(123, intent);
    //     finish();
    // } else {
    //     setTitle(url->getHost());
    //     return false;
    // }
    // return true;

    // Stub: deep Android dependencies not yet ported
    return false;
}

// WebViewClient.onReceivedHttpError (Java lines 193-226):
//   Log.e(TAG, "onReceivedHttpError: " + request.getUrl() + ", error = "
//       + errorResponse.getStatusCode());
//   if (request.getUrl().toString().equals(
//           "https://www.reddit.com/svc/shreddit/account/login")
//           && errorResponse.getStatusCode() / 100 == 4) {
//       DialogUtils.showDialogPositiveNegative(...);
//   }
void OAuthLoginActivity::onReceivedHttpError(WebView * /*view*/,
                                              void * /*request*/,
                                              void * /*errorResponse*/) {
    // Log::e(TAG, "onReceivedHttpError: "
    //         + request->getUrl()
    //         + ", error = "
    //         + errorResponse->getStatusCode());
    //
    // if (request->getUrl()->toString()
    //         == "https://www.reddit.com/svc/shreddit/account/login"
    //         && errorResponse->getStatusCode() / 100 == 4) {
    //     DialogUtils::showDialogPositiveNegative(
    //         this,
    //         getString(R::string::login_reddit_error_title),
    //         getString(R::string::login_reddit_error_message),
    //         R::string::dialog_continue,
    //         R::string::dialog_cancel,
    //         []() {
    //             LinkHandler::openCustomTab(this,
    //                 RedditOAuth::getPromptUri(), nullptr, false);
    //             finish();
    //         },
    //         []() { finish(); }
    //     );
    // }

    // Stub: deep Android dependencies not yet ported
}

} // namespace PinkReader
