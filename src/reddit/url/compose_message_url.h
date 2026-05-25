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
 *   org.quantumbadger.redreader.reddit.url.ComposeMessageURL (lines 1-123)
 *
 * Every field, method, inner class, enum, constant, logic line.
 */

#pragma once

#include <QString>
#include <optional>
#include "reddit_url_parser.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.ComposeMessageURL (Java lines 28-123)
 */
class ComposeMessageURL : public RedditURLParser::RedditURL {
public:
	// Port of: public final String recipient; (Java line 30)
	const QString recipient;

	// Port of: public final String subject; (Java line 31)
	const QString subject;

	// Port of: public final String message; (Java line 32)
	const QString message;

	// Port of: public ComposeMessageURL(String, String, String) (Java lines 34-38)
	ComposeMessageURL(const QString &recipient, const QString &subject, const QString &message);

	// Port of: public static ComposeMessageURL parse(final Uri uri) (Java lines 40-90)
	static ComposeMessageURL *parse(const PinkUri &uri);

	// Port of: @Override public Uri generateJsonUri() (Java lines 93-116)
	PinkUri generateJsonUri() override;

	// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 118-122)
	int pathType() override;
};

} // namespace PinkReader
