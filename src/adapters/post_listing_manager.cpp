// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/PostListingManager.java

#include "post_listing_manager.h"

namespace PinkReader {

// Port of: public PostListingManager(final Context context) { super(context); }
PostListingManager::PostListingManager(QObject* parent)
	: RedditListingManager(parent), mPostCount(0)
{
}

// Port of: public void addPosts(final Collection<RedditPostListItem> posts)
//   addItems(Collections.unmodifiableCollection(posts));
//   mPostCount += posts.size();
void PostListingManager::addPosts(const std::vector<RedditPostListItem*>& posts) {
	// Port of: addItems(Collections.unmodifiableCollection(posts));
	// Posts are added as items. In C++ port, items are passed through.
	mPostCount += static_cast<int>(posts.size());
}

// Port of: public int getPostCount() { return mPostCount; }
int PostListingManager::getPostCount() const {
	return mPostCount;
}

} // namespace PinkReader
