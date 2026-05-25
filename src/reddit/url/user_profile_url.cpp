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
 */

#include "user_profile_url.h"
#include "../../core/constants.h"

namespace PinkReader {

// Port of: public UserProfileURL(final String username) (Java lines 32-34)
UserProfileURL::UserProfileURL(const QString &username)
	: username(username) {}

// Port of: public static UserProfileURL parse(final Uri uri) (Java lines 36-73)
UserProfileURL *UserProfileURL::parse(const PinkUri &uri) {
	// Port of: final String[] pathSegments; (Java lines 38-58)
	QStringList pathSegmentsList = uri.getPathSegments();

	QStringList pathSegmentsFiltered;
	pathSegmentsFiltered.reserve(pathSegmentsList.size());
	for (QString segment : pathSegmentsList) {
		// Port of: while(StringUtils.asciiLowercase(segment).endsWith(".json") || ...endsWith(".xml")) (Java lines 46-49)
		while (segment.toLower().endsWith(QStringLiteral(".json"))
				|| segment.toLower().endsWith(QStringLiteral(".xml"))) {
			segment = segment.left(segment.lastIndexOf('.'));
		}

		// Port of: if(!segment.isEmpty()) pathSegmentsFiltered.add(segment); (Java lines 51-53)
		if (!segment.isEmpty()) {
			pathSegmentsFiltered.append(segment);
		}
	}

	// Port of: pathSegments = pathSegmentsFiltered.toArray(new String[0]); (Java lines 56-57)

	// Port of: if(pathSegments.length != 2) return null; (Java lines 60-62)
	if (pathSegmentsFiltered.size() != 2) {
		return nullptr;
	}

	// Port of: if(!pathSegments[0].equalsIgnoreCase("user") && !pathSegments[0].equalsIgnoreCase("u")) (Java lines 64-67)
	if (!pathSegmentsFiltered[0].compare(QStringLiteral("user"), Qt::CaseInsensitive) == 0
			&& !pathSegmentsFiltered[0].compare(QStringLiteral("u"), Qt::CaseInsensitive) == 0) {
		return nullptr;
	}

	// Port of: // TODO validate username with regex (Java line 69)
	// Port of: final String username = pathSegments[1]; (Java line 70)
	const QString username = pathSegmentsFiltered[1];

	// Port of: return new UserProfileURL(username); (Java line 72)
	return new UserProfileURL(username);
}

// Port of: @Override public Uri generateJsonUri() (Java lines 76-88)
PinkUri UserProfileURL::generateJsonUri() {
	PinkUri::Builder builder;

	// Port of: builder.scheme(Constants.Reddit.getScheme()).authority(Constants.Reddit.getDomain()); (Java lines 79-80)
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	// Port of: builder.appendEncodedPath("user"); (Java line 82)
	builder.appendEncodedPath(QStringLiteral("user"));

	// Port of: builder.appendPath(username); (Java line 83)
	builder.appendPath(username);

	// Port of: builder.appendEncodedPath(".json"); (Java line 85)
	builder.appendEncodedPath(QStringLiteral(".json"));

	// Port of: return builder.build(); (Java line 87)
	return builder.build();
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 90-94)
int UserProfileURL::pathType() {
	return RedditURLParser::USER_PROFILE_URL;
}

// Port of: @Override public String humanReadableName(final Context context, final boolean shorter) (Java lines 96-99)
QString UserProfileURL::humanReadableName(bool /*shorter*/) {
	return username;
}

} // namespace PinkReader
