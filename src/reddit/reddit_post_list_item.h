// Origin: org/quantumbadger/redreader/reddit/RedditPostListItem.java
#pragma once

namespace PinkReader {

// Forward declarations
class BaseActivity;
class PostListingFragment;
class RedditPreparedPost;
class RedditPostView;

// NOTE: In Java, RedditPostListItem extends GroupedRecyclerViewAdapter.Item<RecyclerView.ViewHolder>
// which is an Android RecyclerView abstraction. This is stubbed in C++.

class RedditPostListItem {
private:
	PostListingFragment* m_fragment;
	BaseActivity* m_activity;
	RedditPreparedPost* m_post;
	bool m_leftHandedMode;

public:
	RedditPostListItem(
			RedditPreparedPost* post,
			PostListingFragment* fragment,
			BaseActivity* activity,
			bool leftHandedMode);

	// getViewType() -> in Java returns Class<RedditPostView>
	// In C++ we can use a type identifier
	int getViewType() const;

	// onCreateViewHolder - Android-specific, stubbed
	// onBindViewHolder - Android-specific, stubbed

	bool isHidden() const;

	// Accessors
	RedditPreparedPost* getPost() { return m_post; }
	PostListingFragment* getFragment() { return m_fragment; }
	BaseActivity* getActivity() { return m_activity; }
	bool isLeftHandedMode() const { return m_leftHandedMode; }
};

} // namespace PinkReader
