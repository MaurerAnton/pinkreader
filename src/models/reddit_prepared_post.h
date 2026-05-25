/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: RedditPreparedPost.java
 * File: reddit_prepared_post.h - Prepared Reddit post with thumbnails, subtitles, change tracking
 */

#pragma once

#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QImage>
#include <QColor>
#include <functional>
#include <memory>
#include <set>
#include <atomic>

// Forward declarations for types not yet fully ported
namespace PinkReader {

class RedditParsedPost;
class RedditChangeDataManager;
class RedditPostView;
class BetterSSB;
class RedditIdAndType;
class TimestampUTC;
class CacheManager;
class RedditAccount;
class GenericFactory;
class SeekableInputStream;

// --- Enum ports from PrefsUtility ---

/** Appearance subtitle items (mirrors PrefsUtility.AppearancePostSubtitleItem) */
enum class AppearancePostSubtitleItem {
    SPOILER,
    STICKY,
    CROSSPOST,
    NSFW,
    FLAIR,
    COMMENTS,
    SCORE,
    UPVOTE_RATIO,
    GOLD,
    AGE,
    AUTHOR,
    SUBREDDIT,
    DOMAIN
};

/** Post fling actions */
enum class PostFlingAction {
    UPVOTE,
    DOWNVOTE,
    SAVE,
    HIDE,
    COMMENTS,
    LINK,
    DISABLED
};

// --- Forward-declared listener interface ---

/**
 * @brief Callback for when a better thumbnail is loaded (async).
 * Mirrors: RedditPreparedPost.ThumbnailLoadedCallback interface
 */
class ThumbnailLoadedCallback {
public:
    virtual ~ThumbnailLoadedCallback() = default;
    virtual void betterThumbnailAvailable(const QImage &thumbnail, int usageId) = 0;
};

/**
 * @brief Port of RedditPreparedPost.java
 *
 * Wraps a RedditParsedPost with change tracking, thumbnail management,
 * inline preview control, subtitle building, and accessibility support.
 * Implements RedditChangeDataManager.Listener.
 */
class RedditPreparedPost {
public:
    // --- Constants ---
    static constexpr const char *TAG = "RedditPreparedPost";

    // --- Public fields (mirror Java exactly) ---
    QSharedPointer<RedditParsedPost> src;
    TimestampUTC lastChange;
    bool showSubreddit;
    bool isArchived;
    bool isLocked;
    bool canModerate;
    bool hasThumbnail;
    bool mIsProbablyAnImage;

    // --- Constructor ---
    /**
     * Port of:
     * public RedditPreparedPost(Context, CacheManager, int listId,
     *     RedditParsedPost, TimestampUTC, boolean showSubreddit,
     *     boolean showThumbnails, boolean allowHighResThumbnails,
     *     boolean showInlinePreviews)
     */
    RedditPreparedPost(
        QObject *context,
        CacheManager *cm,
        int listId,
        QSharedPointer<RedditParsedPost> post,
        const TimestampUTC &timestamp,
        bool showSubreddit_,
        bool showThumbnails,
        bool allowHighResThumbnails,
        bool showInlinePreviews);

    ~RedditPreparedPost();

    // --- Public methods (mirror Java methods exactly) ---

    /** @return true if inline preview should be shown for this post */
    bool shouldShowInlinePreview() const;

    /** @return true if this post has a video preview */
    bool isVideoPreview() const;

    /** @return computed score (excluding own vote then adding vote direction) */
    int computeScore() const;

    /**
     * Build formatted subtitle with colored spans.
     * @param context App context
     * @param headerMode Whether in header display mode
     * @return QSharedPointer to BetterSSB with formatted subtitle
     */
    QSharedPointer<BetterSSB> buildSubtitle(QObject *context, bool headerMode);

    /**
     * Build accessibility subtitle string.
     * @param context App context
     * @param headerMode Whether in header display mode
     * @return Accessibility string
     */
    QString buildAccessibilitySubtitle(QObject *context, bool headerMode);

    /**
     * Build accessibility title string.
     * @param context App context
     * @param headerMode Whether in header display mode
     * @return Accessibility title string
     */
    QString buildAccessibilityTitle(QObject *context, bool headerMode);

    /**
     * Build accessibility embellishments for screen readers.
     * @param context App context
     * @param headerMode Whether in header display mode
     * @return Embellishments string
     */
    QString buildAccessibilityEmbellishments(QObject *context, bool headerMode);

    /**
     * Get the thumbnail bitmap, registering a callback for when a better one loads.
     * Port of: public Bitmap getThumbnail(ThumbnailLoadedCallback, int usageId)
     */
    QImage getThumbnail(ThumbnailLoadedCallback *callback, int usageId);

    /** @return true if this is a self/text post */
    bool isSelf() const;

    /** @return true if this post has been marked read */
    bool isRead() const;

    /** Bind to a RedditPostView, listening for change data events */
    void bind(RedditPostView *boundView);

    /** Unbind from a RedditPostView */
    void unbind(RedditPostView *boundView);

    /** Mark post as read (respects user preference) */
    void markAsRead(QObject *context);

    /** Mark post as read/unread explicitly */
    void markAsRead(QObject *context, bool read);

    /** @return true if upvoted by current user */
    bool isUpvoted() const;

    /** @return true if downvoted by current user */
    bool isDownvoted() const;

    /** @return -1 (downvoted), 0 (neutral), 1 (upvoted) */
    int getVoteDirection() const;

    /** @return true if saved by current user */
    bool isSaved() const;

    /** @return true if hidden by current user */
    bool isHidden() const;

    /**
     * Perform a post action (upvote, save, hide, etc.)
     * Port of: public void performAction(BaseActivity, RedditPostActions.Action)
     */
    void performAction(QObject *activity, int action);

    /**
     * Called by RedditChangeDataManager when data changes.
     * Port of: public void onRedditDataChange(RedditIdAndType)
     */
    void onRedditDataChange(const RedditIdAndType &thingIdAndType);

private:
    // --- Private fields (mirror Java exactly) ---
    QSharedPointer<RedditChangeDataManager> m_changeDataManager;
    bool m_showInlinePreviews;
    QImage m_thumbnailCache;
    ThumbnailLoadedCallback *m_thumbnailCallback = nullptr;
    int m_usageId = -1;
    RedditPostView *m_boundView = nullptr;

    // --- Private methods (mirror Java exactly) ---

    /** Check if a post has a valid thumbnail URL */
    static bool hasThumbnail(const RedditParsedPost &post);

    /** Download thumbnail asynchronously */
    void downloadThumbnail(
        QObject *context,
        bool allowHighRes,
        int sizePixels,
        CacheManager *cm,
        int listId);

    /** Handle available thumbnail stream */
    void onThumbnailStreamAvailable(
        GenericFactory *factory,
        int desiredSizePixels);
};

} // namespace PinkReader
