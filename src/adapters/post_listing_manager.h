// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/PostListingManager.java

#pragma once

#include "reddit_listing_manager.h"
#include <vector>

namespace PinkReader {

// Forward declaration
class RedditPostListItem;

/**
 * @brief Port of org.quantumbadger.redreader.adapters.PostListingManager
 *
 * Extends RedditListingManager. Tracks post count and manages post list items.
 * Every field and method from the original Java file.
 */
class PostListingManager : public RedditListingManager {
	Q_OBJECT

protected:
	// Port of: private int mPostCount;
	int mPostCount = 0;

public:
	// Port of: public PostListingManager(final Context context) { super(context); }
	explicit PostListingManager(QObject* parent = nullptr);

	// Port of: public void addPosts(final Collection<RedditPostListItem> posts)
	//   addItems(Collections.unmodifiableCollection(posts));
	//   mPostCount += posts.size();
	void addPosts(const std::vector<RedditPostListItem*>& posts);

	// Port of: public int getPostCount() { return mPostCount; }
	int getPostCount() const;
};

} // namespace PinkReader
