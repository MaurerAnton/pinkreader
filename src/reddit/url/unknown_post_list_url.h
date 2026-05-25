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
 *   org.quantumbadger.redreader.reddit.url.UnknownPostListURL (lines 1-62)
 *
 * Every field, method, inner class, enum, constant, logic line.
 */

#pragma once

#include <QString>
#include <optional>
#include "post_listing_url.h"

namespace PinkReader {

class RedditIdAndType;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.UnknownPostListURL (Java lines 23-61)
 */
class UnknownPostListURL : public PostListingURL {
public:
	// Port of: private final Uri uri; (Java line 25)
	const PinkUri uri;

	// Port of: UnknownPostListURL(final Uri uri) (Java lines 27-29)
	explicit UnknownPostListURL(PinkUri uri);

	// Port of: @Override public PostListingURL after(final RedditIdAndType after) (Java lines 32-36)
	PostListingURL *after(const RedditIdAndType &newAfter) override;

	// Port of: @Override public PostListingURL limit(final Integer limit) (Java lines 39-45)
	PostListingURL *limit(std::optional<int> newLimit) override;

	// Port of: // TODO handle this better (Java line 47)
	// Port of: @Override public Uri generateJsonUri() (Java lines 49-55)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 57-61)
	int pathType() override;
};

} // namespace PinkReader
