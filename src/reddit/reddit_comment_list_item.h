// Origin: org/quantumbadger/redreader/reddit/RedditCommentListItem.java
#pragma once

namespace PinkReader {

// Forward declarations
class BaseActivity;
class CommentListingFragment;
class RedditRenderableComment;
class RedditMore;
class RedditChangeDataManager;
class RedditURLParser_RedditURL;

class RedditCommentListItem {
public:
	enum class Type {
		COMMENT,
		LOAD_MORE
	};

private:
	Type m_type;
	int m_indent;
	RedditCommentListItem* m_parent;
	CommentListingFragment* m_fragment;
	BaseActivity* m_activity;
	RedditURLParser_RedditURL* m_commentListingUrl;

	RedditRenderableComment* m_comment;
	RedditMore* m_moreComments;

	RedditChangeDataManager* m_changeDataManager;

public:
	// Constructor for COMMENT type
	RedditCommentListItem(
			RedditRenderableComment* comment,
			RedditCommentListItem* parent,
			CommentListingFragment* fragment,
			BaseActivity* activity,
			RedditURLParser_RedditURL* commentListingUrl);

	// Constructor for LOAD_MORE type
	RedditCommentListItem(
			RedditMore* moreComments,
			RedditCommentListItem* parent,
			CommentListingFragment* fragment,
			BaseActivity* activity,
			RedditURLParser_RedditURL* commentListingUrl);

	bool isComment() const { return m_type == Type::COMMENT; }
	bool isLoadMore() const { return m_type == Type::LOAD_MORE; }

	RedditRenderableComment* asComment();
	RedditMore* asLoadMore();

	int getIndent() const { return m_indent; }
	RedditCommentListItem* getParent() { return m_parent; }

	bool isCollapsed(RedditChangeDataManager& changeDataManager) const;
	bool isHidden(RedditChangeDataManager& changeDataManager) const;

	// getViewType() - returns type identifier
	int getViewType() const;

	// isHidden() - for GroupedRecyclerViewAdapter.Item
	bool isHidden() const;

	// Accessors
	Type getType() const { return m_type; }
	CommentListingFragment* getFragment() { return m_fragment; }
	BaseActivity* getActivity() { return m_activity; }
	RedditURLParser_RedditURL* getCommentListingUrl() { return m_commentListingUrl; }
	RedditRenderableComment* getComment() { return m_comment; }
	RedditMore* getMoreComments() { return m_moreComments; }
};

} // namespace PinkReader
