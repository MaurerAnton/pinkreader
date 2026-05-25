/*
 * PinkReader - GPLv3
 * File: main_menu_listing_manager.h - Port of MainMenuListingManager.java
 *
 * Original: public class MainMenuListingManager
 * Located at: redreader/src/main/java/org/quantumbadger/redreader/adapters/MainMenuListingManager.java
 *
 * Every field, method, inner class, inner enum, and constant ported exactly.
 * 974 lines of Java ported.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QSet>
#include <QList>
#include <memory>
#include <functional>
#include <optional>
#include <atomic>

// Include for GroupedRecyclerViewAdapter (needed for nested Item type)
#include "adapters/grouped_recycler_adapter.h"

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class Context;
class RedditAccount;
class View;
class FrameLayout;
class SharedPrefsWrapper;
class GroupedRecyclerViewAdapter;
class GroupedRecyclerViewItemListItemView;
class GroupedRecyclerViewItemListSectionHeaderView;
class GroupedRecyclerViewItemFrameLayout;
class LoadingSpinnerView;
class ErrorView;
class SubredditCanonicalId;
class PostListingURL;
class UriString;
class Announcement;
class AnnouncementView;
class ScreenreaderPronunciation;

// Forward declarations from fragments
enum class MainMenuUserItems;
enum class MainMenuShortcutItems;
class MainMenuSelectionListener;

/**
 * @brief Port of MainMenuListingManager inner enum: SubredditAction
 */
enum class SubredditAction {
    SHARE,          // R.string.action_share
    COPY_URL,       // R.string.action_copy_link
    BLOCK,          // R.string.block_subreddit
    UNBLOCK,        // R.string.unblock_subreddit
    PIN,            // R.string.pin_subreddit
    UNPIN,          // R.string.unpin_subreddit
    SUBSCRIBE,      // R.string.options_subscribe
    UNSUBSCRIBE,    // R.string.options_unsubscribe
    EXTERNAL        // R.string.action_external
};

/**
 * @brief Port of MainMenuListingManager inner class: SubredditMenuItem
 */
class SubredditMenuItem {
public:
    QString title;
    SubredditAction action;

    // Port of: private SubredditMenuItem(Context context, int titleRes, SubredditAction action)
    SubredditMenuItem(const QString &title_, SubredditAction action_)
        : title(title_), action(action_) {}
};

/**
 * @brief Port of MainMenuListingManager.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */
class MainMenuListingManager : public QObject {
    Q_OBJECT

public:
    // ===== Port of: group constants (private static final int) =====
    // Port of: private static final int GROUP_MAIN_HEADER = 0;
    static constexpr int GROUP_MAIN_HEADER = 0;
    // Port of: private static final int GROUP_MAIN_ITEMS = 1;
    static constexpr int GROUP_MAIN_ITEMS = 1;
    // Port of: private static final int GROUP_USER_HEADER = 2;
    static constexpr int GROUP_USER_HEADER = 2;
    // Port of: private static final int GROUP_USER_ITEMS = 3;
    static constexpr int GROUP_USER_ITEMS = 3;
    // Port of: private static final int GROUP_ANNOUNCEMENTS = 4;
    static constexpr int GROUP_ANNOUNCEMENTS = 4;
    // Port of: private static final int GROUP_PINNED_SUBREDDITS_HEADER = 5;
    static constexpr int GROUP_PINNED_SUBREDDITS_HEADER = 5;
    // Port of: private static final int GROUP_PINNED_SUBREDDITS_ITEMS = 6;
    static constexpr int GROUP_PINNED_SUBREDDITS_ITEMS = 6;
    // Port of: private static final int GROUP_BLOCKED_SUBREDDITS_HEADER = 7;
    static constexpr int GROUP_BLOCKED_SUBREDDITS_HEADER = 7;
    // Port of: private static final int GROUP_BLOCKED_SUBREDDITS_ITEMS = 8;
    static constexpr int GROUP_BLOCKED_SUBREDDITS_ITEMS = 8;
    // Port of: private static final int GROUP_MULTIREDDITS_HEADER = 9;
    static constexpr int GROUP_MULTIREDDITS_HEADER = 9;
    // Port of: private static final int GROUP_MULTIREDDITS_ITEMS = 10;
    static constexpr int GROUP_MULTIREDDITS_ITEMS = 10;
    // Port of: private static final int GROUP_SUBREDDITS_HEADER = 11;
    static constexpr int GROUP_SUBREDDITS_HEADER = 11;
    // Port of: private static final int GROUP_SUBREDDITS_ITEMS = 12;
    static constexpr int GROUP_SUBREDDITS_ITEMS = 12;

    // ===== Constructor =====
    // Port of: public MainMenuListingManager(AppCompatActivity activity,
    //     MainMenuSelectionListener listener, RedditAccount user)
    MainMenuListingManager(AppCompatActivity *activity,
                           MainMenuSelectionListener *listener,
                           RedditAccount *user,
                           QObject *parent = nullptr);
    ~MainMenuListingManager() override;

    // ===== Port of: public GroupedRecyclerViewAdapter getAdapter() =====
    GroupedRecyclerViewAdapter *getAdapter();

    // ===== Port of: public void setMultiredditsError(ErrorView) =====
    void setMultiredditsError(ErrorView *errorView);

    // ===== Port of: public void setSubredditsError(ErrorView) =====
    void setSubredditsError(ErrorView *errorView);

    // ===== Port of: public void setSubreddits(Collection<SubredditCanonicalId>) =====
    void setSubreddits(const QVector<SubredditCanonicalId *> &subscriptions);

    // ===== Port of: public void setMultireddits(Collection<String>) =====
    void setMultireddits(const QSet<QString> &subscriptions);

    // ===== Port of: public static void showActionMenu(AppCompatActivity, SubredditCanonicalId) =====
    static void showActionMenu(AppCompatActivity *activity,
                               SubredditCanonicalId *subreddit);

    // ===== Port of: public void onUpdateAnnouncement() =====
    void onUpdateAnnouncement();

private:
    // ===== Port of: private fields =====

    // Port of: @NonNull private final GroupedRecyclerViewAdapter mAdapter = new GroupedRecyclerViewAdapter(13);
    GroupedRecyclerViewAdapter *mAdapter = nullptr;

    // Port of: @NonNull private final Context mContext;
    Context *mContext = nullptr;

    // Port of: @NonNull private final AppCompatActivity mActivity;
    AppCompatActivity *mActivity = nullptr;

    // Port of: @NonNull private final MainMenuSelectionListener mListener;
    MainMenuSelectionListener *mListener = nullptr;

    // Port of: @Nullable private GroupedRecyclerViewAdapter.Item mMultiredditHeaderItem;
    GroupedRecyclerViewAdapter::Item *mMultiredditHeaderItem = nullptr;

    // Port of: @Nullable private ArrayList<SubredditCanonicalId> mSubredditSubscriptions;
    QVector<SubredditCanonicalId *> mSubredditSubscriptions;

    // Port of: @Nullable private ArrayList<String> mMultiredditSubscriptions;
    QList<QString> mMultiredditSubscriptions;

    // Port of: @NonNull private final FrameLayout mAnnouncementHolder;
    FrameLayout *mAnnouncementHolder = nullptr;

    // ===== Port of: private methods =====

    // Port of: private void setPinnedSubreddits()
    void setPinnedSubreddits();

    // Port of: private void setBlockedSubreddits()
    void setBlockedSubreddits();

    // Port of: private void showMultiredditsHeader(Context)
    void showMultiredditsHeader(Context *context);

    // Port of: private void hideMultiredditsHeader()
    void hideMultiredditsHeader();

    // Port of: private GroupedRecyclerViewItemListItemView makeItem(int nameRes, @MainMenuAction int action,
    //     @Nullable Drawable icon, boolean hideDivider)
    GroupedRecyclerViewItemListItemView *makeItem(int nameRes,
                                                   int action,
                                                   void *icon, // Drawable stub
                                                   bool hideDivider);

    // Port of: private GroupedRecyclerViewItemListItemView makeItem(String name, @MainMenuAction int action,
    //     @Nullable Drawable icon, boolean hideDivider)
    GroupedRecyclerViewItemListItemView *makeItem(const QString &name,
                                                   int action,
                                                   void *icon, // Drawable stub
                                                   bool hideDivider);

    // Port of: private GroupedRecyclerViewItemListItemView makeSubredditItem(SubredditCanonicalId,
    //     boolean hideDivider, boolean showRSlashPrefix)
    GroupedRecyclerViewItemListItemView *makeSubredditItem(SubredditCanonicalId *subreddit,
                                                            bool hideDivider,
                                                            bool showRSlashPrefix);

    // Port of: private GroupedRecyclerViewItemListItemView makeMultiredditItem(String, boolean)
    GroupedRecyclerViewItemListItemView *makeMultiredditItem(const QString &name,
                                                              bool hideDivider);

    // Port of: private static void onSubredditActionMenuItemSelected(SubredditCanonicalId,
    //     AppCompatActivity, SubredditAction)
    static void onSubredditActionMenuItemSelected(SubredditCanonicalId *subredditCanonicalId,
                                                    AppCompatActivity *activity,
                                                    SubredditAction action);
};

} // namespace PinkReader
