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
 *   org.quantumbadger.redreader.reddit.url.CommentListingURL (lines 1-25)
 *
 * Every field, method, inner class, constant, logic line.
 *
 * Original package: org.quantumbadger.redreader.reddit.url
 * Original class: public abstract class CommentListingURL extends RedditURLParser.RedditURL
 */

#pragma once

#include <QString>
#include "reddit_url_parser.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.CommentListingURL (Java lines 20-24)
 *
 * public abstract class CommentListingURL extends RedditURLParser.RedditURL
 */
class CommentListingURL : public RedditURLParser::RedditURL {
public:
	// Port of: public abstract CommentListingURL after(String after); (Java line 22)
	virtual CommentListingURL *after(const QString &newAfter) = 0;

	// Port of: public abstract CommentListingURL limit(Integer limit); (Java line 24)
	virtual CommentListingURL *limit(std::optional<int> newLimit) = 0;
};

} // namespace PinkReader
