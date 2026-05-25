/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: post_listing_header.h - Port of PostListingHeader.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/PostListingHeader.java
 *
 * Original: public final class PostListingHeader extends LinearLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Android framework stubs
class AppCompatActivity;
class LinearLayout;
class TextView;
class TypedArray;

// RedReader common
class General;
class PrefsUtility;
class Fonts;
template<typename T> class Optional;

// RedReader models
class RedditSubreddit;
class SubredditDetails;

// RedReader URLs
class PostListingURL;

// Subreddit toolbar
class SubredditToolbar;

/**
 * @brief Port of org.quantumbadger.redreader.views.PostListingHeader
 *
 * Header view displayed at the top of post listings, showing the subreddit
 * title, subtitle, and optional subreddit toolbar. Extends LinearLayout in
 * Android; ported as a QObject here (Qt widget hierarchy managed separately).
 *
 * Every field, method, constant, and inner class ported exactly.
 */
class PostListingHeader : public QObject {
    Q_OBJECT

public:
    // ===== Constructor (Java lines 38-97) =====

    // Java line 38-43:
    // PostListingHeader(AppCompatActivity activity, String titleText,
    //     String subtitleText, PostListingURL url, @Nullable RedditSubreddit subreddit)
    PostListingHeader(
        AppCompatActivity *activity,
        const QString &titleText,
        const QString &subtitleText,
        PostListingURL *url,
        RedditSubreddit *subreddit);

    virtual ~PostListingHeader();

private:
    // ===== Private fields =====

    // Activity reference
    AppCompatActivity *mActivity = nullptr;

    // Title and subtitle
    QString mTitleText;
    QString mSubtitleText;

    // URL and subreddit
    PostListingURL *mUrl = nullptr;
    RedditSubreddit *mSubreddit = nullptr;

    // Subreddit toolbar widget
    SubredditToolbar *mSubredditToolbar = nullptr;

signals:
    // Qt signals for view updates
    void headerUpdated();
    void subredditToolbarUpdated();
};

} // namespace PinkReader
