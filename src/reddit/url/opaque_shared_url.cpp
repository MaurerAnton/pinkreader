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
 */

#include "opaque_shared_url.h"

#include <stdexcept>

namespace PinkReader {

// Port of: private OpaqueSharedURL(...) (Java lines 35-43)
OpaqueSharedURL::OpaqueSharedURL(
		std::optional<QString> subreddit,
		std::optional<QString> user,
		std::optional<QString> shareKey)
	: subreddit(std::move(subreddit))
	, user(std::move(user))
	, shareKey(std::move(shareKey)) {}

// Port of: @Override public Uri generateJsonUri() (Java lines 46-48)
PinkUri OpaqueSharedURL::generateJsonUri() {
	return PinkUri();
}

// Port of: @Override public int pathType() (Java lines 51-53)
int OpaqueSharedURL::pathType() {
	return RedditURLParser::OPAQUE_SHARED_URL;
}

// Port of: @Nullable public static OpaqueSharedURL parse(final Uri uri) (Java lines 56-77)
OpaqueSharedURL *OpaqueSharedURL::parse(const PinkUri &uri) {
	// Port of: final List<String> pathSegments = uri.getPathSegments(); (Java line 59)
	QStringList pathSegments = uri.getPathSegments();

	// Port of: if (pathSegments.size() != 4) return null; (Java lines 60-62)
	if (pathSegments.size() != 4) {
		return nullptr;
	}

	// Port of: if (pathSegments.get(2).equals("s")) (Java line 65)
	if (pathSegments[2] == QStringLiteral("s")) {
		// Port of: if (pathSegments.get(0).equals("r")) (Java line 66)
		if (pathSegments[0] == QStringLiteral("r")) {
			// Port of: return new OpaqueSharedURL(pathSegments.get(1), null, pathSegments.get(3)); (Java line 67)
			return new OpaqueSharedURL(pathSegments[1], std::nullopt, pathSegments[3]);

		// Port of: else if(pathSegments.get(0).equals("u")) (Java line 68)
		} else if (pathSegments[0] == QStringLiteral("u")) {
			// Port of: return new OpaqueSharedURL(pathSegments.get(1), pathSegments.get(3), null); (Java line 69)
			return new OpaqueSharedURL(pathSegments[1], pathSegments[3], std::nullopt);

		// Port of: else return null; (Java lines 70-71)
		} else {
			return nullptr;
		}

	// Port of: else return null; (Java lines 74-75)
	} else {
		return nullptr;
	}
}

// Port of: @NonNull public UriString getUrlToFetch() (Java lines 80-90)
QString OpaqueSharedURL::getUrlToFetch() const {
	// Port of: if (subreddit != null) (Java line 81)
	if (subreddit.has_value()) {
		// Port of: return new UriString(String.format("https://www.reddit.com/r/%s/s/%s", subreddit, shareKey)); (Java lines 82-83)
		return QStringLiteral("https://www.reddit.com/r/%1/s/%2")
				.arg(subreddit.value(), shareKey.value_or(QString()));

	// Port of: else if (user != null) (Java line 84)
	} else if (user.has_value()) {
		// Port of: return new UriString(String.format("https://www.reddit.com/u/%s/s/%s", user, shareKey)); (Java lines 85-86)
		return QStringLiteral("https://www.reddit.com/u/%1/s/%2")
				.arg(user.value(), shareKey.value_or(QString()));

	// Port of: else throw new RuntimeException("Neither subreddit nor user set"); (Java lines 87-88)
	} else {
		throw std::runtime_error("Neither subreddit nor user set");
	}
}

} // namespace PinkReader
