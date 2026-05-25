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
 * Every field, method, inner class (RedditURL), enum, constant, logic line
 * ported exactly from the Java source.
 *
 * Original annotations:
 *   @IntDef, @Retention(RetentionPolicy.SOURCE) for PathType
 */

#pragma once

#include <QString>
#include <memory>
#include <optional>

#include "pink_uri.h"

namespace PinkReader {

// ---- Forward declarations for all URL subclasses ----
class SubredditPostListURL;
class MultiredditPostListURL;
class SearchPostListURL;
class UserPostListingURL;
class UserCommentListingURL;
class PostCommentListingURL;
class UserProfileURL;
class ComposeMessageURL;
class UnknownCommentListURL;
class UnknownPostListURL;
class OpaqueSharedURL;

// ---- Forward declarations for types ----
class RedditIdAndType;
class PostSort;
class PostCommentSort;
class UserCommentSort;

// ---- StringUtils helpers (inline replacements) ----
namespace StringUtils {
	inline QString asciiLowercase(const QString &s) { return s.toLower(); }
	inline QString asciiUppercase(const QString &s) { return s.toUpper(); }
	inline bool endsWith(QString s, const QString &suffix) {
		return s.endsWith(suffix, Qt::CaseInsensitive);
	}
}

#if 0
// Port of: @IntDef, @Retention(RetentionPolicy.SOURCE)
// In C++ we use an enum; the IntDef annotation is a documentation aid.
#endif

/**
 * @brief Port of org.quantumbadger.redreader.reddit.url.RedditURLParser
 *
 * Contains path type constants and the RedditURL abstract base class.
 */
class RedditURLParser {
public:
	// ---- Path Type Constants (Java lines 37-47) ----
	static constexpr int SUBREDDIT_POST_LISTING_URL = 0;
	static constexpr int USER_POST_LISTING_URL = 1;
	static constexpr int SEARCH_POST_LISTING_URL = 2;
	static constexpr int UNKNOWN_POST_LISTING_URL = 3;
	static constexpr int USER_PROFILE_URL = 4;
	static constexpr int USER_COMMENT_LISTING_URL = 5;
	static constexpr int UNKNOWN_COMMENT_LISTING_URL = 6;
	static constexpr int POST_COMMENT_LISTING_URL = 7;
	static constexpr int MULTIREDDIT_POST_LISTING_URL = 8;
	static constexpr int COMPOSE_MESSAGE_URL = 9;
	static constexpr int OPAQUE_SHARED_URL = 10;

	// ---- RedditURL abstract base class (Java lines 222-299) ----
	class RedditURL {
	public:
		virtual ~RedditURL() = default;

		// Port of: public abstract Uri generateJsonUri(); (Java line 223)
		virtual PinkUri generateJsonUri() = 0;

		// Port of: public abstract @PathType int pathType(); (Java lines 225-226)
		virtual int pathType() = 0;

		// Port of: public final SubredditPostListURL asSubredditPostListURL() (Java lines 228-230)
		virtual SubredditPostListURL *asSubredditPostListURL();

		// Port of: public final MultiredditPostListURL asMultiredditPostListURL() (Java lines 232-234)
		virtual MultiredditPostListURL *asMultiredditPostListURL();

		// Port of: public final SearchPostListURL asSearchPostListURL() (Java lines 236-238)
		virtual SearchPostListURL *asSearchPostListURL();

		// Port of: public final UserPostListingURL asUserPostListURL() (Java lines 240-242)
		virtual UserPostListingURL *asUserPostListURL();

		// Port of: public UserProfileURL asUserProfileURL() (Java lines 244-246)
		virtual UserProfileURL *asUserProfileURL();

		// Port of: public PostCommentListingURL asPostCommentListURL() (Java lines 248-250)
		virtual PostCommentListingURL *asPostCommentListURL();

		// Port of: public UserCommentListingURL asUserCommentListURL() (Java lines 252-254)
		virtual UserCommentListingURL *asUserCommentListURL();

		// Port of: public ComposeMessageURL asComposeMessageURL() (Java lines 256-258)
		virtual ComposeMessageURL *asComposeMessageURL();

		// Port of: public String humanReadableName(Context, boolean) (Java lines 260-262)
		virtual QString humanReadableName(bool shorter);

		// Port of: public String humanReadableUrl() (Java lines 264-266)
		virtual QString humanReadableUrl();

		// Port of: public String humanReadablePath() (Java lines 268-282)
		virtual QString humanReadablePath();

		// Port of: @NonNull public UriString browserUrl() (Java lines 284-287)
		virtual QString browserUrl();

		// Port of: @Override @NonNull public String toString() (Java lines 289-293)
		virtual QString toString();
	};

	// ---- Static parse methods ----

	// Port of: private static Optional<Uri> tryGetRedditUri(final Uri uri) (Java lines 66-114)
	static std::optional<PinkUri> tryGetRedditUri(const PinkUri &uri);

	// Port of: @Nullable public static RedditURL parse(final Uri rawUri) (Java lines 117-199)
	static RedditURL *parse(const PinkUri &rawUri);

	// Port of: public static RedditURL parseProbableCommentListing(final Uri uri) (Java lines 201-209)
	static RedditURL *parseProbableCommentListing(const PinkUri &uri);

	// Port of: @NonNull public static RedditURL parseProbablePostListing(final Uri uri) (Java lines 211-220)
	static RedditURL *parseProbablePostListing(const PinkUri &uri);
};

} // namespace PinkReader
