/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: prefs_utility.cpp - Implementation of PrefsUtility port
 *
 * Line-by-line translation of PrefsUtility.java
 */

#include "settings/prefs_utility.h"
#include "utils/logging.h"

#include <QSettings>
#include <QStringList>
#include <stdexcept>
#include <cstdlib>

namespace PinkReader {

// Static member initialization (Java line 60-61)
QSettings *PrefsUtility::s_settings = nullptr;

// Default string sets (Java default values from arrays.xml)
static std::set<QString> s_defaultPostSubtitleItems;
static std::set<QString> s_defaultCommentHeaderItems;
static bool s_defaultsInitialized = false;

static void initDefaults()
{
    if (s_defaultsInitialized) return;
    s_defaultsInitialized = true;

    // Default post subtitle items (Java line 601 array)
    s_defaultPostSubtitleItems = {
        "author", "flair", "score", "age", "gold",
        "crosspost", "subreddit", "domain", "sticky",
        "spoiler", "nsfw", "upvote_ratio", "comments"
    };

    // Default comment header items (Java line 667 array)
    s_defaultCommentHeaderItems = {
        "author", "flair", "score", "age", "gold", "subreddit"
    };
}

const std::set<QString> &PrefsUtility::getDefaultPostSubtitleItems()
{
    initDefaults();
    return s_defaultPostSubtitleItems;
}

const std::set<QString> &PrefsUtility::getDefaultCommentHeaderItems()
{
    initDefaults();
    return s_defaultCommentHeaderItems;
}

// ============================================================================
// asciiUppercase — replaces StringUtils.asciiUppercase
// ============================================================================
QString PrefsUtility::asciiUppercase(const QString &s)
{
    return s.toUpper();
}

// ============================================================================
// init — Java lines 180-189
// ============================================================================
void PrefsUtility::init(QSettings &settings)
{
    s_settings = &settings;
    initDefaults();

    Logging::debug("PrefsUtility", "Preferences initialized");
}

// ============================================================================
// getPrefKey — Java lines 63-65 (converted from R.string to direct strings)
// ============================================================================

// ============================================================================
// getString — Java lines 69-74
// ============================================================================
QString PrefsUtility::getString(const QString &key, const QString &defaultValue)
{
    if (!s_settings) {
        Logging::error("PrefsUtility", "getString called before init()");
        return defaultValue;
    }
    return s_settings->value(key, defaultValue).toString();
}

// ============================================================================
// getStringSet — Java lines 76-82
// ============================================================================
std::set<QString> PrefsUtility::getStringSet(
        const QString &key, const std::set<QString> &defaultValue)
{
    if (!s_settings) {
        Logging::error("PrefsUtility", "getStringSet called before init()");
        return defaultValue;
    }

    QStringList list = s_settings->value(key).toStringList();
    if (list.isEmpty()) {
        return defaultValue;
    }
    return std::set<QString>(list.begin(), list.end());
}

// ============================================================================
// getBoolean — Java lines 84-88
// ============================================================================
bool PrefsUtility::getBoolean(const QString &key, bool defaultValue)
{
    if (!s_settings) {
        Logging::error("PrefsUtility", "getBoolean called before init()");
        return defaultValue;
    }
    return s_settings->value(key, defaultValue).toBool();
}

// ============================================================================
// setBoolean — Java lines 90-94
// ============================================================================
void PrefsUtility::setBoolean(const QString &key, bool newValue)
{
    if (!s_settings) {
        Logging::error("PrefsUtility", "setBoolean called before init()");
        return;
    }
    s_settings->setValue(key, newValue);
}

// ============================================================================
// getLong — Java lines 97-101
// ============================================================================
qint64 PrefsUtility::getLong(const QString &key, qint64 defaultValue)
{
    if (!s_settings) {
        Logging::error("PrefsUtility", "getLong called before init()");
        return defaultValue;
    }
    return s_settings->value(key, QVariant::fromValue(defaultValue)).toLongLong();
}

// ============================================================================
// isReLayoutRequired — Java lines 103-109
// ============================================================================
bool PrefsUtility::isReLayoutRequired(const QString &key)
{
    return key == PREF_APPEARANCE_THEME_KEY
        || key == PREF_MENUS_MAINMENU_USERITEMS_KEY
        || key == PREF_MENUS_MAINMENU_SHORTCUTITEMS_KEY;
}

// ============================================================================
// isRefreshRequired — Java lines 111-154
// ============================================================================
bool PrefsUtility::isRefreshRequired(const QString &key)
{
    return key.startsWith("pref_appearance")
        || key == PREF_BEHAVIOUR_FLING_POST_LEFT_KEY
        || key == PREF_BEHAVIOUR_FLING_POST_RIGHT_KEY
        || key == PREF_BEHAVIOUR_NSFW_KEY
        || key == PREF_BEHAVIOUR_POSTCOUNT_KEY
        || key == PREF_BEHAVIOUR_COMMENT_MIN_KEY
        || key == PREF_BEHAVIOUR_PINNED_SUBREDDITSORT_KEY
        || key == PREF_BEHAVIOUR_BLOCKED_SUBREDDITSORT_KEY
        || key == PREF_APPEARANCE_HIDE_HEADERTOOLBAR_COMMENTLIST_KEY
        || key == PREF_APPEARANCE_HIDE_HEADERTOOLBAR_POSTLIST_KEY
        || key == PREF_APPEARANCE_HIDE_COMMENTS_FROM_BLOCKED_USERS_KEY
        || key == PREF_APPEARANCE_HIGHLIGHT_OWN_USERNAME_KEY
        || key == PREF_IMAGES_THUMBNAIL_SIZE_KEY
        || key == PREF_IMAGES_INLINE_IMAGE_PREVIEWS_KEY
        || key == PREF_IMAGES_INLINE_IMAGE_PREVIEWS_NSFW_KEY
        || key == PREF_IMAGES_INLINE_IMAGE_PREVIEWS_SPOILER_KEY
        || key == PREF_IMAGES_HIGH_RES_THUMBNAILS_KEY
        || key == PREF_ACCESSIBILITY_SEPARATE_BODY_TEXT_LINES_KEY
        || key == PREF_ACCESSIBILITY_MIN_COMMENT_HEIGHT_KEY
        || key == PREF_BEHAVIOUR_POST_TITLE_OPENS_COMMENTS_KEY
        || key == PREF_BEHAVIOUR_POST_TAP_ACTION_KEY
        || key == PREF_ACCESSIBILITY_SAY_COMMENT_INDENT_LEVEL_KEY
        || key == PREF_BEHAVIOUR_COLLAPSE_STICKY_COMMENTS_KEY
        || key == PREF_ACCESSIBILITY_CONCISE_MODE_KEY
        || key == PREF_APPEARANCE_POST_HIDE_SUBREDDIT_HEADER_KEY
        || key == REDDIT_USER_AGREEMENT_PREF
        || key == PREF_REDDIT_CLIENT_ID_OVERRIDE_KEY;
}

// ============================================================================
// isRestartRequired — Java lines 156-178
// ============================================================================
bool PrefsUtility::isRestartRequired(const QString &key)
{
    return key == PREF_APPEARANCE_TWOPANE_KEY
        || key == PREF_APPEARANCE_THEME_KEY
        || key == PREF_APPEARANCE_NAVBAR_COLOR_KEY
        || key == PREF_APPEARANCE_LANGFORCE_KEY
        || key == PREF_BEHAVIOUR_BEZEL_TOOLBAR_SWIPEZONE_KEY
        || key == PREF_APPEARANCE_HIDE_USERNAME_MAIN_MENU_KEY
        || key == PREF_APPEARANCE_ANDROID_STATUS_KEY
        || key == PREF_APPEARANCE_COMMENTS_SHOW_FLOATING_TOOLBAR_KEY
        || key == PREF_BEHAVIOUR_ENABLE_SWIPE_REFRESH_KEY
        || key == PREF_MENUS_SHOW_MULTIREDDIT_MAIN_MENU_KEY
        || key == PREF_MENUS_SHOW_SUBSCRIBED_SUBREDDITS_MAIN_MENU_KEY
        || key == PREF_MENUS_MAINMENU_DEV_ANNOUNCEMENTS_KEY
        || key == PREF_APPEARANCE_BOTTOM_TOOLBAR_KEY
        || key == PREF_APPEARANCE_HIDE_TOOLBAR_ON_SCROLL_KEY
        || key == PREF_BEHAVIOUR_BLOCK_SCREENSHOTS_KEY
        || key == PREF_BEHAVIOUR_KEEP_SCREEN_AWAKE_KEY;
}

// ============================================================================
// Appearance accessors
// ============================================================================

PrefsUtility::AppearanceTwopane PrefsUtility::appearance_twopane()
{
    return parseEnum<AppearanceTwopane>(
        PREF_APPEARANCE_TWOPANE_KEY, "auto",
        [](const QString &s) -> AppearanceTwopane {
            if (s == "NEVER")  return AppearanceTwopane::NEVER;
            if (s == "FORCE")  return AppearanceTwopane::FORCE;
            return AppearanceTwopane::AUTO;
        });
}

bool PrefsUtility::isNightMode()
{
    AppearanceTheme theme = appearance_theme();
    return theme == AppearanceTheme::NIGHT
        || theme == AppearanceTheme::NIGHT_LOWCONTRAST
        || theme == AppearanceTheme::ULTRABLACK;
}

PrefsUtility::AppearanceTheme PrefsUtility::appearance_theme()
{
    return parseEnum<AppearanceTheme>(
        PREF_APPEARANCE_THEME_KEY, "red",
        [](const QString &s) -> AppearanceTheme {
            if (s == "GREEN")              return AppearanceTheme::GREEN;
            if (s == "BLUE")               return AppearanceTheme::BLUE;
            if (s == "LTBLUE")             return AppearanceTheme::LTBLUE;
            if (s == "ORANGE")             return AppearanceTheme::ORANGE;
            if (s == "GRAY")               return AppearanceTheme::GRAY;
            if (s == "NIGHT")              return AppearanceTheme::NIGHT;
            if (s == "NIGHT_LOWCONTRAST")  return AppearanceTheme::NIGHT_LOWCONTRAST;
            if (s == "ULTRABLACK")         return AppearanceTheme::ULTRABLACK;
            return AppearanceTheme::RED;
        });
}

PrefsUtility::AppearanceNavbarColour PrefsUtility::appearance_navbar_colour()
{
    return parseEnum<AppearanceNavbarColour>(
        PREF_APPEARANCE_NAVBAR_COLOR_KEY, "black",
        [](const QString &s) -> AppearanceNavbarColour {
            if (s == "WHITE")         return AppearanceNavbarColour::WHITE;
            if (s == "PRIMARY")       return AppearanceNavbarColour::PRIMARY;
            if (s == "PRIMARYDARK")   return AppearanceNavbarColour::PRIMARYDARK;
            return AppearanceNavbarColour::BLACK;
        });
}

PrefsUtility::NeverAlwaysOrWifiOnly PrefsUtility::appearance_thumbnails_show()
{
    return parseEnum<NeverAlwaysOrWifiOnly>(
        PREF_APPEARANCE_THUMBNAILS_SHOW_LIST_KEY, "always",
        [](const QString &s) -> NeverAlwaysOrWifiOnly {
            if (s == "NEVER")    return NeverAlwaysOrWifiOnly::NEVER;
            if (s == "WIFIONLY") return NeverAlwaysOrWifiOnly::WIFIONLY;
            return NeverAlwaysOrWifiOnly::ALWAYS;
        });
}

PrefsUtility::NeverAlwaysOrWifiOnly PrefsUtility::appearance_thumbnails_show_old()
{
    if (!getBoolean(PREF_APPEARANCE_THUMBNAILS_SHOW_KEY, true)) {
        return NeverAlwaysOrWifiOnly::NEVER;
    }
    if (getBoolean(PREF_APPEARANCE_THUMBNAILS_WIFIONLY_KEY, false)) {
        return NeverAlwaysOrWifiOnly::WIFIONLY;
    }
    return NeverAlwaysOrWifiOnly::ALWAYS;
}

bool PrefsUtility::appearance_thumbnails_nsfw_show()
{
    return getBoolean(PREF_APPEARANCE_THUMBNAILS_NSFW_SHOW_KEY, false);
}

bool PrefsUtility::appearance_thumbnails_spoiler_show()
{
    return getBoolean(PREF_APPEARANCE_THUMBNAILS_SPOILER_SHOW_KEY, false);
}

float PrefsUtility::appearance_fontscale_global()
{
    return getString(PREF_APPEARANCE_FONTSCALE_GLOBAL_KEY, "1").toFloat();
}

float PrefsUtility::appearance_fontscale_bodytext()
{
    QString val = getString(PREF_APPEARANCE_FONTSCALE_BODYTEXT_KEY, "-1");
    if (val == "-1") return appearance_fontscale_global();
    return val.toFloat();
}

float PrefsUtility::appearance_fontscale_comment_headers()
{
    QString val = getString(PREF_APPEARANCE_FONTSCALE_COMMENT_HEADERS_KEY, "-1");
    if (val == "-1") return appearance_fontscale_global();
    return val.toFloat();
}

float PrefsUtility::appearance_fontscale_linkbuttons()
{
    QString val = getString(PREF_APPEARANCE_FONTSCALE_LINKBUTTONS_KEY, "-1");
    if (val == "-1") return appearance_fontscale_global();
    return val.toFloat();
}

float PrefsUtility::appearance_fontscale_posts()
{
    QString val = getString(PREF_APPEARANCE_FONTSCALE_POSTS_KEY, "-1");
    if (val == "-1") return appearance_fontscale_global();
    return val.toFloat();
}

float PrefsUtility::appearance_fontscale_post_subtitles()
{
    QString val = getString(PREF_APPEARANCE_FONTSCALE_POST_SUBTITLES_KEY, "-1");
    if (val == "-1") return appearance_fontscale_global();
    return val.toFloat();
}

float PrefsUtility::appearance_fontscale_post_header_titles()
{
    QString val = getString(PREF_APPEARANCE_FONTSCALE_POST_HEADER_TITLES_KEY, "-1");
    if (val == "-1") return appearance_fontscale_global();
    return val.toFloat();
}

float PrefsUtility::appearance_fontscale_post_header_subtitles()
{
    QString val = getString(PREF_APPEARANCE_FONTSCALE_POST_HEADER_SUBTITLES_KEY, "-1");
    if (val == "-1") return appearance_fontscale_global();
    return val.toFloat();
}

bool PrefsUtility::pref_appearance_hide_username_main_menu()
{
    return getBoolean(PREF_APPEARANCE_HIDE_USERNAME_MAIN_MENU_KEY, false);
}

bool PrefsUtility::pref_show_popular_main_menu()
{
    return getBoolean(PREF_MENUS_SHOW_POPULAR_MAIN_MENU_KEY, false);
}

bool PrefsUtility::pref_show_multireddit_main_menu()
{
    return getBoolean(PREF_MENUS_SHOW_MULTIREDDIT_MAIN_MENU_KEY, true);
}

bool PrefsUtility::pref_show_subscribed_subreddits_main_menu()
{
    return getBoolean(PREF_MENUS_SHOW_SUBSCRIBED_SUBREDDITS_MAIN_MENU_KEY, true);
}

bool PrefsUtility::pref_menus_mainmenu_dev_announcements()
{
    return getBoolean(PREF_MENUS_MAINMENU_DEV_ANNOUNCEMENTS_KEY, true);
}

bool PrefsUtility::pref_appearance_show_blocked_subreddits_main_menu()
{
    return getBoolean(PREF_APPEARANCE_SHOW_BLOCKED_SUBREDDITS_MAIN_MENU_KEY, false);
}

bool PrefsUtility::pref_appearance_linkbuttons()
{
    return getBoolean(PREF_APPEARANCE_LINKBUTTONS_KEY, true);
}

PrefsUtility::AppearanceStatusBarMode PrefsUtility::pref_appearance_android_status()
{
    return parseEnum<AppearanceStatusBarMode>(
        PREF_APPEARANCE_ANDROID_STATUS_KEY, "never_hide",
        [](const QString &s) -> AppearanceStatusBarMode {
            if (s == "ALWAYS_HIDE")   return AppearanceStatusBarMode::ALWAYS_HIDE;
            if (s == "HIDE_ON_MEDIA") return AppearanceStatusBarMode::HIDE_ON_MEDIA;
            return AppearanceStatusBarMode::NEVER_HIDE;
        });
}

bool PrefsUtility::pref_appearance_link_text_clickable()
{
    return getBoolean(PREF_APPEARANCE_LINK_TEXT_CLICKABLE_KEY, true);
}

bool PrefsUtility::pref_appearance_image_viewer_show_floating_toolbar()
{
    return getBoolean(PREF_APPEARANCE_IMAGE_VIEWER_SHOW_FLOATING_TOOLBAR_KEY, true);
}

bool PrefsUtility::pref_appearance_show_aspect_ratio_indicator()
{
    return getBoolean(PREF_APPEARANCE_SHOW_ASPECT_RATIO_INDICATOR_KEY, false);
}

bool PrefsUtility::pref_album_skip_to_first()
{
    return getBoolean(PREF_ALBUM_SKIP_TO_FIRST_KEY, false);
}

bool PrefsUtility::pref_appearance_comments_show_floating_toolbar()
{
    return getBoolean(PREF_APPEARANCE_COMMENTS_SHOW_FLOATING_TOOLBAR_KEY, true);
}

bool PrefsUtility::pref_appearance_indentlines()
{
    return getBoolean(PREF_APPEARANCE_INDENTLINES_KEY, false);
}

bool PrefsUtility::pref_appearance_left_handed()
{
    return getBoolean(PREF_APPEARANCE_LEFT_HANDED_KEY, false);
}

bool PrefsUtility::pref_appearance_bottom_toolbar()
{
    return getBoolean(PREF_APPEARANCE_BOTTOM_TOOLBAR_KEY, false);
}

bool PrefsUtility::pref_appearance_hide_toolbar_on_scroll()
{
    return getBoolean(PREF_APPEARANCE_HIDE_TOOLBAR_ON_SCROLL_KEY, false);
}

bool PrefsUtility::pref_appearance_post_hide_subreddit_header()
{
    return getBoolean(PREF_APPEARANCE_POST_HIDE_SUBREDDIT_HEADER_KEY, false);
}

bool PrefsUtility::pref_appearance_hide_headertoolbar_postlist()
{
    return getBoolean(PREF_APPEARANCE_HIDE_HEADERTOOLBAR_POSTLIST_KEY, false);
}

bool PrefsUtility::pref_appearance_hide_headertoolbar_commentlist()
{
    return getBoolean(PREF_APPEARANCE_HIDE_HEADERTOOLBAR_COMMENTLIST_KEY, false);
}

bool PrefsUtility::pref_appearance_hide_comments_from_blocked_users()
{
    return getBoolean(PREF_APPEARANCE_HIDE_COMMENTS_FROM_BLOCKED_USERS_KEY, false);
}

bool PrefsUtility::pref_appearance_highlight_own_username()
{
    return getBoolean(PREF_APPEARANCE_HIGHLIGHT_OWN_USERNAME_KEY, true);
}

std::set<PrefsUtility::AppearancePostSubtitleItem> PrefsUtility::appearance_post_subtitle_items()
{
    std::set<QString> strings = getStringSet(
        PREF_APPEARANCE_POST_SUBTITLE_ITEMS_KEY,
        getDefaultPostSubtitleItems());

    std::set<AppearancePostSubtitleItem> result;
    for (const QString &s : strings) {
        QString upper = asciiUppercase(s);
        if (upper == "AUTHOR")         result.insert(AppearancePostSubtitleItem::AUTHOR);
        else if (upper == "FLAIR")     result.insert(AppearancePostSubtitleItem::FLAIR);
        else if (upper == "SCORE")     result.insert(AppearancePostSubtitleItem::SCORE);
        else if (upper == "AGE")       result.insert(AppearancePostSubtitleItem::AGE);
        else if (upper == "GOLD")      result.insert(AppearancePostSubtitleItem::GOLD);
        else if (upper == "CROSSPOST") result.insert(AppearancePostSubtitleItem::CROSSPOST);
        else if (upper == "SUBREDDIT") result.insert(AppearancePostSubtitleItem::SUBREDDIT);
        else if (upper == "DOMAIN")    result.insert(AppearancePostSubtitleItem::DOMAIN);
        else if (upper == "STICKY")    result.insert(AppearancePostSubtitleItem::STICKY);
        else if (upper == "SPOILER")   result.insert(AppearancePostSubtitleItem::SPOILER);
        else if (upper == "NSFW")      result.insert(AppearancePostSubtitleItem::NSFW);
        else if (upper == "UPVOTE_RATIO") result.insert(AppearancePostSubtitleItem::UPVOTE_RATIO);
        else if (upper == "COMMENTS")  result.insert(AppearancePostSubtitleItem::COMMENTS);
    }
    return result;
}

int PrefsUtility::appearance_post_age_units()
{
    try {
        return getString(PREF_APPEARANCE_POST_AGE_UNITS_KEY, "2").toInt();
    } catch (...) {
        return 2;
    }
}

bool PrefsUtility::appearance_post_subtitle_items_use_different_settings()
{
    return getBoolean(PREF_APPEARANCE_POST_SUBTITLE_ITEMS_USE_DIFFERENT_SETTINGS_KEY, false);
}

std::set<PrefsUtility::AppearancePostSubtitleItem> PrefsUtility::appearance_post_header_subtitle_items()
{
    std::set<QString> strings = getStringSet(
        PREF_APPEARANCE_POST_HEADER_SUBTITLE_ITEMS_KEY,
        getDefaultPostSubtitleItems());

    std::set<AppearancePostSubtitleItem> result;
    for (const QString &s : strings) {
        QString upper = asciiUppercase(s);
        if (upper == "AUTHOR")         result.insert(AppearancePostSubtitleItem::AUTHOR);
        else if (upper == "FLAIR")     result.insert(AppearancePostSubtitleItem::FLAIR);
        else if (upper == "SCORE")     result.insert(AppearancePostSubtitleItem::SCORE);
        else if (upper == "AGE")       result.insert(AppearancePostSubtitleItem::AGE);
        else if (upper == "GOLD")      result.insert(AppearancePostSubtitleItem::GOLD);
        else if (upper == "CROSSPOST") result.insert(AppearancePostSubtitleItem::CROSSPOST);
        else if (upper == "SUBREDDIT") result.insert(AppearancePostSubtitleItem::SUBREDDIT);
        else if (upper == "DOMAIN")    result.insert(AppearancePostSubtitleItem::DOMAIN);
        else if (upper == "STICKY")    result.insert(AppearancePostSubtitleItem::STICKY);
        else if (upper == "SPOILER")   result.insert(AppearancePostSubtitleItem::SPOILER);
        else if (upper == "NSFW")      result.insert(AppearancePostSubtitleItem::NSFW);
        else if (upper == "UPVOTE_RATIO") result.insert(AppearancePostSubtitleItem::UPVOTE_RATIO);
        else if (upper == "COMMENTS")  result.insert(AppearancePostSubtitleItem::COMMENTS);
    }
    return result;
}

int PrefsUtility::appearance_post_header_age_units()
{
    try {
        return getString(PREF_APPEARANCE_POST_HEADER_AGE_UNITS_KEY, "2").toInt();
    } catch (...) {
        return 2;
    }
}

bool PrefsUtility::appearance_post_show_comments_button()
{
    return getBoolean(PREF_APPEARANCE_POST_SHOW_COMMENTS_BUTTON_KEY, true);
}

std::set<PrefsUtility::AppearanceCommentHeaderItem> PrefsUtility::appearance_comment_header_items()
{
    std::set<QString> strings = getStringSet(
        PREF_APPEARANCE_COMMENT_HEADER_ITEMS_KEY,
        getDefaultCommentHeaderItems());

    std::set<AppearanceCommentHeaderItem> result;
    for (const QString &s : strings) {
        // Skip deprecated "ups_downs" — Java line 673
        if (s.compare("ups_downs", Qt::CaseInsensitive) == 0) {
            continue;
        }
        QString upper = asciiUppercase(s);
        if (upper == "AUTHOR")            result.insert(AppearanceCommentHeaderItem::AUTHOR);
        else if (upper == "FLAIR")        result.insert(AppearanceCommentHeaderItem::FLAIR);
        else if (upper == "SCORE")        result.insert(AppearanceCommentHeaderItem::SCORE);
        else if (upper == "CONTROVERSIALITY") result.insert(AppearanceCommentHeaderItem::CONTROVERSIALITY);
        else if (upper == "AGE")          result.insert(AppearanceCommentHeaderItem::AGE);
        else if (upper == "GOLD")         result.insert(AppearanceCommentHeaderItem::GOLD);
        else if (upper == "SUBREDDIT")    result.insert(AppearanceCommentHeaderItem::SUBREDDIT);
        // Unknown values silently ignored — Java line 680
    }
    return result;
}

int PrefsUtility::appearance_comment_age_units()
{
    try {
        return getString(PREF_APPEARANCE_COMMENT_AGE_UNITS_KEY, "2").toInt();
    } catch (...) {
        return 2;
    }
}

bool PrefsUtility::appearance_user_show_avatars()
{
    return getBoolean(PREF_APPEARANCE_USER_SHOW_AVATARS_KEY, true);
}

PrefsUtility::CommentAgeMode PrefsUtility::appearance_comment_age_mode()
{
    return parseEnum<CommentAgeMode>(
        PREF_APPEARANCE_COMMENT_AGE_MODE_KEY, "absolute",
        [](const QString &s) -> CommentAgeMode {
            if (s == "RELATIVE_POST")    return CommentAgeMode::RELATIVE_POST;
            if (s == "RELATIVE_PARENT")  return CommentAgeMode::RELATIVE_PARENT;
            return CommentAgeMode::ABSOLUTE;
        });
}

int PrefsUtility::appearance_inbox_age_units()
{
    try {
        return getString(PREF_APPEARANCE_INBOX_AGE_UNITS_KEY, "2").toInt();
    } catch (...) {
        return 2;
    }
}

// ============================================================================
// Image accessors
// ============================================================================

int PrefsUtility::images_thumbnail_size_dp()
{
    try {
        return getString(PREF_IMAGES_THUMBNAIL_SIZE_KEY, "64").toInt();
    } catch (...) {
        return 64;
    }
}

PrefsUtility::NeverAlwaysOrWifiOnly PrefsUtility::images_inline_image_previews()
{
    return parseEnum<NeverAlwaysOrWifiOnly>(
        PREF_IMAGES_INLINE_IMAGE_PREVIEWS_KEY, "always",
        [](const QString &s) -> NeverAlwaysOrWifiOnly {
            if (s == "NEVER")    return NeverAlwaysOrWifiOnly::NEVER;
            if (s == "WIFIONLY") return NeverAlwaysOrWifiOnly::WIFIONLY;
            return NeverAlwaysOrWifiOnly::ALWAYS;
        });
}

bool PrefsUtility::images_inline_image_previews_nsfw()
{
    return getBoolean(PREF_IMAGES_INLINE_IMAGE_PREVIEWS_NSFW_KEY, false);
}

bool PrefsUtility::images_inline_image_previews_spoiler()
{
    return getBoolean(PREF_IMAGES_INLINE_IMAGE_PREVIEWS_SPOILER_KEY, false);
}

PrefsUtility::NeverAlwaysOrWifiOnly PrefsUtility::images_high_res_thumbnails()
{
    return parseEnum<NeverAlwaysOrWifiOnly>(
        PREF_IMAGES_HIGH_RES_THUMBNAILS_KEY, "wifionly",
        [](const QString &s) -> NeverAlwaysOrWifiOnly {
            if (s == "NEVER")    return NeverAlwaysOrWifiOnly::NEVER;
            if (s == "WIFIONLY") return NeverAlwaysOrWifiOnly::WIFIONLY;
            return NeverAlwaysOrWifiOnly::ALWAYS;
        });
}

// ============================================================================
// Behaviour accessors
// ============================================================================

bool PrefsUtility::pref_behaviour_skiptofrontpage()
{
    return getBoolean(PREF_BEHAVIOUR_SKIPTOFRONTPAGE_KEY, false);
}

bool PrefsUtility::pref_behaviour_useinternalbrowser()
{
    return getBoolean(PREF_BEHAVIOUR_USEINTERNALBROWSER_KEY, true);
}

bool PrefsUtility::pref_behaviour_usecustomtabs()
{
    // Note: network_tor() check omitted — platform-dependent
    return getBoolean(PREF_BEHAVIOUR_USECUSTOMTABS_KEY, true);
}

void PrefsUtility::set_pref_behaviour_notifications(bool enabled)
{
    setBoolean(PREF_BEHAVIOUR_NOTIFICATIONS_KEY, enabled);
}

bool PrefsUtility::pref_behaviour_notifications()
{
    return getBoolean(PREF_BEHAVIOUR_NOTIFICATIONS_KEY, true);
}

bool PrefsUtility::pref_behaviour_enable_swipe_refresh()
{
    return getBoolean(PREF_BEHAVIOUR_ENABLE_SWIPE_REFRESH_KEY, true);
}

bool PrefsUtility::pref_behaviour_video_playback_controls()
{
    return getBoolean(PREF_BEHAVIOUR_VIDEO_PLAYBACK_CONTROLS_KEY, true);
}

bool PrefsUtility::pref_behaviour_video_frame_step()
{
    return getBoolean(PREF_BEHAVIOUR_VIDEO_FRAME_STEP_KEY, false);
}

bool PrefsUtility::pref_behaviour_video_mute_default()
{
    return getBoolean(PREF_BEHAVIOUR_VIDEO_MUTE_DEFAULT_KEY, true);
}

bool PrefsUtility::pref_behaviour_video_zoom_default()
{
    return getBoolean(PREF_BEHAVIOUR_VIDEO_ZOOM_DEFAULT_KEY, false);
}

bool PrefsUtility::pref_videos_download_before_playing()
{
    return getBoolean(PREF_VIDEOS_DOWNLOAD_BEFORE_PLAYING_KEY, false);
}

bool PrefsUtility::pref_behaviour_imagevideo_tap_close()
{
    return getBoolean(PREF_BEHAVIOUR_IMAGEVIDEO_TAP_CLOSE_KEY, true);
}

int PrefsUtility::pref_behaviour_bezel_toolbar_swipezone_dp()
{
    try {
        return getString(PREF_BEHAVIOUR_BEZEL_TOOLBAR_SWIPEZONE_KEY, "10").toInt();
    } catch (...) {
        return 10;
    }
}

bool PrefsUtility::pref_behaviour_back_again()
{
    return getBoolean(PREF_BEHAVIOUR_POSTLIST_BACK_AGAIN_KEY, false);
}

int PrefsUtility::pref_behaviour_gallery_swipe_length_dp()
{
    try {
        return getString(PREF_BEHAVIOUR_GALLERY_SWIPE_LENGTH_KEY, "150").toInt();
    } catch (...) {
        return 150;
    }
}

std::optional<int> PrefsUtility::pref_behaviour_comment_min()
{
    int defaultValue = -4;
    QString value = getString(PREF_BEHAVIOUR_COMMENT_MIN_KEY, QString::number(defaultValue));

    if (value.trimmed().isEmpty()) {
        return std::nullopt;
    }
    try {
        return value.toInt();
    } catch (...) {
        return defaultValue;
    }
}

PrefsUtility::PostTapAction PrefsUtility::pref_behaviour_post_tap_action()
{
    return parseEnum<PostTapAction>(
        PREF_BEHAVIOUR_POST_TAP_ACTION_KEY, "link",
        [](const QString &s) -> PostTapAction {
            if (s == "COMMENTS")        return PostTapAction::COMMENTS;
            if (s == "TITLE_COMMENTS")  return PostTapAction::TITLE_COMMENTS;
            return PostTapAction::LINK;
        });
}

bool PrefsUtility::pref_behaviour_post_title_opens_comments()
{
    return getBoolean(PREF_BEHAVIOUR_POST_TITLE_OPENS_COMMENTS_KEY, false);
}

} // namespace PinkReader
