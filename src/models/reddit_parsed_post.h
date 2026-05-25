/*
 * PinkReader - GPLv3
 * File: reddit_parsed_post.h
 * Exact port of RedditParsedPost.kt (220 lines)
 *
 * Ports every field, method, inner class from the Kotlin original exactly.
 */

#pragma once

#include <QString>
#include <QSharedPointer>
#include <memory>
#include <optional>
#include <vector>

// Forward declarations for types not yet fully ported
namespace PinkReader {

class RedditPost;
class BodyElement;
class UriString;

/**
 * @brief Port of PostCommentSort enum used in RedditParsedPost
 */
namespace PostCommentSort {
    enum Value {
        CONFIDENCE,
        TOP,
        NEW,
        CONTROVERSIAL,
        OLD,
        QA,
        LIVE,
        BLANK,
        UNKNOWN
    };
    Value lookup(const QString &sortStr);
}

/**
 * @brief Port of RedditParsedPost (Kotlin data class, 220 lines)
 *
 * Parsed version of a RedditPost. Wraps the raw RedditPost and provides
 * convenience properties for title, URL, self-text, flair, preview images,
 * and metadata.
 *
 * Implements RedditThingWithIdAndType.
 *
 * Every field, property, method, and inner class is ported exactly.
 */
class RedditParsedPost {
public:
    // ========================================================================
    // ImagePreviewDetails — Port of: data class ImagePreviewDetails (Kotlin lines 140-144)
    // ========================================================================
    /**
     * @brief Port of: data class ImagePreviewDetails(
     *           @JvmField val url: UriString,
     *           @JvmField val width: Int,
     *           @JvmField val height: Int)
     */
    struct ImagePreviewDetails {
        /** Port of: @JvmField val url: UriString */
        std::shared_ptr<UriString> url;

        /** Port of: @JvmField val width: Int */
        int width = 0;

        /** Port of: @JvmField val height: Int */
        int height = 0;

        ImagePreviewDetails() = default;
        ImagePreviewDetails(std::shared_ptr<UriString> u, int w, int h)
            : url(std::move(u)), width(w), height(h) {}
    };

    // ========================================================================
    // Constructor — Kotlin lines 29-33
    // Port of: class RedditParsedPost(
    //   activity: AppCompatActivity,
    //   val src: RedditPost,
    //   parseSelfText: Boolean
    // ) : RedditThingWithIdAndType
    // ========================================================================
    /**
     * @brief Constructs a RedditParsedPost from a raw RedditPost.
     *
     * The init block (Kotlin lines 109-126) is executed during construction:
     *   url = src.findUrl()
     *   selfText = if(parseSelfText && src.selftext_html != null) ...
     *   flairText = if(src.link_flair_text != null && ...) ...
     *   domain = src.domain?.decoded ?: activity.getString(R.string.post_domain_deleted)
     */
    RedditParsedPost(
        void *activity,
        std::shared_ptr<RedditPost> src,
        bool parseSelfText);

    ~RedditParsedPost() = default;

    // Allow copy and move
    RedditParsedPost(const RedditParsedPost &) = default;
    RedditParsedPost &operator=(const RedditParsedPost &) = default;
    RedditParsedPost(RedditParsedPost &&) = default;
    RedditParsedPost &operator=(RedditParsedPost &&) = default;

    // ========================================================================
    // Properties — Kotlin lines 35-107
    // ========================================================================

    /** Port of: val title: String = src.title?.decoded?.replace('\n', ' ')?.trim() ?: "[null]" (line 35) */
    const QString &title() const { return m_title; }

    /** Port of: val url: UriString? (line 37) */
    std::shared_ptr<UriString> url() const { return m_url; }

    /** Port of: val selfText: BodyElement? (line 38) */
    std::shared_ptr<BodyElement> selfText() const { return m_selfText; }

    /** Port of: val flairText: String? (line 39) */
    const std::optional<QString> &flairText() const { return m_flairText; }

    /** Port of: val domain: String (line 40) */
    const QString &domain() const { return m_domain; }

    /** Port of: val permalink = src.permalink.decoded (line 42) */
    const QString &permalink() const { return m_permalink; }

    /** Port of: val isStickied = src.stickied (line 43) */
    bool isStickied() const { return m_isStickied; }

    /** Port of: val thumbnailUrl = UriString.fromNullable(src.thumbnail?.decoded) (line 44) */
    std::shared_ptr<UriString> thumbnailUrl() const { return m_thumbnailUrl; }

    /** Port of: val isPreviewEnabled = src.preview?.enabled == true (line 46) */
    bool isPreviewEnabled() const { return m_isPreviewEnabled; }

    /** Port of: val isVideoPreview = src.preview?.run { ... } ?: false (lines 48-56) */
    bool isVideoPreview() const { return m_isVideoPreview; }

    /** Port of: val suggestedCommentSort: PostCommentSort? (lines 58-61)
     *  get() = if(src.suggested_sort == null) null else PostCommentSort.lookup(src.suggested_sort) */
    std::optional<PostCommentSort::Value> suggestedCommentSort() const { return m_suggestedCommentSort; }

    /** Port of: val isArchived = src.archived (line 63) */
    bool isArchived() const { return m_isArchived; }

    /** Port of: val isLocked = src.locked (line 65) */
    bool isLocked() const { return m_isLocked; }

    /** Port of: val canModerate = src.can_mod_post (line 67) */
    bool canModerate() const { return m_canModerate; }

    /** Port of: val author: String? get() = src.author?.decoded (lines 69-70) */
    const std::optional<QString> &author() const { return m_author; }

    /** Port of: val distinguished: String? get() = src.distinguished (lines 72-73) */
    const std::optional<QString> &distinguished() const { return m_distinguished; }

    /** Port of: val rawSelfTextMarkdown = src.selftext?.decoded (line 75) */
    const std::optional<QString> &rawSelfTextMarkdown() const { return m_rawSelfTextMarkdown; }

    /** Port of: val isSpoiler = src.spoiler (line 77) */
    bool isSpoiler() const { return m_isSpoiler; }

    /** Port of: val unescapedSelfText = src.selftext?.decoded (line 79) */
    const std::optional<QString> &unescapedSelfText() const { return m_unescapedSelfText; }

    /** Port of: val subreddit = src.subreddit.decoded (line 81) */
    const QString &subreddit() const { return m_subreddit; }

    /** Port of: val scoreExcludingOwnVote: Int get() = { ... } (lines 84-93) */
    int scoreExcludingOwnVote() const { return m_scoreExcludingOwnVote; }

    /** Port of: val commentCount = src.num_comments (line 95) */
    int commentCount() const { return m_commentCount; }

    /** Port of: val goldAmount = src.gilded (line 97) */
    int goldAmount() const { return m_goldAmount; }

    /** Port of: val isCrosspost = src.crosspost_parent (line 99) */
    bool isCrosspost() const { return m_isCrosspost; }

    /** Port of: val isNsfw = src.over_18 (line 101) */
    bool isNsfw() const { return m_isNsfw; }

    /** Port of: val createdTimeUTC = src.created_utc.value (line 103) */
    qint64 createdTimeUTC() const { return m_createdTimeUTC; }

    /** Port of: val isSelfPost = src.is_self (line 105) */
    bool isSelfPost() const { return m_isSelfPost; }

    /** Port of: val upvotePercentage = src.upvote_ratio?.times(100.0)?.toInt() (line 107) */
    std::optional<int> upvotePercentage() const { return m_upvotePercentage; }

    // ========================================================================
    // Methods — Kotlin lines 128-218
    // ========================================================================

    /** Port of: override fun getIdAlone(): String = src.idAlone (lines 128-130) */
    QString getIdAlone() const;

    /** Port of: override fun getIdAndType(): RedditIdAndType = src.idAndType (lines 132-134) */
    class RedditIdAndType getIdAndType() const;

    /** Port of: fun hasSelfText(): Boolean = rawSelfTextMarkdown != null
     *            && rawSelfTextMarkdown.length > 1 (lines 136-138) */
    bool hasSelfText() const;

    /** Port of: fun getPreview(minWidth: Int, minHeight: Int) =
     *            src.preview?.images?.get(0)?.run { getPreviewInternal(this, minWidth, minHeight) }
     *            (lines 146-148) */
    std::shared_ptr<ImagePreviewDetails> getPreview(int minWidth, int minHeight) const;

    /** Port of: fun getPreviewMP4(minWidth: Int, minHeight: Int) =
     *            src.preview?.images?.get(0)?.variants?.mp4?.apply {
     *              getPreviewInternal(this, minWidth, minHeight) }
     *            (lines 150-153) */
    std::shared_ptr<ImagePreviewDetails> getPreviewMP4(int minWidth, int minHeight) const;

    /** @brief Access to the underlying raw RedditPost */
    std::shared_ptr<RedditPost> getSrc() const { return m_src; }

private:
    // ========================================================================
    // Port of: private fun getPreviewInternal(image, minWidth, minHeight)
    // (Kotlin lines 155-219)
    // ========================================================================
    /**
     * @brief Finds the best preview image resolution.
     *
     * Port of the full logic:
     *   Iterates through resolutions (including source),
     *   filters out tiny images (<50px),
     *   filters out overly tall estimated images (>3000px),
     *   picks the best-fitting resolution based on minWidth/minHeight.
     */
    std::shared_ptr<ImagePreviewDetails> getPreviewInternal(
        const void *image, int minWidth, int minHeight) const;

    // ========================================================================
    // Fields — initialized in constructor from src
    // ========================================================================

    /** Port of: val src: RedditPost (line 31) */
    std::shared_ptr<RedditPost> m_src;

    /** Port of: val title (line 35) */
    QString m_title;

    /** Port of: val url: UriString? (line 37) */
    std::shared_ptr<UriString> m_url;

    /** Port of: val selfText: BodyElement? (line 38) */
    std::shared_ptr<BodyElement> m_selfText;

    /** Port of: val flairText: String? (line 39) */
    std::optional<QString> m_flairText;

    /** Port of: val domain: String (line 40) */
    QString m_domain;

    /** Port of: val permalink (line 42) */
    QString m_permalink;

    /** Port of: val isStickied (line 43) */
    bool m_isStickied = false;

    /** Port of: val thumbnailUrl (line 44) */
    std::shared_ptr<UriString> m_thumbnailUrl;

    /** Port of: val isPreviewEnabled (line 46) */
    bool m_isPreviewEnabled = false;

    /** Port of: val isVideoPreview (lines 48-56) */
    bool m_isVideoPreview = false;

    /** Port of: val suggestedCommentSort (lines 58-61) */
    std::optional<PostCommentSort::Value> m_suggestedCommentSort;

    /** Port of: val isArchived (line 63) */
    bool m_isArchived = false;

    /** Port of: val isLocked (line 65) */
    bool m_isLocked = false;

    /** Port of: val canModerate (line 67) */
    bool m_canModerate = false;

    /** Port of: val author (lines 69-70) */
    std::optional<QString> m_author;

    /** Port of: val distinguished (lines 72-73) */
    std::optional<QString> m_distinguished;

    /** Port of: val rawSelfTextMarkdown (line 75) */
    std::optional<QString> m_rawSelfTextMarkdown;

    /** Port of: val isSpoiler (line 77) */
    bool m_isSpoiler = false;

    /** Port of: val unescapedSelfText (line 79) */
    std::optional<QString> m_unescapedSelfText;

    /** Port of: val subreddit (line 81) */
    QString m_subreddit;

    /** Port of: val scoreExcludingOwnVote (lines 84-93) */
    int m_scoreExcludingOwnVote = 0;

    /** Port of: val commentCount (line 95) */
    int m_commentCount = 0;

    /** Port of: val goldAmount (line 97) */
    int m_goldAmount = 0;

    /** Port of: val isCrosspost (line 99) */
    bool m_isCrosspost = false;

    /** Port of: val isNsfw (line 101) */
    bool m_isNsfw = false;

    /** Port of: val createdTimeUTC (line 103) */
    qint64 m_createdTimeUTC = 0;

    /** Port of: val isSelfPost (line 105) */
    bool m_isSelfPost = false;

    /** Port of: val upvotePercentage (line 107) */
    std::optional<int> m_upvotePercentage;
};

} // namespace PinkReader
