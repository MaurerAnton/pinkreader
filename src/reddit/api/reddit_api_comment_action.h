// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditAPICommentAction.java
#pragma once

#include <string>
#include <vector>
#include <memory>

namespace PinkReader {

// Forward declarations
class RedditRenderableComment;
class RedditCommentView;
class RedditChangeDataManager;
class RedditComment;

class RedditAPICommentAction {
public:
	enum class RedditCommentAction {
		UPVOTE,
		UNVOTE,
		DOWNVOTE,
		SAVE,
		UNSAVE,
		REPORT,
		SHARE,
		COPY_TEXT,
		COPY_URL,
		REPLY,
		USER_PROFILE,
		COMMENT_LINKS,
		COLLAPSE,
		EDIT,
		DELETE,
		EXTERNAL,
		PROPERTIES,
		CONTEXT,
		GO_TO_COMMENT,
		ACTION_MENU,
		BACK
	};

	static void showActionMenu(
		void* activity,
		void* commentListingFragment,
		const RedditRenderableComment& comment,
		void* commentView,
		RedditChangeDataManager& changeDataManager,
		bool isPostLocked);

	static void onActionMenuItemSelected(
		const RedditRenderableComment& renderableComment,
		void* commentView,
		void* activity,
		void* commentListingFragment,
		RedditCommentAction action,
		RedditChangeDataManager& changeDataManager);

	static void action(
		void* activity,
		const RedditComment& comment,
		int apiAction,
		RedditChangeDataManager& changeDataManager);

private:
	struct RCVMenuItem {
		std::string title;
		RedditCommentAction action;

		RCVMenuItem(std::string title_, RedditCommentAction action_)
			: title(std::move(title_)), action(action_) {}
	};
};

} // namespace PinkReader
