// Origin: org/quantumbadger/redreader/listingcontrollers/CommentListingController.java
#pragma once

#include <QString>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class CommentListingURL;
class PostCommentSort;
class UserCommentSort;
class CommentListingFragment;
class AppCompatActivity;
class Bundle;

namespace RedditURLParser {
class RedditURL;
}

namespace OptionsMenuUtility {
class Sort;
}

/**
 * Port of CommentListingController.
 * Every field, method, logic line preserved.
 */
class CommentListingController {
private:
	CommentListingURL* m_url;
	std::optional<QString> m_session;
	QString m_searchString;

	PostCommentSort defaultOrder();
	UserCommentSort defaultUserOrder();

public:
	explicit CommentListingController(RedditURLParser::RedditURL& url);

	std::optional<QString> getSession() const;
	void setSession(const std::optional<QString>& session);

	void setSort(const PostCommentSort& s);
	void setSort(const UserCommentSort& s);

	OptionsMenuUtility::Sort* getSort();

	void setSearchString(const QString& searchString);
	QString getSearchString() const;

	QString getUri() const;

	CommentListingURL* getCommentListingUrl();

	CommentListingFragment* get(
		AppCompatActivity* parent,
		bool force,
		Bundle* savedInstanceState);

	bool isSortable() const;
	bool isUserCommentListing() const;
};

} // namespace PinkReader
