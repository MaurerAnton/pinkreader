// Origin: org/quantumbadger/redreader/reddit/RedditPostListItem.java
#include "reddit_post_list_item.h"

namespace PinkReader {

RedditPostListItem::RedditPostListItem(
		RedditPreparedPost* post,
		PostListingFragment* fragment,
		BaseActivity* activity,
		bool leftHandedMode)
	: m_fragment(fragment)
	, m_activity(activity)
	, m_post(post)
	, m_leftHandedMode(leftHandedMode)
{
}

int RedditPostListItem::getViewType() const {
	// In Java: return RedditPostView.class;
	return 0; // View type identifier for RedditPostView
}

bool RedditPostListItem::isHidden() const {
	return false;
}

} // namespace PinkReader
