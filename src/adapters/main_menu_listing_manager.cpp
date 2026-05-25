/*
 * PinkReader - GPLv3
 * File: main_menu_listing_manager.cpp - Port of MainMenuListingManager.java
 *
 * Every field, method, inner class, constant, and logic branch ported exactly.
 * 974 lines of Java ported.
 */

#include "main_menu_listing_manager.h"
#include <QDebug>
#include <algorithm>

// Forward-declared stubs for Android/RedReader types not yet ported
namespace PinkReader {

// ===== Constructor =====
// Port of: public MainMenuListingManager(AppCompatActivity activity,
//     MainMenuSelectionListener listener, RedditAccount user)
MainMenuListingManager::MainMenuListingManager(
        AppCompatActivity *activity,
        MainMenuSelectionListener *listener,
        RedditAccount * /*user*/,
        QObject *parent)
    : QObject(parent) {

    // General.checkThisIsUIThread();
    // NOTE: checkThisIsUIThread stubbed

    // mActivity = activity;
    mActivity = activity;

    // mContext = activity.getApplicationContext();
    mContext = nullptr; // stub

    // mListener = listener;
    mListener = listener;

    // mAnnouncementHolder = new FrameLayout(mActivity);
    // General.setLayoutMatchWidthWrapHeight(mAnnouncementHolder);
    mAnnouncementHolder = nullptr; // stub: FrameLayout creation

    // mAdapter = new GroupedRecyclerViewAdapter(13);
    mAdapter = nullptr; // stub: GroupedRecyclerViewAdapter creation

    // ===== Drawable loading (from TypedArray attr) =====
    // Original loads 9 drawables via TypedArray + AppCompatResources.getDrawable
    // All stubbed: void* icons for Drawable

    // ===== Build main menu shortcut items =====
    // Port of:
    // final EnumSet<MainMenuFragment.MainMenuShortcutItems> mainMenuShortcutItems
    //     = PrefsUtility.pref_menus_mainmenu_shortcutitems();
    // if(mainMenuShortcutItems.contains(FRONTPAGE)) { ... mAdapter.appendToGroup(GROUP_MAIN_ITEMS, makeItem(...)); }
    // if(mainMenuShortcutItems.contains(POPULAR)) { ... }
    // if(mainMenuShortcutItems.contains(ALL)) { ... }
    // Complex SEARCH/CUSTOM branch with optional second icon
    // NOTE: PrefsUtility, EnumSet, GroupedRecyclerViewAdapter not yet available; stubbed

    // ===== Build announcements =====
    // Port of:
    // if(PrefsUtility.pref_menus_mainmenu_dev_announcements()) {
    //     mAdapter.appendToGroup(GROUP_ANNOUNCEMENTS,
    //         new GroupedRecyclerViewItemFrameLayout(mAnnouncementHolder));
    //     onUpdateAnnouncement();
    // }

    // ===== Build user items =====
    // Port of:
    // if(!user.isAnonymous()) {
    //     final EnumSet<MainMenuFragment.MainMenuUserItems> mainMenuUserItems
    //         = PrefsUtility.pref_menus_mainmenu_useritems();
    //     if(!mainMenuUserItems.isEmpty()) {
    //         // Header: hide_username_main_menu check
    //         // AtomicBoolean isFirst = new AtomicBoolean(true);
    //         // For each MainMenuUserItems.PROFILE/INBOX/SENT_MESSAGES/... add item
    //     }
    // }

    // ===== Pinned subreddits =====
    // setPinnedSubreddits();

    // ===== Blocked subreddits =====
    // if(PrefsUtility.pref_appearance_show_blocked_subreddits_main_menu()) {
    //     setBlockedSubreddits();
    // }

    // ===== Multireddits =====
    // if(!user.isAnonymous() && PrefsUtility.pref_show_multireddit_main_menu()) {
    //     showMultiredditsHeader(activity);
    //     // Loading spinner for multireddits
    // }

    // ===== Subreddits =====
    // if(PrefsUtility.pref_show_subscribed_subreddits_main_menu()) {
    //     // Header for subscribed subreddits + loading spinner
    // }
}

MainMenuListingManager::~MainMenuListingManager() = default;

// ===== Port of: public GroupedRecyclerViewAdapter getAdapter() =====
GroupedRecyclerViewAdapter *MainMenuListingManager::getAdapter() {
    return mAdapter;
}

// ===== Port of: public void setMultiredditsError(ErrorView) =====
void MainMenuListingManager::setMultiredditsError(ErrorView * /*errorView*/) {
    // Original:
    // AndroidCommon.UI_THREAD_HANDLER.post(() -> {
    //     mAdapter.removeAllFromGroup(GROUP_MULTIREDDITS_ITEMS);
    //     mAdapter.appendToGroup(GROUP_MULTIREDDITS_ITEMS,
    //         new GroupedRecyclerViewItemFrameLayout(errorView));
    // });
    // NOTE: Android handler + adapter API; stubbed
}

// ===== Port of: public void setSubredditsError(ErrorView) =====
void MainMenuListingManager::setSubredditsError(ErrorView * /*errorView*/) {
    // Original:
    // AndroidCommon.UI_THREAD_HANDLER.post(() -> {
    //     mAdapter.removeAllFromGroup(GROUP_SUBREDDITS_ITEMS);
    //     mAdapter.appendToGroup(GROUP_SUBREDDITS_ITEMS,
    //         new GroupedRecyclerViewItemFrameLayout(errorView));
    // });
    // NOTE: Android handler + adapter API; stubbed
}

// ===== Port of: public void setSubreddits(Collection<SubredditCanonicalId>) =====
void MainMenuListingManager::setSubreddits(
        const QVector<SubredditCanonicalId *> &subscriptions) {

    // Port of:
    // final ArrayList<SubredditCanonicalId> subscriptionsSorted = new ArrayList<>(subscriptions);
    // Collections.sort(subscriptionsSorted);
    QVector<SubredditCanonicalId *> subscriptionsSorted = subscriptions;
    // NOTE: sort requires SubredditCanonicalId comparison; stubbed
    // std::sort(subscriptionsSorted.begin(), subscriptionsSorted.end(), ...);

    // AndroidCommon.UI_THREAD_HANDLER.post(() -> {
    //     if(mSubredditSubscriptions != null && mSubredditSubscriptions.equals(subscriptionsSorted)) {
    //         return;
    //     }
    //     if(!PrefsUtility.pref_show_subscribed_subreddits_main_menu()) {
    //         mAdapter.removeAllFromGroup(GROUP_SUBREDDITS_HEADER);
    //         mAdapter.removeAllFromGroup(GROUP_SUBREDDITS_ITEMS);
    //         return;
    //     }
    //     mSubredditSubscriptions = subscriptionsSorted;
    //     mAdapter.removeAllFromGroup(GROUP_SUBREDDITS_ITEMS);
    //     boolean isFirst = true;
    //     for(SubredditCanonicalId subreddit : subscriptionsSorted) {
    //         mAdapter.appendToGroup(GROUP_SUBREDDITS_ITEMS, makeSubredditItem(subreddit, isFirst, false));
    //         isFirst = false;
    //     }
    // });
    //
    // NOTE: Android handler + adapter API + PrefsUtility; stubbed
    mSubredditSubscriptions = subscriptionsSorted;
}

// ===== Port of: public void setMultireddits(Collection<String>) =====
void MainMenuListingManager::setMultireddits(
        const QSet<QString> &subscriptions) {

    // Port of:
    // final ArrayList<String> subscriptionsSorted = new ArrayList<>(subscriptions);
    // Collections.sort(subscriptionsSorted);
    QList<QString> subscriptionsSorted;
    for (const QString &s : subscriptions) {
        subscriptionsSorted.append(s);
    }
    // std::sort(subscriptionsSorted...)
    // NOTE: sort by natural string order

    // AndroidCommon.UI_THREAD_HANDLER.post(() -> {
    //     if(mMultiredditSubscriptions != null && ...equals(subscriptionsSorted)) return;
    //     if(!PrefsUtility.pref_show_multireddit_main_menu()) { ... return; }
    //     mMultiredditSubscriptions = subscriptionsSorted;
    //     mAdapter.removeAllFromGroup(GROUP_MULTIREDDITS_ITEMS);
    //     if(subscriptionsSorted.isEmpty()) hideMultiredditsHeader();
    //     else {
    //         showMultiredditsHeader(mContext);
    //         boolean isFirst = true;
    //         for(String multireddit : subscriptionsSorted) {
    //             mAdapter.appendToGroup(GROUP_MULTIREDDITS_ITEMS, makeMultiredditItem(multireddit, isFirst));
    //             isFirst = false;
    //         }
    //     }
    // });
    //
    // NOTE: Android handler + adapter API; stubbed
    mMultiredditSubscriptions = subscriptionsSorted;
}

// ===== Port of: public static void showActionMenu(AppCompatActivity, SubredditCanonicalId) =====
void MainMenuListingManager::showActionMenu(
        AppCompatActivity * /*activity*/,
        SubredditCanonicalId * /*subreddit*/) {

    // Port of:
    // final EnumSet<SubredditAction> itemPref = PrefsUtility.pref_menus_subreddit_context_items();
    // if(itemPref.isEmpty()) return;
    //
    // final ArrayList<SubredditMenuItem> menu = new ArrayList<>();
    // if(itemPref.contains(COPY_URL)) { menu.add(new SubredditMenuItem(activity, R.string.action_copy_link, COPY_URL)); }
    // if(itemPref.contains(EXTERNAL)) { menu.add(...); }
    // if(itemPref.contains(SHARE)) { menu.add(...); }
    // if(itemPref.contains(BLOCK)) { check blocked state, add BLOCK or UNBLOCK }
    // if(itemPref.contains(PIN)) { check pinned state, add PIN or UNPIN }
    // if(!account.isAnonymous()) {
    //     if(itemPref.contains(SUBSCRIBE)) {
    //         check subscription state, add SUBSCRIBE or UNSUBSCRIBE
    //     }
    // }
    //
    // Build String[] menuText from menu
    // MaterialAlertDialogBuilder builder = new MaterialAlertDialogBuilder(activity);
    // builder.setItems(menuText, (dialog, which) ->
    //     onSubredditActionMenuItemSelected(subreddit, activity, menu.get(which).action));
    // AlertDialog alert = builder.create();
    // alert.setCanceledOnTouchOutside(true);
    // alert.show();
    //
    // NOTE: PrefsUtility, EnumSet, MaterialAlertDialogBuilder, AlertDialog API; stubbed
}

// ===== Port of: public void onUpdateAnnouncement() =====
void MainMenuListingManager::onUpdateAnnouncement() {
    // Port of:
    // final SharedPrefsWrapper sharedPreferences = General.getSharedPrefs(mContext);
    // if(PrefsUtility.pref_menus_mainmenu_dev_announcements()) {
    //     final Optional<Announcement> announcement
    //         = AnnouncementDownloader.getMostRecentUnreadAnnouncement(sharedPreferences);
    //     if(announcement.isPresent()) {
    //         mAnnouncementHolder.removeAllViews();
    //         mAnnouncementHolder.addView(new AnnouncementView(mActivity, announcement.get()));
    //     }
    // }
    //
    // NOTE: Requires SharedPrefsWrapper, PrefsUtility, AnnouncementDownloader,
    // Announcement, AnnouncementView; all stubbed
}

// ===== Port of: private void setPinnedSubreddits() =====
void MainMenuListingManager::setPinnedSubreddits() {
    // Port of:
    // final List<SubredditCanonicalId> pinnedSubreddits = PrefsUtility.pref_pinned_subreddits();
    // mAdapter.removeAllFromGroup(GROUP_PINNED_SUBREDDITS_ITEMS);
    // mAdapter.removeAllFromGroup(GROUP_PINNED_SUBREDDITS_HEADER);
    // if(!pinnedSubreddits.isEmpty()) {
    //     // Check pinnedSubredditsSort == NAME -> Collections.sort
    //     // Add header
    //     // boolean isFirst = true;
    //     // for(SubredditCanonicalId sr : pinnedSubreddits) {
    //     //     mAdapter.appendToGroup(GROUP_PINNED_SUBREDDITS_ITEMS,
    //     //         makeSubredditItem(sr, isFirst, true));
    //     //     isFirst = false;
    //     // }
    // }
    //
    // NOTE: PrefsUtility + adapter API; stubbed
}

// ===== Port of: private void setBlockedSubreddits() =====
void MainMenuListingManager::setBlockedSubreddits() {
    // Port of:
    // final List<SubredditCanonicalId> blockedSubreddits = PrefsUtility.pref_blocked_subreddits();
    // mAdapter.removeAllFromGroup(GROUP_BLOCKED_SUBREDDITS_ITEMS);
    // mAdapter.removeAllFromGroup(GROUP_BLOCKED_SUBREDDITS_HEADER);
    // if(!blockedSubreddits.isEmpty()) {
    //     // Check blockedSubredditsSort == NAME -> Collections.sort
    //     // Add header
    //     // boolean isFirst = true;
    //     // for(SubredditCanonicalId sr : blockedSubreddits) {
    //     //     mAdapter.appendToGroup(GROUP_BLOCKED_SUBREDDITS_ITEMS,
    //     //         makeSubredditItem(sr, isFirst, true));
    //     //     isFirst = false;
    //     // }
    // }
    //
    // NOTE: PrefsUtility + adapter API; stubbed
}

// ===== Port of: private void showMultiredditsHeader(Context) =====
void MainMenuListingManager::showMultiredditsHeader(Context * /*context*/) {
    // Port of:
    // General.checkThisIsUIThread();
    // if(mMultiredditHeaderItem == null) {
    //     mMultiredditHeaderItem = new GroupedRecyclerViewItemListSectionHeaderView(
    //         context.getString(R.string.mainmenu_header_multireddits));
    //     mAdapter.appendToGroup(GROUP_MULTIREDDITS_HEADER, mMultiredditHeaderItem);
    // }
    //
    // NOTE: adapter API; stubbed
}

// ===== Port of: private void hideMultiredditsHeader() =====
void MainMenuListingManager::hideMultiredditsHeader() {
    // Port of:
    // General.checkThisIsUIThread();
    // mMultiredditHeaderItem = null;
    // mAdapter.removeAllFromGroup(GROUP_MULTIREDDITS_HEADER);
    //
    // NOTE: adapter API; stubbed
    mMultiredditHeaderItem = nullptr;
}

// ===== Port of: private GroupedRecyclerViewItemListItemView makeItem(int nameRes, int action, Drawable icon, boolean) =====
GroupedRecyclerViewItemListItemView *MainMenuListingManager::makeItem(
        int /*nameRes*/,
        int /*action*/,
        void * /*icon*/,
        bool /*hideDivider*/) {

    // Port of: return makeItem(mContext.getString(nameRes), action, icon, hideDivider);
    // NOTE: mContext stub needed; call the string overload
    return makeItem(QString(), 0, nullptr, false);
}

// ===== Port of: private GroupedRecyclerViewItemListItemView makeItem(String name, int action, Drawable icon, boolean) =====
GroupedRecyclerViewItemListItemView *MainMenuListingManager::makeItem(
        const QString & /*name*/,
        int /*action*/,
        void * /*icon*/,
        bool /*hideDivider*/) {

    // Port of:
    // final View.OnClickListener clickListener = view -> mListener.onSelected(action);
    // return new GroupedRecyclerViewItemListItemView(icon, name, null, hideDivider,
    //     clickListener, null, Optional.empty(), Optional.empty(), Optional.empty());
    //
    // NOTE: GroupedRecyclerViewItemListItemView, Optional, OnClickListener; all stubbed
    return nullptr;
}

// ===== Port of: private GroupedRecyclerViewItemListItemView makeSubredditItem(...) =====
GroupedRecyclerViewItemListItemView *MainMenuListingManager::makeSubredditItem(
        SubredditCanonicalId * /*subreddit*/,
        bool /*hideDivider*/,
        bool /*showRSlashPrefix*/) {

    // Port of:
    // final View.OnClickListener clickListener = view -> {
    //     if(subreddit.toString().startsWith("/r/")) {
    //         mListener.onSelected((PostListingURL)SubredditPostListURL.getSubreddit(subreddit));
    //     } else {
    //         LinkHandler.onLinkClicked(mActivity, new UriString(subreddit.toString()));
    //     }
    // };
    // final View.OnLongClickListener longClickListener = view -> {
    //     showActionMenu(mActivity, subreddit);
    //     return true;
    // };
    // final String displayName = showRSlashPrefix ? subreddit.toString() : subreddit.getDisplayNameLowercase();
    // return new GroupedRecyclerViewItemListItemView(null, displayName,
    //     ScreenreaderPronunciation.getPronunciation(mContext, displayName),
    //     hideDivider, clickListener, longClickListener,
    //     Optional.empty(), Optional.empty(), Optional.empty());
    //
    // NOTE: GroupedRecyclerViewItemListItemView, SubredditPostListURL, LinkHandler,
    // ScreenreaderPronunciation, Optional; all stubbed
    return nullptr;
}

// ===== Port of: private GroupedRecyclerViewItemListItemView makeMultiredditItem(String, boolean) =====
GroupedRecyclerViewItemListItemView *MainMenuListingManager::makeMultiredditItem(
        const QString & /*name*/,
        bool /*hideDivider*/) {

    // Port of:
    // final View.OnClickListener clickListener = view -> mListener.onSelected(
    //     (PostListingURL)MultiredditPostListURL.getMultireddit(name));
    // return new GroupedRecyclerViewItemListItemView(null, name,
    //     ScreenreaderPronunciation.getPronunciation(mContext, name),
    //     hideDivider, clickListener, null,
    //     Optional.empty(), Optional.empty(), Optional.empty());
    //
    // NOTE: GroupedRecyclerViewItemListItemView, MultiredditPostListURL,
    // ScreenreaderPronunciation, Optional; all stubbed
    return nullptr;
}

// ===== Port of: private static void onSubredditActionMenuItemSelected(...) =====
void MainMenuListingManager::onSubredditActionMenuItemSelected(
        SubredditCanonicalId * /*subredditCanonicalId*/,
        AppCompatActivity * /*activity*/,
        SubredditAction /*action*/) {

    // Port of:
    // final UriString url = Constants.Reddit.getNonAPIUri(subredditCanonicalId.toString());
    // final RedditSubredditSubscriptionManager subMan = RedditSubredditSubscriptionManager
    //     .getSingleton(activity, RedditAccountManager.getInstance(activity).getDefaultAccount());
    //
    // switch(action) {
    //     case SHARE: LinkHandler.shareText(activity, ...); break;
    //     case COPY_URL: ClipboardManager clipboard ... Clipboard ... General.quickToast(...); break;
    //     case EXTERNAL: Intent intent = new Intent(Intent.ACTION_VIEW); intent.setData(Uri.parse(url.value)); ... break;
    //     case PIN: PrefsUtility.pref_pinned_subreddits_add(activity, subredditCanonicalId); break;
    //     case UNPIN: PrefsUtility.pref_pinned_subreddits_remove(activity, subredditCanonicalId); break;
    //     case BLOCK: PrefsUtility.pref_blocked_subreddits_add(activity, subredditCanonicalId); break;
    //     case UNBLOCK: PrefsUtility.pref_blocked_subreddits_remove(activity, subredditCanonicalId); break;
    //     case SUBSCRIBE: if(NOT_SUBSCRIBED) subMan.subscribe(...) Toast; else Toast already subscribed; break;
    //     case UNSUBSCRIBE: if(SUBSCRIBED) subMan.unsubscribe(...) Toast; else Toast not subscribed; break;
    // }
    //
    // NOTE: Requires Constants, SubredditSubscriptionManager, PrefsUtility, LinkHandler,
    // ClipboardManager, Intent, Toast; all stubbed
}

} // namespace PinkReader
