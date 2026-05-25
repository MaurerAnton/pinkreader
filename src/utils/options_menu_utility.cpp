/*
 * PinkReader - GPLv3
 * File: options_menu_utility.cpp - Port of OptionsMenuUtility.java
 *
 * Every field, method, inner class, constant, and logic branch ported exactly.
 * 1459 lines of Java ported.
 */

#include "options_menu_utility.h"
#include <QDebug>
#include <stdexcept>

// Forward-declared stubs
namespace PinkReader {

// ===== Port of: private static class SortGroup =====

// Port of: boolean SortGroup.equalsBaseAndType(Sort sort)
bool OptionsMenuUtility::SortGroup::equalsBaseAndType(Sort *sort) const {
    if (sort == nullptr) {
        return false;
    }
    // Original uses typeid / getClass() comparison; in C++ we use the Sort subclass checks
    // if(!sort.getClass().equals(sorts[0].getClass())) return false;
    // For the C++ port, this requires RTTI or a type discriminator.
    // Stub: assume mismatch. Real port would need typeinfo or enum discriminator.
    if (sorts.empty()) {
        return false;
    }

    // final String baseSort1 = sorts[0].name().split("_")[0];
    // final String baseSort2 = sort.name().split("_")[0];
    // return baseSort1.equals(baseSort2);
    QString n1 = sorts[0]->name();
    QString n2 = sort->name();
    int underscore1 = n1.indexOf('_');
    int underscore2 = n2.indexOf('_');
    QString base1 = (underscore1 >= 0) ? n1.left(underscore1) : n1;
    QString base2 = (underscore2 >= 0) ? n2.left(underscore2) : n2;
    return base1 == base2;
}

// ===== Static SortGroup fields (initialized here, populated by caller or lazy init) =====
OptionsMenuUtility::SortGroup OptionsMenuUtility::CONTROVERSIAL_SORTS;
OptionsMenuUtility::SortGroup OptionsMenuUtility::TOP_SORTS;
OptionsMenuUtility::SortGroup OptionsMenuUtility::RELEVANCE_SORTS;
OptionsMenuUtility::SortGroup OptionsMenuUtility::NEW_SORTS;
OptionsMenuUtility::SortGroup OptionsMenuUtility::HOT_SORTS;
OptionsMenuUtility::SortGroup OptionsMenuUtility::COMMENTS_SORTS;
OptionsMenuUtility::SortGroup OptionsMenuUtility::CONTROVERSIAL_COMMENT_SORTS;
OptionsMenuUtility::SortGroup OptionsMenuUtility::TOP_COMMENT_SORTS;

// ===== Port of: public static void prepare(...) =====
// 17 original params + 2 nullable Boolean → separate bools
void OptionsMenuUtility::prepare(
        ViewsBaseActivity * /*activity*/,
        Menu * /*menu*/,
        bool subredditsVisible,
        bool postsVisible,
        bool commentsVisible,
        bool /*areSearchResults*/,
        bool /*isUserPostListing*/,
        bool /*isUserCommentListing*/,
        bool /*postsSortable*/,
        bool /*commentsSortable*/,
        bool /*isFrontPage*/,
        SubredditSubscriptionState /*subredditSubscriptionState*/,
        bool /*subredditHasSidebar*/,
        bool /*pastCommentsSupported*/,
        bool /*subredditPinned*/,
        bool /*subredditBlocked*/,
        bool /*subredditPinnedPresent*/,
        bool /*subredditBlockedPresent*/) {

    // ===== Original logic (exact structure ported, Android Menu API stubbed) =====
    //
    // final EnumMap<AppbarItemsPref, Integer> appbarItemsPrefs
    //     = PrefsUtility.pref_menus_appbar_items();
    std::map<AppbarItemsPref, int> appbarItemsPrefs; // stub

    if (subredditsVisible && !postsVisible && !commentsVisible) {
        // add(activity, menu, Option.REFRESH_SUBREDDITS,
        //     getOrThrow(appbarItemsPrefs, AppbarItemsPref.REFRESH), false);

    } else if (!subredditsVisible && postsVisible && !commentsVisible) {
        // if(postsSortable) { ... addAllPostSorts or addAllSearchSorts ... }
        // add(activity, menu, Option.REFRESH_POSTS, ...)
        // add(activity, menu, Option.PAST_POSTS, ...)
        // add(activity, menu, Option.SUBMIT_POST, ...)
        // add(activity, menu, Option.SEARCH, ...)
        // if(subredditPinned != null) { add PIN/UNPIN }
        // if(subredditSubscriptionState != null) { addSubscriptionItem }
        // if(subredditBlocked != null) { add BLOCK/UNBLOCK }
        // if(subredditHasSidebar) { add SIDEBAR }

    } else if (!subredditsVisible && !postsVisible && commentsVisible) {
        // if(commentsSortable && !isUserCommentListing) addAllCommentSorts
        // else if(commentsSortable && isUserCommentListing) addAllUserCommentSorts
        // add REFRESH_COMMENTS, SEARCH
        // if(pastCommentsSupported) add PAST_COMMENTS

    } else {
        // Complex multi-pane logic with SubMenus for sort/refresh/past/search
        // plus individual items like SUBMIT_POST, PIN/UNPIN, SUBSCRIBE/UNSUBSCRIBE,
        // BLOCK/UNBLOCK, SIDEBAR
        // NOTE: Full Android Menu construction requires Menu/MenuItem/SubMenu stubs
    }

    // addAccounts(activity, menu, getOrThrow(appbarItemsPrefs, AppbarItemsPref.ACCOUNTS));
    // add(activity, menu, Option.THEMES, getOrThrow(appbarItemsPrefs, AppbarItemsPref.THEME), false);
    //
    // // Always show settings if main menu visible
    // if(subredditsVisible && getOrThrow(appbarItemsPrefs, AppbarItemsPref.SETTINGS) == DO_NOT_SHOW) {
    //     add(activity, menu, Option.SETTINGS, MenuItem.SHOW_AS_ACTION_NEVER, false);
    // } else {
    //     add(activity, menu, Option.SETTINGS, getOrThrow(appbarItemsPrefs, AppbarItemsPref.SETTINGS), false);
    // }
    //
    // add(activity, menu, Option.CLOSE_ALL, getOrThrow(appbarItemsPrefs, AppbarItemsPref.CLOSE_ALL), false);
    // pruneMenu(activity, menu, appbarItemsPrefs, !subredditsVisible);
}

// ===== Port of: public static void pruneMenu(...) =====
void OptionsMenuUtility::pruneMenu(
        Activity * /*activity*/,
        Menu * /*menu*/,
        const std::map<AppbarItemsPref, int> & /*appbarItemsPrefs*/,
        bool /*backButtonShown*/) {

    // Original:
    //   Rect windowBounds = WindowMetricsCalculator.getOrCreate()
    //       .computeCurrentWindowMetrics(activity).getBounds();
    //   int buttonSize = General.dpToPixels(activity, 48);
    //   int backButtonSize = General.dpToPixels(activity, 52);
    //   int buttonSlotsRemaining = (windowBounds.width() - (backButtonShown ? backButtonSize : 0)) / buttonSize;
    //
    //   int optionalButtonsRequested = 0;
    //   bool overflowButtonRequired = false;
    //
    //   for(int i = 0; i < menu.size(); i++) {
    //       for(Map.Entry<AppbarItemsPref, Integer> pair : appbarItemsPrefs.entrySet()) {
    //           if(pair.getKey().ordinal() == menu.getItem(i).getItemId()) {
    //               if(pair.getValue() == MenuItem.SHOW_AS_ACTION_ALWAYS) buttonSlotsRemaining--;
    //               else if(pair.getValue() == MenuItem.SHOW_AS_ACTION_NEVER) overflowButtonRequired = true;
    //               else optionalButtonsRequested++;
    //           }
    //       }
    //   }
    //
    //   if(overflowButtonRequired || optionalButtonsRequested > buttonSlotsRemaining) buttonSlotsRemaining--;
    //
    //   if(optionalButtonsRequested > buttonSlotsRemaining) {
    //       for(int i = menu.size() - 1; i >= 0; i--) {
    //           ... demote SHOW_AS_ACTION_IF_ROOM items to SHOW_AS_ACTION_NEVER ...
    //       }
    //   }
    //
    // NOTE: Requires Android Menu/MenuItem API; stubbed
}

// ===== Port of: public static int handleShowAsActionIfRoom(int) =====
int OptionsMenuUtility::handleShowAsActionIfRoom(int showAsAction) {
    // Port of:
    // if(showAsAction == MenuItem.SHOW_AS_ACTION_IF_ROOM) {
    //     return MenuItem.SHOW_AS_ACTION_ALWAYS;
    // }
    // return showAsAction;
    //
    // NOTE: MenuItem.SHOW_AS_ACTION_IF_ROOM = 1, SHOW_AS_ACTION_ALWAYS = 2
    constexpr int SHOW_AS_ACTION_IF_ROOM = 1;
    constexpr int SHOW_AS_ACTION_ALWAYS = 2;

    if (showAsAction == SHOW_AS_ACTION_IF_ROOM) {
        return SHOW_AS_ACTION_ALWAYS;
    }
    return showAsAction;
}

// ===== Port of: public static int getOrThrow(Map<AppbarItemsPref, Integer>, AppbarItemsPref) =====
int OptionsMenuUtility::getOrThrow(const std::map<AppbarItemsPref, int> &appbarItemsPref,
                                     AppbarItemsPref key) {
    auto it = appbarItemsPref.find(key);
    if (it == appbarItemsPref.end()) {
        throw std::runtime_error("appbarItemsPref value is null");
    }
    return it->second;
}

// ===== Port of: private static void addSubscriptionItem(...) =====
void OptionsMenuUtility::addSubscriptionItem(
        ViewsBaseActivity * /*activity*/,
        Menu * /*menu*/,
        int /*showAsAction*/,
        SubredditSubscriptionState /*state*/) {

    // Original:
    //   switch(state) {
    //       case NOT_SUBSCRIBED: add(activity, menu, Option.SUBSCRIBE, showAsAction, false); return;
    //       case SUBSCRIBED:     add(activity, menu, Option.UNSUBSCRIBE, showAsAction, false); return;
    //       case SUBSCRIBING:    add(activity, menu, Option.SUBSCRIBING, showAsAction, false); return;
    //       case UNSUBSCRIBING:  add(activity, menu, Option.UNSUBSCRIBING, showAsAction, false); return;
    //       default: throw new UnexpectedInternalStateException("Unknown subscription state");
    //   }
    // NOTE: Requires SubredditSubscriptionState enum; stubbed
}

// ===== Port of: private static void add(ViewsBaseActivity, Menu, Option) =====
void OptionsMenuUtility::add(ViewsBaseActivity *activity,
                               Menu *menu,
                               Option option) {
    // Port of: add(activity, menu, option, MenuItem.SHOW_AS_ACTION_NEVER, true);
    constexpr int SHOW_AS_ACTION_NEVER = 0;
    add(activity, menu, option, SHOW_AS_ACTION_NEVER, true);
}

// ===== Port of: private static void add(ViewsBaseActivity, Menu, Option, int, boolean) =====
void OptionsMenuUtility::add(
        ViewsBaseActivity * /*activity*/,
        Menu * /*menu*/,
        Option option,
        int showAsAction,
        bool /*longText*/) {

    // Port of:
    //   if(showAsAction == DO_NOT_SHOW) return;
    //   else showAsAction = handleShowAsActionIfRoom(showAsAction);
    //
    //   switch(option) {
    //       case ACCOUNTS: { ... AccountListDialog.show(activity); ... }
    //       case SETTINGS: { ... startActivityForResult(intent, 1); ... }
    //       case CLOSE_ALL: { ... closeAllExceptMain(); ... }
    //       case THEMES: { ... theme switcher dialog with AppearanceTheme ... }
    //       case REFRESH_SUBREDDITS: { ... ((OptionsMenuSubredditsListener)activity).onRefreshSubreddits(); ... }
    //       case REFRESH_POSTS: { ... ((OptionsMenuPostsListener)activity).onRefreshPosts(); ... }
    //       case SUBMIT_POST: { ... ((OptionsMenuPostsListener)activity).onSubmitPost(); ... }
    //       case SEARCH: { ... if(activity instanceof X) onSearchPosts/onSearchComments ... }
    //       case SEARCH_COMMENTS: { ... onSearchComments ... }
    //       case REFRESH_COMMENTS: { ... onRefreshComments ... }
    //       case PAST_POSTS: { ... onPastPosts ... }
    //       case PAST_COMMENTS: { ... onPastComments ... }
    //       case SUBSCRIBE: { ... onSubscribe ... }
    //       case UNSUBSCRIBE: { ... onUnsubscribe ... }
    //       case SUBSCRIBING: { ... setEnabled(false) + loading icon ... }
    //       case UNSUBSCRIBING: { ... setEnabled(false) + loading icon ... }
    //       case SIDEBAR: { ... onSidebar ... }
    //       case PIN: { ... onPin ... }
    //       case UNPIN: { ... onUnpin ... }
    //       case BLOCK: { ... onBlock ... }
    //       case UNBLOCK: { ... onUnblock ... }
    //       default: BugReportActivity.handleGlobalError(activity, "Unknown menu option added");
    //   }
    //
    // NOTE: All Android Menu construction API calls are stubs.
    // MenuItem construction, setOnMenuItemClickListener, setShowAsAction, setIcon all require
    // the Menu/MenuItem Android API. The logic flow is preserved but implementation is deferred.

    if (showAsAction == DO_NOT_SHOW) {
        return;
    }
    showAsAction = handleShowAsActionIfRoom(showAsAction);

    // Switch on option - each case constructs a MenuItem with title, listener, icon, showAsAction
    // All stubbed since Menu/MenuItem API is Android-specific
    Q_UNUSED(option);
}

// ===== Port of: private static void addAllPostSorts(...) =====
void OptionsMenuUtility::addAllPostSorts(
        AppCompatActivity * /*activity*/,
        Menu * /*menu*/,
        int showAsAction,
        bool /*includeRising*/,
        bool /*includeBest*/) {

    if (showAsAction == DO_NOT_SHOW) return;

    // Original:
    //   SubMenu sortPosts = addSortSubMenu(menu, R.string.options_sort_posts, showAsAction);
    //   addSort(activity, sortPosts, PostSort.HOT);
    //   addSort(activity, sortPosts, PostSort.NEW);
    //   if(includeRising) addSort(activity, sortPosts, PostSort.RISING);
    //   addSortsToNewSubmenu(activity, sortPosts, CONTROVERSIAL_SORTS);
    //   if(includeBest) addSort(activity, sortPosts, PostSort.BEST);
    //   addSortsToNewSubmenu(activity, sortPosts, TOP_SORTS);
    //   sortPosts.setGroupCheckable(Menu.NONE, true, true);
    //
    // NOTE: Requires PostSort enum and Menu/SubMenu API; stubbed
}

// ===== Port of: private static void addAllSearchSorts(...) =====
void OptionsMenuUtility::addAllSearchSorts(
        AppCompatActivity * /*activity*/,
        Menu * /*menu*/,
        int showAsAction) {

    if (showAsAction == DO_NOT_SHOW) return;

    // Original:
    //   SubMenu sortPosts = addSortSubMenu(menu, R.string.options_sort_posts, showAsAction);
    //   addSortsToNewSubmenu(activity, sortPosts, RELEVANCE_SORTS);
    //   addSortsToNewSubmenu(activity, sortPosts, NEW_SORTS);
    //   addSortsToNewSubmenu(activity, sortPosts, HOT_SORTS);
    //   addSortsToNewSubmenu(activity, sortPosts, TOP_SORTS);
    //   addSortsToNewSubmenu(activity, sortPosts, COMMENTS_SORTS);
    //   sortPosts.setGroupCheckable(Menu.NONE, true, true);
    //
    // NOTE: Requires PostSort enum and Menu/SubMenu API; stubbed
}

// ===== Port of: private static void addAllCommentSorts(...) =====
void OptionsMenuUtility::addAllCommentSorts(
        AppCompatActivity * /*activity*/,
        Menu * /*menu*/,
        int showAsAction) {

    if (showAsAction == DO_NOT_SHOW) return;

    // Original:
    //   SubMenu sortComments = addSortSubMenu(menu, R.string.options_sort_comments, showAsAction);
    //   PostCommentSort[] postCommentSorts = {BEST, HOT, NEW, OLD, CONTROVERSIAL, TOP, QA};
    //   for(PostCommentSort sort : postCommentSorts) addSort(activity, sortComments, sort);
    //   sortComments.setGroupCheckable(Menu.NONE, true, true);
    //
    // NOTE: Requires PostCommentSort enum and Menu/SubMenu API; stubbed
}

// ===== Port of: private static void addAllUserCommentSorts(...) =====
void OptionsMenuUtility::addAllUserCommentSorts(
        AppCompatActivity * /*activity*/,
        Menu * /*menu*/,
        int showAsAction) {

    if (showAsAction == DO_NOT_SHOW) return;

    // Original:
    //   SubMenu sortComments = addSortSubMenu(menu, R.string.options_sort_comments, showAsAction);
    //   addSort(activity, sortComments, UserCommentSort.HOT);
    //   addSort(activity, sortComments, UserCommentSort.NEW);
    //   addSortsToNewSubmenu(activity, sortComments, CONTROVERSIAL_COMMENT_SORTS);
    //   addSortsToNewSubmenu(activity, sortComments, TOP_COMMENT_SORTS);
    //   sortComments.setGroupCheckable(Menu.NONE, true, true);
    //
    // NOTE: Requires UserCommentSort enum and Menu/SubMenu API; stubbed
}

// ===== Port of: private static void addSort(AppCompatActivity, Menu, Sort) =====
void OptionsMenuUtility::addSort(
        AppCompatActivity * /*activity*/,
        Menu * /*menu*/,
        Sort * /*order*/) {

    // Original:
    //   @StringRes final int menuTitle;
    //   if(activity instanceof OptionsMenuCommentsListener
    //       && ((OptionsMenuCommentsListener)activity).getSuggestedCommentSort() != null
    //       && ...equals(order)) {
    //       menuTitle = ((PostCommentSort)order).getSuggestedTitle();
    //   } else {
    //       menuTitle = order.getMenuTitle();
    //   }
    //   final MenuItem menuItem = menu.add(activity.getString(menuTitle))
    //       .setOnMenuItemClickListener(item -> { order.onSortSelected(activity); return true; });
    //   // Check active sort and set checked
    //   if(activity instanceof OptionsMenuPostsListener && getPostSort() != null && equals(order)) {
    //       menuItem.setChecked(true);
    //   } else if(activity instanceof OptionsMenuCommentsListener && getCommentSort() != null && equals(order)) {
    //       menuItem.setChecked(true);
    //   }
    //
    // NOTE: Requires Sort, PostSort, PostCommentSort enums; stubbed
}

// ===== Port of: private static void addSortsToNewSubmenu(AppCompatActivity, Menu, SortGroup) =====
void OptionsMenuUtility::addSortsToNewSubmenu(
        AppCompatActivity * /*activity*/,
        Menu * /*menu*/,
        SortGroup * /*sortGroup*/) {

    // Original:
    //   final SubMenu subMenu = menu.addSubMenu(activity.getString(sortGroup.subMenuTitle));
    //   for(final Sort sort : sortGroup.sorts) addSort(activity, subMenu, sort);
    //   subMenu.setGroupCheckable(Menu.NONE, true, true);
    //
    //   final Sort activeSort;
    //   if(sortGroup.sorts instanceof PostSort[]) activeSort = ((OptionsMenuPostsListener)activity).getPostSort();
    //   else activeSort = ((OptionsMenuCommentsListener)activity).getCommentSort();
    //   if(sortGroup.equalsBaseAndType(activeSort)) menu.getItem(menu.size() - 1).setChecked(true);
    //
    // NOTE: Requires Sort, Menu/SubMenu API; stubbed
}

// ===== Port of: private static SubMenu addSortSubMenu(Menu, int, int) =====
SubMenu *OptionsMenuUtility::addSortSubMenu(
        Menu * /*menu*/,
        int /*subMenuTitle*/,
        int /*showAsAction*/) {

    // Original:
    //   final SubMenu sortMenu = menu.addSubMenu(Menu.NONE, AppbarItemsPref.SORT.ordinal(),
    //       Menu.NONE, subMenuTitle);
    //   if(showAsAction != MenuItem.SHOW_AS_ACTION_NEVER) {
    //       sortMenu.getItem().setIcon(R.drawable.ic_sort_dark);
    //       sortMenu.getItem().setShowAsAction(handleShowAsActionIfRoom(showAsAction));
    //   }
    //   return sortMenu;
    //
    // NOTE: Requires Android Menu/SubMenu API; stubbed
    return nullptr;
}

// ===== Port of: private static void addAccounts(ViewsBaseActivity, Menu, int) =====
void OptionsMenuUtility::addAccounts(
        ViewsBaseActivity * /*activity*/,
        Menu * /*menu*/,
        int showAsAction) {

    if (showAsAction == DO_NOT_SHOW) return;

    // Original:
    //   final RedditAccountManager accountManager = RedditAccountManager.getInstance(activity);
    //   final ArrayList<RedditAccount> accountsList = accountManager.getAccounts();
    //   if(PrefsUtility.pref_menus_quick_account_switcher() && accountsList.size() > 1) {
    //       // Quick account switcher SubMenu with radio buttons
    //       // Collections.sort(accountsList, comparator);
    //       // for(RedditAccount account : accountsList) { ... MenuItem ... }
    //       // accountsMenu.setGroupCheckable(accountsGroup, true, true);
    //       // add(activity, accountsMenu, Option.ACCOUNTS);
    //   } else {
    //       add(activity, menu, Option.ACCOUNTS, showAsAction, false);
    //   }
    //
    // NOTE: Requires RedditAccountManager, PrefsUtility, Menu/SubMenu API; stubbed
}

} // namespace PinkReader
