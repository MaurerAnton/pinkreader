/*
 * PinkReader - GPLv3
 * File: reddit_url_parser.h - Port of RedditURLParser.java
 *
 * Exact line-by-line port of:
 *   redreader/.../reddit/url/RedditURLParser.java (300 lines)
 *
 * All path type constants, RedditURL abstract class, and static parse methods.
 */

#pragma once

#include <QString>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class UriString;

/**
 * @brief Port of RedditURLParser (Java public class, lines 35-300)
 *
 * Path type constants: SUBREDDIT_POST_LISTING_URL=0 through OPAQUE_SHARED_URL=10.
 * Inner abstract class RedditURL with generateJsonUri(), pathType(), and cast methods.
 */
class RedditURLParser {
public:
    // ---- PathType constants (Java lines 37-47) ----
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

    // ---- Abstract base class RedditURL (Java lines 222-299) ----
    class RedditURL {
    public:
        virtual ~RedditURL() = default;

        // Java line 223: public abstract Uri generateJsonUri();
        virtual QString generateJsonUri() const = 0;

        // Java line 225-226: public abstract @PathType int pathType();
        virtual int pathType() const = 0;

        // Java lines 228-258: Cast methods (unsafe casts in Java)
        // In C++ we use dynamic_cast safety where possible,
        // but the original does raw (SubredditPostListURL)this casts.

        // Java line 228-230: asSubredditPostListURL
        virtual const class SubredditPostListURL *asSubredditPostListURL() const { return nullptr; }

        // Java line 232-234: asMultiredditPostListURL
        virtual const class MultiredditPostListURL *asMultiredditPostListURL() const { return nullptr; }

        // Java line 236-238: asSearchPostListURL
        virtual const class SearchPostListURL *asSearchPostListURL() const { return nullptr; }

        // Java line 240-242: asUserPostListURL
        virtual const class UserPostListingURL *asUserPostListURL() const { return nullptr; }

        // Java line 244-246: asUserProfileURL
        virtual const class UserProfileURL *asUserProfileURL() const { return nullptr; }

        // Java line 248-250: asPostCommentListURL
        virtual const class PostCommentListingURL *asPostCommentListURL() const { return nullptr; }

        // Java line 252-254: asUserCommentListURL
        virtual const class UserCommentListingURL *asUserCommentListURL() const { return nullptr; }

        // Java line 256-258: asComposeMessageURL
        virtual const class ComposeMessageURL *asComposeMessageURL() const { return nullptr; }

        // Java line 260-262: humanReadableName
        virtual QString humanReadableName(bool shorter) const {
            return humanReadablePath();
        }

        // Java line 264-266: humanReadableUrl
        QString humanReadableUrl() const {
            return QStringLiteral("reddit.com") + humanReadablePath();
        }

        // Java lines 268-282: humanReadablePath
        // Reconstructs path from generateJsonUri() by joining path segments,
        // skipping ".json"
        QString humanReadablePath() const;

        // Java lines 284-287: browserUrl
        QString browserUrl() const;

        // Java lines 289-293: toString() -> generateJsonUri().toString()
        QString toString() const {
            return generateJsonUri();
        }

        // Java lines 295-298: toUriString
        class UriString toUriString() const;
    };

    // ---- Static parse methods (Java lines 66-220) ----

    // Java lines 66-114: tryGetRedditUri — private helper
    // Checks if a URI is a reddit URI, handling scheme "reddit",
    // reddit.app.link redirects, Google AMP redirects, and domain checks.

    // Java lines 116-199: parse(Uri) -> RedditURL
    // Tries each known URL type parser in order.
    static std::shared_ptr<RedditURL> parse(const QString &rawUri);

    // Java lines 201-209: parseProbableCommentListing
    // Falls back to UnknownCommentListURL if no match.
    static std::shared_ptr<RedditURL> parseProbableCommentListing(const QString &uri);

    // Java lines 211-220: parseProbablePostListing
    // Falls back to UnknownPostListURL if no match.
    static std::shared_ptr<RedditURL> parseProbablePostListing(const QString &uri);

private:
    // Java lines 66-114: tryGetRedditUri
    static std::optional<QString> tryGetRedditUri(const QString &uri);
};

} // namespace PinkReader
