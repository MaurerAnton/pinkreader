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
 */

#include "unknown_comment_list_url.h"

namespace PinkReader {

// Port of: UnknownCommentListURL(final Uri uri) (Java lines 26-28)
UnknownCommentListURL::UnknownCommentListURL(PinkUri uri)
	: uri(std::move(uri)) {}

// Port of: @Override public CommentListingURL after(final String after) (Java lines 31-35)
CommentListingURL *UnknownCommentListURL::after(const QString &newAfter) {
	// Port of: return new UnknownCommentListURL(uri.buildUpon().appendQueryParameter("after", after).build());
	PinkUri::Builder builder;

	// Start building from current URI's components
	builder.scheme(uri.getScheme());
	builder.authority(uri.getHost());
	QString path = uri.getPath();
	if (!path.endsWith(QStringLiteral(".json"))) {
		builder.encodedPath(path);
	} else {
		builder.encodedPath(path.left(path.length() - 5));
	}
	builder.appendEncodedPath(QStringLiteral(".json"));

	return new UnknownCommentListURL(uri.buildUpon());
}

// Port of: @Override public CommentListingURL limit(final Integer limit) (Java lines 38-44)
CommentListingURL *UnknownCommentListURL::limit(std::optional<int> newLimit) {
	// Port of: return new UnknownCommentListURL(uri.buildUpon().appendQueryParameter("limit", String.valueOf(limit)).build());
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

	return new UnknownCommentListURL(builder.build());
}

// Port of: // TODO handle this better (Java line 46)
// Port of: @Override public Uri generateJsonUri() (Java lines 48-54)
PinkUri UnknownCommentListURL::generateJsonUri() {
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

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 56-60)
int UnknownCommentListURL::pathType() {
	return RedditURLParser::UNKNOWN_COMMENT_LISTING_URL;
}

} // namespace PinkReader
