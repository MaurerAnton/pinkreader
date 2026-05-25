// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditPostActions.kt
// NOTE: Original is Kotlin (`object RedditPostActions`), ported as C++ namespace/class
#pragma once

#include <string>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class RedditPreparedPost;
class UriString;

// PostFlingAction enum (originally PrefsUtility.PostFlingAction)
enum class PostFlingAction {
	UPVOTE,
	DOWNVOTE,
	SAVE,
	HIDE,
	MARK_READ,
	COMMENTS,
	LINK,
	BROWSER,
	REPORT,
	SAVE_IMAGE,
	GOTO_SUBREDDIT,
	SHARE,
	SHARE_COMMENTS,
	SHARE_IMAGE,
	COPY,
	USER_PROFILE,
	PROPERTIES,
	ACTION_MENU,
	BACK,
	DISABLED
};

class RedditPostActions {
public:
	enum class Action {
		UPVOTE,
		UNVOTE,
		DOWNVOTE,
		SAVE,
		HIDE,
		UNSAVE,
		UNHIDE,
		MARK_READ,
		MARK_UNREAD,
		EDIT,
		DELETE,
		REPORT,
		SHARE,
		REPLY,
		USER_PROFILE,
		EXTERNAL,
		PROPERTIES,
		COMMENTS,
		LINK,
		COMMENTS_SWITCH,
		LINK_SWITCH,
		SHARE_COMMENTS,
		SHARE_IMAGE,
		GOTO_SUBREDDIT,
		ACTION_MENU,
		SAVE_IMAGE,
		COPY,
		COPY_SELFTEXT,
		SELFTEXT_LINKS,
		BACK,
		BLOCK,
		UNBLOCK,
		PIN,
		UNPIN,
		SUBSCRIBE,
		UNSUBSCRIBE,
		CROSSPOST_ORIGIN
	};

	struct ActionDescriptionPair {
		Action action;
		int descriptionRes; // String resource ID

		ActionDescriptionPair(Action a, int res) : action(a), descriptionRes(res) {}

		static std::optional<ActionDescriptionPair> from(
			const RedditPreparedPost& post, PostFlingAction pref);
	};

	static RedditPostActions& instance();

	void setupAccessibilityActions(
		void* accessibilityActionManager,
		const RedditPreparedPost& post,
		void* activity,
		bool isOpen);

	void onActionMenuItemSelected(
		const RedditPreparedPost& post,
		void* activity,
		Action action);

	void action(const RedditPreparedPost& post, void* activity, int apiAction);

private:
	RedditPostActions() = default;

	struct RPVMenuItem {
		std::string title;
		Action action;

		RPVMenuItem(std::string t, Action a) : title(std::move(t)), action(a) {}
	};
};

} // namespace PinkReader
