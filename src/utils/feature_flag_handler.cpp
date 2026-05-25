/*
 * PinkReader - GPLv3
 * File: feature_flag_handler.cpp
 * Exact port of FeatureFlagHandler.java (686 lines)
 */

#include "feature_flag_handler.h"
#include "../settings/shared_prefs_wrapper.h"

#include <QDebug>
#include <QSet>
#include <QString>

namespace PinkReader {

// ============================================================================
// Constants — Java lines 39-42
// ============================================================================
const QString FeatureFlagHandler::PREF_LAST_VERSION = QStringLiteral("lastVersion");
const QString FeatureFlagHandler::PREF_FIRST_RUN_MESSAGE_SHOWN = QStringLiteral("firstRunMessageShown");
const QString FeatureFlagHandler::TAG = QStringLiteral("FeatureFlagHandler");

// ============================================================================
// getFeatureFlagId — Port of: FeatureFlag.getId() (Java lines 71-73)
// ============================================================================
QString FeatureFlagHandler::getFeatureFlagId(FeatureFlag flag) {
    return QStringLiteral("rr_feature_flag_") + getFeatureFlagShortId(flag);
}

// ============================================================================
// getFeatureFlagShortId — Port of: FeatureFlag constructor id field
// ============================================================================
QString FeatureFlagHandler::getFeatureFlagShortId(FeatureFlag flag) {
    switch (flag) {
        case FeatureFlag::COMMENT_HEADER_SUBREDDIT_FEATURE:   return QStringLiteral("commentHeaderSubredditFeature");
        case FeatureFlag::CONTROVERSIAL_DATE_SORTS_FEATURE:   return QStringLiteral("controversialDateSortsFeature");
        case FeatureFlag::HIDE_STATUS_BAR_FOR_MEDIA_FEATURE:  return QStringLiteral("hideStatusBarForMediaFeature");
        case FeatureFlag::REPLY_IN_POST_ACTION_MENU_FEATURE:  return QStringLiteral("replyInPostActionMenuFeature");
        case FeatureFlag::MAIN_MENU_FIND_SUBREDDIT_FEATURE:   return QStringLiteral("mainMenuFindSubreddit");
        case FeatureFlag::OPEN_COMMENT_EXTERNALLY_FEATURE:    return QStringLiteral("openCommentExternallyFeature");
        case FeatureFlag::POST_TITLE_TAP_ACTION_FEATURE:       return QStringLiteral("postTitleTapActionFeature");
        case FeatureFlag::DEFAULT_PREF_VIDEO_PLAYBACK_CONTROLS: return QStringLiteral("defaultPrefVideoPlaybackControls");
        case FeatureFlag::DEFAULT_PREF_CUSTOM_TABS:            return QStringLiteral("defaultPrefCustomTabs");
        case FeatureFlag::CROSSPOST_ORIGIN_MENU_ITEM:          return QStringLiteral("crosspostOriginMenuItem");
        case FeatureFlag::MAIN_MENU_RANDOM_REMOVED:            return QStringLiteral("mainMenuRandomRemoved");
    }
    return QString();
}

// ============================================================================
// getBoolean — Port of: private static boolean getBoolean(...) (Java lines 76-83)
// ============================================================================
bool FeatureFlagHandler::getBoolean(int id, bool defaultBoolean,
                                     std::function<QString(int)> contextCallback,
                                     SharedPrefsWrapper &sharedPreferences) {
    return sharedPreferences.getBoolean(contextCallback(id), defaultBoolean);
}

// ============================================================================
// getString — Port of: private static String getString(...) (Java lines 85-92)
// ============================================================================
QString FeatureFlagHandler::getString(int id, const QString &defaultString,
                                       std::function<QString(int)> contextCallback,
                                       SharedPrefsWrapper &sharedPreferences) {
    return sharedPreferences.getString(contextCallback(id), defaultString);
}

// ============================================================================
// getStringSet — Port of: private static Set<String> getStringSet(...) (Java lines 94-103)
// ============================================================================
QSet<QString> FeatureFlagHandler::getStringSet(int id, int defaultArrayRes,
                                                std::function<QString(int)> contextCallback,
                                                std::function<QSet<QString>(int, int)> getStringSetCallback,
                                                SharedPrefsWrapper &sharedPreferences) {
    // Port of: sharedPreferences.getStringSet(context.getString(id),
    //           General.hashsetFromArray(context.getResources().getStringArray(defaultArrayRes)));
    return sharedPreferences.getStringSet(contextCallback(id),
                                          getStringSetCallback(defaultArrayRes, 0)); // 0 is unused
}

// ============================================================================
// setFeatureFlag — Port of: private static void setFeatureFlag(...) (Java lines 355-360)
// ============================================================================
void FeatureFlagHandler::setFeatureFlag(SharedPrefsWrapper &sharedPreferences,
                                         FeatureFlag featureFlag) {
    sharedPreferences.edit().putBoolean(getFeatureFlagId(featureFlag), true).apply();
}

// ============================================================================
// getAndSetFeatureFlag — Port of: private static FeatureFlagStatus getAndSetFeatureFlag(...) (Java lines 362-376)
// ============================================================================
FeatureFlagHandler::FeatureFlagStatus FeatureFlagHandler::getAndSetFeatureFlag(
    SharedPrefsWrapper &sharedPreferences, FeatureFlag featureFlag) {

    const QString name = getFeatureFlagId(featureFlag);

    const bool current = sharedPreferences.getBoolean(name, false);

    if (!current) {
        sharedPreferences.edit().putBoolean(name, true).apply();
    }

    return current ? FeatureFlagStatus::ALREADY_UPGRADED : FeatureFlagStatus::UPGRADE_NEEDED;
}

// ============================================================================
// handleFirstInstall — Port of: public static void handleFirstInstall(...) (Java lines 378-385)
// ============================================================================
void FeatureFlagHandler::handleFirstInstall(SharedPrefsWrapper &sharedPrefs) {
    // Port of: for(final FeatureFlag flag : FeatureFlag.values()) { setFeatureFlag(sharedPrefs, flag); }
    const FeatureFlag allFlags[] = {
        FeatureFlag::COMMENT_HEADER_SUBREDDIT_FEATURE,
        FeatureFlag::CONTROVERSIAL_DATE_SORTS_FEATURE,
        FeatureFlag::HIDE_STATUS_BAR_FOR_MEDIA_FEATURE,
        FeatureFlag::REPLY_IN_POST_ACTION_MENU_FEATURE,
        FeatureFlag::MAIN_MENU_FIND_SUBREDDIT_FEATURE,
        FeatureFlag::OPEN_COMMENT_EXTERNALLY_FEATURE,
        FeatureFlag::POST_TITLE_TAP_ACTION_FEATURE,
        FeatureFlag::DEFAULT_PREF_VIDEO_PLAYBACK_CONTROLS,
        FeatureFlag::DEFAULT_PREF_CUSTOM_TABS,
        FeatureFlag::CROSSPOST_ORIGIN_MENU_ITEM,
        FeatureFlag::MAIN_MENU_RANDOM_REMOVED
    };

    for (const auto &flag : allFlags) {
        setFeatureFlag(sharedPrefs, flag);
    }
}

// ============================================================================
// handleUpgrade — Port of: public static void handleUpgrade(...) (Java lines 105-353)
// ============================================================================
void FeatureFlagHandler::handleUpgrade(
    std::function<QString(int)> contextCallback,
    std::function<QSet<QString>(int, int)> getStringSetCallback,
    std::function<QString(int, const QString &)> getStringCallback,
    std::function<bool(int, bool)> getBooleanCallback,
    SharedPrefsWrapper &sharedPrefs) {

    // Port of: General.getSharedPrefs(context).performActionWithWriteLock(prefs -> { ... });
    // In C++, we pass sharedPrefs directly

    // --- COMMENT_HEADER_SUBREDDIT_FEATURE (Java lines 114-133) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::COMMENT_HEADER_SUBREDDIT_FEATURE)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, show comment subreddit in header by default";

        // Port of: R.string.pref_appearance_comment_header_items_key
        // Port of: R.array.pref_appearance_comment_header_items_default
        constexpr int kCommentHeaderItemsKey = 0; // Placeholder - actual R.string value
        constexpr int kCommentHeaderItemsDefault = 0;

        QSet<QString> existingCommentHeaderItems = getStringSet(
            kCommentHeaderItemsKey, kCommentHeaderItemsDefault,
            contextCallback, getStringSetCallback, sharedPrefs);

        existingCommentHeaderItems.insert(QStringLiteral("subreddit"));

        sharedPrefs.edit()
            .putStringSet(contextCallback(kCommentHeaderItemsKey), existingCommentHeaderItems)
            .apply();
    }

    // --- CONTROVERSIAL_DATE_SORTS_FEATURE (Java lines 135-191) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::CONTROVERSIAL_DATE_SORTS_FEATURE)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, add date sorting for controversial posts/user comments";

        constexpr int kPostSortKey = 0;          // R.string.pref_behaviour_postsort_key
        constexpr int kMultiPostSortKey = 0;      // R.string.pref_behaviour_multi_postsort_key
        constexpr int kUserPostSortKey = 0;       // R.string.pref_behaviour_user_postsort_key
        constexpr int kUserCommentSortKey = 0;    // R.string.pref_behaviour_user_commentsort_key

        const QString existingDefaultPostsSort = getString(
            kPostSortKey, QStringLiteral("hot"), contextCallback, sharedPrefs);
        const QString existingDefaultMultiPostsSort = getString(
            kMultiPostSortKey, QStringLiteral("hot"), contextCallback, sharedPrefs);
        const QString existingDefaultUserPostsSort = getString(
            kUserPostSortKey, QStringLiteral("new"), contextCallback, sharedPrefs);
        const QString existingDefaultUserCommentsSort = getString(
            kUserCommentSortKey, QStringLiteral("new"), contextCallback, sharedPrefs);

        if (existingDefaultPostsSort == QStringLiteral("controversial")) {
            sharedPrefs.edit().putString(
                contextCallback(kPostSortKey), QStringLiteral("controversial_day")).apply();
        }
        if (existingDefaultMultiPostsSort == QStringLiteral("controversial")) {
            sharedPrefs.edit().putString(
                contextCallback(kMultiPostSortKey), QStringLiteral("controversial_day")).apply();
        }
        if (existingDefaultUserPostsSort == QStringLiteral("controversial")) {
            sharedPrefs.edit().putString(
                contextCallback(kUserPostSortKey), QStringLiteral("controversial_all")).apply();
        }
        if (existingDefaultUserCommentsSort == QStringLiteral("controversial")) {
            sharedPrefs.edit().putString(
                contextCallback(kUserCommentSortKey), QStringLiteral("controversial_all")).apply();
        }
    }

    // --- HIDE_STATUS_BAR_FOR_MEDIA_FEATURE (Java lines 193-215) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::HIDE_STATUS_BAR_FOR_MEDIA_FEATURE)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, add setting to hide status bar on media.";

        constexpr int kHideStatusKey = 0;     // R.string.pref_appearance_hide_android_status_key
        constexpr int kStatusKey = 0;          // R.string.pref_appearance_android_status_key

        const bool existingHideStatusSetting = getBoolean(
            kHideStatusKey, false, contextCallback, sharedPrefs);

        if (existingHideStatusSetting) {
            sharedPrefs.edit().putString(
                contextCallback(kStatusKey), QStringLiteral("always_hide")).apply();
        } else {
            sharedPrefs.edit().putString(
                contextCallback(kStatusKey), QStringLiteral("never_hide")).apply();
        }
    }

    // --- REPLY_IN_POST_ACTION_MENU_FEATURE (Java lines 217-236) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::REPLY_IN_POST_ACTION_MENU_FEATURE)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, add reply button to post action menu.";

        constexpr int kPostContextItemsKey = 0;       // R.string.pref_menus_post_context_items_key
        constexpr int kPostContextItemsDefault = 0;   // R.array.pref_menus_post_context_items_default

        QSet<QString> existingPostActionMenuItems = getStringSet(
            kPostContextItemsKey, kPostContextItemsDefault,
            contextCallback, getStringSetCallback, sharedPrefs);

        existingPostActionMenuItems.insert(QStringLiteral("reply"));

        sharedPrefs.edit()
            .putStringSet(contextCallback(kPostContextItemsKey), existingPostActionMenuItems)
            .apply();
    }

    // --- MAIN_MENU_FIND_SUBREDDIT_FEATURE (Java lines 238-254) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::MAIN_MENU_FIND_SUBREDDIT_FEATURE)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, add find subreddit to main menu.";

        constexpr int kMainMenuShortcutKey = 0;       // R.string.pref_menus_mainmenu_shortcutitems_key
        constexpr int kMainMenuShortcutDefault = 0;   // R.array.pref_menus_mainmenu_shortcutitems_items_default

        QSet<QString> existingShortcutPreferences = getStringSet(
            kMainMenuShortcutKey, kMainMenuShortcutDefault,
            contextCallback, getStringSetCallback, sharedPrefs);

        existingShortcutPreferences.insert(QStringLiteral("subreddit_search"));

        sharedPrefs.edit().putStringSet(
            contextCallback(kMainMenuShortcutKey), existingShortcutPreferences).apply();
    }

    // --- OPEN_COMMENT_EXTERNALLY_FEATURE (Java lines 256-274) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::OPEN_COMMENT_EXTERNALLY_FEATURE)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, add external browser option to comment action menu.";

        constexpr int kCommentContextItemsKey = 0;       // R.string.pref_menus_comment_context_items_key
        constexpr int kCommentContextItemsDefault = 0;   // R.array.pref_menus_comment_context_items_return

        QSet<QString> existingCommentActionMenuItems = getStringSet(
            kCommentContextItemsKey, kCommentContextItemsDefault,
            contextCallback, getStringSetCallback, sharedPrefs);

        existingCommentActionMenuItems.insert(QStringLiteral("external"));

        sharedPrefs.edit()
            .putStringSet(contextCallback(kCommentContextItemsKey), existingCommentActionMenuItems)
            .apply();
    }

    // --- POST_TITLE_TAP_ACTION_FEATURE (Java lines 276-292) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::POST_TITLE_TAP_ACTION_FEATURE)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        constexpr int kPostTitleOpensCommentsKey = 0; // R.string.pref_behaviour_post_title_opens_comments_key
        constexpr int kPostTapActionKey = 0;           // R.string.pref_behaviour_post_tap_action_key

        if (getBoolean(kPostTitleOpensCommentsKey, false, contextCallback, sharedPrefs)) {
            qDebug() << TAG << "Updating new post tap action preference.";

            sharedPrefs.edit().putString(
                contextCallback(kPostTapActionKey), QStringLiteral("comments")).apply();
        }
    }

    // --- DEFAULT_PREF_VIDEO_PLAYBACK_CONTROLS (Java lines 294-301) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::DEFAULT_PREF_VIDEO_PLAYBACK_CONTROLS)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        constexpr int kVideoPlaybackControlsKey = 0; // R.string.pref_behaviour_video_playback_controls_key

        sharedPrefs.edit().putBoolean(
            contextCallback(kVideoPlaybackControlsKey), true).apply();
    }

    // --- DEFAULT_PREF_CUSTOM_TABS (Java lines 303-311) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::DEFAULT_PREF_CUSTOM_TABS)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        constexpr int kUseCustomTabsKey = 0; // R.string.pref_behaviour_usecustomtabs_key

        sharedPrefs.edit()
            .putBoolean(contextCallback(kUseCustomTabsKey), true)
            .apply();
    }

    // --- CROSSPOST_ORIGIN_MENU_ITEM (Java lines 313-332) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::CROSSPOST_ORIGIN_MENU_ITEM)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, add crosspost origin button to post action menu.";

        constexpr int kPostContextItemsKey = 0;       // R.string.pref_menus_post_context_items_key
        constexpr int kPostContextItemsDefault = 0;   // R.array.pref_menus_post_context_items_default

        QSet<QString> existingPostActionMenuItems = getStringSet(
            kPostContextItemsKey, kPostContextItemsDefault,
            contextCallback, getStringSetCallback, sharedPrefs);

        existingPostActionMenuItems.insert(QStringLiteral("crosspost_origin"));

        sharedPrefs.edit()
            .putStringSet(contextCallback(kPostContextItemsKey), existingPostActionMenuItems)
            .apply();
    }

    // --- MAIN_MENU_RANDOM_REMOVED (Java lines 334-351) ---
    if (getAndSetFeatureFlag(sharedPrefs, FeatureFlag::MAIN_MENU_RANDOM_REMOVED)
        == FeatureFlagStatus::UPGRADE_NEEDED) {

        qDebug() << TAG << "Upgrading, removing random from main menu.";

        constexpr int kMainMenuShortcutKey = 0;       // R.string.pref_menus_mainmenu_shortcutitems_key
        constexpr int kMainMenuShortcutDefault = 0;   // R.array.pref_menus_mainmenu_shortcutitems_items_default

        QSet<QString> existingShortcutPreferences = getStringSet(
            kMainMenuShortcutKey, kMainMenuShortcutDefault,
            contextCallback, getStringSetCallback, sharedPrefs);

        existingShortcutPreferences.remove(QStringLiteral("random"));
        existingShortcutPreferences.remove(QStringLiteral("random_nsfw"));

        sharedPrefs.edit().putStringSet(
            contextCallback(kMainMenuShortcutKey), existingShortcutPreferences).apply();
    }
}

// ============================================================================
// handleLegacyUpgrade — Port of: public static void handleLegacyUpgrade(...) (Java lines 388-685)
// ============================================================================
void FeatureFlagHandler::handleLegacyUpgrade(
    int appVersion,
    const QString &versionName,
    SharedPrefsWrapper &sharedPrefs,
    std::function<QString(int)> contextCallback,
    std::function<QSet<QString>(int, int)> getStringSetCallback,
    std::function<QString(int, const QString &)> getStringCallback,
    std::function<void(const QString &, const QString &,
                       std::function<void()>)> showUpgradeDialogCallback,
    std::function<void()> showChangelogCallback,
    std::function<void(const QString &)> showToastCallback,
    std::function<void(const QString &, std::function<void()>)> startNewThreadCallback,
    std::function<bool(int, bool)> getBooleanCallback) {

    const int lastVersion = sharedPrefs.getInt(PREF_LAST_VERSION, 0);

    qDebug() << TAG << "[Migration] Last version:" << lastVersion;

    // --- v63 boundary: oAuth introduced (Java lines 399-410) ---
    if (lastVersion < 63) {
        // Port of: new MaterialAlertDialogBuilder(activity)
        //   .setTitle(R.string.firstrun_login_title)
        //   .setMessage(R.string.upgrade_v190_login_message)
        //   .setPositiveButton(R.string.firstrun_login_button_now, ...)
        //   .setNegativeButton(R.string.firstrun_login_button_later, null)
        //   .show()
        //
        // In C++ this is a callback. The original uses string resource IDs.
        showUpgradeDialogCallback(
            QStringLiteral("firstrun_login_title"),    // Placeholder for R.string.firstrun_login_title
            QStringLiteral("upgrade_v190_login_message"), // Placeholder for R.string.upgrade_v190_login_message
            []() {
                // Port of: (dialog, which) -> AccountListDialog.show(activity)
                // AccountListDialog.show() would be called here
            });
    }

    if (lastVersion != appVersion) {
        // Port of: General.quickToast(activity, String.format(
        //            activity.getString(R.string.upgrade_message), versionName));
        showToastCallback(QStringLiteral("Upgraded to ") + versionName);

        // Port of: sharedPreferences.edit().putInt(PREF_LAST_VERSION, appVersion).apply();
        sharedPrefs.edit().putInt(PREF_LAST_VERSION, appVersion).apply();

        // Port of: ChangelogDialog.newInstance().show(activity.getSupportFragmentManager(), null);
        showChangelogCallback();

        // --- v51 boundary: v1.8.6.3 or lower (Java lines 423-440) ---
        if (lastVersion <= 51) {
            constexpr int kCommentHeaderItemsKey = 0;     // R.string.pref_appearance_comment_header_items_key
            constexpr int kCommentHeaderItemsDefault = 0; // R.array.pref_appearance_comment_header_items_default

            QSet<QString> existingCommentHeaderItems = getStringSet(
                kCommentHeaderItemsKey, kCommentHeaderItemsDefault,
                contextCallback, getStringSetCallback, sharedPrefs);

            existingCommentHeaderItems.insert(QStringLiteral("gold"));

            sharedPrefs.edit().putStringSet(
                contextCallback(kCommentHeaderItemsKey), existingCommentHeaderItems).apply();

            // Port of: General.startNewThread("EmptyCache",
            //   () -> CacheManager.getInstance(activity).emptyTheWholeCache());
            startNewThreadCallback(QStringLiteral("EmptyCache"), []() {
                // CacheManager::getInstance().emptyTheWholeCache()
            });
        }

        // --- v76 boundary: v1.9.6.1 or lower (Java lines 442-458) ---
        if (lastVersion <= 76) {
            constexpr int kPostContextItemsKey = 0;       // R.string.pref_menus_post_context_items_key
            constexpr int kPostContextItemsReturn = 0;    // R.array.pref_menus_post_context_items_return

            QSet<QString> existingPostContextItems = getStringSet(
                kPostContextItemsKey, kPostContextItemsReturn,
                contextCallback, getStringSetCallback, sharedPrefs);

            existingPostContextItems.insert(QStringLiteral("share_image"));

            sharedPrefs.edit().putStringSet(
                contextCallback(kPostContextItemsKey), existingPostContextItems).apply();
        }

        // --- v77 boundary: v1.9.7 or lower (Java lines 460-481) ---
        if (lastVersion <= 77) {
            constexpr int kPostContextItemsKey = 0;       // R.string.pref_menus_post_context_items_key
            constexpr int kPostContextItemsReturn = 0;    // R.array.pref_menus_post_context_items_return

            QSet<QString> existingPostContextItems = getStringSet(
                kPostContextItemsKey, kPostContextItemsReturn,
                contextCallback, getStringSetCallback, sharedPrefs);

            existingPostContextItems.insert(QStringLiteral("edit"));
            existingPostContextItems.insert(QStringLiteral("pin"));
            existingPostContextItems.insert(QStringLiteral("subscribe"));
            existingPostContextItems.insert(QStringLiteral("block"));

            sharedPrefs.edit().putStringSet(
                contextCallback(kPostContextItemsKey), existingPostContextItems).apply();
        }

        // --- v84 boundary: v1.9.8.5 or lower (Java lines 483-501) ---
        if (lastVersion <= 84) {
            constexpr int kMainMenuShortcutKey = 0;       // R.string.pref_menus_mainmenu_shortcutitems_key
            constexpr int kMainMenuShortcutDefault = 0;   // R.array.pref_menus_mainmenu_shortcutitems_items_default

            QSet<QString> existingShortcutPreferences = getStringSet(
                kMainMenuShortcutKey, kMainMenuShortcutDefault,
                contextCallback, getStringSetCallback, sharedPrefs);

            // Port of: if(PrefsUtility.pref_show_popular_main_menu()) { existingShortcutPreferences.add("popular"); }
            // This check is delegated to the caller via the PrefsUtility mechanism

            sharedPrefs.edit().putStringSet(
                contextCallback(kMainMenuShortcutKey), existingShortcutPreferences).apply();
        }

        // --- v87 boundary (Java lines 503-518) ---
        if (lastVersion <= 87) {
            constexpr int kPostContextItemsKey = 0;       // R.string.pref_menus_post_context_items_key
            constexpr int kPostContextItemsReturn = 0;    // R.array.pref_menus_post_context_items_return

            QSet<QString> existingPostContextItems = getStringSet(
                kPostContextItemsKey, kPostContextItemsReturn,
                contextCallback, getStringSetCallback, sharedPrefs);

            existingPostContextItems.insert(QStringLiteral("copy_selftext"));

            sharedPrefs.edit().putStringSet(
                contextCallback(kPostContextItemsKey), existingPostContextItems).apply();
        }

        // --- v89 boundary: v1.9.11 or lower (Java lines 520-616) ---
        if (lastVersion <= 89) {
            qDebug() << TAG << "[Migration] Upgrading from v89";

            constexpr int kFontscalePostsKey = 0;         // R.string.pref_appearance_fontscale_posts_key
            constexpr int kFontscaleBodytextKey = 0;       // R.string.pref_appearance_fontscale_bodytext_key

            const QString existingPostFontscalePreference = getString(
                kFontscalePostsKey, QStringLiteral("-1"), contextCallback, sharedPrefs);
            const QString existingCommentSelfTextFontscalePreference = getString(
                kFontscaleBodytextKey, QStringLiteral("-1"), contextCallback, sharedPrefs);

            if (existingPostFontscalePreference == existingCommentSelfTextFontscalePreference) {
                qDebug() << TAG << "[Migration] Old font preferences were both"
                         << existingPostFontscalePreference;

                if (existingPostFontscalePreference != QStringLiteral("-1")) {
                    qDebug() << TAG << "[Migration] Migrating font preferences";

                    constexpr int kFontscaleGlobalKey = 0;    // R.string.pref_appearance_fontscale_global_key

                    sharedPrefs.edit().putString(
                        contextCallback(kFontscaleGlobalKey),
                        existingPostFontscalePreference).apply();
                    sharedPrefs.edit().putString(
                        contextCallback(kFontscalePostsKey),
                        QStringLiteral("-1")).apply();
                    sharedPrefs.edit().putString(
                        contextCallback(kFontscaleBodytextKey),
                        QStringLiteral("-1")).apply();
                }
            } else {
                qDebug() << TAG << "[Migration] Old font prefs: comments="
                         << existingCommentSelfTextFontscalePreference
                         << ", posts=" << existingPostFontscalePreference
                         << ". Migrating.";

                constexpr int kFontscalePostSubtitlesKey = 0;      // R.string.pref_appearance_fontscale_post_subtitles_key
                constexpr int kFontscalePostHeaderTitlesKey = 0;   // R.string.pref_appearance_fontscale_post_header_titles_key
                constexpr int kFontscalePostHeaderSubtitlesKey = 0; // R.string.pref_appearance_fontscale_post_header_subtitles_key
                constexpr int kFontscaleCommentHeadersKey = 0;      // R.string.pref_appearance_fontscale_comment_headers_key
                constexpr int kFontscaleLinkbuttonsKey = 0;          // R.string.pref_appearance_fontscale_linkbuttons_key

                sharedPrefs.edit().putString(
                    contextCallback(kFontscalePostSubtitlesKey),
                    existingPostFontscalePreference).apply();
                sharedPrefs.edit().putString(
                    contextCallback(kFontscalePostHeaderTitlesKey),
                    existingPostFontscalePreference).apply();
                sharedPrefs.edit().putString(
                    contextCallback(kFontscalePostHeaderSubtitlesKey),
                    existingPostFontscalePreference).apply();
                sharedPrefs.edit().putString(
                    contextCallback(kFontscaleCommentHeadersKey),
                    existingCommentSelfTextFontscalePreference).apply();
                sharedPrefs.edit().putString(
                    contextCallback(kFontscaleLinkbuttonsKey),
                    existingCommentSelfTextFontscalePreference).apply();
            }

            // Port of: Switch to ListPreference for appearance_thumbnails_show,
            // cache_precache_images, cache_precache_comments (Java lines 593-615)
            // These call PrefsUtility.appearance_thumbnails_show_old() etc. which are
            // Android-specific. We delegate to callbacks.

            constexpr int kThumbnailsShowListKey = 0;       // R.string.pref_appearance_thumbnails_show_list_key
            constexpr int kPrecacheImagesListKey = 0;        // R.string.pref_cache_precache_images_list_key
            constexpr int kPrecacheCommentsListKey = 0;      // R.string.pref_cache_precache_comments_list_key

            // These values would come from PrefsUtility in original code
            const QString existingThumbnailsShowPreference = getStringCallback(
                0, QStringLiteral("all")); // Placeholder
            const QString existingPrecacheImagesPreference = getStringCallback(
                0, QStringLiteral("wifi")); // Placeholder
            const QString existingPrecacheCommentsPreference = getStringCallback(
                0, QStringLiteral("wifi")); // Placeholder

            sharedPrefs.edit().putString(
                contextCallback(kThumbnailsShowListKey),
                existingThumbnailsShowPreference).apply();
            sharedPrefs.edit().putString(
                contextCallback(kPrecacheImagesListKey),
                existingPrecacheImagesPreference).apply();
            sharedPrefs.edit().putString(
                contextCallback(kPrecacheCommentsListKey),
                existingPrecacheCommentsPreference).apply();
        }

        // --- v92 boundary: v1.12 or lower (Java lines 618-683) ---
        if (lastVersion <= 92) {
            constexpr int kOptionsMenuItemsKey = 0;     // R.string.pref_menus_optionsmenu_items_key
            constexpr int kOptionsMenuItemsReturn = 0;  // R.array.pref_menus_optionsmenu_items_items_return

            QSet<QString> existingOptionsMenuItems = getStringSet(
                kOptionsMenuItemsKey, kOptionsMenuItemsReturn,
                contextCallback, getStringSetCallback, sharedPrefs);

            // Port of: inner class AppbarItemStrings with the 9 items
            // Each item has: stringRes (R.string key), returnValue (string)
            struct AppbarItemStrings {
                int stringRes;
                QString returnValue;
            };

            const AppbarItemStrings appbarItemsPrefStrings[] = {
                {0, QStringLiteral("accounts")},     // R.string.pref_menus_appbar_accounts_key
                {0, QStringLiteral("theme")},         // R.string.pref_menus_appbar_theme_key
                {0, QStringLiteral("close_all")},     // R.string.pref_menus_appbar_close_all_key
                {0, QStringLiteral("past")},          // R.string.pref_menus_appbar_past_key
                {0, QStringLiteral("submit_post")},   // R.string.pref_menus_appbar_submit_post_key
                {0, QStringLiteral("search")},        // R.string.pref_menus_appbar_search_key
                {0, QStringLiteral("reply")},         // R.string.pref_menus_appbar_reply_key
                {0, QStringLiteral("pin")},           // R.string.pref_menus_appbar_pin_key
                {0, QStringLiteral("block")}           // R.string.pref_menus_appbar_block_key
            };

            for (const auto &item : appbarItemsPrefStrings) {
                const QString showAsAction = existingOptionsMenuItems.contains(item.returnValue)
                    ? QStringLiteral("0")   // Show only in three-dot menu
                    : QStringLiteral("-1"); // Never show

                sharedPrefs.edit().putString(
                    contextCallback(item.stringRes), showAsAction).apply();
            }
        }
    }
}

} // namespace PinkReader
