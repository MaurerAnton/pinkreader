/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: web_view_activity.h - Port of WebViewActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/WebViewActivity.java
 *
 * Original: public class WebViewActivity extends ViewsBaseActivity
 *     implements RedditPostView.PostSelectionListener
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class ViewsBaseActivity;

// Android framework stubs
class Bundle;
class Intent;
class Menu;
class MenuItem;
class View;

// RedReader fragments
class WebViewFragment;

// RedReader models
class RedditPost;
class RedditPreparedPost;
class UriString;

// Common
class General;
class LinkHandler;
class PrefsUtility;

/**
 * @brief Port of org.quantumbadger.redreader.activities.WebViewActivity
 *
 * WebView activity for displaying web content in-app.
 * Implements RedditPostView.PostSelectionListener.
 *
 * Fields:
 *   private WebViewFragment webView;
 *   public static final int VIEW_IN_BROWSER = 10;
 *   public static final int CLEAR_CACHE = 20;
 *   public static final int USE_HTTPS = 30;
 *   public static final int SHARE = 40;
 *   private RedditPost mPost;
 *
 * Methods:
 *   onCreate(Bundle)
 *   onBackPressed()
 *   onPostSelected(RedditPreparedPost)
 *   onPostCommentsSelected(RedditPreparedPost)
 *   onOptionsItemSelected(MenuItem)
 *   onCreateOptionsMenu(Menu)
 *   getCurrentUrl()
 *
 * Every field, method, constant ported exactly.
 */
class WebViewActivity : public QObject {
    Q_OBJECT

public:
    // ===== Static constants (mirrors Java exactly) =====

    // public static final int VIEW_IN_BROWSER = 10;
    static const int VIEW_IN_BROWSER;

    // public static final int CLEAR_CACHE = 20;
    static const int CLEAR_CACHE;

    // public static final int USE_HTTPS = 30;
    static const int USE_HTTPS;

    // public static final int SHARE = 40;
    static const int SHARE;

    // ===== Constructor / Lifecycle =====
    explicit WebViewActivity(QObject *parent = nullptr);
    virtual ~WebViewActivity();

    // ===== Activity lifecycle (port of overrides) =====

    // Port of: @Override public void onCreate(final Bundle savedInstanceState)
    void onCreate(Bundle *savedInstanceState);

    // Port of: @Override public void onBackPressed()
    void onBackPressed();

    // Port of: @Override public boolean onCreateOptionsMenu(final Menu menu)
    bool onCreateOptionsMenu(Menu *menu);

    // Port of: @Override public boolean onOptionsItemSelected(final MenuItem item)
    bool onOptionsItemSelected(MenuItem *item);

    // ===== Interface method stubs (port of RedditPostView.PostSelectionListener) =====

    // Port of: @Override public void onPostSelected(final RedditPreparedPost post)
    virtual void onPostSelected(RedditPreparedPost *post);

    // Port of: @Override public void onPostCommentsSelected(final RedditPreparedPost post)
    virtual void onPostCommentsSelected(RedditPreparedPost *post);

    // ===== Public methods =====

    // Port of: public UriString getCurrentUrl()
    UriString *getCurrentUrl();

private:
    // ===== Private fields (port exactly from Java) =====

    // private WebViewFragment webView;
    WebViewFragment *webView = nullptr;

    // private RedditPost mPost;
    RedditPost *mPost = nullptr;
};

} // namespace PinkReader
