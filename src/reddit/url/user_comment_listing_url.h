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
 *   org.quantumbadger.redreader.reddit.url.UserCommentListingURL (lines 1-177)
 *
 * Every field, method, inner class, enum, constant, logic line.
 */

#pragma once

#include <QString>
#include <optional>
#include "comment_listing_url.h"

namespace PinkReader {

// Forward
class UserCommentSort;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.UserCommentListingURL (Java lines 31-176)
 */
class UserCommentListingURL : public CommentListingURL {
public:
	// Port of: public final String user; (Java line 33)
	const QString user;

	// Port of: public final UserCommentSort order; (Java line 34)
	UserCommentSort *order;

	// Port of: public final Integer limit; (Java line 35)
	std::optional<int> limit;

	// Port of: public final String after; (Java line 36)
	QString after;

	// Port of: UserCommentListingURL(String, UserCommentSort, Integer, String) (Java lines 38-47)
	UserCommentListingURL(const QString &user, UserCommentSort *order,
			std::optional<int> limit, const QString &after);

	// Port of: @Override public UserCommentListingURL after(final String newAfter) (Java lines 50-52)
	CommentListingURL *after(const QString &newAfter) override;

	// Port of: @Override public UserCommentListingURL limit(final Integer newLimit) (Java lines 54-57)
	CommentListingURL *limit(std::optional<int> newLimit) override;

	// Port of: public UserCommentListingURL order(final UserCommentSort newOrder) (Java lines 59-61)
	UserCommentListingURL *order(UserCommentSort *newOrder);

	// Port of: public static UserCommentListingURL parse(final Uri uri) (Java lines 63-129)
	static UserCommentListingURL *parse(const PinkUri &uri);

	// Port of: @Override public Uri generateJsonUri() (Java lines 132-157)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 159-163)
	int pathType() override;

	// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 165-175)
	QString humanReadableName(bool shorter) override;
};

} // namespace PinkReader
