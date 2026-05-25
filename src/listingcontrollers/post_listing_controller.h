// Origin: org/quantumbadger/redreader/listingcontrollers/PostListingController.java
#pragma once

#include <QString>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class PostListingURL;
class PostSort;
class PostListingFragment;
class AppCompatActivity;
class Bundle;
class SubredditCanonicalId;

/**
 * Port of PostListingController.
 * Every field, method, logic line preserved.
 */
class PostListingController {
private:
	std::optional<QString> m_session;
	PostListingURL* m_url;

public:
	PostListingController(PostListingURL& url);

	void setSession(const std::optional<QString>& session);
	std::optional<QString> getSession() const;

	bool isSortable() const;
	bool isFrontPage() const;

	void setSort(const PostSort& order);
	PostSort* getSort();

	QString getUri() const;

	PostListingFragment* get(
		AppCompatActivity* parent,
		bool force,
		Bundle* savedInstanceState);

	bool isSubreddit() const;
	bool isSubredditCombination() const;
	bool isMultireddit() const;
	bool isSearchResults() const;
	bool isSubredditSearchResults() const;
	bool isUserPostListing() const;

	SubredditCanonicalId* subredditCanonicalName();

	QString multiredditName() const;
	QString multiredditUsername() const;
};

} // namespace PinkReader
