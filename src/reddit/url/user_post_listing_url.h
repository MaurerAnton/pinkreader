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
 *   org.quantumbadger.redreader.reddit.url.UserPostListingURL (lines 1-281)
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
 * @brief Port of org.quantumbadger.redreader.reddit.url.UserPostListingURL (Java lines 32-280)
 */
class UserPostListingURL : public PostListingURL {
public:
	// Port of: public enum Type { SAVED, HIDDEN, UPVOTED, DOWNVOTED, SUBMITTED } (Java lines 76-78)
	enum class Type {
		SAVED,
		HIDDEN,
		UPVOTED,
		DOWNVOTED,
		SUBMITTED
	};

	// Port of: public final Type type; (Java line 54)
	const Type type;

	// Port of: public final String user; (Java line 55)
	const QString user;

	// Port of: public final PostSort order; (Java line 56)
	PostSort *order;

	// Port of: public final Integer limit; (Java line 57)
	std::optional<int> limit;

	// Port of: public final String before; (Java line 58)
	QString before;

	// Port of: public final RedditIdAndType after; (Java line 59)
	RedditIdAndType *after;

	// ---- Static factory methods (Java lines 34-52) ----

	// Port of: public static UserPostListingURL getSaved(final String username) (Java lines 34-36)
	static UserPostListingURL *getSaved(const QString &username);

	// Port of: public static UserPostListingURL getHidden(final String username) (Java lines 38-40)
	static UserPostListingURL *getHidden(const QString &username);

	// Port of: public static UserPostListingURL getLiked(final String username) (Java lines 42-44)
	static UserPostListingURL *getLiked(const QString &username);

	// Port of: public static UserPostListingURL getDisliked(final String username) (Java lines 46-48)
	static UserPostListingURL *getDisliked(const QString &username);

	// Port of: public static UserPostListingURL getSubmitted(final String username) (Java lines 50-52)
	static UserPostListingURL *getSubmitted(const QString &username);

	// Port of: UserPostListingURL(Type, String, PostSort, Integer, String, RedditIdAndType) (Java lines 61-74)
	UserPostListingURL(Type type, const QString &user, PostSort *order,
			std::optional<int> limit, const QString &before, RedditIdAndType *after);

	// Port of: @Override public UserPostListingURL after(final RedditIdAndType newAfter) (Java lines 81-83)
	PostListingURL *after(const RedditIdAndType &newAfter) override;

	// Port of: @Override public UserPostListingURL limit(final Integer newLimit) (Java lines 85-88)
	PostListingURL *limit(std::optional<int> newLimit) override;

	// Port of: public UserPostListingURL sort(final PostSort newOrder) (Java lines 90-92)
	UserPostListingURL *sort(PostSort *newOrder);

	// Port of: @Override public PostSort getOrder() (Java lines 94-97)
	PostSort *getOrder() override;

	// Port of: public static UserPostListingURL parse(final Uri uri) (Java lines 99-173)
	static UserPostListingURL *parse(const PinkUri &uri);

	// Port of: @Override public Uri generateJsonUri() (Java lines 176-205)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 207-211)
	int pathType() override;

	// Port of: @Override public String humanReadablePath() (Java lines 214-242)
	QString humanReadablePath() override;

	// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 244-280)
	QString humanReadableName(bool shorter) override;
};

} // namespace PinkReader
