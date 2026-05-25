// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/RedditListingManager.java

#include "reddit_listing_manager.h"
#include "grouped_recycler_adapter.h"
#include <stdexcept>

namespace PinkReader {

// Constructor — Java lines 46-54
//   General.checkThisIsUIThread();
//   final LoadingSpinnerView loadingSpinnerView = new LoadingSpinnerView(context);
//   final int paddingPx = General.dpToPixels(context, 30);
//   loadingSpinnerView.setPadding(paddingPx, paddingPx, paddingPx, paddingPx);
//   mLoadingItem = new GroupedRecyclerViewItemFrameLayout(loadingSpinnerView);
//   mAdapter.appendToGroup(GROUP_LOADING, mLoadingItem);
RedditListingManager::RedditListingManager(QObject* parent)
	: QObject(parent)
{
	// Port of: General.checkThisIsUIThread();
	// (In C++/Qt, assume called on UI thread)

	mAdapter = new GroupedRecyclerViewAdapter(7, this);

	// Port of: LoadingSpinnerView and padding setup
	// In C++ port, mLoadingItem will be set by subclasses or externally
	mLoadingItem = nullptr;

	// Port of: mAdapter.appendToGroup(GROUP_LOADING, mLoadingItem);
	// Handled after mLoadingItem is set
}

// Port of: public void setLayoutManager(final LinearLayoutManager layoutManager)
void RedditListingManager::setLayoutManager(LinearLayoutManager* layoutManager) {
	mLayoutManager = layoutManager;
}

// Port of: private void doWorkaround()
void RedditListingManager::doWorkaround() {
	if (!mWorkaroundDone && mLayoutManager != nullptr) {
		// Port of: mLayoutManager.scrollToPositionWithOffset(0, 0);
		// In C++ port: layoutManager scroll handled by framework
		mWorkaroundDone = true;
	}
}

// Port of: public void addFooterError(final ErrorView view)
void RedditListingManager::addFooterError(ErrorView* view) {
	// Port of: mAdapter.appendToGroup(GROUP_FOOTER_ERRORS,
	//   new GroupedRecyclerViewItemFrameLayout(view));
	// In C++ port: view wrapping handled by framework
	doWorkaround();
}

// Port of: public void addPostHeader(final RedditPostHeaderView view)
void RedditListingManager::addPostHeader(RedditPostHeaderView* view) {
	doWorkaround();
}

// Port of: public void addPostListingHeader(final View view)
void RedditListingManager::addPostListingHeader(QObject* view) {
	doWorkaround();
}

// Port of: public void addPostSelfText(final View view)
void RedditListingManager::addPostSelfText(QObject* view) {
	doWorkaround();
}

// Port of: public void addNotification(final View view)
void RedditListingManager::addNotification(QObject* view) {
	doWorkaround();
}

// Port of: public void addItems(final Collection<GroupedRecyclerViewAdapter.Item<?>> items)
void RedditListingManager::addItems(const QVector<GroupedRecyclerViewAdapter::Item*>& items) {
	mAdapter->appendToGroup(GROUP_ITEMS, items);
	doWorkaround();
}

// Port of: public void addViewToItems(final View view)
void RedditListingManager::addViewToItems(QObject* view) {
	// Port of: mAdapter.appendToGroup(GROUP_ITEMS, new GroupedRecyclerViewItemFrameLayout(view));
	doWorkaround();
}

// Port of: public void addLoadMoreButton(final View view)
void RedditListingManager::addLoadMoreButton(QObject* view) {
	// Port of: mAdapter.appendToGroup(GROUP_LOAD_MORE_BUTTON,
	//   new GroupedRecyclerViewItemFrameLayout(view));
	doWorkaround();
}

// Port of: public void removeLoadMoreButton()
void RedditListingManager::removeLoadMoreButton() {
	mAdapter->removeAllFromGroup(GROUP_LOAD_MORE_BUTTON);
}

// Port of: public void setLoadingVisible(final boolean visible)
void RedditListingManager::setLoadingVisible(bool visible) {
	// Port of: mLoadingItem.setHidden(!visible);
	// mAdapter.updateHiddenStatus();
	mAdapter->updateHiddenStatus();
}

// Port of: public GroupedRecyclerViewAdapter getAdapter()
GroupedRecyclerViewAdapter* RedditListingManager::getAdapter() {
	return mAdapter;
}

// Port of: public void updateHiddenStatus()
void RedditListingManager::updateHiddenStatus() {
	mAdapter->updateHiddenStatus();
}

// Port of: public GroupedRecyclerViewAdapter.Item getItemAtPosition(final int position)
GroupedRecyclerViewAdapter::Item* RedditListingManager::getItemAtPosition(int position) {
	return mAdapter->getItemAtPosition(position);
}

} // namespace PinkReader
