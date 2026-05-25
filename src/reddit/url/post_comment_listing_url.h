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
 *   org.quantumbadger.redreader.reddit.url.PostCommentListingURL (lines 1-269)
 *
 * Every field, method, inner class, enum, constant, logic line.
 */

#pragma once

#include <QString>
#include <optional>
#include "comment_listing_url.h"

namespace PinkReader {

// Forward
class PostCommentSort;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.PostCommentListingURL (Java lines 31-268)
 */
class PostCommentListingURL : public CommentListingURL {
public:
	// Port of: public final String after; (Java line 33)
	QString after_str;

	// Port of: public final String postId; (Java line 35)
	const QString postId;

	// Port of: public final String commentId; (Java line 36)
	QString commentId;

	// Port of: public final Integer context; (Java line 38)
	std::optional<int> context;

	// Port of: public final Integer limit; (Java line 39)
	std::optional<int> limit;

	// Port of: public final PostCommentSort order; (Java line 41)
	PostCommentSort *order;

	// Port of: public final boolean video; (Java line 43)
	const bool video;

	// Port of: public static PostCommentListingURL forPostId(final String postId) (Java lines 45-47)
	static PostCommentListingURL *forPostId(const QString &postId);

	// Port of: public PostCommentListingURL(String after, String postId, String commentId,
	//             Integer context, Integer limit, PostCommentSort order, boolean video) (Java lines 49-73)
	PostCommentListingURL(const QString &after, QString postId, QString commentId,
			std::optional<int> context, std::optional<int> limit,
			PostCommentSort *order, bool video);

	// Port of: @Override public PostCommentListingURL after(final String after) (Java lines 76-78)
	CommentListingURL *after(const QString &newAfter) override;

	// Port of: @Override public PostCommentListingURL limit(final Integer limit) (Java lines 80-83)
	CommentListingURL *limit(std::optional<int> newLimit) override;

	// Port of: public PostCommentListingURL context(final Integer context) (Java lines 85-87)
	PostCommentListingURL *withContext(std::optional<int> newContext);

	// Port of: public PostCommentListingURL order(final PostCommentSort order) (Java lines 89-91)
	PostCommentListingURL *withOrder(PostCommentSort *newOrder);

	// Port of: public PostCommentListingURL commentId(String commentId) (Java lines 93-100)
	PostCommentListingURL *withCommentId(QString newCommentId);

	// Port of: @Override public Uri generateJsonUri() (Java lines 103-114)
	PinkUri generateJsonUri() override;

	// Port of: public Uri generateNonJsonUri() (Java lines 116-123)
	PinkUri generateNonJsonUri();

	// Port of: private void internalGenerateCommon(@NonNull final Uri.Builder builder) (Java lines 125-155)
	void internalGenerateCommon(PinkUri::Builder &builder);

	// Port of: public static PostCommentListingURL parse(final Uri uri) (Java lines 157-256)
	static PostCommentListingURL *parse(const PinkUri &uri);

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 258-262)
	int pathType() override;

	// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 264-267)
	QString humanReadableName(bool shorter) override;
};

} // namespace PinkReader
