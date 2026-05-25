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
 *   org.quantumbadger.redreader.reddit.url.OpaqueSharedURL (lines 1-91)
 *
 * Every field, method, inner class, enum, constant, logic line.
 */

#pragma once

#include <QString>
#include <optional>
#include "reddit_url_parser.h"

namespace PinkReader {

// Forward
class UriString;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.OpaqueSharedURL (Java lines 29-90)
 */
class OpaqueSharedURL : public RedditURLParser::RedditURL {
public:
	// Port of: @Nullable public final String subreddit; (Java line 31)
	std::optional<QString> subreddit;

	// Port of: @Nullable public final String user; (Java line 32)
	std::optional<QString> user;

	// Port of: @Nullable public final String shareKey; (Java line 33)
	std::optional<QString> shareKey;

	// Port of: private OpaqueSharedURL(...) (Java lines 35-43)
	OpaqueSharedURL(
			std::optional<QString> subreddit,
			std::optional<QString> user,
			std::optional<QString> shareKey);

	// Port of: @Override public Uri generateJsonUri() (Java lines 46-48)
	PinkUri generateJsonUri() override;

	// Port of: @Override public int pathType() (Java lines 51-53)
	int pathType() override;

	// Port of: @Nullable public static OpaqueSharedURL parse(final Uri uri) (Java lines 56-77)
	static OpaqueSharedURL *parse(const PinkUri &uri);

	// Port of: @NonNull public UriString getUrlToFetch() (Java lines 80-90)
	QString getUrlToFetch() const;
};

} // namespace PinkReader
