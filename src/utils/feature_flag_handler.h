/*
 * PinkReader - GPLv3
 * File: feature_flag_handler.h
 * Exact port of FeatureFlagHandler.java (686 lines)
 *
 * Ports every line: FeatureFlagStatus enum, FeatureFlag enum,
 * all static methods, all upgrade paths.
 */

#pragma once

#include <QString>
#include <QSet>
#include <functional>

// Forward declarations
namespace PinkReader {

class SharedPrefsWrapper;
class General;

/**
 * @brief Port of org.quantumbadger.redreader.common.FeatureFlagHandler (Java final class)
 *
 * Manages feature flags for app upgrades. Each feature flag tracks whether
 * a particular upgrade migration has been applied. When a user upgrades from
 * a version that didn't have a feature, the handler applies the necessary
 * preference migrations.
 *
 * Every field, method, constant matches the Java original exactly.
 */
class FeatureFlagHandler {
public:
    FeatureFlagHandler() = delete; // Cannot instantiate (final class, private constructor)

    // ========================================================================
    // Constants — Java lines 39-40
    // ========================================================================
    /** @brief Port of: @NonNull public static final String PREF_LAST_VERSION = "lastVersion" */
    static const QString PREF_LAST_VERSION;

    /** @brief Port of: @NonNull public static final String PREF_FIRST_RUN_MESSAGE_SHOWN = "firstRunMessageShown" */
    static const QString PREF_FIRST_RUN_MESSAGE_SHOWN;

    // ========================================================================
    // FeatureFlagStatus enum — Java lines 46-48
    // ========================================================================
    /** @brief Port of: private enum FeatureFlagStatus { ALREADY_UPGRADED, UPGRADE_NEEDED } */
    enum class FeatureFlagStatus {
        ALREADY_UPGRADED,
        UPGRADE_NEEDED
    };

    // ========================================================================
    // FeatureFlag enum — Java lines 50-74
    // ========================================================================
    /** @brief Port of: private enum FeatureFlag */
    enum class FeatureFlag {
        /** @brief Port of: COMMENT_HEADER_SUBREDDIT_FEATURE("commentHeaderSubredditFeature") */
        COMMENT_HEADER_SUBREDDIT_FEATURE,
        /** @brief Port of: CONTROVERSIAL_DATE_SORTS_FEATURE("controversialDateSortsFeature") */
        CONTROVERSIAL_DATE_SORTS_FEATURE,
        /** @brief Port of: HIDE_STATUS_BAR_FOR_MEDIA_FEATURE("hideStatusBarForMediaFeature") */
        HIDE_STATUS_BAR_FOR_MEDIA_FEATURE,
        /** @brief Port of: REPLY_IN_POST_ACTION_MENU_FEATURE("replyInPostActionMenuFeature") */
        REPLY_IN_POST_ACTION_MENU_FEATURE,
        /** @brief Port of: MAIN_MENU_FIND_SUBREDDIT_FEATURE("mainMenuFindSubreddit") */
        MAIN_MENU_FIND_SUBREDDIT_FEATURE,
        /** @brief Port of: OPEN_COMMENT_EXTERNALLY_FEATURE("openCommentExternallyFeature") */
        OPEN_COMMENT_EXTERNALLY_FEATURE,
        /** @brief Port of: POST_TITLE_TAP_ACTION_FEATURE("postTitleTapActionFeature") */
        POST_TITLE_TAP_ACTION_FEATURE,
        /** @brief Port of: DEFAULT_PREF_VIDEO_PLAYBACK_CONTROLS("defaultPrefVideoPlaybackControls") */
        DEFAULT_PREF_VIDEO_PLAYBACK_CONTROLS,
        /** @brief Port of: DEFAULT_PREF_CUSTOM_TABS("defaultPrefCustomTabs") */
        DEFAULT_PREF_CUSTOM_TABS,
        /** @brief Port of: CROSSPOST_ORIGIN_MENU_ITEM("crosspostOriginMenuItem") */
        CROSSPOST_ORIGIN_MENU_ITEM,
        /** @brief Port of: MAIN_MENU_RANDOM_REMOVED("mainMenuRandomRemoved") */
        MAIN_MENU_RANDOM_REMOVED
    };

    // ========================================================================
    // Public static methods
    // ========================================================================

    /**
     * @brief Port of: public static void handleUpgrade(@NonNull final Context context)
     * Java lines 105-353
     *
     * Applies all feature-flag-based upgrade migrations. Iterates through
     * all feature flags and applies preference changes for those that
     * return UPGRADE_NEEDED.
     *
     * @param contextCallback Callback to get string resources by ID
     * @param getStringSetCallback Callback to get a string set preference
     * @param getStringCallback Callback to get a string preference
     * @param getBooleanCallback Callback to get a boolean preference
     * @param sharedPrefs The shared preferences wrapper
     */
    static void handleUpgrade(
        std::function<QString(int stringResId)> contextCallback,
        std::function<QSet<QString>(int stringResId, int defaultArrayResId)> getStringSetCallback,
        std::function<QString(int stringResId, const QString &defaultVal)> getStringCallback,
        std::function<bool(int stringResId, bool defaultVal)> getBooleanCallback,
        SharedPrefsWrapper &sharedPrefs);

    /**
     * @brief Port of: public static void handleFirstInstall(@NonNull final SharedPrefsWrapper sharedPrefs)
     * Java lines 378-385
     *
     * Sets all feature flags when first installing the app, so no
     * upgrade migrations run on the first launch.
     *
     * @param sharedPrefs The shared preferences wrapper
     */
    static void handleFirstInstall(SharedPrefsWrapper &sharedPrefs);

    /**
     * @brief Port of: public static void handleLegacyUpgrade(...)
     * Java lines 388-685
     *
     * Handles legacy (pre-feature-flag) upgrades based on the last version
     * code stored in preferences. Applies migrations for version boundaries
     * (v63, v51, v76, v77, v84, v87, v89, v92).
     *
     * Android-specific UI (MaterialAlertDialogBuilder, ChangelogDialog, etc.)
     * are provided as callbacks.
     *
     * @param appVersion The current app version code
     * @param versionName The current app version name string
     * @param sharedPrefs The shared preferences wrapper
     * @param contextCallback For getting string resources
     * @param getStringSetCallback For getting string set preferences
     * @param getStringCallback For getting string preferences
     * @param showUpgradeDialogCallback For showing the v63 login dialog
     * @param showChangelogCallback For showing the changelog dialog
     * @param showToastCallback For showing upgrade toast
     * @param startNewThreadCallback For background cache clearing
     * @param getBooleanCallback For getting boolean preferences (v87 migration)
     */
    static void handleLegacyUpgrade(
        int appVersion,
        const QString &versionName,
        SharedPrefsWrapper &sharedPrefs,
        std::function<QString(int stringResId)> contextCallback,
        std::function<QSet<QString>(int stringResId, int defaultArrayResId)> getStringSetCallback,
        std::function<QString(int stringResId, const QString &defaultVal)> getStringCallback,
        std::function<void(const QString &title, const QString &message,
                           std::function<void()> onPositive)> showUpgradeDialogCallback,
        std::function<void()> showChangelogCallback,
        std::function<void(const QString &text)> showToastCallback,
        std::function<void(const QString &name, std::function<void()> runnable)> startNewThreadCallback,
        std::function<bool(int stringResId, bool defaultVal)> getBooleanCallback);

    // ========================================================================
    // Public helper: get feature flag ID
    // ========================================================================
    /**
     * @brief Port of: FeatureFlag.getId() — returns "rr_feature_flag_" + id
     * Java line 71-73
     */
    static QString getFeatureFlagId(FeatureFlag flag);

    // ========================================================================
    // Public helper: get feature flag id string (the Java enum's @NonNull id field)
    // ========================================================================
    /**
     * @brief Port of: FeatureFlag.id (the constructor parameter)
     * Returns the short ID string used in the getId() prefix.
     */
    static QString getFeatureFlagShortId(FeatureFlag flag);

private:
    // ========================================================================
    // Private constants
    // ========================================================================
    /** @brief Port of: private static final String TAG = "FeatureFlagHandler" (Java line 42) */
    static const QString TAG;

    // ========================================================================
    // Private static methods
    // ========================================================================

    /**
     * @brief Port of: private static boolean getBoolean(...) (Java lines 76-83)
     */
    static bool getBoolean(int id, bool defaultBoolean,
                           std::function<QString(int)> contextCallback,
                           SharedPrefsWrapper &sharedPreferences);

    /**
     * @brief Port of: private static String getString(...) (Java lines 85-92)
     */
    static QString getString(int id, const QString &defaultString,
                             std::function<QString(int)> contextCallback,
                             SharedPrefsWrapper &sharedPreferences);

    /**
     * @brief Port of: private static Set<String> getStringSet(...) (Java lines 94-103)
     */
    static QSet<QString> getStringSet(int id, int defaultArrayRes,
                                      std::function<QString(int)> contextCallback,
                                      std::function<QSet<QString>(int, int)> getStringSetCallback,
                                      SharedPrefsWrapper &sharedPreferences);

    /**
     * @brief Port of: private static void setFeatureFlag(...) (Java lines 355-360)
     */
    static void setFeatureFlag(SharedPrefsWrapper &sharedPreferences, FeatureFlag featureFlag);

    /**
     * @brief Port of: private static FeatureFlagStatus getAndSetFeatureFlag(...) (Java lines 362-376)
     */
    static FeatureFlagStatus getAndSetFeatureFlag(SharedPrefsWrapper &sharedPreferences,
                                                   FeatureFlag featureFlag);
};

} // namespace PinkReader
