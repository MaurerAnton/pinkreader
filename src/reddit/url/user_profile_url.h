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
 *   org.quantumbadger.redreader.reddit.url.UserProfileURL (lines 1-100)
 *
 * Every field, method, inner class, enum, constant, logic line.
 */

#pragma once

#include <QString>
#include "reddit_url_parser.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.UserProfileURL (Java lines 28-99)
 */
class UserProfileURL : public RedditURLParser::RedditURL {
public:
	// Port of: public final String username; (Java line 30)
	const QString username;

	// Port of: public UserProfileURL(final String username) (Java lines 32-34)
	explicit UserProfileURL(const QString &username);

	// Port of: public static UserProfileURL parse(final Uri uri) (Java lines 36-73)
	static UserProfileURL *parse(const PinkUri &uri);

	// Port of: @Override public Uri generateJsonUri() (Java lines 76-88)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 90-94)
	int pathType() override;

	// Port of: @Override public String humanReadableName(final Context context, final boolean shorter) (Java lines 96-99)
	QString humanReadableName(bool shorter) override;
};

} // namespace PinkReader
