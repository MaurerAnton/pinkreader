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
 *   org.quantumbadger.redreader.reddit.url.RedditURLParser (lines 1-300)
 *
 * Every method and logic line from the Java source.
 */

#include "reddit_url_parser.h"

// Include all URL subclass headers so parse() can construct them
#include "subreddit_post_list_url.h"
#include "multireddit_post_list_url.h"
#include "search_post_list_url.h"
#include "user_post_listing_url.h"
#include "user_comment_listing_url.h"
#include "post_comment_listing_url.h"
#include "user_profile_url.h"
#include "compose_message_url.h"
#include "opaque_shared_url.h"
#include "unknown_comment_list_url.h"
#include "unknown_post_list_url.h"

// For humanReadablePath: need generateJsonUri which returns PinkUri with path segments
#include <QUrl>

namespace PinkReader {

// =============================================================================
// RedditURL base class methods
// =============================================================================

// Port of: public final SubredditPostListURL asSubredditPostListURL() (Java lines 228-230)
SubredditPostListURL *RedditURLParser::RedditURL::asSubredditPostListURL() {
	return dynamic_cast<SubredditPostListURL *>(this);
}

MultiredditPostListURL *RedditURLParser::RedditURL::asMultiredditPostListURL() {
	return dynamic_cast<MultiredditPostListURL *>(this);
}

SearchPostListURL *RedditURLParser::RedditURL::asSearchPostListURL() {
	return dynamic_cast<SearchPostListURL *>(this);
}

UserPostListingURL *RedditURLParser::RedditURL::asUserPostListURL() {
	return dynamic_cast<UserPostListingURL *>(this);
}

UserProfileURL *RedditURLParser::RedditURL::asUserProfileURL() {
	return dynamic_cast<UserProfileURL *>(this);
}

PostCommentListingURL *RedditURLParser::RedditURL::asPostCommentListURL() {
	return dynamic_cast<PostCommentListingURL *>(this);
}

UserCommentListingURL *RedditURLParser::RedditURL::asUserCommentListURL() {
	return dynamic_cast<UserCommentListingURL *>(this);
}

ComposeMessageURL *RedditURLParser::RedditURL::asComposeMessageURL() {
	return dynamic_cast<ComposeMessageURL *>(this);
}

// Port of: public String humanReadableName(Context, boolean) (Java lines 260-262)
QString RedditURLParser::RedditURL::humanReadableName(bool /*shorter*/) {
	return humanReadablePath();
}

// Port of: public String humanReadableUrl() (Java lines 264-266)
QString RedditURLParser::RedditURL::humanReadableUrl() {
	return QStringLiteral("reddit.com") + humanReadablePath();
}

// Port of: public String humanReadablePath() (Java lines 268-282)
QString RedditURLParser::RedditURL::humanReadablePath() {
	// Port of: final Uri src = generateJsonUri();
	PinkUri src = generateJsonUri();

	// Port of: final StringBuilder builder = new StringBuilder();
	QString builder;

	// Port of: for (final String pathElement : src.getPathSegments())
	QStringList segments = src.getPathSegments();
	for (const QString &pathElement : segments) {
		// Port of: if (!pathElement.equals(".json"))
		if (pathElement != QStringLiteral(".json")) {
			builder += '/';
			builder += pathElement;
		}
	}

	return builder;
}

// Port of: @NonNull public UriString browserUrl() (Java lines 284-287)
QString RedditURLParser::RedditURL::browserUrl() {
	return QStringLiteral("https://") + humanReadableUrl();
}

// Port of: @Override @NonNull public String toString() (Java lines 289-293)
QString RedditURLParser::RedditURL::toString() {
	return generateJsonUri().toString();
}

// =============================================================================
// tryGetRedditUri (Java lines 66-114)
// =============================================================================

std::optional<PinkUri> RedditURLParser::tryGetRedditUri(const PinkUri &uri) {
	// Port of: if(uri == null || uri.getHost() == null || uri.getPath() == null) (Java line 68)
	if (uri.getHost().isEmpty() || uri.getPath().isEmpty()) {
		return std::nullopt;
	}

	// Port of: if("reddit".equals(uri.getScheme()) && "reddit".equals(uri.getHost())) (Java lines 72-77)
	if (uri.getScheme() == QStringLiteral("reddit") && uri.getHost() == QStringLiteral("reddit")) {
		PinkUri::Builder builder;
		builder.scheme(QStringLiteral("https"));
		builder.authority(QStringLiteral("reddit.com"));
		return builder.build();
	}

	// Port of: if("reddit.app.link".equals(uri.getHost())) (Java lines 79-85)
	if (uri.getHost() == QStringLiteral("reddit.app.link")) {
		QString redirect = uri.getQueryParameter(QStringLiteral("$og_redirect"));
		if (!redirect.isEmpty()) {
			PinkUri parsed(redirect);
			if (!parsed.getScheme().isEmpty()) {
				return parsed;
			}
		}
	}

	// Port of: final String ampPrefix = "/amp/s/amp.reddit.com"; (Java lines 87-95)
	const QString ampPrefix = QStringLiteral("/amp/s/amp.reddit.com");
	QString host = uri.getHost();
	QString path = uri.getPath();
	if ((host == QStringLiteral("google.com") || host.endsWith(QStringLiteral(".google.com")))
			&& path.startsWith(ampPrefix)) {
		QString newUriStr = QStringLiteral("https://reddit.com") + path.mid(ampPrefix.length());
		PinkUri parsed(newUriStr);
		if (!parsed.getScheme().isEmpty()) {
			return parsed;
		}
	}

	// Port of: final String[] hostSegments = StringUtils.asciiLowercase(uri.getHost()).split("\\."); (Java lines 97-98)
	QStringList hostSegments = uri.getHost().toLower().split('.');

	// Port of: if(hostSegments.length < 2) return Optional.empty(); (Java lines 99-101)
	if (hostSegments.size() < 2) {
		return std::nullopt;
	}

	// Port of: if(hostSegments[hostSegments.length - 1].equals("com") && hostSegments[hostSegments.length - 2].equals("reddit")) (Java lines 103-106)
	if (hostSegments.last() == QStringLiteral("com")
			&& hostSegments.at(hostSegments.size() - 2) == QStringLiteral("reddit")) {
		return uri;
	}

	// Port of: if(hostSegments[hostSegments.length - 1].equals("it") && hostSegments[hostSegments.length - 2].equals("redd")) (Java lines 108-111)
	if (hostSegments.last() == QStringLiteral("it")
			&& hostSegments.at(hostSegments.size() - 2) == QStringLiteral("redd")) {
		return uri;
	}

	// Port of: return Optional.empty(); (Java line 113)
	return std::nullopt;
}

// =============================================================================
// parse (Java lines 117-199)
// =============================================================================

RedditURLParser::RedditURL *RedditURLParser::parse(const PinkUri &rawUri) {
	// Port of: if(rawUri == null) return null; (Java lines 119-121)
	// PinkUri value semantics, so no null check needed at this level.

	std::optional<PinkUri> optionalUri = tryGetRedditUri(rawUri);

	// Port of: if(optionalUri.isEmpty()) return null; (Java lines 125-127)
	if (!optionalUri.has_value()) {
		return nullptr;
	}

	PinkUri uri = optionalUri.value();

	// Port of: OpaqueSharedURL.parse(uri) (Java lines 131-136)
	{
		OpaqueSharedURL *opaqueSharedURL = OpaqueSharedURL::parse(uri);
		if (opaqueSharedURL != nullptr) {
			return opaqueSharedURL;
		}
	}

	// Port of: SubredditPostListURL.parse(uri) (Java lines 138-143)
	{
		SubredditPostListURL *subredditPostListURL = SubredditPostListURL::parse(uri);
		if (subredditPostListURL != nullptr) {
			return subredditPostListURL;
		}
	}

	// Port of: MultiredditPostListURL.parse(uri) (Java lines 145-151)
	{
		MultiredditPostListURL *multiredditPostListURL = MultiredditPostListURL::parse(uri);
		if (multiredditPostListURL != nullptr) {
			return multiredditPostListURL;
		}
	}

	// Port of: SearchPostListURL.parse(uri) (Java lines 153-158)
	{
		SearchPostListURL *searchPostListURL = SearchPostListURL::parse(uri);
		if (searchPostListURL != nullptr) {
			return searchPostListURL;
		}
	}

	// Port of: UserPostListingURL.parse(uri) (Java lines 160-165)
	{
		UserPostListingURL *userPostListURL = UserPostListingURL::parse(uri);
		if (userPostListURL != nullptr) {
			return userPostListURL;
		}
	}

	// Port of: UserCommentListingURL.parse(uri) (Java lines 167-173)
	{
		UserCommentListingURL *userCommentListURL = UserCommentListingURL::parse(uri);
		if (userCommentListURL != nullptr) {
			return userCommentListURL;
		}
	}

	// Port of: PostCommentListingURL.parse(uri) (Java lines 175-181)
	{
		PostCommentListingURL *commentListingURL = PostCommentListingURL::parse(uri);
		if (commentListingURL != nullptr) {
			return commentListingURL;
		}
	}

	// Port of: UserProfileURL.parse(uri) (Java lines 183-188)
	{
		UserProfileURL *userProfileURL = UserProfileURL::parse(uri);
		if (userProfileURL != nullptr) {
			return userProfileURL;
		}
	}

	// Port of: ComposeMessageURL.parse(uri) (Java lines 190-196)
	{
		ComposeMessageURL *composeMessageURL = ComposeMessageURL::parse(uri);
		if (composeMessageURL != nullptr) {
			return composeMessageURL;
		}
	}

	// Port of: return null; (Java line 198)
	return nullptr;
}

// Port of: public static RedditURL parseProbableCommentListing(final Uri uri) (Java lines 201-209)
RedditURLParser::RedditURL *RedditURLParser::parseProbableCommentListing(const PinkUri &uri) {
	RedditURL *matchURL = parse(uri);
	if (matchURL != nullptr) {
		return matchURL;
	}

	return new UnknownCommentListURL(uri);
}

// Port of: @NonNull public static RedditURL parseProbablePostListing(final Uri uri) (Java lines 211-220)
RedditURLParser::RedditURL *RedditURLParser::parseProbablePostListing(const PinkUri &uri) {
	RedditURL *matchURL = parse(uri);
	if (matchURL != nullptr) {
		return matchURL;
	}

	return new UnknownPostListURL(uri);
}

} // namespace PinkReader
