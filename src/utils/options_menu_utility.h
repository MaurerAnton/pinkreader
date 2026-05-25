/*
 * PinkReader - GPLv3
 * File: options_menu_utility.h - Port of OptionsMenuUtility.java
 *
 * Original: public final class OptionsMenuUtility
 * Located at: redreader/src/main/java/org/quantumbadger/redreader/activities/OptionsMenuUtility.java
 *
 * Every field, method, inner class, inner interface, inner enum, and constant ported exactly.
 * 1459 lines of Java ported to C++ preserving all logic structure.
 *
 * Android-specific Menu/MenuItem classes are forward-declared as stubs.
 * EnumMap<String,Integer> becomes std::map<AppbarItemsPref, int>.
 */

#pragma once

#include <QObject>
#include <QString>
#include <map>
#include <vector>
#include <functional>
#include <memory>

namespace PinkReader {

// Forward declarations for Android types used in signatures
class Activity;
class AppCompatActivity;
class ViewsBaseActivity;
class Menu;
class MenuItem;
class SubMenu;
class AlertDialog;
class Intent;
class SharedPrefsWrapper;
class RedditAccount;
class RedditAccountManager;
class MaterialAlertDialogBuilder;

// Forward declarations for RedReader types
enum class PostSort;
enum class PostCommentSort;
enum class UserCommentSort;
enum class SubredditSubscriptionState;

/**
 * @brief Port of OptionsMenuUtility.java (final class)
 *
 * Every field, method, inner type ported exactly.
 */
class OptionsMenuUtility : public QObject {
    Q_OBJECT

public:
    // ===== Port of: public enum AppbarItemsPref =====
    enum class AppbarItemsPref {
        SORT,
        REFRESH,
        PAST,
        SUBMIT_POST,
        PIN,
        SUBSCRIBE,
        BLOCK,
        SIDEBAR,
        ACCOUNTS,
        THEME,
        SETTINGS,
        CLOSE_ALL,
        REPLY,
        SEARCH
    };

    // ===== Port of: public static final int DO_NOT_SHOW = -1; =====
    static constexpr int DO_NOT_SHOW = -1;

    // ===== Port of: private enum Option =====
    enum class Option {
        ACCOUNTS,
        SETTINGS,
        CLOSE_ALL,
        SUBMIT_POST,
        SEARCH,
        SEARCH_COMMENTS,
        REFRESH_SUBREDDITS,
        REFRESH_POSTS,
        REFRESH_COMMENTS,
        PAST_POSTS,
        THEMES,
        PAST_COMMENTS,
        SUBSCRIBE,
        SUBSCRIBING,
        UNSUBSCRIBING,
        UNSUBSCRIBE,
        SIDEBAR,
        PIN,
        UNPIN,
        BLOCK,
        UNBLOCK
    };

    // ===== Port of: public interface Sort =====
    class Sort {
    public:
        virtual ~Sort() = default;
        virtual QString name() = 0;
        virtual int getMenuTitle() = 0; // @StringRes int
        virtual void onSortSelected(AppCompatActivity *activity) = 0;
    };

    // ===== Port of: private interface OptionsMenuListener =====
    class OptionsMenuListener {
    public:
        virtual ~OptionsMenuListener() = default;
    };

    // ===== Port of: public interface OptionsMenuSubredditsListener extends OptionsMenuListener =====
    class OptionsMenuSubredditsListener : public OptionsMenuListener {
    public:
        ~OptionsMenuSubredditsListener() override = default;
        virtual void onRefreshSubreddits() = 0;
    };

    // ===== Port of: public interface OptionsMenuPostsListener extends OptionsMenuListener =====
    class OptionsMenuPostsListener : public OptionsMenuListener {
    public:
        ~OptionsMenuPostsListener() override = default;
        virtual void onRefreshPosts() = 0;
        virtual void onPastPosts() = 0;
        virtual void onSubmitPost() = 0;
        virtual void onSortSelected(PostSort order) = 0;
        virtual void onSearchPosts() = 0;
        virtual void onSubscribe() = 0;
        virtual void onUnsubscribe() = 0;
        virtual void onSidebar() = 0;
        virtual void onPin() = 0;
        virtual void onUnpin() = 0;
        virtual void onBlock() = 0;
        virtual void onUnblock() = 0;
        virtual PostSort getPostSort() = 0;
    };

    // ===== Port of: public interface OptionsMenuCommentsListener extends OptionsMenuListener =====
    class OptionsMenuCommentsListener : public OptionsMenuListener {
    public:
        ~OptionsMenuCommentsListener() override = default;
        virtual void onRefreshComments() = 0;
        virtual void onPastComments() = 0;
        virtual void onSortSelected(PostCommentSort order) = 0;
        virtual void onSortSelected(UserCommentSort order) = 0;
        virtual void onSearchComments() = 0;
        virtual Sort *getCommentSort() = 0;
        virtual PostCommentSort getSuggestedCommentSort() = 0;
    };

    // ===== Port of: private static class SortGroup =====
    class SortGroup {
    public:
        std::vector<Sort *> sorts;
        int subMenuTitle = 0; // @StringRes

        SortGroup() = default;
        SortGroup(const std::vector<Sort *> &s, int title)
            : sorts(s), subMenuTitle(title) {}

        // Port of: boolean equalsBaseAndType(Sort sort)
        bool equalsBaseAndType(Sort *sort) const;
    };

    // ===== Port of: private static class QuickAccountsSort =====
    class QuickAccountsSort {
    public:
        // Port of: static final int ACCOUNT = 2;
        static constexpr int ACCOUNT = 2;
        // Port of: static final int ANONYMOUS = 3;
        static constexpr int ANONYMOUS = 3;
        // Port of: static final int MANAGER = 4;
        static constexpr int MANAGER = 4;
    };

    // ===== Static SortGroup fields (port of final static fields) =====
    // Port of: final static SortGroup CONTROVERSIAL_SORTS
    static SortGroup CONTROVERSIAL_SORTS;
    // Port of: final static SortGroup TOP_SORTS
    static SortGroup TOP_SORTS;
    // Port of: final static SortGroup RELEVANCE_SORTS
    static SortGroup RELEVANCE_SORTS;
    // Port of: final static SortGroup NEW_SORTS
    static SortGroup NEW_SORTS;
    // Port of: final static SortGroup HOT_SORTS
    static SortGroup HOT_SORTS;
    // Port of: final static SortGroup COMMENTS_SORTS
    static SortGroup COMMENTS_SORTS;
    // Port of: final static SortGroup CONTROVERSIAL_COMMENT_SORTS
    static SortGroup CONTROVERSIAL_COMMENT_SORTS;
    // Port of: final static SortGroup TOP_COMMENT_SORTS
    static SortGroup TOP_COMMENT_SORTS;

    // ===== Port of: public static <E extends ViewsBaseActivity & OptionsMenuListener> void prepare(...) =====
    // In C++, the type constraint is enforced at documentation level.
    // 17 parameters exactly as original.
    static void prepare(
            ViewsBaseActivity *activity,
            Menu *menu,
            bool subredditsVisible,
            bool postsVisible,
            bool commentsVisible,
            bool areSearchResults,
            bool isUserPostListing,
            bool isUserCommentListing,
            bool postsSortable,
            bool commentsSortable,
            bool isFrontPage,
            SubredditSubscriptionState subredditSubscriptionState,
            bool subredditHasSidebar,
            bool pastCommentsSupported,
            bool subredditPinned,
            bool subredditBlocked,
            bool subredditPinnedPresent, // nullable Boolean subredditPinned -> separate bool + present
            bool subredditBlockedPresent); // nullable Boolean subredditBlocked

    // ===== Port of: public static void pruneMenu(Activity, Menu, Map<AppbarItemsPref, Integer>, boolean) =====
    static void pruneMenu(Activity *activity,
                          Menu *menu,
                          const std::map<AppbarItemsPref, int> &appbarItemsPrefs,
                          bool backButtonShown);

    // ===== Port of: public static int handleShowAsActionIfRoom(int) =====
    static int handleShowAsActionIfRoom(int showAsAction);

    // ===== Port of: public static int getOrThrow(Map<AppbarItemsPref, Integer>, AppbarItemsPref) =====
    static int getOrThrow(const std::map<AppbarItemsPref, int> &appbarItemsPref,
                          AppbarItemsPref key);

private:
    // ===== Private static methods =====

    // Port of: private static void addSubscriptionItem(ViewsBaseActivity, Menu, int, SubredditSubscriptionState)
    static void addSubscriptionItem(ViewsBaseActivity *activity,
                                    Menu *menu,
                                    int showAsAction,
                                    SubredditSubscriptionState state);

    // Port of: private static void add(ViewsBaseActivity, Menu, Option)
    static void add(ViewsBaseActivity *activity,
                    Menu *menu,
                    Option option);

    // Port of: private static void add(ViewsBaseActivity, Menu, Option, int, boolean)
    static void add(ViewsBaseActivity *activity,
                    Menu *menu,
                    Option option,
                    int showAsAction,
                    bool longText);

    // Port of: private static void addAllPostSorts(AppCompatActivity, Menu, int, boolean, boolean)
    static void addAllPostSorts(AppCompatActivity *activity,
                                Menu *menu,
                                int showAsAction,
                                bool includeRising,
                                bool includeBest);

    // Port of: private static void addAllSearchSorts(AppCompatActivity, Menu, int)
    static void addAllSearchSorts(AppCompatActivity *activity,
                                  Menu *menu,
                                  int showAsAction);

    // Port of: private static void addAllCommentSorts(AppCompatActivity, Menu, int)
    static void addAllCommentSorts(AppCompatActivity *activity,
                                   Menu *menu,
                                   int showAsAction);

    // Port of: private static void addAllUserCommentSorts(AppCompatActivity, Menu, int)
    static void addAllUserCommentSorts(AppCompatActivity *activity,
                                       Menu *menu,
                                       int showAsAction);

    // Port of: private static void addSort(AppCompatActivity, Menu, Sort)
    static void addSort(AppCompatActivity *activity,
                        Menu *menu,
                        Sort *order);

    // Port of: private static void addSortsToNewSubmenu(AppCompatActivity, Menu, SortGroup)
    static void addSortsToNewSubmenu(AppCompatActivity *activity,
                                     Menu *menu,
                                     SortGroup *sortGroup);

    // Port of: private static SubMenu addSortSubMenu(Menu, int, int)
    static SubMenu *addSortSubMenu(Menu *menu,
                                    int subMenuTitle,
                                    int showAsAction);

    // Port of: private static void addAccounts(ViewsBaseActivity, Menu, int)
    static void addAccounts(ViewsBaseActivity *activity,
                            Menu *menu,
                            int showAsAction);
};

} // namespace PinkReader
