/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_search_activity.h
 *   Port of SubredditSearchActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/SubredditSearchActivity.java
 *
 * Original: public class SubredditSearchActivity extends ViewsBaseActivity implements
 *     RedditSubredditSubscriptionManager.SubredditSubscriptionStateChangeListener
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

// Android framework
class Bundle;
class SearchView;
class RecyclerView;
class LinearLayoutManager;
class ViewGroup;
class LayoutInflater;

// RedReader base
class ViewsBaseActivity;

// RedReader common
class OptionalBase;
template<typename T> class Optional;
class ThreadCheckedVarBase;
template<typename T> class ThreadCheckedVar;
class GenerationalCacheBase;
template<typename K, typename V> class GenerationalCache;
class EventListenerSetBase;
template<typename T> class EventListenerSet;
class RRError;
class AndroidCommon;
class General;
class PrefsUtility;
class StringUtils;
class CollectionStreamBase;

// RedReader account
class RedditAccount;
class RedditAccountManager;

// RedReader cache
class CacheManager;

// RedReader adapters
class GroupedRecyclerViewAdapter;
class GroupedRecyclerViewItemLoadingSpinner;
class GroupedRecyclerViewItemRRError;

// RedReader viewholders
class SubredditItemViewHolder;

// RedReader views
class SubredditSearchQuickLinks;

// RedReader reddit
class RedditAPI;
class RedditSubredditSubscriptionManager;
class SubredditDetails;
class SubredditCanonicalId;
class RedditSubreddit;

// RedReader API
class APIResponseHandler;
class BugReportActivity;

// RedReader common functional
class FunctionOneArgNoReturnBase;

// ============================================================================
// SubredditItem — inner class (Java lines 125-153)
//
// Port of: private class SubredditItem
//     extends GroupedRecyclerViewAdapter.Item<SubredditItemViewHolder>
// ============================================================================

class SubredditItem : public QObject {
    Q_OBJECT

public:
    // Port of: private SubredditItem(@NonNull final SubredditDetails subreddit)
    explicit SubredditItem(SubredditDetails *subreddit, QObject *parent = nullptr);
    ~SubredditItem() override;

    // Port of: @Override public Class<RedditSubreddit> getViewType()
    // Returns the view type class identifier
    const void *getViewType() const;

    // Port of: @Override public SubredditItemViewHolder onCreateViewHolder(
    //     final ViewGroup viewGroup)
    SubredditItemViewHolder *onCreateViewHolder(ViewGroup *viewGroup);

    // Port of: @Override public void onBindViewHolder(
    //     final SubredditItemViewHolder holder)
    void onBindViewHolder(SubredditItemViewHolder *holder);

    // Port of: @Override public boolean isHidden()
    bool isHidden() const;

private:
    // Port of: @NonNull private final SubredditDetails mSubreddit;
    SubredditDetails *mSubreddit = nullptr;
};

// ============================================================================
// SubredditSearchActivity — port of Java class (Java lines 64-531)
//
// Port of: org.quantumbadger.redreader.activities.SubredditSearchActivity
//
// Extends ViewsBaseActivity, implements
//   RedditSubredditSubscriptionManager.SubredditSubscriptionStateChangeListener
// Every field, method, constant, and inner class ported exactly.
// ============================================================================

class SubredditSearchActivity : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Static constants (Java lines 67, 102-104)
    // ========================================================================

    // Port of: private static final String TAG = "SubredditSearchActivity";
    static const QString TAG;

    // Port of: private static final int GROUP_QUICK_LINKS = 0;
    static constexpr int GROUP_QUICK_LINKS = 0;

    // Port of: private static final int GROUP_SUBREDDITS = 1;
    static constexpr int GROUP_SUBREDDITS = 1;

    // Port of: private static final int GROUP_LOADING_SPINNER = 2;
    static constexpr int GROUP_LOADING_SPINNER = 2;

    // ========================================================================
    // Constructor / Destructor
    // ========================================================================
    explicit SubredditSearchActivity(QObject *parent = nullptr);
    ~SubredditSearchActivity() override;

    // ========================================================================
    // Interface: SubredditSubscriptionStateChangeListener (Java lines 107-123)
    // ========================================================================

    // Port of: @Override public void onSubredditSubscriptionListUpdated(
    //     final RedditSubredditSubscriptionManager subredditSubscriptionManager)
    void onSubredditSubscriptionListUpdated(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager);

    // Port of: @Override public void onSubredditSubscriptionAttempted(
    //     final RedditSubredditSubscriptionManager subredditSubscriptionManager)
    void onSubredditSubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager);

    // Port of: @Override public void onSubredditUnsubscriptionAttempted(
    //     final RedditSubredditSubscriptionManager subredditSubscriptionManager)
    void onSubredditUnsubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager);

    // ========================================================================
    // Override methods (from ViewsBaseActivity) (Java lines 155-158)
    // ========================================================================

    // Port of: @Override protected boolean baseActivityIsToolbarSearchBarEnabled()
    bool baseActivityIsToolbarSearchBarEnabled();

    // ========================================================================
    // Public lifecycle methods
    // ========================================================================

    // Port of: @Override protected void onCreate(final Bundle savedInstanceState) (Java lines 284-372)
    void onCreate(Bundle *savedInstanceState);

    // Port of: @Override protected void onDestroy() (Java lines 525-530)
    void onDestroy();

private:
    // ========================================================================
    // Private methods
    // ========================================================================

    // Port of: private void updateList() (Java lines 160-282)
    void updateList();

    // Port of: private void handleQueryChanged(@NonNull final String text) (Java lines 374-407)
    void handleQueryChanged(const QString &text);

    // Port of: private void doSearchRequest(@NonNull final String text) (Java lines 409-469)
    void doSearchRequest(const QString &text);

    // Port of: private void requestSubscriptions() (Java lines 471-523)
    void requestSubscriptions();

    // ========================================================================
    // Private fields (Java lines 69-103)
    // ========================================================================

    // Port of: private RedditSubredditSubscriptionManager mSubredditSubscriptionManager;
    RedditSubredditSubscriptionManager *mSubredditSubscriptionManager = nullptr;

    // Port of: @NonNull private final ThreadCheckedVar<SearchView> mSearchView
    //     = new ThreadCheckedVar<>(null);
    void *mSearchView = nullptr; // ThreadCheckedVar<SearchView *> stub

    // Port of: @NonNull private final ThreadCheckedVar<Optional<ArrayList<SubredditDetails>>>
    //     mSubscriptions = new ThreadCheckedVar<>(Optional.empty());
    void *mSubscriptions = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<HashSet<String>> mQueriesPending
    //     = new ThreadCheckedVar<>(new HashSet<>());
    void *mQueriesPending = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<Boolean> mSubscriptionListPending
    //     = new ThreadCheckedVar<>(false);
    void *mSubscriptionListPending = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<HashMap<String, ArrayList<SubredditDetails>>>
    //     mQueryResults = new ThreadCheckedVar<>(new HashMap<>());
    void *mQueryResults = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final GenerationalCache<SubredditDetails, SubredditItem>
    //     mSubredditItemCache = new GenerationalCache<>(SubredditItem::new);
    void *mSubredditItemCache = nullptr; // GenerationalCache stub

    // Port of: @NonNull private Optional<RedditSubredditSubscriptionManager.ListenerContext>
    //     mSubredditSubscriptionListenerContext = Optional.empty();
    void *mSubredditSubscriptionListenerContext = nullptr; // Optional stub

    // Port of: private LinearLayoutManager mRecyclerViewLayout;
    LinearLayoutManager *mRecyclerViewLayout = nullptr;

    // Port of: private GroupedRecyclerViewItemLoadingSpinner mLoadingItem;
    GroupedRecyclerViewItemLoadingSpinner *mLoadingItem = nullptr;

    // Port of: @NonNull private final ThreadCheckedVar<Optional<GroupedRecyclerViewItemRRError>>
    //     mSubscriptionsErrorItem = new ThreadCheckedVar<>(Optional.empty());
    void *mSubscriptionsErrorItem = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<Optional<GroupedRecyclerViewItemRRError>>
    //     mQueryErrorItem = new ThreadCheckedVar<>(Optional.empty());
    void *mQueryErrorItem = nullptr; // ThreadCheckedVar stub

    // Port of: private GroupedRecyclerViewAdapter mRecyclerViewAdapter;
    GroupedRecyclerViewAdapter *mRecyclerViewAdapter = nullptr;

    // Reference to ViewsBaseActivity base class
    ViewsBaseActivity *m_base = nullptr;
};

} // namespace PinkReader
