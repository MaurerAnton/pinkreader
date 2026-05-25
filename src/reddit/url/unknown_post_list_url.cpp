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
 */

#include "unknown_post_list_url.h"
#include "../kthings/reddit_id_and_type.h"

namespace PinkReader {

// Port of: UnknownPostListURL(final Uri uri) (Java lines 27-29)
UnknownPostListURL::UnknownPostListURL(PinkUri uri)
	: uri(std::move(uri)) {}

// Port of: @Override public PostListingURL after(final RedditIdAndType after) (Java lines 32-36)
PostListingURL *UnknownPostListURL::after(const RedditIdAndType &newAfter) {
	// Port of: return new UnknownPostListURL(uri.buildUpon().appendQueryParameter("after", after.getValue()).build());
	PinkUri::Builder builder;
	builder.scheme(uri.getScheme());
	builder.authority(uri.getHost());
	QString path = uri.getPath();
	if (!path.endsWith(QStringLiteral(".json"))) {
		builder.encodedPath(path);
	} else {
		builder.encodedPath(path.left(path.length() - 5));
	}
	builder.appendEncodedPath(QStringLiteral(".json"));
	builder.appendQueryParameter(QStringLiteral("after"), newAfter.value);

	return new UnknownPostListURL(builder.build());
}

// Port of: @Override public PostListingURL limit(final Integer limit) (Java lines 39-45)
PostListingURL *UnknownPostListURL::limit(std::optional<int> newLimit) {
	PinkUri::Builder builder;
	builder.scheme(uri.getScheme());
	builder.authority(uri.getHost());
	QString path = uri.getPath();
	if (!path.endsWith(QStringLiteral(".json"))) {
		builder.encodedPath(path);
	} else {
		builder.encodedPath(path.left(path.length() - 5));
	}
	builder.appendEncodedPath(QStringLiteral(".json"));
	if (newLimit.has_value()) {
		builder.appendQueryParameter(QStringLiteral("limit"), QString::number(newLimit.value()));
	}

	return new UnknownPostListURL(builder.build());
}

// Port of: // TODO handle this better (Java line 47)
// Port of: @Override public Uri generateJsonUri() (Java lines 49-55)
PinkUri UnknownPostListURL::generateJsonUri() {
	// Port of: if(uri.getPath().endsWith(".json")) return uri; else return uri.buildUpon().appendEncodedPath(".json").build();
	if (uri.getPath().endsWith(QStringLiteral(".json"))) {
		return uri;
	} else {
		PinkUri::Builder builder;
		builder.scheme(uri.getScheme());
		builder.authority(uri.getHost());
		builder.encodedPath(uri.getPath());
		builder.appendEncodedPath(QStringLiteral(".json"));
		return builder.build();
	}
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 57-61)
int UnknownPostListURL::pathType() {
	return RedditURLParser::UNKNOWN_POST_LISTING_URL;
}

} // namespace PinkReader
