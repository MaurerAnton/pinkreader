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
 *   org.quantumbadger.redreader.reddit.url.UnknownCommentListURL (lines 1-61)
 *
 * Every field, method, inner class, enum, constant, logic line.
 */

#pragma once

#include <QString>
#include <optional>
#include "comment_listing_url.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.UnknownCommentListURL (Java lines 22-60)
 */
class UnknownCommentListURL : public CommentListingURL {
public:
	// Port of: private final Uri uri; (Java line 24)
	const PinkUri uri;

	// Port of: UnknownCommentListURL(final Uri uri) (Java lines 26-28)
	explicit UnknownCommentListURL(PinkUri uri);

	// Port of: @Override public CommentListingURL after(final String after) (Java lines 31-35)
	CommentListingURL *after(const QString &newAfter) override;

	// Port of: @Override public CommentListingURL limit(final Integer limit) (Java lines 38-44)
	CommentListingURL *limit(std::optional<int> newLimit) override;

	// Port of: // TODO handle this better (Java line 46)
	// Port of: @Override public Uri generateJsonUri() (Java lines 48-54)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 56-60)
	int pathType() override;
};

} // namespace PinkReader
