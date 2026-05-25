// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/RedditListingManager.java

#pragma once

#include <QObject>
#include <QVector>
#include <memory>

namespace PinkReader {

// Forward declarations
class GroupedRecyclerViewAdapter;
class GroupedRecyclerViewItemFrameLayout;
class LinearLayoutManager;
class LoadingSpinnerView;
class RedditPostHeaderView;
class ErrorView;

/**
 * @brief Port of org.quantumbadger.redreader.adapters.RedditListingManager
 *
 * Abstract base class managing a GroupedRecyclerViewAdapter with
 * header, notification, self-text, items, load-more, loading, and footer error groups.
 *
 * Every field, method, constant, and logic branch from the original Java file.
 */
class RedditListingManager : public QObject {
	Q_OBJECT

protected:
	// Port of: private final GroupedRecyclerViewAdapter mAdapter = new GroupedRecyclerViewAdapter(7);
	GroupedRecyclerViewAdapter* mAdapter = nullptr;
	// Port of: private LinearLayoutManager mLayoutManager;
	LinearLayoutManager* mLayoutManager = nullptr;

public:
	// Port of: private static final int GROUP_HEADER = 0;
	static constexpr int GROUP_HEADER = 0;
	// Port of: private static final int GROUP_NOTIFICATIONS = 1;
	static constexpr int GROUP_NOTIFICATIONS = 1;
	// Port of: private static final int GROUP_POST_SELFTEXT = 2;
	static constexpr int GROUP_POST_SELFTEXT = 2;
	// Port of: private static final int GROUP_ITEMS = 3;
	static constexpr int GROUP_ITEMS = 3;
	// Port of: private static final int GROUP_LOAD_MORE_BUTTON = 4;
	static constexpr int GROUP_LOAD_MORE_BUTTON = 4;
	// Port of: private static final int GROUP_LOADING = 5;
	static constexpr int GROUP_LOADING = 5;
	// Port of: private static final int GROUP_FOOTER_ERRORS = 6;
	static constexpr int GROUP_FOOTER_ERRORS = 6;

protected:
	// Port of: private final GroupedRecyclerViewItemFrameLayout mLoadingItem;
	GroupedRecyclerViewItemFrameLayout* mLoadingItem = nullptr;
	// Port of: private boolean mWorkaroundDone = false;
	bool mWorkaroundDone = false;

public:
	// Port of: protected RedditListingManager(final Context context)
	//   General.checkThisIsUIThread();
	//   final LoadingSpinnerView loadingSpinnerView = new LoadingSpinnerView(context);
	//   final int paddingPx = General.dpToPixels(context, 30);
	//   loadingSpinnerView.setPadding(paddingPx, paddingPx, paddingPx, paddingPx);
	//   mLoadingItem = new GroupedRecyclerViewItemFrameLayout(loadingSpinnerView);
	//   mAdapter.appendToGroup(GROUP_LOADING, mLoadingItem);
	explicit RedditListingManager(QObject* parent = nullptr);

	virtual ~RedditListingManager() = default;

	// Port of: public void setLayoutManager(final LinearLayoutManager layoutManager)
	//   General.checkThisIsUIThread();
	//   mLayoutManager = layoutManager;
	virtual void setLayoutManager(LinearLayoutManager* layoutManager);

	// Port of: public void addFooterError(final ErrorView view)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_FOOTER_ERRORS, new GroupedRecyclerViewItemFrameLayout(view));
	virtual void addFooterError(ErrorView* view);

	// Port of: public void addPostHeader(final RedditPostHeaderView view)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_HEADER, new GroupedRecyclerViewItemFrameLayout(view));
	//   doWorkaround();
	virtual void addPostHeader(RedditPostHeaderView* view);

	// Port of: public void addPostListingHeader(final View view)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_HEADER, new GroupedRecyclerViewItemFrameLayout(view));
	//   doWorkaround();
	virtual void addPostListingHeader(QObject* view);

	// Port of: public void addPostSelfText(final View view)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_POST_SELFTEXT, new GroupedRecyclerViewItemFrameLayout(view));
	//   doWorkaround();
	virtual void addPostSelfText(QObject* view);

	// Port of: public void addNotification(final View view)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_NOTIFICATIONS, new GroupedRecyclerViewItemFrameLayout(view));
	//   doWorkaround();
	virtual void addNotification(QObject* view);

	// Port of: public void addItems(final Collection<GroupedRecyclerViewAdapter.Item<?>> items)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_ITEMS, items);
	//   doWorkaround();
	virtual void addItems(const QVector<GroupedRecyclerViewAdapter::Item*>& items);

	// Port of: public void addViewToItems(final View view)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_ITEMS, new GroupedRecyclerViewItemFrameLayout(view));
	//   doWorkaround();
	virtual void addViewToItems(QObject* view);

	// Port of: public void addLoadMoreButton(final View view)
	//   General.checkThisIsUIThread();
	//   mAdapter.appendToGroup(GROUP_LOAD_MORE_BUTTON, new GroupedRecyclerViewItemFrameLayout(view));
	//   doWorkaround();
	virtual void addLoadMoreButton(QObject* view);

	// Port of: public void removeLoadMoreButton()
	//   General.checkThisIsUIThread();
	//   mAdapter.removeAllFromGroup(GROUP_LOAD_MORE_BUTTON);
	virtual void removeLoadMoreButton();

	// Port of: public void setLoadingVisible(final boolean visible)
	//   General.checkThisIsUIThread();
	//   mLoadingItem.setHidden(!visible);
	//   mAdapter.updateHiddenStatus();
	virtual void setLoadingVisible(bool visible);

	// Port of: public GroupedRecyclerViewAdapter getAdapter()
	//   General.checkThisIsUIThread();
	//   return mAdapter;
	virtual GroupedRecyclerViewAdapter* getAdapter();

	// Port of: public void updateHiddenStatus()
	//   General.checkThisIsUIThread();
	//   mAdapter.updateHiddenStatus();
	virtual void updateHiddenStatus();

	// Port of: public GroupedRecyclerViewAdapter.Item getItemAtPosition(final int position)
	//   return mAdapter.getItemAtPosition(position);
	virtual GroupedRecyclerViewAdapter::Item* getItemAtPosition(int position);

protected:
	// Port of: private void doWorkaround()
	//   if(!mWorkaroundDone && mLayoutManager != null) {
	//     mLayoutManager.scrollToPositionWithOffset(0, 0);
	//     mWorkaroundDone = true;
	//   }
	virtual void doWorkaround();
};

} // namespace PinkReader
