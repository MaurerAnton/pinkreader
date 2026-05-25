// Origin: org/quantumbadger/redreader/reddit/RedditCommentListItem.java
#include "reddit_comment_list_item.h"
#include <stdexcept>

namespace PinkReader {

// Constructor for COMMENT type
RedditCommentListItem::RedditCommentListItem(
		RedditRenderableComment* comment,
		RedditCommentListItem* parent,
		CommentListingFragment* fragment,
		BaseActivity* activity,
		RedditURLParser_RedditURL* commentListingUrl)
	: m_type(Type::COMMENT)
	, m_indent(parent == nullptr ? 0 : parent->getIndent() + 1)
	, m_parent(parent)
	, m_fragment(fragment)
	, m_activity(activity)
	, m_commentListingUrl(commentListingUrl)
	, m_comment(comment)
	, m_moreComments(nullptr)
	, m_changeDataManager(nullptr)
{
	// m_changeDataManager = RedditChangeDataManager.getInstance(
	//     RedditAccountManager.getInstance(activity).getDefaultAccount());
}

// Constructor for LOAD_MORE type
RedditCommentListItem::RedditCommentListItem(
		RedditMore* moreComments,
		RedditCommentListItem* parent,
		CommentListingFragment* fragment,
		BaseActivity* activity,
		RedditURLParser_RedditURL* commentListingUrl)
	: m_type(Type::LOAD_MORE)
	, m_indent(parent == nullptr ? 0 : parent->getIndent() + 1)
	, m_parent(parent)
	, m_fragment(fragment)
	, m_activity(activity)
	, m_commentListingUrl(commentListingUrl)
	, m_comment(nullptr)
	, m_moreComments(moreComments)
	, m_changeDataManager(nullptr)
{
	// m_changeDataManager = RedditChangeDataManager.getInstance(
	//     RedditAccountManager.getInstance(activity).getDefaultAccount());
}

RedditRenderableComment* RedditCommentListItem::asComment() {
	if (!isComment()) {
		throw std::runtime_error("Called asComment() on non-comment item");
	}
	return m_comment;
}

RedditMore* RedditCommentListItem::asLoadMore() {
	if (!isLoadMore()) {
		throw std::runtime_error("Called asLoadMore() on non-load-more item");
	}
	return m_moreComments;
}

bool RedditCommentListItem::isCollapsed(RedditChangeDataManager& changeDataManager) const {
	if (!isComment()) {
		return false;
	}
	// return m_comment.isCollapsed(changeDataManager);
	(void)changeDataManager;
	return false;
}

bool RedditCommentListItem::isHidden(RedditChangeDataManager& changeDataManager) const {
	if (m_parent != nullptr) {
		return m_parent->isCollapsed(changeDataManager) || m_parent->isHidden(changeDataManager);
	}
	return false;
}

int RedditCommentListItem::getViewType() const {
	if (isComment()) {
		return 0; // RedditCommentView.class identifier
	}
	if (isLoadMore()) {
		return 1; // LoadMoreCommentsView.class identifier
	}
	throw std::runtime_error("Unknown item type");
}

bool RedditCommentListItem::isHidden() const {
	// return isHidden(m_changeDataManager);
	return false;
}

} // namespace PinkReader
