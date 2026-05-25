/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * File: constants.h
 * Description: Application-wide constants
 */


#pragma once

#include <QString>
#include <QUrl>
#include <QVersionNumber>
#include <cstdint>

/**
 * @namespace PinkReader::Constants
 * @brief Application-wide constants used throughout PinkReader
 *
 * Contains all hardcoded values, API configuration, cache parameters,
 * feature flags, and platform-specific constants.
 *
 * Centralizing constants here makes it easy to update values globally
 * and ensures consistency across the codebase.
 */
namespace PinkReader {
namespace Constants {

// ---------------------------------------------------------------------------
// Application Identity
// ---------------------------------------------------------------------------

/** @brief Application display name */
constexpr const char *APP_NAME = "PinkReader";

/** @brief Application ID (used for QSettings, Android package, etc.) */
constexpr const char *APP_ID = "org.pinkreader.app";

/** @brief Application organization name */
constexpr const char *ORG_NAME = "PinkReader";

/** @brief Application organization domain */
constexpr const char *ORG_DOMAIN = "pinkreader.org";

/** @brief F-Droid application ID for future F-Droid submission */
constexpr const char *FDROID_ID = "org.pinkreader.app";

/** @brief GitHub repository URL */
constexpr const char *GITHUB_REPO = "https://github.com/pinkreader/pinkreader";

/** @brief Website URL */
constexpr const char *WEBSITE_URL = "https://pinkreader.org";

/** @brief Privacy policy URL */
constexpr const char *PRIVACY_URL = "https://pinkreader.org/privacy";

/** @brief Bug report URL */
constexpr const char *BUG_REPORT_URL = "https://github.com/pinkreader/pinkreader/issues";

/** @brief User agent string for HTTP requests */
constexpr const char *USER_AGENT = "PinkReader/1.0.0 (Android; Qt 6.x)";

// ---------------------------------------------------------------------------
// Reddit API Configuration
// ---------------------------------------------------------------------------

/** @brief Reddit OAuth base URL */
constexpr const char *REDDIT_OAUTH_BASE = "https://oauth.reddit.com";

/** @brief Reddit API base URL */
constexpr const char *REDDIT_API_BASE = "https://www.reddit.com/api";

/** @brief Reddit OAuth authorization endpoint */
constexpr const char *REDDIT_AUTH_URL = "https://www.reddit.com/api/v1/authorize";

/** @brief Reddit OAuth token endpoint */
constexpr const char *REDDIT_TOKEN_URL = "https://www.reddit.com/api/v1/access_token";

/** @brief Reddit OAuth redirect URI (used for app-based auth) */
constexpr const char *REDDIT_REDIRECT_URI = "pinkreader://oauth/callback";

/** @brief Default Reddit OAuth client ID (can be overridden in settings) */
constexpr const char *DEFAULT_CLIENT_ID = "pinkreader_android_app";

/** @brief OAuth scopes requested by the app */
constexpr const char *OAUTH_SCOPES =
    "identity,read,edit,submit,flair,modconfig,modflair,"
    "modlog,modposts,modwiki,privatemessages,report,"
    "save,submit,vote,wikiread,history,subscribe";

/** @brief Maximum number of posts per API request */
constexpr int MAX_POSTS_PER_REQUEST = 100;

/** @brief Maximum number of comments per API request */
constexpr int MAX_COMMENTS_PER_REQUEST = 500;

/** @brief Default posts to fetch in a listing */
constexpr int DEFAULT_LISTING_LIMIT = 25;

/** @brief Maximum number of items in the main feed */
constexpr int MAX_FEED_ITEMS = 250;

// ---------------------------------------------------------------------------
// API Rate Limiting
// ---------------------------------------------------------------------------

/** @brief Minimum interval between API requests (milliseconds) */
constexpr int API_REQUEST_INTERVAL_MS = 1000;

/** @brief Maximum requests per minute (Reddit's standard rate limit) */
constexpr int MAX_REQUESTS_PER_MINUTE = 60;

/** @brief Backoff multiplier for rate-limited requests */
constexpr double RATE_LIMIT_BACKOFF_MULTIPLIER = 2.0;

/** @brief Maximum backoff time (seconds) */
constexpr int MAX_BACKOFF_SECONDS = 60;

// ---------------------------------------------------------------------------
// Cache Configuration
// ---------------------------------------------------------------------------

/** @brief SQLite database filename for cache metadata */
constexpr const char *CACHE_DB_NAME = "pinkreader_cache.db";

/** @brief SQLite database version */
constexpr int CACHE_DB_VERSION = 1;

/** @brief Maximum cache size in megabytes */
constexpr int MAX_CACHE_SIZE_MB = 512;

/** @brief Maximum disk cache age in days */
constexpr int MAX_CACHE_AGE_DAYS = 30;

/** @brief Default thumbnail cache size */
constexpr int THUMBNAIL_CACHE_COUNT = 500;

/** @brief Maximum image cache size in megabytes */
constexpr int MAX_IMAGE_CACHE_MB = 256;

/** @brief Cache compression level (1-22 for zstd, higher = better compression) */
constexpr int CACHE_COMPRESSION_LEVEL = 15;

// ---------------------------------------------------------------------------
// Image/Media Settings
// ---------------------------------------------------------------------------

/** @brief Maximum image dimension for thumbnails */
constexpr int THUMBNAIL_MAX_DIMENSION = 256;

/** @brief Maximum image dimension for previews */
constexpr int PREVIEW_MAX_DIMENSION = 800;

/** @brief JPEG quality for cached images (0-100) */
constexpr int IMAGE_CACHE_JPEG_QUALITY = 85;

/** @brief Maximum GIF file size to download (bytes) */
constexpr int64_t MAX_GIF_SIZE_BYTES = 10 * 1024 * 1024; // 10 MB

/** @brief Maximum video file size for inline playback (bytes) */
constexpr int64_t MAX_VIDEO_SIZE_BYTES = 50 * 1024 * 1024; // 50 MB

// ---------------------------------------------------------------------------
// UI/Display Constants
// ---------------------------------------------------------------------------

/** @brief Default font size for body text (points) */
constexpr int DEFAULT_FONT_SIZE = 14;

/** @brief Minimum font size (points) */
constexpr int MIN_FONT_SIZE = 8;

/** @brief Maximum font size (points) */
constexpr int MAX_FONT_SIZE = 24;

/** @brief Font scaling step */
constexpr double FONT_SCALE_STEP = 0.1;

/** @brief Minimum font scale factor */
constexpr double MIN_FONT_SCALE = 0.5;

/** @brief Maximum font scale factor */
constexpr double MAX_FONT_SCALE = 3.0;

/** @brief Default comment indent width (pixels per level) */
constexpr int COMMENT_INDENT_PX = 12;

/** @brief Maximum comment nesting level to indent */
constexpr int MAX_COMMENT_DEPTH = 10;

/** @brief Minimum height for comment rows (accessibility) */
constexpr int MIN_COMMENT_HEIGHT_PX = 48;

/** @brief Default post card thumbnail width */
constexpr int POST_THUMBNAIL_WIDTH = 72;

/** @brief Default post card thumbnail height */
constexpr int POST_THUMBNAIL_HEIGHT = 72;

/** @brief Swipe threshold for triggering actions (pixels) */
constexpr int SWIPE_THRESHOLD_PX = 80;

/** @brief Bezel toolbar swipe zone width (pixels) */
constexpr int BEZEL_SWIPE_ZONE_WIDTH = 20;

/** @brief Pull-to-refresh trigger distance (pixels) */
constexpr int PULL_REFRESH_DISTANCE = 100;

/** @brief Pull-to-refresh maximum overshoot (pixels) */
constexpr int PULL_REFRESH_MAX_OVERSCROLL = 200;

// ---------------------------------------------------------------------------
// Timer Intervals
// ---------------------------------------------------------------------------

/** @brief How often to check for new messages (milliseconds) */
constexpr int INBOX_CHECK_INTERVAL_MS = 15 * 60 * 1000; // 15 minutes

/** @brief How often to auto-refresh old post lists (milliseconds) */
constexpr int AUTO_REFRESH_INTERVAL_MS = 30 * 60 * 1000; // 30 minutes

/** @brief Cache pruning interval (milliseconds) */
constexpr int CACHE_PRUNE_INTERVAL_MS = 60 * 60 * 1000; // 1 hour

/** @brief Draft auto-save interval (milliseconds) */
constexpr int DRAFT_AUTOSAVE_INTERVAL_MS = 30 * 1000; // 30 seconds

/** @brief Session auto-save interval (milliseconds) */
constexpr int SESSION_AUTOSAVE_INTERVAL_MS = 60 * 1000; // 1 minute

// ---------------------------------------------------------------------------
// Intent/Activity Request Codes
// ---------------------------------------------------------------------------

/** @brief Request code for OAuth login activity */
constexpr int OAUTH_LOGIN_REQUEST_CODE = 1001;

/** @brief Request code for image picker activity */
constexpr int IMAGE_PICKER_REQUEST_CODE = 1002;

/** @brief Request code for file picker activity */
constexpr int FILE_PICKER_REQUEST_CODE = 1003;

/** @brief Request code for settings backup activity */
constexpr int BACKUP_REQUEST_CODE = 1004;

/** @brief Request code for settings restore activity */
constexpr int RESTORE_REQUEST_CODE = 1005;

// ---------------------------------------------------------------------------
// Feature Flags
// ---------------------------------------------------------------------------

/** @brief Enable experimental features */
constexpr bool FEATURE_EXPERIMENTAL = false;

/** @brief Enable debugging overlay */
constexpr bool FEATURE_DEBUG_OVERLAY = false;

/** @brief Enable performance profiling */
constexpr bool FEATURE_PROFILING = false;

/** @brief Enable anonymous usage statistics */
constexpr bool FEATURE_ANALYTICS = false;

/** @brief Enable crash reporting */
constexpr bool FEATURE_CRASH_REPORTING = true;

/** @brief Enable Material You dynamic colors (Android 12+) */
constexpr bool FEATURE_MATERIAL_YOU = true;

/** @brief Enable Tor/Orbot integration */
constexpr bool FEATURE_TOR_SUPPORT = true;

/** @brief Enable image precaching */
constexpr bool FEATURE_IMAGE_PRECACHE = true;

/** @brief Enable video playback in feed */
constexpr bool FEATURE_INLINE_VIDEO = true;

/** @brief Enable markdown preview in editor */
constexpr bool FEATURE_MARKDOWN_PREVIEW = true;

/** @brief Enable swipe gestures */
constexpr bool FEATURE_SWIPE_GESTURES = true;

/** @brief Enable two-pane tablet layout */
constexpr bool FEATURE_TWO_PANE = true;

// ---------------------------------------------------------------------------
// File/Path Constants
// ---------------------------------------------------------------------------

/** @brief Settings filename */
constexpr const char *SETTINGS_FILE = "preferences.json";

/** @brief Accounts storage filename */
constexpr const char *ACCOUNTS_FILE = "accounts.json";

/** @brief Drafts storage directory */
constexpr const char *DRAFTS_DIR = "drafts";

/** @brief Log file name */
constexpr const char *LOG_FILE = "pinkreader.log";

/** @brief Crash dump directory */
constexpr const char *CRASH_DUMP_DIR = "crashes";

/** @brief Backup directory */
constexpr const char *BACKUP_DIR = "backups";

/** @brief Custom themes directory */
constexpr const char *THEMES_DIR = "themes";

/** @brief Plugins directory */
constexpr const char *PLUGINS_DIR = "plugins";

// ---------------------------------------------------------------------------
// Sort Orders
// ---------------------------------------------------------------------------

/** @brief Available sort orders for posts */
enum class PostSortOrder {
    Hot,
    New,
    Top,
    Rising,
    Controversial,
    Best,
    Relevance,
    Comments
};

/** @brief Available sort orders for comments */
enum class CommentSortOrder {
    Best,
    Top,
    New,
    Controversial,
    Old,
    QA
};

/** @brief Available time ranges for top/controversial */
enum class TimeRange {
    Hour,
    Day,
    Week,
    Month,
    Year,
    All
};

/** @brief Convert post sort order to Reddit API string */
inline QString postSortToString(PostSortOrder order)
{
    switch (order) {
        case PostSortOrder::Hot:          return QStringLiteral("hot");
        case PostSortOrder::New:          return QStringLiteral("new");
        case PostSortOrder::Top:          return QStringLiteral("top");
        case PostSortOrder::Rising:       return QStringLiteral("rising");
        case PostSortOrder::Controversial: return QStringLiteral("controversial");
        case PostSortOrder::Best:         return QStringLiteral("best");
        case PostSortOrder::Relevance:    return QStringLiteral("relevance");
        case PostSortOrder::Comments:     return QStringLiteral("comments");
    }
    return QStringLiteral("hot");
}

/** @brief Convert comment sort order to Reddit API string */
inline QString commentSortToString(CommentSortOrder order)
{
    switch (order) {
        case CommentSortOrder::Best:         return QStringLiteral("confidence");
        case CommentSortOrder::Top:          return QStringLiteral("top");
        case CommentSortOrder::New:          return QStringLiteral("new");
        case CommentSortOrder::Controversial: return QStringLiteral("controversial");
        case CommentSortOrder::Old:          return QStringLiteral("old");
        case CommentSortOrder::QA:           return QStringLiteral("qa");
    }
    return QStringLiteral("confidence");
}

/** @brief Convert time range to Reddit API string */
inline QString timeRangeToString(TimeRange range)
{
    switch (range) {
        case TimeRange::Hour:  return QStringLiteral("hour");
        case TimeRange::Day:   return QStringLiteral("day");
        case TimeRange::Week:  return QStringLiteral("week");
        case TimeRange::Month: return QStringLiteral("month");
        case TimeRange::Year:  return QStringLiteral("year");
        case TimeRange::All:   return QStringLiteral("all");
    }
    return QStringLiteral("day");
}

// ---------------------------------------------------------------------------
// Subreddit Constants
// ---------------------------------------------------------------------------

/** @brief Front page pseudo-subreddit identifier */
constexpr const char *FRONT_PAGE = "";

/** @brief /r/all subreddit identifier */
constexpr const char *R_ALL = "all";

/** @brief /r/popular subreddit identifier */
constexpr const char *R_POPULAR = "popular";

/** @brief Default subreddit to show on first launch */
constexpr const char *DEFAULT_SUBREDDIT = "all";

/** @brief Maximum number of pinned subreddits */
constexpr int MAX_PINNED_SUBREDDITS = 50;

/** @brief Maximum subreddits in recent list */
constexpr int MAX_RECENT_SUBREDDITS = 50;

// ---------------------------------------------------------------------------
// Post/Comment Limits
// ---------------------------------------------------------------------------

/** @brief Maximum title length for posts */
constexpr int MAX_POST_TITLE_LENGTH = 300;

/** @brief Maximum body length for text posts */
constexpr int MAX_POST_BODY_LENGTH = 40000;

/** @brief Maximum comment body length */
constexpr int MAX_COMMENT_LENGTH = 10000;

/** @brief Maximum characters for private messages */
constexpr int MAX_PM_LENGTH = 10000;

/** @brief Maximum report reason length */
constexpr int MAX_REPORT_LENGTH = 100;

// ---------------------------------------------------------------------------
// Voting
// ---------------------------------------------------------------------------

/** @brief Vote direction enumeration */
enum class VoteDirection {
    None = 0,
    Upvote = 1,
    Downvote = -1
};

/** @brief Convert vote direction to API value */
inline int voteToApiValue(VoteDirection vote)
{
    return static_cast<int>(vote);
}

// ---------------------------------------------------------------------------
// Save/Hide States
// ---------------------------------------------------------------------------

/** @brief Content save state */
enum class SaveState {
    NotSaved,
    Saved
};

/** @brief Content hide state */
enum class HideState {
    Visible,
    Hidden
};

/** @brief Content read state */
enum class ReadState {
    Unread,
    Read
};

// ---------------------------------------------------------------------------
// Login/Account Constants
// ---------------------------------------------------------------------------

/** @brief Maximum number of accounts */
constexpr int MAX_ACCOUNTS = 10;

/** @brief Minimum username length */
constexpr int MIN_USERNAME_LENGTH = 3;

/** @brief Maximum username length */
constexpr int MAX_USERNAME_LENGTH = 20;

/** @brief OAuth token refresh threshold (seconds before expiry) */
constexpr int TOKEN_REFRESH_THRESHOLD_SECS = 300; // 5 minutes

/** @brief OAuth state length for CSRF protection */
constexpr int OAUTH_STATE_LENGTH = 32;

// ---------------------------------------------------------------------------
// NSFW/Content Filtering
// ---------------------------------------------------------------------------

/** @brief NSFW content preference */
enum class NsfwPreference {
    Hide,
    Show,
    BlurThumbnail
};

/** @brief Spoiler content preference */
enum class SpoilerPreference {
    HideText,
    ShowText,
    BlurThumbnail
};

} // namespace Constants
} // namespace PinkReader
