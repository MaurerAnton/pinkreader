// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/MainMenuSelectionListener.java

#pragma once

namespace PinkReader {

// Forward declaration
class UriString;
class PostListingURL;

/**
 * @brief Port of org.quantumbadger.redreader.adapters.MainMenuSelectionListener
 *
 * Interface for handling main menu item selections.
 * Inherits MainMenuFragment.MainMenuAction int constants and PostListingURL overload.
 */
class MainMenuSelectionListener {
public:
	virtual ~MainMenuSelectionListener() = default;

	// Port of: @MainMenuFragment.MainMenuAction int type constants
	// MainMenuAction constants from MainMenuFragment:
	enum MainMenuAction {
		FRONTPAGE = 0,
		PROFILE = 1,
		SAVED = 2,
		UPVOTED = 3,
		DOWNVOTED = 4,
		HIDDEN = 5,
		SUBMITTED = 6,
		MODMAIL = 7,
		ALL = 8,
		POPULAR = 9,
		RANDOM = 10,
		RANDOM_NSFW = 11,
		CUSTOM = 12,
		SUBREDDIT = 13,
		MULTIREDDIT = 14,
		DOMAIN = 15,
		SEARCH = 16,
		INBOX = 17,
		SETTINGS = 18,
		CHANGELOG = 19
	};

	// Port of: void onSelected(@MainMenuFragment.MainMenuAction int type);
	virtual void onSelected(MainMenuAction type) = 0;

	// Port of: void onSelected(PostListingURL url);
	virtual void onSelected(const PostListingURL& url) = 0;
};

} // namespace PinkReader
