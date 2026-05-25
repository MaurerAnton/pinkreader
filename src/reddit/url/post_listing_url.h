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
 *   org.quantumbadger.redreader.reddit.url.PostListingURL (lines 1-32)
 *
 * Every field, method, inner class, constant, logic line.
 *
 * Original package: org.quantumbadger.redreader.reddit.url
 * Original class: public abstract class PostListingURL extends RedditURLParser.RedditURL
 */

#pragma once

#include <QString>
#include <optional>
#include "reddit_url_parser.h"

namespace PinkReader {

// Forward
class RedditIdAndType;
class PostSort;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.PostListingURL (Java lines 23-31)
 *
 * public abstract class PostListingURL extends RedditURLParser.RedditURL
 */
class PostListingURL : public RedditURLParser::RedditURL {
public:
	// Port of: public abstract PostListingURL after(RedditIdAndType after); (Java line 25)
	virtual PostListingURL *after(const RedditIdAndType &newAfter) = 0;

	// Port of: public abstract PostListingURL limit(Integer limit); (Java line 27)
	virtual PostListingURL *limit(std::optional<int> newLimit) = 0;

	// Port of: public PostSort getOrder() { return null; } (Java lines 29-31)
	virtual PostSort *getOrder();
};

} // namespace PinkReader
