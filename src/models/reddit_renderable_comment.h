/*
 * PinkReader - GPLv3
 * File: reddit_renderable_comment.h
 * Exact port of RedditRenderableComment.java (678 lines)
 *
 * Ports every field, method, inner class, and logic branch exactly.
 * Android-specific UI types (BetterSSB, SpannableStringBuilder, Color)
 * are forward-declared or adapted.
 */

#pragma once

#include <QString>
#include <memory>
#include <optional>
#include <functional>

// Forward declarations for types not yet fully ported
namespace PinkReader {

class RedditParsedComment;
class RedditComment;
class RedditChangeDataManager;
class RedditIdAndType;
class BetterSSB;
class RRThemeAttributes;
class BaseActivity;
class TimestampUTC;
class RedditAccount;
class OptionalInt; // Simple wrapper for optional<int>

namespace PrefsUtility {
    enum class CommentAgeMode;
    enum class AppearanceCommentHeaderItem;
}

/**
 * @brief Port of RedditRenderableComment (Java public class, 678 lines)
 *
 * Implements RedditRenderableInboxItem and RedditThingWithIdAndType.
 * Wraps a RedditParsedComment and provides methods for rendering
 * the comment header (with author flair, score, age, etc.), the
 * accessibility header, and inbox interaction handling.
 *
 * Every field, method, and logic branch from the Java original is ported exactly.
 */
class RedditRenderableComment {
public:
    // ========================================================================
    // Constructor — Java lines 63-79
    // Port of: public RedditRenderableComment(
    //   final RedditParsedComment comment,
    //   @Nullable final String parentPostAuthor,
    //   final Integer minimumCommentScore,
    //   final String currentCanonicalUserName,
    //   final boolean showScore,
    //   final boolean showSubreddit,
    //   final boolean neverAutoCollapse)
    // ========================================================================
    RedditRenderableComment(
        std::shared_ptr<RedditParsedComment> comment,
        const std::optional<QString> &parentPostAuthor,
        const std::optional<int> &minimumCommentScore,
        const QString &currentCanonicalUserName,
        bool showScore,
        bool showSubreddit,
        bool neverAutoCollapse);

    ~RedditRenderableComment() = default;

    // Disallow copy (large state), allow move
    RedditRenderableComment(const RedditRenderableComment &) = delete;
    RedditRenderableComment &operator=(const RedditRenderableComment &) = delete;
    RedditRenderableComment(RedditRenderableComment &&) = default;
    RedditRenderableComment &operator=(RedditRenderableComment &&) = default;

    // ========================================================================
    // setBlockedUser — Java lines 81-83
    // Port of: public void setBlockedUser(final boolean blocked)
    // ========================================================================
    void setBlockedUser(bool blocked);

    // ========================================================================
    // computeScore — Port of: private int computeScore(...) (Java lines 85-105)
    // ========================================================================
    /**
     * @brief Computes the effective score of this comment.
     *
     * Port of:
     *   final RedditComment rawComment = mComment.getRawComment();
     *   int score = rawComment.getUps() - rawComment.getDowns();
     *   if(Boolean.TRUE.equals(rawComment.getLikes())) { score--; }
     *   if(Boolean.FALSE.equals(rawComment.getLikes())) { score++; }
     *   if(changeDataManager.isUpvoted(getIdAndType())) { score++; }
     *   else if(changeDataManager.isDownvoted(getIdAndType())) { score--; }
     *   return score;
     */
    int computeScore(const RedditChangeDataManager &changeDataManager) const;

    // ========================================================================
    // getHeader — Port of: public BetterSSB getHeader(...) (Java lines 107-318)
    // ========================================================================
    /**
     * @brief Builds the comment header as a formatted BetterSSB.
     *
     * This is the main rendering method. It constructs a spannable string
     * with author info (color-coded for OP/mod/admin), flair, score,
     * controversiality marker, gold count, age, and subreddit.
     *
     * Every conditional branch from the Java original is preserved.
     */
    BetterSSB getHeader(
        const RRThemeAttributes &theme,
        const RedditChangeDataManager &changeDataManager,
        std::function<QString(int)> contextGetString,
        int commentAgeUnits,
        const std::optional<TimestampUTC> &postCreated,
        const std::optional<TimestampUTC> &parentCommentCreated) const;

    // ========================================================================
    // getAccessibilityHeader — Port of: public String getAccessibilityHeader(...) (Java lines 320-528)
    // ========================================================================
    /**
     * @brief Builds an accessibility-friendly text header.
     *
     * Port of the full method with all conditional branches for
     * author type (submitter/mod/admin), score, controversiality,
     * gold, age, edited state, and subreddit.
     */
    QString getAccessibilityHeader(
        const RRThemeAttributes &theme,
        const RedditChangeDataManager &changeDataManager,
        std::function<QString(int)> contextGetString,
        std::function<QString(int, int)> contextGetQuantityString,
        int commentAgeUnits,
        const std::optional<TimestampUTC> &postCreated,
        const std::optional<TimestampUTC> &parentCommentCreated,
        bool collapsed,
        const std::optional<int> &indentLevel) const;

    // ========================================================================
    // getBody — Port of: public View getBody(...) (Java lines 569-578)
    // ========================================================================
    /**
     * @brief Returns the body view for this comment.
     *
     * Port of: return mComment.getBody().generateView(activity, textColor, textSize, showLinkButtons);
     */
    void *getBody(void *activity, int textColor, float textSize, bool showLinkButtons) const;

    // ========================================================================
    // handleInboxClick — Port of: public void handleInboxClick(...) (Java lines 580-586)
    // ========================================================================
    /**
     * @brief Handles a click on this comment in the inbox.
     *
     * Port of:
     *   final UriString commentContext =
     *       Constants.Reddit.getUri(mComment.getRawComment().getContext().getDecoded());
     *   LinkHandler.onLinkClicked(activity, commentContext);
     */
    void handleInboxClick(void *activity) const;

    // ========================================================================
    // handleInboxLongClick — Port of: public void handleInboxLongClick(...) (Java lines 588-603)
    // ========================================================================
    /**
     * @brief Handles a long-click on this comment in the inbox.
     *
     * Port of:
     *   RedditChangeDataManager changeDataManager = RedditChangeDataManager.getInstance(
     *       RedditAccountManager.getInstance(activity).getDefaultAccount());
     *   RedditAPICommentAction.showActionMenu(activity, null, this, null, changeDataManager, false);
     */
    void handleInboxLongClick(void *activity) const;

    // ========================================================================
    // getIdAlone — Port of: public String getIdAlone() (Java lines 605-608)
    // ========================================================================
    QString getIdAlone() const;

    // ========================================================================
    // getIdAndType — Port of: public RedditIdAndType getIdAndType() (Java lines 610-613)
    // ========================================================================
    RedditIdAndType getIdAndType() const;

    // ========================================================================
    // getParsedComment — Port of: public RedditParsedComment getParsedComment() (Java lines 615-617)
    // ========================================================================
    std::shared_ptr<RedditParsedComment> getParsedComment() const;

    // ========================================================================
    // isCollapsed — Port of: public boolean isCollapsed(...) (Java lines 632-677)
    // ========================================================================
    /**
     * @brief Determines whether this comment should be displayed as collapsed.
     *
     * Port of the full logic:
     *   1. Check changeDataManager.isHidden()
     *   2. Always collapse blocked users
     *   3. Never collapse if mNeverAutoCollapse
     *   4. Never collapse own comments
     *   5. Check sticky comment collapse behavior (ALWAYS/ONLY_BOTS/NEVER)
     *   6. Check score below threshold
     */
    bool isCollapsed(const RedditChangeDataManager &changeDataManager) const;

private:
    // ========================================================================
    // formatAge — Port of: private String formatAge(...) (Java lines 530-567)
    // ========================================================================
    /**
     * @brief Formats the comment age according to the user's preference.
     *
     * Port of the switch statement:
     *   RELATIVE_PARENT -> relative to parent comment
     *   RELATIVE_POST -> relative to post creation
     *   ABSOLUTE -> absolute time ago from now
     */
    QString formatAge(
        std::function<QString(int, const QString &, int)> timeFormatHelper,
        PrefsUtility::CommentAgeMode commentAgeMode,
        int commentAgeUnits,
        qint64 commentTime,
        const std::optional<TimestampUTC> &postCreated,
        const std::optional<TimestampUTC> &parentCommentCreated) const;

    // ========================================================================
    // isScoreBelowThreshold — Port of: private boolean isScoreBelowThreshold(...) (Java lines 619-630)
    // ========================================================================
    /**
     * @brief Checks if the comment score is below the minimum threshold.
     *
     * Port of:
     *   if(mMinimumCommentScore == null) { return false; }
     *   if(mComment.getRawComment().getScore_hidden()) { return false; }
     *   return (computeScore(changeDataManager) < mMinimumCommentScore);
     */
    bool isScoreBelowThreshold(const RedditChangeDataManager &changeDataManager) const;

    // ========================================================================
    // Fields — Java lines 54-61
    // ========================================================================

    /** Port of: private final RedditParsedComment mComment; (Java line 54) */
    std::shared_ptr<RedditParsedComment> m_comment;

    /** Port of: @Nullable private final String mParentPostAuthor; (Java line 55) */
    std::optional<QString> m_parentPostAuthor;

    /** Port of: private final Integer mMinimumCommentScore; (Java line 56) */
    std::optional<int> m_minimumCommentScore;

    /** Port of: private final String mCurrentCanonicalUserName; (Java line 57) */
    QString m_currentCanonicalUserName;

    /** Port of: private final boolean mShowScore; (Java line 58) */
    bool m_showScore;

    /** Port of: private final boolean mShowSubreddit; (Java line 59) */
    bool m_showSubreddit;

    /** Port of: private final boolean mNeverAutoCollapse; (Java line 60) */
    bool m_neverAutoCollapse;

    /** Port of: private boolean isBlockedUser = false; (Java line 61) */
    bool m_isBlockedUser;
};

} // namespace PinkReader
