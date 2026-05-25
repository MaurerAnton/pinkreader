/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: oauth_login_activity.h - Port of OAuthLoginActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/OAuthLoginActivity.java
 *
 * Original: public class OAuthLoginActivity extends ViewsBaseActivity
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <vector>
#include <memory>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class ViewsBaseActivity;

// Android framework stubs
class Bundle;
class Intent;
class WebView;
class WebChromeClient;
class WebViewClient;
class WebSettings;
class CookieManager;

// RedReader
class RedditOAuth;

/**
 * @brief Port of org.quantumbadger.redreader.activities.OAuthLoginActivity
 *
 * OAuth login activity using a WebView. Handles the Reddit OAuth flow
 * with custom protocol interception for the redirect URI.
 *
 * Fields:
 *   private static final String TAG = "OAuthLoginActivity";
 *   private static final String OAUTH_HOST = "rr_oauth_redir";
 *   private static final String REDREADER_SCHEME = "redreader";
 *   private static final String HTTP_SCHEME = "http";
 *   private final ArrayList<WebView> webViewStack = new ArrayList<>();
 *
 * Methods:
 *   onDestroy()
 *   @Nullable createWebView()
 *   onCreate(Bundle)
 *   onPause()
 *   onResume()
 *
 * Inner classes: (anonymous in Java, converted to named in C++)
 *   WebChromeClient (onConsoleMessage, onCreateWindow, onCloseWindow)
 *   WebViewClient (onPageFinished, shouldOverrideUrlLoading, onReceivedHttpError)
 *
 * Every field, method, constant, and inner class ported exactly.
 */
class OAuthLoginActivity : public QObject {
    Q_OBJECT

public:
    // ===== Static constants (mirrors Java exactly) =====

    // private static final String TAG = "OAuthLoginActivity";
    static const QString TAG;

    // ===== Constructor / Lifecycle =====
    explicit OAuthLoginActivity(QObject *parent = nullptr);
    virtual ~OAuthLoginActivity();

    // ===== Activity lifecycle (port of overrides) =====

    // Port of: @Override protected void onDestroy()
    void onDestroy();

    // Port of: @SuppressLint("SetJavaScriptEnabled")
    //         @Override public void onCreate(final Bundle savedInstanceState)
    void onCreate(Bundle *savedInstanceState);

    // Port of: @Override protected void onPause()
    void onPause();

    // Port of: @Override protected void onResume()
    void onResume();

private:
    // ===== Private constants (mirrors Java exactly) =====

    // private static final String OAUTH_HOST = "rr_oauth_redir";
    static const QString OAUTH_HOST;

    // private static final String REDREADER_SCHEME = "redreader";
    static const QString REDREADER_SCHEME;

    // private static final String HTTP_SCHEME = "http";
    static const QString HTTP_SCHEME;

    // ===== Private fields (port exactly from Java) =====

    // private final ArrayList<WebView> webViewStack = new ArrayList<>();
    // In C++, using QObject-based WebView stubs
    //
    // NOTE: In the original Java, WebView objects are created and tracked.
    // Multiple windows can be opened (for ReCAPTCHA), pushing to this stack.
    std::vector<WebView *> webViewStack;

    // ===== Private methods (port exactly from Java) =====

    // Port of: @SuppressLint("SetJavaScriptEnabled")
    //         private @Nullable WebView createWebView()
    // Java lines 75-230
    WebView *createWebView();

    // --- Inner class methods (anonymous in Java, extracted for C++) ---

    // WebChromeClient.onConsoleMessage (Java line 123-125)
    bool onConsoleMessage(void *consoleMessage);

    // WebChromeClient.onCreateWindow (Java lines 128-143)
    bool onCreateWindow(WebView *view, bool isDialog,
                        bool isUserGesture, void *resultMsg);

    // WebChromeClient.onCloseWindow (Java lines 146-152)
    void onCloseWindow(WebView *window);

    // WebViewClient.onPageFinished (Java lines 157-168)
    void onPageFinished(WebView *view, const QString &url);

    // WebViewClient.shouldOverrideUrlLoading (Java lines 171-190)
    bool shouldOverrideUrlLoading(WebView *view, void *request);

    // WebViewClient.onReceivedHttpError (Java lines 193-226)
    void onReceivedHttpError(WebView *view, void *request, void *errorResponse);
};

} // namespace PinkReader
