/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: RedditPreparedPost.java
 * File: reddit_prepared_post.cpp - Prepared post implementation
 */

#include "reddit_prepared_post.h"

// Stub includes for dependencies not yet fully ported
#include "../common/better_ssb.h"
#include "../common/general.h"
#include "../common/constants.h"
#include "../common/link_handler.h"
#include "../common/timestamp_utc.h"
#include "../common/time_format_helper.h"
#include "../common/screenreader_pronunciation.h"
#include "../settings/prefs_utility.h"
#include "../cache/cache_manager.h"
#include "../cache/cache_request.h"
#include "../accounts/reddit_account_manager.h"
#include "../accounts/reddit_account.h"
#include "reddit_change_data_manager.h"
#include "reddit_parsed_post.h"

#include <QDebug>
#include <QImage>
#include <QColor>
#include <QStringBuilder>
#include <set>

namespace PinkReader {

// --- Constructor ---
RedditPreparedPost::RedditPreparedPost(
    QObject *context,
    CacheManager *cm,
    int listId,
    QSharedPointer<RedditParsedPost> post,
    const TimestampUTC &timestamp,
    bool showSubreddit_,
    bool showThumbnails,
    bool allowHighResThumbnails,
    bool showInlinePreviews)
    : src(post)
    , showSubreddit(showSubreddit_)
    , m_showInlinePreviews(showInlinePreviews)
{
    // final RedditAccount user = RedditAccountManager.getInstance(context).getDefaultAccount();
    const auto &user = RedditAccountManager::instance()->defaultAccount();
    m_changeDataManager = RedditChangeDataManager::getInstance(user);

    // isArchived = post.isArchived();
    isArchived = post->isArchived();
    // isLocked = post.isLocked();
    isLocked = post->isLocked();
    // canModerate = post.getCanModerate();
    canModerate = post->canModerate();

    // mIsProbablyAnImage = LinkHandler.isProbablyAnImage(post.getUrl());
    mIsProbablyAnImage = LinkHandler::isProbablyAnImage(post->url ? post->url->value : QString());

    // hasThumbnail = showThumbnails && hasThumbnail(post);
    hasThumbnail = showThumbnails && hasThumbnail(*post);

    // final int thumbnailWidth = General.dpToPixels(context, PrefsUtility.images_thumbnail_size_dp());
    const int thumbnailWidth = General::dpToPixels(context, PrefsUtility::imagesThumbnailSizeDp());

    // if(hasThumbnail && hasThumbnail(post) && !shouldShowInlinePreview())
    if (hasThumbnail && hasThumbnail(*post) && !shouldShowInlinePreview()) {
        downloadThumbnail(context, allowHighResThumbnails, thumbnailWidth, cm, listId);
    }

    // lastChange = timestamp;
    // mChangeDataManager.update(timestamp, post.getSrc());
    lastChange = timestamp;
    m_changeDataManager->update(timestamp, post->src);
}

RedditPreparedPost::~RedditPreparedPost() = default;

// --- shouldShowInlinePreview ---
bool RedditPreparedPost::shouldShowInlinePreview() const {
    // return mShowInlinePreviews && (src.isPreviewEnabled()
    //     || "gfycat.com".equals(src.getDomain())
    //     || "i.imgur.com".equals(src.getDomain())
    //     || "streamable.com".equals(src.getDomain())
    //     || "i.redd.it".equals(src.getDomain())
    //     || "v.redd.it".equals(src.getDomain()));
    if (!m_showInlinePreviews) return false;
    if (src->isPreviewEnabled) return true;
    const QString &domain = src->domain;
    return domain == QStringLiteral("gfycat.com")
        || domain == QStringLiteral("i.imgur.com")
        || domain == QStringLiteral("streamable.com")
        || domain == QStringLiteral("i.redd.it")
        || domain == QStringLiteral("v.redd.it");
}

// --- isVideoPreview ---
bool RedditPreparedPost::isVideoPreview() const {
    return src->isVideoPreview;
}

// --- computeScore ---
int RedditPreparedPost::computeScore() const {
    // int score = src.getScoreExcludingOwnVote();
    int score = src->scoreExcludingOwnVote;

    // if(isUpvoted()) score++; else if(isDownvoted()) score--;
    if (isUpvoted()) {
        score++;
    } else if (isDownvoted()) {
        score--;
    }
    return score;
}

// --- hasThumbnail (static) ---
bool RedditPreparedPost::hasThumbnail(const RedditParsedPost &post) {
    // final UriString url = post.getThumbnailUrl();
    // return url != null && !url.value.isEmpty()
    //     && !url.value.equalsIgnoreCase("nsfw")
    //     && !url.value.equalsIgnoreCase("self")
    //     && !url.value.equalsIgnoreCase("default");
    const QString &url = post.thumbnailUrl ? post.thumbnailUrl->value : QString();
    if (url.isEmpty()) return false;
    const QString lower = url.toLower();
    return lower != QStringLiteral("nsfw")
        && lower != QStringLiteral("self")
        && lower != QStringLiteral("default");
}

// --- buildSubtitle ---
QSharedPointer<BetterSSB> RedditPreparedPost::buildSubtitle(QObject *context, bool headerMode) {
    // Determine which subtitle items and age units to use
    std::set<AppearancePostSubtitleItem> postSubtitleItems;
    int postAgeUnits;

    if (headerMode && PrefsUtility::appearancePostSubtitleItemsUseDifferentSettings()) {
        postSubtitleItems = PrefsUtility::appearancePostHeaderSubtitleItems();
        postAgeUnits = PrefsUtility::appearancePostHeaderAgeUnits();
    } else {
        postSubtitleItems = PrefsUtility::appearancePostSubtitleItems();
        postAgeUnits = PrefsUtility::appearancePostAgeUnits();
    }

    // final TypedArray appearance = context.obtainStyledAttributes(...)
    // Color values from theme:
    const int boldCol = headerMode ? 0xFFFFFFFF : 0xFF000000; // TODO: from theme
    const int rrPostSubtitleUpvoteCol = 0xFFFF8B60;   // orangered
    const int rrPostSubtitleDownvoteCol = 0xFF9494FF;  // periwinkle
    const int rrFlairBackCol = 0xFFE0E0E0;
    const int rrFlairTextCol = 0xFF000000;
    const int rrGoldTextCol = 0xFF8B6914;
    const int rrGoldBackCol = 0xFFFFD700;
    const int rrCrosspostTextCol = 0xFFFFFFFF;
    const int rrCrosspostBackCol = 0xFF1A4B7C;

    auto postListDescSb = QSharedPointer<BetterSSB>::create();

    int pointsCol;
    const int score = computeScore();

    if (isUpvoted()) {
        pointsCol = rrPostSubtitleUpvoteCol;
    } else if (isDownvoted()) {
        pointsCol = rrPostSubtitleDownvoteCol;
    } else {
        pointsCol = boldCol;
    }

    // SPOILER tag
    if (postSubtitleItems.count(AppearancePostSubtitleItem::SPOILER)) {
        if (src->isSpoiler) {
            // " SPOILER " with BOLD | FOREGROUND_COLOR | BACKGROUND_COLOR
            postListDescSb->append(QStringLiteral(" SPOILER "),
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR | BetterSSB::BACKGROUND_COLOR,
                QColor(Qt::white), QColor(50, 50, 50), 1.0f);
            postListDescSb->append(QStringLiteral("  "), 0);
        }
    }

    // STICKY tag
    if (postSubtitleItems.count(AppearancePostSubtitleItem::STICKY)) {
        if (src->isStickied) {
            postListDescSb->append(QStringLiteral(" STICKY "),
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR | BetterSSB::BACKGROUND_COLOR,
                QColor(Qt::white), QColor(0, 170, 0), 1.0f);
            postListDescSb->append(QStringLiteral("  "), 0);
        }
    }

    // CROSSPOST tag
    if (postSubtitleItems.count(AppearancePostSubtitleItem::CROSSPOST)) {
        if (!src->isCrosspost.isEmpty()) {
            postListDescSb->append(QStringLiteral(" ") + QStringLiteral("crosspost") + QStringLiteral(" "),
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR | BetterSSB::BACKGROUND_COLOR,
                QColor(rrCrosspostTextCol), QColor(rrCrosspostBackCol), 1.0f);
            postListDescSb->append(QStringLiteral("  "), 0);
        }
    }

    // NSFW tag
    if (postSubtitleItems.count(AppearancePostSubtitleItem::NSFW)) {
        if (src->isNsfw) {
            postListDescSb->append(QStringLiteral(" NSFW "),
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR | BetterSSB::BACKGROUND_COLOR,
                QColor(Qt::white), QColor(Qt::red), 1.0f);
            postListDescSb->append(QStringLiteral("  "), 0);
        }
    }

    // FLAIR tag
    if (postSubtitleItems.count(AppearancePostSubtitleItem::FLAIR)) {
        if (!src->flairText.isEmpty()) {
            const QString ltrMark = QStringLiteral("\u200E"); // General.LTR_OVERRIDE_MARK
            postListDescSb->append(QStringLiteral(" ") + src->flairText + ltrMark + QStringLiteral(" "),
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR | BetterSSB::BACKGROUND_COLOR,
                QColor(rrFlairTextCol), QColor(rrFlairBackCol), 1.0f);
            postListDescSb->append(QStringLiteral("  "), 0);
        }
    }

    // COMMENTS count
    if (postSubtitleItems.count(AppearancePostSubtitleItem::COMMENTS)) {
        postListDescSb->append(QString::number(src->commentCount),
            BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR,
            QColor(boldCol), QColor(), 1.0f);
        postListDescSb->append(QStringLiteral("\u00A0") // NBSP
            + QStringLiteral(" comments") + QStringLiteral(" "), 0);
    }

    // SCORE
    if (postSubtitleItems.count(AppearancePostSubtitleItem::SCORE)) {
        postListDescSb->append(QString::number(score),
            BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR,
            QColor(pointsCol), QColor(), 1.0f);
        postListDescSb->append(QStringLiteral("\u00A0") // NBSP
            + QStringLiteral(" points") + QStringLiteral(" "), 0);
    }

    // UPVOTE_RATIO
    if (postSubtitleItems.count(AppearancePostSubtitleItem::UPVOTE_RATIO)) {
        postListDescSb->append(QStringLiteral("("), 0);
        postListDescSb->append(QString::number(src->upvotePercentage) + QStringLiteral("%"),
            BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR,
            QColor(boldCol), QColor(), 1.0f);
        postListDescSb->append(QStringLiteral("\u00A0")
            + QStringLiteral(" upvoted") + QStringLiteral(") "), 0);
    }

    // GOLD
    if (postSubtitleItems.count(AppearancePostSubtitleItem::GOLD)) {
        if (src->goldAmount > 0) {
            if (postSubtitleItems.count(AppearancePostSubtitleItem::SCORE)
                || postSubtitleItems.count(AppearancePostSubtitleItem::UPVOTE_RATIO)) {
                postListDescSb->append(QStringLiteral(" "), 0);
            }
            postListDescSb->append(QStringLiteral(" ") + QStringLiteral("gold")
                + QStringLiteral("\u00A0") + QStringLiteral("x")
                + QString::number(src->goldAmount) + QStringLiteral(" "),
                BetterSSB::FOREGROUND_COLOR | BetterSSB::BACKGROUND_COLOR,
                QColor(rrGoldTextCol), QColor(rrGoldBackCol), 1.0f);
            postListDescSb->append(QStringLiteral("  "), 0);
        }
    }

    // AGE
    if (postSubtitleItems.count(AppearancePostSubtitleItem::AGE)) {
        const QString ageStr = TimeFormatHelper::format(
            src->createdTimeUTC->elapsedPeriod(), context, postAgeUnits);
        postListDescSb->append(ageStr,
            BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR,
            QColor(boldCol), QColor(), 1.0f);
        postListDescSb->append(QStringLiteral(" "), 0);
    }

    // AUTHOR
    if (postSubtitleItems.count(AppearancePostSubtitleItem::AUTHOR)) {
        postListDescSb->append(QStringLiteral("by "), 0);

        bool setBackgroundColour = false;
        int backgroundColour = 0;

        const QString &dist = src->distinguished;
        if (dist == QStringLiteral("moderator")) {
            setBackgroundColour = true;
            backgroundColour = 0xFF00AA00; // Color.rgb(0, 170, 0)
        } else if (dist == QStringLiteral("admin")) {
            setBackgroundColour = true;
            backgroundColour = 0xFFAA0000; // Color.rgb(170, 0, 0)
        }

        if (setBackgroundColour) {
            postListDescSb->append(QStringLiteral("\u00A0") + src->author + QStringLiteral("\u00A0"),
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR | BetterSSB::BACKGROUND_COLOR,
                QColor(Qt::white), QColor(backgroundColour), 1.0f);
        } else {
            postListDescSb->append(src->author,
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR,
                QColor(boldCol), QColor(), 1.0f);
        }

        postListDescSb->append(QStringLiteral(" "), 0);
    }

    // SUBREDDIT
    if (postSubtitleItems.count(AppearancePostSubtitleItem::SUBREDDIT)) {
        if (showSubreddit) {
            postListDescSb->append(QStringLiteral("to "), 0);
            postListDescSb->append(src->subreddit,
                BetterSSB::BOLD | BetterSSB::FOREGROUND_COLOR,
                QColor(boldCol), QColor(), 1.0f);
            postListDescSb->append(QStringLiteral(" "), 0);
        }
    }

    // DOMAIN
    if (postSubtitleItems.count(AppearancePostSubtitleItem::DOMAIN)) {
        postListDescSb->append(QStringLiteral("(") + src->domain + QStringLiteral(")"), 0);
    }

    // return postListDescSb.get();
    return postListDescSb;
}

// --- buildAccessibilitySubtitle ---
QString RedditPreparedPost::buildAccessibilitySubtitle(QObject *context, bool headerMode) {
    std::set<AppearancePostSubtitleItem> postSubtitleItems;
    int postAgeUnits;

    if (headerMode && PrefsUtility::appearancePostSubtitleItemsUseDifferentSettings()) {
        postSubtitleItems = PrefsUtility::appearancePostHeaderSubtitleItems();
        postAgeUnits = PrefsUtility::appearancePostHeaderAgeUnits();
    } else {
        postSubtitleItems = PrefsUtility::appearancePostSubtitleItems();
        postAgeUnits = PrefsUtility::appearancePostAgeUnits();
    }

    QString accessibilitySubtitle;
    const QString separator = QStringLiteral(" \n");
    const int score = computeScore();
    const bool conciseMode = PrefsUtility::prefAccessibilityConciseMode();

    if (!conciseMode) {
        accessibilitySubtitle += buildAccessibilityEmbellishments(context, headerMode);
    }

    // COMMENTS
    if (postSubtitleItems.count(AppearancePostSubtitleItem::COMMENTS)) {
        accessibilitySubtitle += QString::number(src->commentCount)
            + QStringLiteral(" comments.") + separator;
    }

    // SCORE
    if (postSubtitleItems.count(AppearancePostSubtitleItem::SCORE)) {
        accessibilitySubtitle += QString::number(score)
            + QStringLiteral(" points.") + separator;

        if (isUpvoted()) {
            accessibilitySubtitle += QStringLiteral("Upvoted.") + separator;
        }
        if (isDownvoted()) {
            accessibilitySubtitle += QStringLiteral("Downvoted.") + separator;
        }
    }

    // UPVOTE_RATIO
    if (postSubtitleItems.count(AppearancePostSubtitleItem::UPVOTE_RATIO)) {
        if (conciseMode) {
            accessibilitySubtitle += QString::number(src->upvotePercentage)
                + QStringLiteral("% upvoted.") + separator;
        } else {
            accessibilitySubtitle += QString::number(src->upvotePercentage)
                + QStringLiteral("% upvoted.") + separator;
        }
    }

    // GOLD
    if (postSubtitleItems.count(AppearancePostSubtitleItem::GOLD)) {
        if (src->goldAmount > 0) {
            accessibilitySubtitle += QString::number(src->goldAmount)
                + QStringLiteral(" gold.") + separator;
        }
    }

    // AGE
    if (postSubtitleItems.count(AppearancePostSubtitleItem::AGE)) {
        const QString ageStr = TimeFormatHelper::format(
            src->createdTimeUTC->elapsedPeriod(), context, postAgeUnits);
        accessibilitySubtitle += ageStr + separator;
    }

    // SUBREDDIT
    if (postSubtitleItems.count(AppearancePostSubtitleItem::SUBREDDIT)) {
        if (showSubreddit) {
            const QString pron = ScreenreaderPronunciation::getPronunciation(context, src->subreddit);
            if (conciseMode) {
                accessibilitySubtitle += pron + separator;
            } else {
                accessibilitySubtitle += QStringLiteral("to ") + pron + separator;
            }
        }
    }

    // DOMAIN
    if (postSubtitleItems.count(AppearancePostSubtitleItem::DOMAIN)) {
        const QString domain = src->domain.toLower();

        if (src->isSelfPost) {
            if (conciseMode) {
                accessibilitySubtitle += QStringLiteral("Self post.") + separator;
            } else {
                accessibilitySubtitle += QStringLiteral("Self post.") + separator;
            }

        } else {
            const QString pron = ScreenreaderPronunciation::getPronunciation(context, domain);
            if (conciseMode) {
                accessibilitySubtitle += pron + separator;
            } else {
                accessibilitySubtitle += pron + separator;
            }

            if (src->hasSelfText()) {
                if (conciseMode) {
                    accessibilitySubtitle += QStringLiteral("Self post.") + separator;
                } else {
                    accessibilitySubtitle += QStringLiteral("Has self text.") + separator;
                }
            }
        }
    }

    // AUTHOR
    if (postSubtitleItems.count(AppearancePostSubtitleItem::AUTHOR)) {
        QString authorString;
        const QString &dist = src->distinguished;

        if (dist == QStringLiteral("moderator")) {
            authorString = conciseMode
                ? QStringLiteral("Moderator.")
                : QStringLiteral("Posted by moderator ");
        } else if (dist == QStringLiteral("admin")) {
            authorString = conciseMode
                ? QStringLiteral("Admin.")
                : QStringLiteral("Posted by admin ");
        } else {
            authorString = conciseMode
                ? QString()
                : QStringLiteral("Posted by ");
        }
        authorString += ScreenreaderPronunciation::getPronunciation(context, src->author);

        accessibilitySubtitle += authorString + separator;
    }

    return accessibilitySubtitle;
}

// --- buildAccessibilityTitle ---
QString RedditPreparedPost::buildAccessibilityTitle(QObject *context, bool headerMode) {
    QString a11yTitle;

    // When in concise mode, add embellishments to the title
    if (PrefsUtility::prefAccessibilityConciseMode()) {
        a11yTitle += buildAccessibilityEmbellishments(context, headerMode);
    }

    a11yTitle += src->title;

    // Append full stop so that subtitle doesn't become part of title
    a11yTitle += QStringLiteral(".\n");

    return a11yTitle;
}

// --- buildAccessibilityEmbellishments ---
QString RedditPreparedPost::buildAccessibilityEmbellishments(QObject *context, bool headerMode) {
    std::set<AppearancePostSubtitleItem> postSubtitleItems;

    if (headerMode && PrefsUtility::appearancePostSubtitleItemsUseDifferentSettings()) {
        postSubtitleItems = PrefsUtility::appearancePostHeaderSubtitleItems();
    } else {
        postSubtitleItems = PrefsUtility::appearancePostSubtitleItems();
    }

    QString a11yEmbellish;
    const QString separator = QStringLiteral(" \n");
    const bool conciseMode = PrefsUtility::prefAccessibilityConciseMode();

    // Already read
    if (isRead()) {
        a11yEmbellish += ScreenreaderPronunciation::getAccessibilityString(
            context, QStringLiteral("accessibility_post_already_read_withperiod"))
            + separator;
    }

    // SPOILER
    if (postSubtitleItems.count(AppearancePostSubtitleItem::SPOILER)) {
        if (src->isSpoiler) {
            a11yEmbellish += QStringLiteral("Spoiler.") + separator;
        }
    }

    // STICKY
    if (postSubtitleItems.count(AppearancePostSubtitleItem::STICKY)) {
        if (src->isStickied) {
            a11yEmbellish += QStringLiteral("Sticky.") + separator;
        }
    }

    // NSFW
    if (postSubtitleItems.count(AppearancePostSubtitleItem::NSFW)) {
        if (src->isNsfw) {
            if (conciseMode) {
                a11yEmbellish += QStringLiteral("NSFW.") + separator;
            } else {
                a11yEmbellish += QStringLiteral("Not safe for work.") + separator;
            }
        }
    }

    // FLAIR
    if (postSubtitleItems.count(AppearancePostSubtitleItem::FLAIR)) {
        if (!src->flairText.isEmpty()) {
            const QString ltrMark = QStringLiteral("\u200E");
            if (conciseMode) {
                a11yEmbellish += src->flairText + ltrMark + separator;
            } else {
                a11yEmbellish += QStringLiteral("Flair: ") + src->flairText + ltrMark + separator;
            }
        }
    }

    // CROSSPOST
    if (postSubtitleItems.count(AppearancePostSubtitleItem::CROSSPOST)) {
        if (!src->isCrosspost.isEmpty()) {
            a11yEmbellish += QStringLiteral("Crosspost") + separator;
        }
    }

    return a11yEmbellish;
}

// --- getThumbnail ---
QImage RedditPreparedPost::getThumbnail(ThumbnailLoadedCallback *callback, int usageId) {
    // this.thumbnailCallback = callback;
    // this.usageId = usageId;
    // return thumbnailCache;
    m_thumbnailCallback = callback;
    m_usageId = usageId;
    return m_thumbnailCache;
}

// --- isSelf ---
bool RedditPreparedPost::isSelf() const {
    return src->isSelfPost;
}

// --- isRead ---
bool RedditPreparedPost::isRead() const {
    return m_changeDataManager->isRead(src->getIdAndType());
}

// --- bind ---
void RedditPreparedPost::bind(RedditPostView *boundView) {
    m_boundView = boundView;
    m_changeDataManager->addListener(src->getIdAndType(), this);
}

// --- unbind ---
void RedditPreparedPost::unbind(RedditPostView *boundView) {
    if (m_boundView == boundView) {
        m_boundView = nullptr;
        m_changeDataManager->removeListener(src->getIdAndType(), this);
    }
}

// --- onRedditDataChange ---
void RedditPreparedPost::onRedditDataChange(const RedditIdAndType &thingIdAndType) {
    if (m_boundView) {
        // In Java:
        // final Context context = mBoundView.getContext();
        // if(context != null) mBoundView.updateAppearance();
        // Port: call updateAppearance directly
        m_boundView->updateAppearance();
    }
}

// --- markAsRead (no args) ---
void RedditPreparedPost::markAsRead(QObject *context) {
    // if (PrefsUtility.pref_behaviour_mark_posts_as_read())
    if (PrefsUtility::prefBehaviourMarkPostsAsRead()) {
        markAsRead(context, true);
    }
}

// --- markAsRead (with bool) ---
void RedditPreparedPost::markAsRead(QObject *context, bool read) {
    // final RedditAccount user = RedditAccountManager.getInstance(context).getDefaultAccount();
    // RedditChangeDataManager.getInstance(user).markRead(TimestampUTC.now(), src.getIdAndType(), read);
    const auto &user = RedditAccountManager::instance()->defaultAccount();
    auto cdm = RedditChangeDataManager::getInstance(user);
    cdm->markRead(TimestampUTC::now(), src->getIdAndType(), read);
}

// --- isUpvoted ---
bool RedditPreparedPost::isUpvoted() const {
    return m_changeDataManager->isUpvoted(src->getIdAndType());
}

// --- isDownvoted ---
bool RedditPreparedPost::isDownvoted() const {
    return m_changeDataManager->isDownvoted(src->getIdAndType());
}

// --- getVoteDirection ---
int RedditPreparedPost::getVoteDirection() const {
    // return isUpvoted() ? 1 : (isDownvoted() ? -1 : 0);
    if (isUpvoted()) return 1;
    if (isDownvoted()) return -1;
    return 0;
}

// --- isSaved ---
bool RedditPreparedPost::isSaved() const {
    return m_changeDataManager->isSaved(src->getIdAndType());
}

// --- isHidden ---
bool RedditPreparedPost::isHidden() const {
    // return Boolean.TRUE.equals(mChangeDataManager.isHidden(src.getIdAndType()));
    return m_changeDataManager->isHidden(src->getIdAndType());
}

// --- performAction ---
void RedditPreparedPost::performAction(QObject *activity, int action) {
    // Port of: RedditPostActions.INSTANCE.onActionMenuItemSelected(this, activity, action)
    // TODO: integrate RedditPostActions when ported
    Q_UNUSED(activity);
    Q_UNUSED(action);
}

// --- downloadThumbnail (private) ---
void RedditPreparedPost::downloadThumbnail(
    QObject *context,
    bool allowHighRes,
    int sizePixels,
    CacheManager *cm,
    int listId)
{
    // Port of:
    // final RedditParsedPost.ImagePreviewDetails preview = allowHighRes
    //     ? src.getPreview(sizePixels, sizePixels) : null;
    // final UriString uri = preview != null ? preview.url : src.getThumbnailUrl();

    // TODO: integrate when RedditParsedPost::getPreview is ported
    // For now use thumbnail URL directly
    const QString &uri = src->thumbnailUrl ? src->thumbnailUrl->value : QString();
    if (uri.isEmpty()) return;

    const int priority = Constants::Priority::THUMBNAIL;
    const int fileType = Constants::FileType::THUMBNAIL;

    const auto &anon = RedditAccountManager::getAnon();

    // cm.makeRequest(new CacheRequest(uri, anon, null,
    //     new Priority(priority, listId), DownloadStrategyIfNotCached.INSTANCE,
    //     fileType, CacheRequest.DownloadQueueType.IMMEDIATE, context,
    //     new CacheRequestCallbacks() { ... }));
    //
    // The callback:
    // onDataStreamComplete -> onThumbnailStreamAvailable(factory, sizePixels)
    // onFailure -> Log.e
    //
    // TODO: implement when CacheManager/CacheRequest are fully ported
    Q_UNUSED(anon);
    Q_UNUSED(priority);
    Q_UNUSED(fileType);
    Q_UNUSED(listId);
    Q_UNUSED(cm);
    Q_UNUSED(context);
}

// --- onThumbnailStreamAvailable (private) ---
void RedditPreparedPost::onThumbnailStreamAvailable(
    GenericFactory *factory,
    int desiredSizePixels)
{
    // Port of:
    // try(SeekableInputStream seekableInputStream = factory.create()) {
    //     BitmapFactory.Options justDecodeBounds ...;
    //     compute inSampleSize factor
    //     decode with scaledOptions
    //     thumbnailCache = ThumbnailScaler.scale(data, desiredSizePixels);
    //     if(thumbnailCache != data) data.recycle();
    //     if(thumbnailCallback != null) thumbnailCallback.betterThumbnailAvailable(thumbnailCache, usageId);
    // } catch(Throwable t) { Log.e(...) }
    //
    // In Qt port: use QImageReader with setScaledSize
    // TODO: implement when GenericFactory/SeekableInputStream are ported
    Q_UNUSED(factory);
    Q_UNUSED(desiredSizePixels);
}

} // namespace PinkReader
