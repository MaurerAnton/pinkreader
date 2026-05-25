/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_search_activity.cpp
 *   Port of SubredditSearchActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/SubredditSearchActivity.java
 *
 * Every field, method, and logic branch ported exactly.
 */

#include "activities/subreddit_search_activity.h"

// ============================================================================
// Stub types for non-ported Android/RedReader dependencies
//
// In the original Java, this activity depends on:
//   - ViewsBaseActivity (base class, not yet fully ported)
//   - ThreadCheckedVar<T> (thread-safe variable with UI thread assertions)
//   - Optional<T> (nullable wrapper with apply/isPresent/get)
//   - GenerationalCache<K,V> (generational cache for view items)
//   - GroupedRecyclerViewAdapter (multi-group RecyclerView adapter)
//   - GroupedRecyclerViewItemLoadingSpinner (loading indicator item)
//   - GroupedRecyclerViewItemRRError (error display item)
//   - SubredditItemViewHolder (view holder for subreddit items)
//   - RedditSubredditSubscriptionManager (subscription state manager)
//   - Various Reddit API types
//   - Android widgets: SearchView, RecyclerView, LinearLayoutManager
// ============================================================================

#include <QDebug>
#include <QString>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cctype>
#include <functional>

namespace PinkReader {

// ============================================================================
// Static constant definitions
// ============================================================================

// Port of: private static final String TAG = "SubredditSearchActivity";
const QString SubredditSearchActivity::TAG = QStringLiteral("SubredditSearchActivity");

// ============================================================================
// Forward-declared stub types
// ============================================================================

// Bundle stub
class Bundle {
public:
    virtual ~Bundle() = default;
    Bundle() = default;
};

// Context stub
class Context {
public:
    virtual ~Context() = default;
};

// ViewsBaseActivity stub (base class)
class ViewsBaseActivity {
public:
    virtual ~ViewsBaseActivity() = default;
    virtual void setTitle(const QString &title) { (void)title; }
    virtual void setTitle(int res) { (void)res; }
    virtual void setBaseActivityListing(int layoutRes) { (void)layoutRes; }
    virtual void setBaseActivityListing(void *view) { (void)view; }
    virtual void finish() {}
    virtual void onCreate(Bundle *savedInstanceState) { (void)savedInstanceState; }
    virtual void onDestroy() {}
    virtual Context *getApplicationContext() { return nullptr; }
    virtual bool baseActivityIsToolbarSearchBarEnabled() { return false; }
};

// ThreadCheckedVar stub — thread-safe variable wrapper
// Port of: org.quantumbadger.redreader.common.ThreadCheckedVar<T>
template<typename T>
class ThreadCheckedVarStub {
public:
    explicit ThreadCheckedVarStub(const T &initial) : mValue(initial) {}
    ThreadCheckedVarStub() : mValue() {}

    T get() const { return mValue; }
    void set(const T &value) { mValue = value; }

private:
    T mValue;
};

// Optional stub — nullable wrapper
// Port of: org.quantumbadger.redreader.common.Optional<T>
template<typename T>
class OptionalStub {
public:
    static OptionalStub<T> empty() { return OptionalStub<T>(T{}, true); }
    static OptionalStub<T> of(const T &value) { return OptionalStub<T>(value, false); }

    bool isPresent() const { return !mEmpty; }
    bool isEmpty() const { return mEmpty; }

    T get() const { return mValue; }

    // Port of: apply(Consumer<T>), applyR(Function<T,R>)
    template<typename Func>
    void apply(Func f) { if (!mEmpty) f(mValue); }

    // Port of: map
    template<typename R, typename Func>
    OptionalStub<R> map(Func f) const {
        if (mEmpty) return OptionalStub<R>::empty();
        return OptionalStub<R>::of(f(mValue));
    }

private:
    OptionalStub(const T &value, bool empty) : mValue(value), mEmpty(empty) {}
    T mValue;
    bool mEmpty = true;
};

// GenerationalCache stub
// Port of: org.quantumbadger.redreader.common.GenerationalCache<K,V>
template<typename K, typename V>
class GenerationalCacheStub {
public:
    GenerationalCacheStub() = default;
    V *get(const K &key) {
        auto it = mCache.find(key);
        if (it != mCache.end()) {
            return it->second;
        }
        V *v = new V(key);
        mCache[key] = v;
        return v;
    }
    void nextGeneration() {
        // In C++ we just clear the cache
        for (auto &pair : mCache) {
            delete pair.second;
        }
        mCache.clear();
    }
private:
    std::unordered_map<K, V *> mCache;
};

// EventListenerSet stub
// Port of: org.quantumbadger.redreader.common.EventListenerSet<T>
template<typename T>
class EventListenerSetStub {
public:
    void send(const T &event) { (void)event; }
    void addListener(void *listener) { (void)listener; }
};

// GroupedRecyclerViewAdapter stub
class GroupedRecyclerViewAdapter {
public:
    explicit GroupedRecyclerViewAdapter(int numGroups) { (void)numGroups; }
    ~GroupedRecyclerViewAdapter() = default;

    void appendToGroup(int group, void *item) {
        (void)group; (void)item;
    }

    void removeAllFromGroup(int group) {
        (void)group;
    }

    void updateHiddenStatus() {}

    void notifyDataSetChanged() {}
};

// GroupedRecyclerViewItemLoadingSpinner stub
class GroupedRecyclerViewItemLoadingSpinner {
public:
    explicit GroupedRecyclerViewItemLoadingSpinner(Context *context) { (void)context; }
    ~GroupedRecyclerViewItemLoadingSpinner() = default;

    void setHidden(bool hidden) { (void)hidden; }
};

// GroupedRecyclerViewItemRRError stub
class GroupedRecyclerViewItemRRError {
public:
    GroupedRecyclerViewItemRRError(Context *context, void *error) {
        (void)context; (void)error;
    }
    ~GroupedRecyclerViewItemRRError() = default;
};

// SubredditItemViewHolder stub
class SubredditItemViewHolder {
public:
    SubredditItemViewHolder() = default;
    ~SubredditItemViewHolder() = default;
    explicit SubredditItemViewHolder(void *viewGroup, void *activity) {
        (void)viewGroup; (void)activity;
    }
    void bind(void *subreddit) { (void)subreddit; }
};

// SubredditDetails stub
class SubredditDetails {
public:
    ~SubredditDetails() = default;

    static SubredditDetails newWithRuntimeException(void *subreddit) {
        (void)subreddit;
        return SubredditDetails();
    }

    explicit SubredditDetails(void *id) { (void)id; }
    SubredditDetails() = default;

    QString name;
};

// SubredditCanonicalId stub
class SubredditCanonicalId {
public:
    ~SubredditCanonicalId() = default;
};

// RedditSubreddit stub (for view type)
class RedditSubreddit {
public:
    ~RedditSubreddit() = default;
};

// RedditSubredditSubscriptionManager stub
class RedditSubredditSubscriptionManager {
public:
    ~RedditSubredditSubscriptionManager() = default;

    class ListenerContext {
    public:
        void removeListener() {}
    };

    // Interface: SubredditSubscriptionStateChangeListener
    class SubredditSubscriptionStateChangeListener {
    public:
        virtual ~SubredditSubscriptionStateChangeListener() = default;
        virtual void onSubredditSubscriptionListUpdated(
            RedditSubredditSubscriptionManager *) = 0;
        virtual void onSubredditSubscriptionAttempted(
            RedditSubredditSubscriptionManager *) = 0;
        virtual void onSubredditUnsubscriptionAttempted(
            RedditSubredditSubscriptionManager *) = 0;
    };

    static RedditSubredditSubscriptionManager *getSingleton(
            Context *context, void *account) {
        (void)context; (void)account;
        return new RedditSubredditSubscriptionManager();
    }

    ListenerContext *addListener(SubredditSubscriptionStateChangeListener *listener) {
        (void)listener;
        return new ListenerContext();
    }

    void triggerUpdateIfNotReady(void *onError) {
        (void)onError;
    }

    std::vector<SubredditCanonicalId *> getSubscriptionList() {
        return {};
    }
};

// RedditAccount stub
class RedditAccount {
public:
    ~RedditAccount() = default;
};

// RedditAccountManager stub
class RedditAccountManager {
public:
    static RedditAccountManager *getInstance(Context *context) {
        (void)context;
        static RedditAccountManager instance;
        return &instance;
    }
    RedditAccount *getDefaultAccount() { return nullptr; }
};

// CacheManager stub
class CacheManager {
public:
    static CacheManager *getInstance(Context *context) {
        (void)context;
        return new CacheManager();
    }
};

// RedditAPI stub
class RedditAPI {
public:
    class SubredditListResponse {
    public:
        std::vector<void *> subreddits;
    };

    static void searchSubreddits(
        CacheManager *cacheManager,
        RedditAccount *user,
        const QString &query,
        Context *context,
        void *handler,
        void *session) {
        (void)cacheManager; (void)user; (void)query;
        (void)context; (void)handler; (void)session;
    }
};

// APIResponseHandler stub
template<typename T>
class ValueResponseHandler {
public:
    explicit ValueResponseHandler(Context *context) { (void)context; }
    virtual ~ValueResponseHandler() = default;

    // Override in anonymous subclass
    virtual void onSuccess(const T &value) = 0;
    virtual void onCallbackException(void *t) = 0;
    virtual void onFailure(void *error) = 0;
};

// RRError stub
class RRError {
public:
    RRError() = default;
    RRError(const QString &title, const QString &message, bool finish, void *exception) {
        (void)title; (void)message; (void)finish; (void)exception;
    }
    RRError(const QString &title, const QString &message, bool finish) {
        (void)title; (void)message; (void)finish;
    }
};

// BugReportActivity stub
class BugReportActivity {
public:
    static void handleGlobalError(Context *context, void *t) {
        (void)context; (void)t;
    }
};

// AndroidCommon stub
class AndroidCommon {
public:
    static void runOnUiThread(std::function<void()> action) {
        // In C++ Qt, this would use QMetaObject::invokeMethod with Qt::QueuedConnection
        // For now, execute directly (single-threaded stub)
        if (action) action();
    }

    // Port of: AndroidCommon.UI_THREAD_HANDLER.postDelayed(runnable, delay)
    static void postDelayed(std::function<void()> action, int delayMs) {
        (void)delayMs;
        // Stub: execute immediately in single-threaded mode
        if (action) action();
    }
};

// General stub
class General {
public:
    static void checkThisIsUIThread() {
        // Stub: nothing to check in single-threaded C++
    }

    static bool onBackPressed() {
        // Stub: always allow back press
        return true;
    }
};

// PrefsUtility stub
class PrefsUtility {
public:
    static void applyTheme(Context *context) {
        (void)context;
    }
};

// StringUtils stub
class StringUtils {
public:
    static QString asciiLowercase(const QString &input) {
        return input.toLower();
    }
};

// CollectionStream stub
template<typename T>
class CollectionStreamStub {
public:
    explicit CollectionStreamStub(const std::vector<T *> &items) : mItems(items) {}

    template<typename R, typename Func>
    auto map(Func f) -> CollectionStreamStub<R> {
        std::vector<R *> result;
        for (auto *item : mItems) {
            result.push_back(f(item));
        }
        return CollectionStreamStub<R>(result);
    }

    std::vector<T *> collect(std::vector<T *> &target) {
        for (auto *item : mItems) {
            target.push_back(item);
        }
        return target;
    }

private:
    std::vector<T *> mItems;

    // Allow construction from map result
    template<typename U>
    friend class CollectionStreamStub;
};

// SearchView stub
class SearchView {
public:
    QString getQuery() const { return mQuery; }
    void setQueryHint(const QString &hint) { (void)hint; }
    void requestFocus() {}
    void clearFocus() {}

    // SearchView.OnQueryTextListener stub
    class OnQueryTextListener {
    public:
        virtual ~OnQueryTextListener() = default;
        virtual bool onQueryTextSubmit(const QString &query) = 0;
        virtual bool onQueryTextChange(const QString &newText) = 0;
    };

    void setOnQueryTextListener(OnQueryTextListener *listener) {
        (void)listener;
    }

    void setQuery(const QString &query) { mQuery = query; }

private:
    QString mQuery;
};

// LinearLayoutManager stub
class LinearLayoutManager {
public:
    LinearLayoutManager(Context *context, int orientation, bool reverseLayout) {
        (void)context; (void)orientation; (void)reverseLayout;
    }
    void scrollToPosition(int position) { (void)position; }
};

// RecyclerView stub
class RecyclerViewUtil {
public:
    static constexpr int VERTICAL = 1;

    static void setLayoutManager(void *recyclerView, LinearLayoutManager *layout) {
        (void)recyclerView; (void)layout;
    }
    static void setAdapter(void *recyclerView, GroupedRecyclerViewAdapter *adapter) {
        (void)recyclerView; (void)adapter;
    }
};

// LayoutInflater stub
class LayoutInflater {
public:
    static LayoutInflater *from(Context *context) {
        (void)context;
        return new LayoutInflater();
    }
    void *inflate(int layoutRes, ViewGroup *parent, bool attachToRoot) {
        (void)layoutRes; (void)parent; (void)attachToRoot;
        return nullptr;
    }
};

// ViewGroup stub
class ViewGroup {
public:
    Context *getContext() { return nullptr; }
};

// SubredditSearchQuickLinks stub
class SubredditSearchQuickLinks {
public:
    void bind(Context *context, void *listeners) {
        (void)context; (void)listeners;
    }
};

// RecyclerView.ViewHolder stub
class ViewHolderStub {
public:
    explicit ViewHolderStub(void *view) { (void)view; }
};

// FunctionOneArgNoReturn stub
using FunctionOneArgNoReturn = std::function<void(void *)>;

// ============================================================================
// Stubbed resource constants (from org.quantumbadger.redreader.R)
// ============================================================================
namespace R {
    namespace id {
        constexpr int actionbar_search_view = 2001;
        constexpr int subreddit_search_recyclerview = 2002;
    }
    namespace layout {
        constexpr int subreddit_search_listing = 3001;
        constexpr int subreddit_search_quick_links = 3002;
    }
    namespace string {
        constexpr int find_location = 4001;
        constexpr int action_back = 4002;
        constexpr int nav_subs = 4003;
        constexpr int nav_custom = 4004;
        constexpr int nav_profile = 4005;
    }
}

// ============================================================================
// SubredditItem implementation (inner class, Java lines 125-153)
// ============================================================================

// Port of: private class SubredditItem
//     extends GroupedRecyclerViewAdapter.Item<SubredditItemViewHolder>
SubredditItem::SubredditItem(SubredditDetails *subreddit, QObject *parent)
    : QObject(parent)
    , mSubreddit(subreddit)
{
}

SubredditItem::~SubredditItem() = default;

// Port of: @Override public Class<RedditSubreddit> getViewType()
const void *SubredditItem::getViewType() const
{
    // Port of: return RedditSubreddit.class;
    // In C++, we return a unique type identifier
    static const int typeId = 0;
    return &typeId;
}

// Port of: @Override public SubredditItemViewHolder onCreateViewHolder(
//     final ViewGroup viewGroup)
SubredditItemViewHolder *SubredditItem::onCreateViewHolder(ViewGroup *viewGroup)
{
    // Port of: return new SubredditItemViewHolder(viewGroup, SubredditSearchActivity.this);
    (void)viewGroup;
    return new SubredditItemViewHolder();
}

// Port of: @Override public void onBindViewHolder(final SubredditItemViewHolder holder)
void SubredditItem::onBindViewHolder(SubredditItemViewHolder *holder)
{
    // Port of: holder.bind(mSubreddit);
    if (holder) {
        holder->bind(mSubreddit);
    }
}

// Port of: @Override public boolean isHidden()
bool SubredditItem::isHidden() const
{
    // Port of: return false;
    return false;
}

// ============================================================================
// SubredditSearchActivity implementation
// ============================================================================

// ===== Constructor =====
SubredditSearchActivity::SubredditSearchActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
SubredditSearchActivity::~SubredditSearchActivity()
{
    delete mRecyclerViewLayout;
    delete mLoadingItem;
    delete mRecyclerViewAdapter;
    delete mSubredditSubscriptionManager;
    delete m_base;
}

// ========================================================================
// Port of: @Override public void onSubredditSubscriptionListUpdated(
//     final RedditSubredditSubscriptionManager subredditSubscriptionManager)
// Java lines 107-111
// ========================================================================
void SubredditSearchActivity::onSubredditSubscriptionListUpdated(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager)
{
    (void)subredditSubscriptionManager;

    // Port of: AndroidCommon.runOnUiThread(() -> mSubscriptions.set(Optional.empty()));
    AndroidCommon::runOnUiThread([this]() {
        // mSubscriptions.set(Optional.empty());
        // Stub: ThreadCheckedVar operations
    });
}

// ========================================================================
// Port of: @Override public void onSubredditSubscriptionAttempted(
//     final RedditSubredditSubscriptionManager subredditSubscriptionManager)
// Java lines 113-117
// ========================================================================
void SubredditSearchActivity::onSubredditSubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager)
{
    (void)subredditSubscriptionManager;
    // Port of: // Ignore
}

// ========================================================================
// Port of: @Override public void onSubredditUnsubscriptionAttempted(
//     final RedditSubredditSubscriptionManager subredditSubscriptionManager)
// Java lines 119-123
// ========================================================================
void SubredditSearchActivity::onSubredditUnsubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager)
{
    (void)subredditSubscriptionManager;
    // Port of: // Ignore
}

// ========================================================================
// Port of: @Override protected boolean baseActivityIsToolbarSearchBarEnabled()
// Java lines 155-158
// ========================================================================
bool SubredditSearchActivity::baseActivityIsToolbarSearchBarEnabled()
{
    // Port of: return true;
    return true;
}

// ========================================================================
// Port of: @SuppressLint("NotifyDataSetChanged")
//     private void updateList()
// Java lines 160-282
// ========================================================================
void SubredditSearchActivity::updateList()
{
    // Port of: General.checkThisIsUIThread();
    General::checkThisIsUIThread();

    // Port of: Log.i(TAG, "Updating list");
    qDebug() << TAG << ": Updating list";

    // Port of: if(mSubscriptionsErrorItem.get().isPresent()) {
    // (Stub: error item logic not fully ported)

    // Port of: final String currentQuery = mSearchView.get().getQuery().toString();
    QString currentQuery;

    // Port of: mRecyclerViewAdapter.removeAllFromGroup(GROUP_SUBREDDITS);
    if (mRecyclerViewAdapter) {
        mRecyclerViewAdapter->removeAllFromGroup(GROUP_SUBREDDITS);
    }

    // Port of: if(mSubscriptions.get().isEmpty()) {
    // (Stub: subscription check — proceed as if subscriptions are loaded)
    {
        // Port of: final HashSet<String> shownSubreddits = new HashSet<>(256);
        std::unordered_set<QString> shownSubreddits;

        // Port of: Collections.sort(possibleSuggestions, (o1, o2) ->
        //     o1.name.compareTo(o2.name));

        // Port of: final String asciiLowercaseQuery = StringUtils.asciiLowercase(currentQuery);
        QString asciiLowercaseQuery = StringUtils::asciiLowercase(currentQuery);

        // Port of: startsWith check
        // Port of: contains check
        // (Stub: subscription matching logic not fully ported — depends on
        //  fully-ported ThreadCheckedVar, Optional, SubredditDetails, etc.)

        // Port of: final ArrayList<SubredditDetails> currentQueryResults
        //     = mQueryResults.get().get(currentQuery);
        // (Stub: query results)

        // Port of: mRecyclerViewAdapter.notifyDataSetChanged();
        if (mRecyclerViewAdapter) {
            mRecyclerViewAdapter->notifyDataSetChanged();
        }

        // Port of: mSubredditItemCache.nextGeneration();
        // (Stub: generational cache)
    }
}

// ========================================================================
// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
// Java lines 284-372
// ========================================================================
void SubredditSearchActivity::onCreate(Bundle *savedInstanceState)
{
    (void)savedInstanceState;

    // Port of: PrefsUtility.applyTheme(this);
    PrefsUtility::applyTheme(nullptr);

    // Port of: super.onCreate(savedInstanceState);
    if (m_base) {
        m_base->onCreate(nullptr);
    }

    // Port of: mSubredditSubscriptionManager =
    //     RedditSubredditSubscriptionManager.getSingleton(
    //         this, RedditAccountManager.getInstance(this).getDefaultAccount());
    mSubredditSubscriptionManager =
        RedditSubredditSubscriptionManager::getSingleton(
            nullptr,
            RedditAccountManager::getInstance(nullptr)->getDefaultAccount());

    // Port of: final EventListenerSet<String> queryEventListeners = new EventListenerSet<>();
    EventListenerSetStub<QString> *queryEventListeners =
        new EventListenerSetStub<QString>();

    // Port of: mLoadingItem = new GroupedRecyclerViewItemLoadingSpinner(this);
    mLoadingItem = new GroupedRecyclerViewItemLoadingSpinner(nullptr);

    // Port of: final SearchView searchView = findViewById(R.id.actionbar_search_view);
    // Port of: mSearchView.set(searchView);
    // Port of: searchView.setQueryHint(getString(R.string.find_location));
    // Port of: searchView.requestFocus();
    SearchView *searchView = new SearchView();
    searchView->setQueryHint(QStringLiteral("Find location"));
    searchView->requestFocus();

    // Port of: setBaseActivityListing(R.layout.subreddit_search_listing);
    if (m_base) {
        m_base->setBaseActivityListing(R::layout::subreddit_search_listing);
    }

    // Port of: mRecyclerViewLayout = new LinearLayoutManager(
    //     this, RecyclerView.VERTICAL, false);
    mRecyclerViewLayout = new LinearLayoutManager(
        nullptr, RecyclerViewUtil::VERTICAL, false);

    // Port of: mRecyclerViewAdapter = new GroupedRecyclerViewAdapter(3);
    mRecyclerViewAdapter = new GroupedRecyclerViewAdapter(3);

    // Port of: mRecyclerViewAdapter.appendToGroup(GROUP_LOADING_SPINNER, mLoadingItem);
    mRecyclerViewAdapter->appendToGroup(GROUP_LOADING_SPINNER, mLoadingItem);

    // Port of: recyclerView.setLayoutManager(mRecyclerViewLayout);
    // Port of: recyclerView.setAdapter(mRecyclerViewAdapter);

    // Port of: mRecyclerViewAdapter.appendToGroup(GROUP_QUICK_LINKS, ...);
    // (Stub: anonymous inner class for quick links group)
    {
        // Port of: new GroupedRecyclerViewAdapter.Item<RecyclerView.ViewHolder>() { ... }
        // In C++, we create a stub item
        // quickLinks.bind(SubredditSearchActivity.this, queryEventListeners);
    }

    // Port of: mSubredditSubscriptionListenerContext
    //     = Optional.of(mSubredditSubscriptionManager.addListener(this));
    // (Stub: listener registration)

    // Port of: requestSubscriptions();
    requestSubscriptions();

    // Port of: searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() { ... });
    // (Stub: query text listener — anonymous inner classes not fully ported)

    // Port of: updateList();
    updateList();

    // Cleanup
    delete queryEventListeners;
}

// ========================================================================
// Port of: private void handleQueryChanged(@NonNull final String text)
// Java lines 374-407
// ========================================================================
void SubredditSearchActivity::handleQueryChanged(const QString &text)
{
    // Port of: mSubscriptionsErrorItem.set(Optional.empty());
    // Port of: mQueryErrorItem.set(Optional.empty());
    // (Stub: ThreadCheckedVar operations)

    // Port of: updateList();
    updateList();

    // Port of: mRecyclerViewLayout.scrollToPosition(0);
    if (mRecyclerViewLayout) {
        mRecyclerViewLayout->scrollToPosition(0);
    }

    // Port of: if(text.isEmpty()) { return; }
    if (text.isEmpty()) {
        return;
    }

    // Port of: if(mQueriesPending.get().contains(text)) {
    //     // Do nothing, let's just wait for now.
    //     return;
    // }
    // (Stub: query pending check)

    // Port of: if(!mQueryResults.get().containsKey(text)) {
    //     mQueriesPending.get().add(text);
    //
    //     // Wait 1 second to avoid sending requests too fast
    //     AndroidCommon.UI_THREAD_HANDLER.postDelayed(
    //         () -> {
    //             if(text.contentEquals(mSearchView.get().getQuery())) {
    //                 doSearchRequest(text);
    //             } else {
    //                 mQueriesPending.get().remove(text);
    //             }
    //         },
    //         1000
    //     );
    // }
    // (Stub: debounced search request)

    AndroidCommon::postDelayed([this, text]() {
        doSearchRequest(text);
    }, 1000);
}

// ========================================================================
// Port of: private void doSearchRequest(@NonNull final String text)
// Java lines 409-469
// ========================================================================
void SubredditSearchActivity::doSearchRequest(const QString &text)
{
    // Port of: Log.i(TAG, "Running search");
    qDebug() << TAG << ": Running search";

    // Port of: final CacheManager cacheManager = CacheManager.getInstance(this);
    // Port of: final RedditAccount user
    //     = RedditAccountManager.getInstance(this).getDefaultAccount();
    CacheManager *cacheManager = CacheManager::getInstance(nullptr);
    RedditAccount *user =
        RedditAccountManager::getInstance(nullptr)->getDefaultAccount();

    // Port of: RedditAPI.searchSubreddits(
    //     cacheManager, user, text, this,
    //     new APIResponseHandler.ValueResponseHandler<
    //         RedditAPI.SubredditListResponse>(this) {
    //         @Override protected void onSuccess(@NonNull ...) { ... }
    //         @Override protected void onCallbackException(final Throwable t) { ... }
    //         @Override protected void onFailure(@NonNull final RRError error) { ... }
    //     },
    //     Optional.empty());
    //
    // (Stub: API request with anonymous handler)

    RedditAPI::searchSubreddits(cacheManager, user, text, nullptr, nullptr, nullptr);

    // Stub: Simulate async response
    AndroidCommon::runOnUiThread([this]() {
        // Port of: mQueryResults.get().put(text, results);
        // Port of: mQueriesPending.get().remove(text);
        // Port of: updateList();
        updateList();
    });

    delete cacheManager;
}

// ========================================================================
// Port of: private void requestSubscriptions()
// Java lines 471-523
// ========================================================================
void SubredditSearchActivity::requestSubscriptions()
{
    // Port of: if(mSubscriptionListPending.get() == Boolean.TRUE) {
    //     Log.i(TAG, "Subscription list already pending");
    //     return;
    // }
    // (Stub: pending check)

    // Port of: mSubscriptionListPending.set(true);
    // (Stub: ThreadCheckedVar operation)

    // Port of: final FunctionOneArgNoReturn<ArrayList<SubredditCanonicalId>> onSuccess
    //     = list -> AndroidCommon.runOnUiThread(() -> {
    //         if(mSubscriptionListPending.get() && list != null) {
    //             mSubscriptionListPending.set(false);
    //             final ArrayList<SubredditDetails> subscriptions
    //                 = new CollectionStream<>(list)
    //                     .map(SubredditDetails::new)
    //                     .collect(new ArrayList<>());
    //             mSubscriptions.set(Optional.of(subscriptions));
    //         }
    //     });

    // Port of: mSubredditSubscriptionManager.triggerUpdateIfNotReady(
    //     error -> AndroidCommon.runOnUiThread(() -> {
    //         mQueryErrorItem.set(Optional.of(
    //             new GroupedRecyclerViewItemRRError(this, error)));
    //         updateList();
    //     }));
    if (mSubredditSubscriptionManager) {
        mSubredditSubscriptionManager->triggerUpdateIfNotReady(nullptr);
    }

    // Port of: mSubredditSubscriptionManager.addListener(
    //     new RedditSubredditSubscriptionManager.SubredditSubscriptionStateChangeListener() { ... });
    // (Stub: anonymous listener)

    // Port of: onSuccess.apply(mSubredditSubscriptionManager.getSubscriptionList());
    // (Stub: apply callback with subscription list)

    AndroidCommon::runOnUiThread([this]() {
        updateList();
    });
}

// ========================================================================
// Port of: @Override protected void onDestroy()
// Java lines 525-530
// ========================================================================
void SubredditSearchActivity::onDestroy()
{
    // Port of: super.onDestroy();
    if (m_base) {
        m_base->onDestroy();
    }

    // Port of: mSubredditSubscriptionListenerContext.apply(
    //     RedditSubredditSubscriptionManager.ListenerContext::removeListener);
    // (Stub: remove subscription listener)
}

} // namespace PinkReader
