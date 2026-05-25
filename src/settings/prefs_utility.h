/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: prefs_utility.h - Port of RedReader's PrefsUtility.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/common/PrefsUtility.java
 *
 * All preference key constants, enums, and static accessor methods ported 1:1.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QSettings>
#include <QStringList>
#include <set>
#include <optional>
#include <functional>

namespace PinkReader {

// Forward declarations
class SharedPrefsWrapper;

// ============================================================================
// PrefsUtility — static preferences accessor
// Port of PrefsUtility.java (1832 lines)
// ============================================================================

class PrefsUtility {
public:
    // ========================================================================
    // Enums from PrefsUtility.java
    // ========================================================================

    // ---- AppearanceTwopane (Java line 197-199) ----
    enum class AppearanceTwopane {
        NEVER,
        AUTO,
        FORCE
    };

    // ---- AppearanceTheme (referenced in PrefsUtility, defined elsewhere) ----
    enum class AppearanceTheme {
        RED,
        GREEN,
        BLUE,
        LTBLUE,
        ORANGE,
        GRAY,
        NIGHT,
        NIGHT_LOWCONTRAST,
        ULTRABLACK
    };

    // ---- AppearanceNavbarColour (Java line 223-225) ----
    enum class AppearanceNavbarColour {
        BLACK,
        WHITE,
        PRIMARY,
        PRIMARYDARK
    };

    // ---- NeverAlwaysOrWifiOnly ----
    enum class NeverAlwaysOrWifiOnly {
        NEVER,
        WIFIONLY,
        ALWAYS
    };

    // ---- AppearanceStatusBarMode (Java lines 487-489) ----
    enum class AppearanceStatusBarMode {
        ALWAYS_HIDE,
        HIDE_ON_MEDIA,
        NEVER_HIDE
    };

    // ---- AppearancePostSubtitleItem (Java lines 581-595) ----
    enum class AppearancePostSubtitleItem {
        AUTHOR,
        FLAIR,
        SCORE,
        AGE,
        GOLD,
        CROSSPOST,
        SUBREDDIT,
        DOMAIN,
        STICKY,
        SPOILER,
        NSFW,
        UPVOTE_RATIO,
        COMMENTS
    };

    // ---- AppearanceCommentHeaderItem (Java lines 659-661) ----
    enum class AppearanceCommentHeaderItem {
        AUTHOR,
        FLAIR,
        SCORE,
        CONTROVERSIALITY,
        AGE,
        GOLD,
        SUBREDDIT
    };

    // ---- CommentAgeMode (Java lines 697-699) ----
    enum class CommentAgeMode {
        ABSOLUTE,
        RELATIVE_POST,
        RELATIVE_PARENT
    };

    // ---- PostTapAction (Java lines 874-876) ----
    enum class PostTapAction {
        LINK,
        COMMENTS,
        TITLE_COMMENTS
    };

    // ---- ImageViewMode (Java lines 893-903) ----
    enum class ImageViewMode {
        INTERNAL_OPENGL,
        INTERNAL_BROWSER,
        EXTERNAL_BROWSER
    };
    static bool imageViewMode_downloadInApp(ImageViewMode mode);

    // ---- PostSort enum ----
    enum class PostSort {
        HOT,
        NEW,
        RISING,
        TOP_HOUR,
        TOP_DAY,
        TOP_WEEK,
        TOP_MONTH,
        TOP_YEAR,
        TOP_ALL,
        CONTROVERSIAL,
        BEST
    };

    // ---- PostCommentSort enum (Java line 1122) ----
    enum class PostCommentSort {
        BEST,
        HOT,
        NEW,
        TOP,
        CONTROVERSIAL,
        OLD,
        QA
    };

    // ---- UserCommentSort enum (Java line 1128) ----
    enum class UserCommentSort {
        NEW,
        HOT,
        TOP,
        CONTROVERSIAL
    };

    // ---- ImageViewMode (Java lines 893-903) ----
    enum class ImageViewMode {
        INTERNAL_OPENGL,
        INTERNAL_BROWSER,
        EXTERNAL_BROWSER
    };

    // ---- AlbumViewMode (Java lines 913-917) ----
    enum class AlbumViewMode {
        INTERNAL_LIST,
        INTERNAL_BROWSER,
        EXTERNAL_BROWSER
    };

    // ---- GifViewMode (Java lines 928-939) ----
    enum class GifViewMode {
        INTERNAL_MOVIE,
        INTERNAL_LEGACY,
        INTERNAL_BROWSER,
        EXTERNAL_BROWSER
    };
    static bool gifViewMode_downloadInApp(GifViewMode mode);

    // ---- VideoViewMode (Java lines 950-961) ----
    enum class VideoViewMode {
        INTERNAL_VIDEOVIEW,
        INTERNAL_BROWSER,
        EXTERNAL_BROWSER,
        EXTERNAL_APP_VLC
    };
    static bool videoViewMode_downloadInApp(VideoViewMode mode);

    // ---- PostFlingAction (Java lines 972-993) ----
    enum class PostFlingAction {
        UPVOTE, DOWNVOTE, SAVE, HIDE, COMMENTS, LINK,
        ACTION_MENU, BROWSER, BACK, REPORT, SAVE_IMAGE,
        GOTO_SUBREDDIT, SHARE, SHARE_COMMENTS, SHARE_IMAGE,
        COPY, USER_PROFILE, PROPERTIES, MARK_READ, DISABLED
    };

    // ---- SelfpostAction (Java lines 1009-1011) ----
    enum class SelfpostAction {
        COLLAPSE, NOTHING
    };

    // ---- CommentFlingAction (Java lines 1022-1040) ----
    enum class CommentFlingAction {
        UPVOTE, DOWNVOTE, SAVE, REPORT, REPLY, CONTEXT,
        GO_TO_COMMENT, COMMENT_LINKS, SHARE, COPY_TEXT, COPY_URL,
        USER_PROFILE, COLLAPSE, ACTION_MENU, PROPERTIES, BACK, DISABLED
    };

    // ---- CommentAction (Java lines 1054-1056) ----
    enum class CommentAction {
        COLLAPSE, ACTION_MENU, NOTHING
    };

    // ---- PinnedSubredditSort (Java lines 1134-1136) ----
    enum class PinnedSubredditSort {
        NAME, DATE
    };

    // ---- BlockedSubredditSort (Java lines 1144-1146) ----
    enum class BlockedSubredditSort {
        NAME, DATE
    };

    // ---- SharingDomain (Java lines 1174-1185) ----
    enum class SharingDomain {
        STANDARD_REDDIT,
        SHORT_REDDIT,
        OLD_REDDIT,
        NEW_REDDIT,
        NP_REDDIT
    };
    static QString sharingDomain_domain(SharingDomain domain);

    // ---- PostCount (Java lines 1198-1200) ----
    enum class PostCount {
        R25, R50, R100, ALL
    };

    // ---- ScreenOrientation (Java lines 1208-1210) ----
    enum class ScreenOrientation {
        AUTO, PORTRAIT, LANDSCAPE
    };

    // ---- SaveLocation (Java lines 1218-1220) ----
    enum class SaveLocation {
        PROMPT_EVERY_TIME, SYSTEM_DEFAULT
    };

    // ---- BehaviourCollapseStickyComments (Java lines 1765-1767) ----
    enum class BehaviourCollapseStickyComments {
        ALWAYS, ONLY_BOTS, NEVER
    };

    // ========================================================================
    // Preference key constants (replacing Android R.string references)
    // ========================================================================

    // -- Appearance keys --
    static constexpr const char *PREF_APPEARANCE_TWOPANE_KEY = "pref_appearance_twopane";
    static constexpr const char *PREF_APPEARANCE_THEME_KEY = "pref_appearance_theme";
    static constexpr const char *PREF_APPEARANCE_NAVBAR_COLOR_KEY = "pref_appearance_navbar_color";
    static constexpr const char *PREF_APPEARANCE_LANGFORCE_KEY = "pref_appearance_langforce";
    static constexpr const char *PREF_APPEARANCE_THUMBNAILS_SHOW_LIST_KEY = "pref_appearance_thumbnails_show_list";
    static constexpr const char *PREF_APPEARANCE_THUMBNAILS_SHOW_KEY = "pref_appearance_thumbnails_show";
    static constexpr const char *PREF_APPEARANCE_THUMBNAILS_WIFIONLY_KEY = "pref_appearance_thumbnails_wifionly";
    static constexpr const char *PREF_APPEARANCE_THUMBNAILS_NSFW_SHOW_KEY = "pref_appearance_thumbnails_nsfw_show";
    static constexpr const char *PREF_APPEARANCE_THUMBNAILS_SPOILER_SHOW_KEY = "pref_appearance_thumbnails_spoiler_show";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_GLOBAL_KEY = "pref_appearance_fontscale_global";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_BODYTEXT_KEY = "pref_appearance_fontscale_bodytext";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_COMMENT_HEADERS_KEY = "pref_appearance_fontscale_comment_headers";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_LINKBUTTONS_KEY = "pref_appearance_fontscale_linkbuttons";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_POSTS_KEY = "pref_appearance_fontscale_posts";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_POST_SUBTITLES_KEY = "pref_appearance_fontscale_post_subtitles";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_POST_HEADER_TITLES_KEY = "pref_appearance_fontscale_post_header_titles";
    static constexpr const char *PREF_APPEARANCE_FONTSCALE_POST_HEADER_SUBTITLES_KEY = "pref_appearance_fontscale_post_header_subtitles";
    static constexpr const char *PREF_APPEARANCE_HIDE_USERNAME_MAIN_MENU_KEY = "pref_appearance_hide_username_main_menu";
    static constexpr const char *PREF_APPEARANCE_LINKBUTTONS_KEY = "pref_appearance_linkbuttons";
    static constexpr const char *PREF_APPEARANCE_ANDROID_STATUS_KEY = "pref_appearance_android_status";
    static constexpr const char *PREF_APPEARANCE_LINK_TEXT_CLICKABLE_KEY = "pref_appearance_link_text_clickable";
    static constexpr const char *PREF_APPEARANCE_IMAGE_VIEWER_SHOW_FLOATING_TOOLBAR_KEY = "pref_appearance_image_viewer_show_floating_toolbar";
    static constexpr const char *PREF_APPEARANCE_SHOW_ASPECT_RATIO_INDICATOR_KEY = "pref_appearance_show_aspect_ratio_indicator";
    static constexpr const char *PREF_APPEARANCE_COMMENTS_SHOW_FLOATING_TOOLBAR_KEY = "pref_appearance_comments_show_floating_toolbar";
    static constexpr const char *PREF_APPEARANCE_INDENTLINES_KEY = "pref_appearance_indentlines";
    static constexpr const char *PREF_APPEARANCE_LEFT_HANDED_KEY = "pref_appearance_left_handed";
    static constexpr const char *PREF_APPEARANCE_BOTTOM_TOOLBAR_KEY = "pref_appearance_bottom_toolbar";
    static constexpr const char *PREF_APPEARANCE_HIDE_TOOLBAR_ON_SCROLL_KEY = "pref_appearance_hide_toolbar_on_scroll";
    static constexpr const char *PREF_APPEARANCE_POST_HIDE_SUBREDDIT_HEADER_KEY = "pref_appearance_post_hide_subreddit_header";
    static constexpr const char *PREF_APPEARANCE_HIDE_HEADERTOOLBAR_POSTLIST_KEY = "pref_appearance_hide_headertoolbar_postlist";
    static constexpr const char *PREF_APPEARANCE_HIDE_HEADERTOOLBAR_COMMENTLIST_KEY = "pref_appearance_hide_headertoolbar_commentlist";
    static constexpr const char *PREF_APPEARANCE_HIDE_COMMENTS_FROM_BLOCKED_USERS_KEY = "pref_appearance_hide_comments_from_blocked_users";
    static constexpr const char *PREF_APPEARANCE_HIGHLIGHT_OWN_USERNAME_KEY = "pref_appearance_highlight_own_username";
    static constexpr const char *PREF_APPEARANCE_POST_SUBTITLE_ITEMS_KEY = "pref_appearance_post_subtitle_items";
    static constexpr const char *PREF_APPEARANCE_POST_SUBTITLE_ITEMS_USE_DIFFERENT_SETTINGS_KEY = "pref_appearance_post_subtitle_items_use_different_settings";
    static constexpr const char *PREF_APPEARANCE_POST_HEADER_SUBTITLE_ITEMS_KEY = "pref_appearance_post_header_subtitle_items";
    static constexpr const char *PREF_APPEARANCE_POST_AGE_UNITS_KEY = "pref_appearance_post_age_units";
    static constexpr const char *PREF_APPEARANCE_POST_HEADER_AGE_UNITS_KEY = "pref_appearance_post_header_age_units";
    static constexpr const char *PREF_APPEARANCE_POST_SHOW_COMMENTS_BUTTON_KEY = "pref_appearance_post_show_comments_button";
    static constexpr const char *PREF_APPEARANCE_COMMENT_HEADER_ITEMS_KEY = "pref_appearance_comment_header_items";
    static constexpr const char *PREF_APPEARANCE_COMMENT_AGE_UNITS_KEY = "pref_appearance_comment_age_units";
    static constexpr const char *PREF_APPEARANCE_COMMENT_AGE_MODE_KEY = "pref_appearance_comment_age_mode";
    static constexpr const char *PREF_APPEARANCE_USER_SHOW_AVATARS_KEY = "pref_appearance_user_show_avatars";
    static constexpr const char *PREF_APPEARANCE_INBOX_AGE_UNITS_KEY = "pref_appearance_inbox_age_units";
    static constexpr const char *PREF_APPEARANCE_SHOW_BLOCKED_SUBREDDITS_MAIN_MENU_KEY = "pref_appearance_show_blocked_subreddits_main_menu";

    // -- Menus keys --
    static constexpr const char *PREF_MENUS_MAINMENU_USERITEMS_KEY = "pref_menus_mainmenu_useritems";
    static constexpr const char *PREF_MENUS_MAINMENU_SHORTCUTITEMS_KEY = "pref_menus_mainmenu_shortcutitems";
    static constexpr const char *PREF_MENUS_SHOW_POPULAR_MAIN_MENU_KEY = "pref_menus_show_popular_main_menu";
    static constexpr const char *PREF_MENUS_SHOW_MULTIREDDIT_MAIN_MENU_KEY = "pref_menus_show_multireddit_main_menu";
    static constexpr const char *PREF_MENUS_SHOW_SUBSCRIBED_SUBREDDITS_MAIN_MENU_KEY = "pref_menus_show_subscribed_subreddits_main_menu";
    static constexpr const char *PREF_MENUS_MAINMENU_DEV_ANNOUNCEMENTS_KEY = "pref_menus_mainmenu_dev_announcements";

    // -- Behaviour keys --
    static constexpr const char *PREF_BEHAVIOUR_FLING_POST_LEFT_KEY = "pref_behaviour_fling_post_left";
    static constexpr const char *PREF_BEHAVIOUR_FLING_POST_RIGHT_KEY = "pref_behaviour_fling_post_right";
    static constexpr const char *PREF_BEHAVIOUR_NSFW_KEY = "pref_behaviour_nsfw";
    static constexpr const char *PREF_BEHAVIOUR_POSTCOUNT_KEY = "pref_behaviour_postcount";
    static constexpr const char *PREF_BEHAVIOUR_COMMENT_MIN_KEY = "pref_behaviour_comment_min";
    static constexpr const char *PREF_BEHAVIOUR_PINNED_SUBREDDITSORT_KEY = "pref_behaviour_pinned_subredditsort";
    static constexpr const char *PREF_BEHAVIOUR_BLOCKED_SUBREDDITSORT_KEY = "pref_behaviour_blocked_subredditsort";
    static constexpr const char *PREF_BEHAVIOUR_SKIPTOFRONTPAGE_KEY = "pref_behaviour_skiptofrontpage";
    static constexpr const char *PREF_BEHAVIOUR_USEINTERNALBROWSER_KEY = "pref_behaviour_useinternalbrowser";
    static constexpr const char *PREF_BEHAVIOUR_USECUSTOMTABS_KEY = "pref_behaviour_usecustomtabs";
    static constexpr const char *PREF_BEHAVIOUR_ENABLE_SWIPE_REFRESH_KEY = "pref_behaviour_enable_swipe_refresh";
    static constexpr const char *PREF_BEHAVIOUR_NOTIFICATIONS_KEY = "pref_behaviour_notifications";
    static constexpr const char *PREF_BEHAVIOUR_VIDEO_PLAYBACK_CONTROLS_KEY = "pref_behaviour_video_playback_controls";
    static constexpr const char *PREF_BEHAVIOUR_VIDEO_FRAME_STEP_KEY = "pref_behaviour_video_frame_step";
    static constexpr const char *PREF_BEHAVIOUR_VIDEO_MUTE_DEFAULT_KEY = "pref_behaviour_video_mute_default";
    static constexpr const char *PREF_BEHAVIOUR_VIDEO_ZOOM_DEFAULT_KEY = "pref_behaviour_video_zoom_default";
    static constexpr const char *PREF_BEHAVIOUR_IMAGEVIDEO_TAP_CLOSE_KEY = "pref_behaviour_imagevideo_tap_close";
    static constexpr const char *PREF_BEHAVIOUR_BEZEL_TOOLBAR_SWIPEZONE_KEY = "pref_behaviour_bezel_toolbar_swipezone";
    static constexpr const char *PREF_BEHAVIOUR_POSTLIST_BACK_AGAIN_KEY = "pref_behaviour_postlist_back_again";
    static constexpr const char *PREF_BEHAVIOUR_GALLERY_SWIPE_LENGTH_KEY = "pref_behaviour_gallery_swipe_length";
    static constexpr const char *PREF_BEHAVIOUR_POST_TAP_ACTION_KEY = "pref_behaviour_post_tap_action";
    static constexpr const char *PREF_BEHAVIOUR_POST_TITLE_OPENS_COMMENTS_KEY = "pref_behaviour_post_title_opens_comments";
    static constexpr const char *PREF_BEHAVIOUR_BLOCK_SCREENSHOTS_KEY = "pref_behaviour_block_screenshots";
    static constexpr const char *PREF_BEHAVIOUR_KEEP_SCREEN_AWAKE_KEY = "pref_behaviour_keep_screen_awake";
    static constexpr const char *PREF_BEHAVIOUR_COLLAPSE_STICKY_COMMENTS_KEY = "pref_behaviour_collapse_sticky_comments";
    static constexpr const char *PREF_BEHAVIOUR_IMAGEVIEW_MODE_KEY = "pref_behaviour_imageview_mode";

    // -- Image keys --
    static constexpr const char *PREF_IMAGES_THUMBNAIL_SIZE_KEY = "pref_images_thumbnail_size";
    static constexpr const char *PREF_IMAGES_INLINE_IMAGE_PREVIEWS_KEY = "pref_images_inline_image_previews";
    static constexpr const char *PREF_IMAGES_INLINE_IMAGE_PREVIEWS_NSFW_KEY = "pref_images_inline_image_previews_nsfw";
    static constexpr const char *PREF_IMAGES_INLINE_IMAGE_PREVIEWS_SPOILER_KEY = "pref_images_inline_image_previews_spoiler";
    static constexpr const char *PREF_IMAGES_HIGH_RES_THUMBNAILS_KEY = "pref_images_high_res_thumbnails";

    // -- Accessibility keys --
    static constexpr const char *PREF_ACCESSIBILITY_SEPARATE_BODY_TEXT_LINES_KEY = "pref_accessibility_separate_body_text_lines";
    static constexpr const char *PREF_ACCESSIBILITY_MIN_COMMENT_HEIGHT_KEY = "pref_accessibility_min_comment_height";
    static constexpr const char *PREF_ACCESSIBILITY_SAY_COMMENT_INDENT_LEVEL_KEY = "pref_accessibility_say_comment_indent_level";
    static constexpr const char *PREF_ACCESSIBILITY_CONCISE_MODE_KEY = "pref_accessibility_concise_mode";

    // -- Miscellaneous --
    static constexpr const char *REDDIT_USER_AGREEMENT_PREF = "reddit_user_agreement";
    static constexpr const char *PREF_REDDIT_CLIENT_ID_OVERRIDE_KEY = "pref_reddit_client_id_override";
    static constexpr const char *PREF_ALBUM_SKIP_TO_FIRST_KEY = "pref_album_skip_to_first";
    static constexpr const char *PREF_VIDEOS_DOWNLOAD_BEFORE_PLAYING_KEY = "pref_videos_download_before_playing";

    // ========================================================================
    // Default values for string set preferences
    // ========================================================================
    static const std::set<QString> &getDefaultPostSubtitleItems();
    static const std::set<QString> &getDefaultCommentHeaderItems();

    // ========================================================================
    // Initialization (Java line 180-189)
    // ========================================================================
    static void init(QSettings &settings);

    // ========================================================================
    // Core getters/setters (Java lines 63-101)
    // ========================================================================
    static QString getString(const QString &key, const QString &defaultValue);
    static std::set<QString> getStringSet(const QString &key, const std::set<QString> &defaultValue);
    static bool getBoolean(const QString &key, bool defaultValue);
    static void setBoolean(const QString &key, bool newValue);
    static qint64 getLong(const QString &key, qint64 defaultValue);

    // ========================================================================
    // isReLayoutRequired (Java lines 103-109)
    // ========================================================================
    static bool isReLayoutRequired(const QString &key);

    // ========================================================================
    // isRefreshRequired (Java lines 111-154)
    // ========================================================================
    static bool isRefreshRequired(const QString &key);

    // ========================================================================
    // isRestartRequired (Java lines 156-178)
    // ========================================================================
    static bool isRestartRequired(const QString &key);

    // ========================================================================
    // Appearance accessors (Java lines 195-711)
    // ========================================================================

    static AppearanceTwopane appearance_twopane();
    static bool isNightMode();
    static AppearanceTheme appearance_theme();
    static AppearanceNavbarColour appearance_navbar_colour();

    static NeverAlwaysOrWifiOnly appearance_thumbnails_show();
    static NeverAlwaysOrWifiOnly appearance_thumbnails_show_old();
    static bool appearance_thumbnails_nsfw_show();
    static bool appearance_thumbnails_spoiler_show();

    static float appearance_fontscale_global();
    static float appearance_fontscale_bodytext();
    static float appearance_fontscale_comment_headers();
    static float appearance_fontscale_linkbuttons();
    static float appearance_fontscale_posts();
    static float appearance_fontscale_post_subtitles();
    static float appearance_fontscale_post_header_titles();
    static float appearance_fontscale_post_header_subtitles();

    static bool pref_appearance_hide_username_main_menu();
    static bool pref_show_popular_main_menu();
    static bool pref_show_multireddit_main_menu();
    static bool pref_show_subscribed_subreddits_main_menu();
    static bool pref_menus_mainmenu_dev_announcements();
    static bool pref_appearance_show_blocked_subreddits_main_menu();
    static bool pref_appearance_linkbuttons();

    static AppearanceStatusBarMode pref_appearance_android_status();
    static bool pref_appearance_link_text_clickable();
    static bool pref_appearance_image_viewer_show_floating_toolbar();
    static bool pref_appearance_show_aspect_ratio_indicator();
    static bool pref_album_skip_to_first();
    static bool pref_appearance_comments_show_floating_toolbar();
    static bool pref_appearance_indentlines();
    static bool pref_appearance_left_handed();
    static bool pref_appearance_bottom_toolbar();
    static bool pref_appearance_hide_toolbar_on_scroll();
    static bool pref_appearance_post_hide_subreddit_header();
    static bool pref_appearance_hide_headertoolbar_postlist();
    static bool pref_appearance_hide_headertoolbar_commentlist();
    static bool pref_appearance_hide_comments_from_blocked_users();
    static bool pref_appearance_highlight_own_username();

    static std::set<AppearancePostSubtitleItem> appearance_post_subtitle_items();
    static int appearance_post_age_units();
    static bool appearance_post_subtitle_items_use_different_settings();
    static std::set<AppearancePostSubtitleItem> appearance_post_header_subtitle_items();
    static int appearance_post_header_age_units();
    static bool appearance_post_show_comments_button();

    static std::set<AppearanceCommentHeaderItem> appearance_comment_header_items();
    static int appearance_comment_age_units();
    static bool appearance_user_show_avatars();
    static CommentAgeMode appearance_comment_age_mode();
    static int appearance_inbox_age_units();

    // ========================================================================
    // Image accessors (Java lines 723-757)
    // ========================================================================
    static int images_thumbnail_size_dp();
    static NeverAlwaysOrWifiOnly images_inline_image_previews();
    static bool images_inline_image_previews_nsfw();
    static bool images_inline_image_previews_spoiler();
    static NeverAlwaysOrWifiOnly images_high_res_thumbnails();

    // ========================================================================
    // Behaviour accessors (Java lines 759-899)
    // ========================================================================
    static bool pref_behaviour_skiptofrontpage();
    static bool pref_behaviour_useinternalbrowser();
    static bool pref_behaviour_usecustomtabs();
    static void set_pref_behaviour_notifications(bool enabled);
    static bool pref_behaviour_notifications();
    static bool pref_behaviour_enable_swipe_refresh();
    static bool pref_behaviour_video_playback_controls();
    static bool pref_behaviour_video_frame_step();
    static bool pref_behaviour_video_mute_default();
    static bool pref_behaviour_video_zoom_default();
    static bool pref_videos_download_before_playing();
    static bool pref_behaviour_imagevideo_tap_close();
    static int pref_behaviour_bezel_toolbar_swipezone_dp();
    static bool pref_behaviour_back_again();
    static int pref_behaviour_gallery_swipe_length_dp();
    static std::optional<int> pref_behaviour_comment_min();
    static PostTapAction pref_behaviour_post_tap_action();
    static bool pref_behaviour_post_title_opens_comments();

    // ========================================================================
    // isNightMode (Java lines 208-215)
    // ========================================================================
    // (declared above in Appearance section)

private:
    static QSettings *s_settings;

    // ---- Helper: uppercase a string for enum lookup (like StringUtils.asciiUppercase) ----
    static QString asciiUppercase(const QString &s);

    // ---- Parse enum from string with fallback ----
    template<typename E>
    static E parseEnum(const QString &key, const QString &defaultVal,
                       const std::function<E(const QString &)> &parser);
};

// ========================================================================
// Template implementation
// ========================================================================

template<typename E>
E PrefsUtility::parseEnum(const QString &key, const QString &defaultVal,
                          const std::function<E(const QString &)> &parser)
{
    return parser(asciiUppercase(getString(key, defaultVal)));
}

} // namespace PinkReader
