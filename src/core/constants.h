/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: constants.h - Port of RedReader's Constants.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/common/Constants.java
 *
 * All constants, inner classes, and static methods ported 1:1.
 */

#pragma once

#include <QString>
#include <QStringList>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <cctype>

namespace PinkReader {

// ============================================================================
// Forward declarations for types used in Constants
// ============================================================================

class SubredditCanonicalId;

// ============================================================================
// Priority — port of Java inner class Constants.Priority (Java lines 244-261)
// ============================================================================

namespace Priority {
    constexpr int CAPTCHA                 = -600;
    constexpr int API_ACTION              = -500;
    constexpr int API_MULTIREDDIT_LIST    = -200;
    constexpr int API_SUBREDDIT_LIST      = -100;
    constexpr int API_SUBREDDIT_SEARCH    = -500;
    constexpr int API_SUBREDDIT_INVIDIVUAL = -250;
    constexpr int API_POST_LIST           = -200;
    constexpr int API_COMMENT_LIST        = -300;
    constexpr int THUMBNAIL               = 100;
    constexpr int INLINE_IMAGE_PREVIEW    = 100;
    constexpr int IMAGE_PRECACHE          = 500;
    constexpr int COMMENT_PRECACHE        = 500;
    constexpr int IMAGE_VIEW              = -400;
    constexpr int API_USER_ABOUT          = -500;
    constexpr int API_INBOX_LIST          = -500;
    constexpr int DEV_ANNOUNCEMENTS       = 600;
}

// ============================================================================
// FileType — port of Java inner class Constants.FileType (Java lines 263-277)
// ============================================================================

namespace FileType {
    constexpr int NOCACHE               = -1;
    constexpr int SUBREDDIT_LIST        = 100;
    constexpr int SUBREDDIT_ABOUT       = 101;
    constexpr int MULTIREDDIT_LIST      = 102;
    constexpr int POST_LIST             = 110;
    constexpr int COMMENT_LIST          = 120;
    constexpr int USER_ABOUT            = 130;
    constexpr int INBOX_LIST            = 140;
    constexpr int THUMBNAIL             = 200;
    constexpr int IMAGE                 = 201;
    constexpr int CAPTCHA               = 202;
    constexpr int INLINE_IMAGE_PREVIEW  = 203;
    constexpr int IMAGE_INFO            = 300;
}

// ============================================================================
// Mime — port of Java inner class Constants.Mime (Java lines 42-60)
// ============================================================================

namespace Mime {

inline bool isImage(const QString &mimetype) {
    // Java: StringUtils.asciiLowercase(mimetype).startsWith("image/") && !isImageGif(mimetype)
    const QString lower = mimetype.toLower();
    return lower.startsWith(QStringLiteral("image/")) && !(mimetype.compare(QStringLiteral("image/gif"), Qt::CaseInsensitive) == 0);
}

inline bool isImageGif(const QString &mimetype) {
    return mimetype.compare(QStringLiteral("image/gif"), Qt::CaseInsensitive) == 0;
}

inline bool isVideo(const QString &mimetype) {
    return mimetype.startsWith(QStringLiteral("video/"));
}

inline bool isOctetStream(const QString &mimetype) {
    return mimetype == QStringLiteral("application/octet-stream");
}

} // namespace Mime

// ============================================================================
// Reddit — port of Java inner class Constants.Reddit (Java lines 62-236)
// ============================================================================

namespace Reddit {

    // ---- URL Constants (Java lines 142-160) ----
    constexpr const char *SCHEME_HTTPS = "https";
    constexpr const char *DOMAIN_HTTPS = "oauth.reddit.com";
    constexpr const char *DOMAIN_HTTPS_HUMAN = "reddit.com";
    constexpr const char *PATH_VOTE = "/api/vote";
    constexpr const char *PATH_SAVE = "/api/save";
    constexpr const char *PATH_HIDE = "/api/hide";
    constexpr const char *PATH_UNSAVE = "/api/unsave";
    constexpr const char *PATH_UNHIDE = "/api/unhide";
    constexpr const char *PATH_REPORT = "/api/report";
    constexpr const char *PATH_DELETE = "/api/del";
    constexpr const char *PATH_SUBSCRIBE = "/api/subscribe";
    constexpr const char *PATH_SUBREDDITS_MINE_SUBSCRIBER = "/subreddits/mine/subscriber.json?limit=100";
    constexpr const char *PATH_SUBREDDITS_MINE_MODERATOR  = "/subreddits/mine/moderator.json?limit=100";
    constexpr const char *PATH_SUBREDDITS_POPULAR         = "/subreddits/popular.json";
    constexpr const char *PATH_MULTIREDDITS_MINE          = "/api/multi/mine.json";
    constexpr const char *PATH_COMMENTS                  = "/comments/";
    constexpr const char *PATH_ME                        = "/api/v1/me";

    // ---- DEFAULT_SUBREDDITS (Java lines 68-123) ----
    // Lazy-initialized; ported as a function returning const ref
    // Original Java uses CollectionStream to map strings -> SubredditCanonicalId
    // We port the string list and leave SubredditCanonicalId construction to callers
    inline const QStringList &getDefaultSubredditStrings() {
        static const QStringList list = {
            QStringLiteral("/r/Art"),
            QStringLiteral("/r/AskReddit"),
            QStringLiteral("/r/askscience"),
            QStringLiteral("/r/aww"),
            QStringLiteral("/r/books"),
            QStringLiteral("/r/creepy"),
            QStringLiteral("/r/dataisbeautiful"),
            QStringLiteral("/r/DIY"),
            QStringLiteral("/r/Documentaries"),
            QStringLiteral("/r/EarthPorn"),
            QStringLiteral("/r/explainlikeimfive"),
            QStringLiteral("/r/Fitness"),
            QStringLiteral("/r/food"),
            QStringLiteral("/r/funny"),
            QStringLiteral("/r/Futurology"),
            QStringLiteral("/r/gadgets"),
            QStringLiteral("/r/gaming"),
            QStringLiteral("/r/GetMotivated"),
            QStringLiteral("/r/gifs"),
            QStringLiteral("/r/history"),
            QStringLiteral("/r/IAmA"),
            QStringLiteral("/r/InternetIsBeautiful"),
            QStringLiteral("/r/Jokes"),
            QStringLiteral("/r/LifeProTips"),
            QStringLiteral("/r/listentothis"),
            QStringLiteral("/r/mildlyinteresting"),
            QStringLiteral("/r/movies"),
            QStringLiteral("/r/Music"),
            QStringLiteral("/r/news"),
            QStringLiteral("/r/nosleep"),
            QStringLiteral("/r/nottheonion"),
            QStringLiteral("/r/oldschoolcool"),
            QStringLiteral("/r/personalfinance"),
            QStringLiteral("/r/philosophy"),
            QStringLiteral("/r/photoshopbattles"),
            QStringLiteral("/r/pics"),
            QStringLiteral("/r/reddit"),
            QStringLiteral("/r/science"),
            QStringLiteral("/r/Showerthoughts"),
            QStringLiteral("/r/space"),
            QStringLiteral("/r/sports"),
            QStringLiteral("/r/television"),
            QStringLiteral("/r/tifu"),
            QStringLiteral("/r/todayilearned"),
            QStringLiteral("/r/TwoXChromosomes"),
            QStringLiteral("/r/UpliftingNews"),
            QStringLiteral("/r/videos"),
            QStringLiteral("/r/worldnews"),
            QStringLiteral("/r/writingprompts")
        };
        return list;
    }

    // ---- BOT_USERNAMES_LOWERCASE (Java lines 125-139) ----
    inline const std::set<QString> &getBotUsernamesLowercase() {
        static const std::set<QString> set = {
            QStringLiteral("automoderator"),
            QStringLiteral("qualityvote"),
            QStringLiteral("visualmod"),
            QStringLiteral("a-mirror-bot"),
            QStringLiteral("unexbot"),
            QStringLiteral("rfauxmoi"),
            QStringLiteral("ukbot-nicolabot"),
            QStringLiteral("qualityvote2"),
            QStringLiteral("trendingtattler"),
            QStringLiteral("cannabun"),
            QStringLiteral("pcmrbot"),
            QStringLiteral("spotlight-app"),
            QStringLiteral("flairassistant"),
            QStringLiteral("sponge-tron")
        };
        return set;
    }

    // ---- getScheme (Java line 162-164) ----
    inline QString getScheme() {
        return QString::fromLatin1(SCHEME_HTTPS);
    }

    // ---- getDomain (Java lines 166-168) ----
    inline QString getDomain() {
        return QString::fromLatin1(DOMAIN_HTTPS);
    }

    // ---- getHumanReadableDomain (Java lines 170-172) ----
    inline QString getHumanReadableDomain() {
        return QString::fromLatin1(DOMAIN_HTTPS_HUMAN);
    }

    // ---- getUriBuilder (Java lines 174-176) ----
    // Returns the URI string; builder pattern replaced with simple string construction
    inline QString getUriBuilder(const QString &path) {
        return QString::fromLatin1(SCHEME_HTTPS) + QStringLiteral("://")
                + QString::fromLatin1(DOMAIN_HTTPS) + path;
    }

    // ---- getUri (Java lines 178-180) ----
    inline QString getUri(const QString &path) {
        return getScheme() + QStringLiteral("://") + getDomain() + path;
    }

    // ---- getNonAPIUri (Java lines 182-184) ----
    inline QString getNonAPIUri(const QString &path) {
        return getScheme() + QStringLiteral("://reddit.com") + path;
    }

    // ---- isApiErrorUser (Java lines 186-189) ----
    inline bool isApiErrorUser(const QString &str) {
        return str == QStringLiteral(".error.USER_REQUIRED")
                || str.compare(QStringLiteral("please login to do that"), Qt::CaseInsensitive) == 0;
    }

    // ---- isApiErrorCaptcha (Java lines 191-194) ----
    inline bool isApiErrorCaptcha(const QString &str) {
        return str == QStringLiteral(".error.BAD_CAPTCHA.field-captcha")
                || str.compare(QStringLiteral("care to try these again?"), Qt::CaseInsensitive) == 0;
    }

    // ---- isApiErrorNotAllowed (Java lines 196-199) ----
    inline bool isApiErrorNotAllowed(const QString &str) {
        return str == QStringLiteral(".error.SUBREDDIT_NOTALLOWED.field-sr")
                || str.compare(QStringLiteral("you aren't allowed to post there."), Qt::CaseInsensitive) == 0;
    }

    // ---- isApiErrorSubredditRequired (Java lines 201-204) ----
    inline bool isApiErrorSubredditRequired(const QString &str) {
        return str == QStringLiteral(".error.SUBREDDIT_REQUIRED.field-sr")
                || str.compare(QStringLiteral("you must specify a subreddit"), Qt::CaseInsensitive) == 0;
    }

    // ---- isApiErrorURLRequired (Java lines 206-209) ----
    inline bool isApiErrorURLRequired(const QString &str) {
        return str == QStringLiteral(".error.NO_URL.field-url")
                || str.compare(QStringLiteral("a url is required"), Qt::CaseInsensitive) == 0;
    }

    // ---- isApiTooFast (Java lines 211-214) ----
    inline bool isApiTooFast(const QString &str) {
        return str == QStringLiteral(".error.RATELIMIT.field-ratelimit")
                || (!str.isNull() && str.contains(QStringLiteral("you are doing that too much"), Qt::CaseInsensitive));
    }

    // ---- isApiTooLong (Java lines 216-219) ----
    inline bool isApiTooLong(const QString &str) {
        return str == QStringLiteral("TOO_LONG")
                || (!str.isNull() && str.contains(QStringLiteral("this is too long"), Qt::CaseInsensitive));
    }

    // ---- isApiAlreadySubmitted (Java lines 221-225) ----
    inline bool isApiAlreadySubmitted(const QString &str) {
        return str == QStringLiteral(".error.ALREADY_SUB.field-url")
                || (!str.isNull() && str.contains(QStringLiteral("that link has already been submitted"), Qt::CaseInsensitive));
    }

    // ---- isPostFlairRequired (Java lines 227-231) ----
    inline bool isPostFlairRequired(const QString &str) {
        return str == QStringLiteral(".error.SUBMIT_VALIDATION_FLAIR_REQUIRED.field-flair")
                || (!str.isNull() && str.contains(QStringLiteral("Your post must contain post flair."), Qt::CaseInsensitive));
    }

    // ---- isApiError (Java lines 233-235) ----
    inline bool isApiError(const QString &str) {
        return !str.isNull() && str.startsWith(QStringLiteral(".error."));
    }

} // namespace Reddit

// ============================================================================
// OA_CS / OA_CI — port of Java fields (Java lines 279-280)
// ============================================================================

constexpr const char *OA_CS = "client_secret";
constexpr const char *OA_CI = "client_id";

// ============================================================================
// version(Context) — port of Java static method (Java lines 33-40)
// In C++ this is just a string constant; the actual app version is set at build time
// ============================================================================

// This would be defined in CMakeLists.txt or similar build config
#ifndef PINKREADER_VERSION
#define PINKREADER_VERSION "1.0.0"
#endif

inline QString version() {
    return QStringLiteral(PINKREADER_VERSION);
}

// ============================================================================
// ua(Context) — port of Java static method (Java lines 238-242)
// Builds user-agent string from canonical class name + version
// ============================================================================

inline QString ua() {
    return QStringLiteral("org.quantumbadger.redreader/") + version();
}

// ============================================================================
// PrefsUtility::createFileTypeMap helper — port of Java pattern used in CacheManager
// Maps file type int -> TimeDuration (or long for usage stats)
// ============================================================================

// Forward: TimeDuration is defined in time_duration.h
class TimeDuration;

namespace PrefsUtility {

// createFileTypeMap(Listing, Thumbnail, Image) → map<int, TimeDuration>
// Java signature (from CacheManager usage):
//   PrefsUtility.createFileTypeMap(TimeDuration, TimeDuration, TimeDuration)
// This is declared here and defined in prefs_utility.h/cpp
std::map<int, TimeDuration> createFileTypeMap(
        const TimeDuration &listing,
        const TimeDuration &thumbnail,
        const TimeDuration &image);

// createFileTypeMap(Long, Long, Long) → map<int, Long>
// Java signature (from CacheManager.getCacheDataUsages):
//   PrefsUtility.createFileTypeMap(0L, 0L, 0L)
std::map<int, qint64> createFileTypeMap(
        qint64 listing,
        qint64 thumbnail,
        qint64 image);

} // namespace PrefsUtility

} // namespace PinkReader
