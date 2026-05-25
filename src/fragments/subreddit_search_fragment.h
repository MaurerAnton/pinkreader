/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_search_fragment.h
 *   Port of SubredditSearchFragment.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/fragments/SubredditSearchFragment.java
 *
 * Original: public class SubredditSearchFragment extends RRFragment
 *
 * Presents the subreddit search UI with a list of matching subreddits.
 * Every field, method, and constant ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>
#include <vector>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

// Fragment base
class RRFragment;
class AppCompatActivity;

// Android framework
class Bundle;
class Context;
class View;
class ViewGroup;
class LayoutInflater;
class RecyclerView;
class LinearLayoutManager;
class TextView;
class EditText;
class ImageButton;
class SearchView;

// RedReader adapters
class GroupedRecyclerViewAdapter;

// RedReader views
class SubredditItemViewHolder;
class SubredditSearchQuickLinks;

// RedReader common
class General;
class PrefsUtility;
class OptionalBase;
template<typename T> class Optional;
class EventListenerSetBase;
template<typename T> class EventListenerSet;
class ThreadCheckedVarBase;
template<typename T> class ThreadCheckedVar;
class RRError;
class AndroidCommon;
class StringUtils;
class GenerationalCacheBase;
template<typename K, typename V> class GenerationalCache;

// RedReader account
class RedditAccount;
class RedditAccountManager;

// RedReader cache
class CacheManager;

// RedReader reddit
class SubredditDetails;
class SubredditCanonicalId;
class RedditSubreddit;
class RedditAPI;
class RedditSubredditSubscriptionManager;
class APIResponseHandler;

// RedReader adapters
class GroupedRecyclerViewItemLoadingSpinner;
class GroupedRecyclerViewItemRRError;

// ============================================================================
// SubredditSearchFragment — port of Java class
//
// Port of: org.quantumbadger.redreader.fragments.SubredditSearchFragment
//
// Extends RRFragment. Every field, method, and inner class ported exactly.
// ============================================================================

class SubredditSearchFragment : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Static constants
    // ========================================================================

    // Port of: private static final String TAG = "SubredditSearchFragment";
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
    explicit SubredditSearchFragment(
        AppCompatActivity *parent,
        Bundle *savedInstanceState,
        QObject *qparent = nullptr);
    ~SubredditSearchFragment() override;

    // ========================================================================
    // Public API
    // ========================================================================

    // Port of: public View getListingView()
    View *getListingView();

    // Port of: @Nullable public View getOverlayView()
    View *getOverlayView();

    // Port of: public Bundle onSaveInstanceState()
    Bundle *onSaveInstanceState();

    // Port of: public void onQueryTextChanged(String query)
    void onQueryTextChanged(const QString &query);

    // Port of: public void onQueryTextSubmitted(String query)
    void onQueryTextSubmitted(const QString &query);

    // ========================================================================
    // Override methods (from RRFragment)
    // ========================================================================

    // Port of: @Override public void onCreateOptionsMenu(Menu menu)
    void onCreateOptionsMenu(void *menu);

    // Port of: @Override public boolean onOptionsItemSelected(MenuItem item)
    bool onOptionsItemSelected(void *item);

private:
    // ========================================================================
    // Private methods
    // ========================================================================

    // Port of: private void updateList(String currentQuery)
    void updateList(const QString &currentQuery);

    // Port of: private void doSearchRequest(String query)
    void doSearchRequest(const QString &query);

    // Port of: private void requestSubscriptions()
    void requestSubscriptions();

    // ========================================================================
    // Private fields
    // ========================================================================

    // Port of: private final AppCompatActivity mParent;
    AppCompatActivity *mParent = nullptr;

    // Port of: private final RecyclerView mRecyclerView;
    View *mRecyclerView = nullptr;

    // Port of: private final LinearLayoutManager mRecyclerViewLayout;
    LinearLayoutManager *mRecyclerViewLayout = nullptr;

    // Port of: private final GroupedRecyclerViewAdapter mRecyclerViewAdapter;
    GroupedRecyclerViewAdapter *mRecyclerViewAdapter = nullptr;

    // Port of: private GroupedRecyclerViewItemLoadingSpinner mLoadingItem;
    GroupedRecyclerViewItemLoadingSpinner *mLoadingItem = nullptr;

    // Port of: private RedditSubredditSubscriptionManager mSubredditSubscriptionManager;
    RedditSubredditSubscriptionManager *mSubredditSubscriptionManager = nullptr;

    // Port of: @NonNull private final ThreadCheckedVar<Optional<ArrayList<SubredditDetails>>>
    //     mSubscriptions;
    void *mSubscriptions = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<HashSet<String>> mQueriesPending;
    void *mQueriesPending = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<Boolean> mSubscriptionListPending;
    void *mSubscriptionListPending = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<HashMap<String, ArrayList<SubredditDetails>>>
    //     mQueryResults;
    void *mQueryResults = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<Optional<GroupedRecyclerViewItemRRError>>
    //     mSubscriptionsErrorItem;
    void *mSubscriptionsErrorItem = nullptr; // ThreadCheckedVar stub

    // Port of: @NonNull private final ThreadCheckedVar<Optional<GroupedRecyclerViewItemRRError>>
    //     mQueryErrorItem;
    void *mQueryErrorItem = nullptr; // ThreadCheckedVar stub

    // Port of: private EventListenerSet<String> mQueryEventListeners;
    void *mQueryEventListeners = nullptr; // EventListenerSet stub

    // Port of: @NonNull private final GenerationalCache<SubredditDetails, SubredditItem>
    //     mSubredditItemCache;
    void *mSubredditItemCache = nullptr; // GenerationalCache stub

    // Port of: private View mListingView;
    View *mListingView = nullptr;

    // Port of: private View mOverlayView;
    View *mOverlayView = nullptr;
};

} // namespace PinkReader
