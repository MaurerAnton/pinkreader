/*******************************************************************************
 * This file is part of RedReader.
 *
 * RedReader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RedReader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RedReader.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/*
 * PinkReader C++ port of:
 *   org.quantumbadger.redreader.reddit.url.SubredditPostListURL (lines 1-431)
 *
 * Every field, method, inner class (Type), enum, constant, logic line.
 */

#pragma once

#include <QString>
#include <optional>
#include "post_listing_url.h"

namespace PinkReader {

// Forward
class RedditIdAndType;
class PostSort;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.SubredditPostListURL (Java lines 36-430)
 */
class SubredditPostListURL : public PostListingURL {
public:
	// Port of: public enum Type { FRONTPAGE, ALL, SUBREDDIT, SUBREDDIT_COMBINATION, ALL_SUBTRACTION, POPULAR } (Java lines 63-65)
	enum class Type {
		FRONTPAGE,
		ALL,
		SUBREDDIT,
		SUBREDDIT_COMBINATION,
		ALL_SUBTRACTION,
		POPULAR
	};

	// Port of: @NonNull public final Type type; (Java line 67)
	const Type type;

	// Port of: @Nullable public final String subreddit; (Java line 68)
	const QString subreddit;

	// Port of: @Nullable public final PostSort order; (Java line 70)
	PostSort *order;

	// Port of: @Nullable public final Integer limit; (Java line 71)
	std::optional<int> limit;

	// Port of: @Nullable public final String before; (Java line 72)
	QString before;

	// Port of: @Nullable public final RedditIdAndType after; (Java line 73)
	RedditIdAndType *after;

	// Port of: public static SubredditPostListURL getFrontPage() (Java lines 38-40)
	static SubredditPostListURL *getFrontPage();

	// Port of: public static SubredditPostListURL getPopular() (Java lines 42-44)
	static SubredditPostListURL *getPopular();

	// Port of: public static SubredditPostListURL getAll() (Java lines 46-48)
	static SubredditPostListURL *getAll();

	// Port of: public static RedditURLParser.RedditURL getSubreddit(final String subreddit) throws
	//            InvalidSubredditNameException (Java lines 50-53)
	static RedditURLParser::RedditURL *getSubreddit(const QString &subreddit);

	// Port of: public static RedditURLParser.RedditURL getSubreddit(final SubredditCanonicalId subreddit) (Java lines 55-61)
	static RedditURLParser::RedditURL *getSubredditCanonical(const QString &subredditPath);

	// Port of: private SubredditPostListURL(Type, String, PostSort, Integer, String, RedditIdAndType) (Java lines 75-89)
	SubredditPostListURL(Type type, const QString &subreddit, PostSort *order,
			std::optional<int> limit, const QString &before, RedditIdAndType *after);

	// Port of: @Override public SubredditPostListURL after(final RedditIdAndType newAfter) (Java lines 91-94)
	PostListingURL *after(const RedditIdAndType &newAfter) override;

	// Port of: @Override public SubredditPostListURL limit(final Integer newLimit) (Java lines 96-99)
	PostListingURL *limit(std::optional<int> newLimit) override;

	// Port of: public SubredditPostListURL sort(final PostSort newOrder) (Java lines 101-103)
	SubredditPostListURL *sort(PostSort *newOrder);

	// Port of: @Nullable @Override public PostSort getOrder() (Java lines 105-109)
	PostSort *getOrder() override;

	// Port of: @Override public Uri generateJsonUri() (Java lines 111-159)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 161-165)
	int pathType() override;

	// Port of: public static SubredditPostListURL parse(final Uri uri) (Java lines 167-367)
	static SubredditPostListURL *parse(const PinkUri &uri);

	// Port of: @Override public String humanReadablePath() (Java lines 369-396)
	QString humanReadablePath() override;

	// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 398-426)
	QString humanReadableName(bool shorter) override;

	// Port of: public SubredditPostListURL changeSubreddit(final String newSubreddit) (Java lines 428-430)
	SubredditPostListURL *changeSubreddit(const QString &newSubreddit);
};

} // namespace PinkReader
