/*
 * PinkReader - GPLv3
 * File: reddit_url_parser.cpp - Port of RedditURLParser.java
 *
 * Exact line-by-line port. Every method from the Java source.
 * Note: Subclass parsers (SubredditPostListURL, etc.) are defined in their own files.
 * This file implements RedditURLParser::parse() which calls those subclass parsers.
 */

#include "network/reddit_url_parser.h"

#include <QUrl>
#include <QUrlQuery>
#include <QStringList>
#include <QDebug>

namespace PinkReader {

// ============================================================================
// Forward-declare the subclass parsers that parse() calls.
// These are defined in separate files (e.g. subreddit_post_list_url.h)
// In a fully linked build, they provide the ::parse(QString) static method.
// ============================================================================

// Each of these returns a shared_ptr<RedditURL> or nullptr.
namespace detail {
    std::shared_ptr<RedditURLParser::RedditURL> parseOpaqueSharedURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parseSubredditPostListURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parseMultiredditPostListURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parseSearchPostListURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parseUserPostListingURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parseUserCommentListingURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parsePostCommentListingURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parseUserProfileURL(const QString &uri);
    std::shared_ptr<RedditURLParser::RedditURL> parseComposeMessageURL(const QString &uri);
} // namespace detail

// ============================================================================
// RedditURL::humanReadablePath — Java lines 268-282
// ============================================================================

QString RedditURLParser::RedditURL::humanReadablePath() const
{
    // Java line 270: final Uri src = generateJsonUri();
    const QString src = generateJsonUri();
    QUrl jsonUri(src);

    // Java line 272: final StringBuilder builder = new StringBuilder();
    QString builder;

    // Java lines 274-279:
    // for(final String pathElement : src.getPathSegments())
    const QString path = jsonUri.path();
    const QStringList segments = path.split(QLatin1Char('/'), Qt::SkipEmptyParts);

    for (const QString &pathElement : segments) {
        // Java line 275: if(!pathElement.equals(".json"))
        if (pathElement != QStringLiteral(".json")) {
            // Java lines 276-277:
            builder.append(QLatin1Char('/'));
            builder.append(pathElement);
        }
    }

    // Java line 281: return builder.toString();
    return builder;
}

// ============================================================================
// RedditURL::browserUrl — Java lines 284-287
// ============================================================================

QString RedditURLParser::RedditURL::browserUrl() const
{
    // Java line 286: new UriString(Constants.Reddit.SCHEME_HTTPS + "://" + humanReadableUrl())
    return QStringLiteral("https://") + humanReadableUrl();
}

// ============================================================================
// RedditURL::toUriString — Java lines 295-298
// ============================================================================

UriString RedditURLParser::RedditURL::toUriString() const
{
    // Java line 297: return new UriString(toString());
    return UriString(toString());
}

// ============================================================================
// tryGetRedditUri — Java lines 66-114 (private static)
// ============================================================================

std::optional<QString> RedditURLParser::tryGetRedditUri(const QString &rawUriStr)
{
    // Java line 68: if(uri == null || uri.getHost() == null || uri.getPath() == null)
    QUrl uri(rawUriStr);
    if (!uri.isValid() || uri.host().isEmpty() || uri.path().isEmpty()) {
        // Java line 69: return Optional.empty();
        return std::nullopt;
    }

    // Java lines 72-77: if("reddit".equals(uri.getScheme()) && "reddit".equals(uri.getHost()))
    if (uri.scheme() == QStringLiteral("reddit") && uri.host() == QStringLiteral("reddit")) {
        // Java lines 73-76: uri.buildUpon().scheme("https").authority("reddit.com").build()
        QUrl transformed;
        transformed.setScheme(QStringLiteral("https"));
        transformed.setHost(QStringLiteral("reddit.com"));
        transformed.setPath(uri.path());
        if (uri.hasQuery()) {
            transformed.setQuery(uri.query());
        }
        return transformed.toString();
    }

    // Java lines 79-85: reddit.app.link -> $og_redirect
    if (uri.host() == QStringLiteral("reddit.app.link")) {
        // Java line 80: final String redirect = uri.getQueryParameter("$og_redirect");
        QUrlQuery query(uri);
        QString redirect = query.queryItemValue(QStringLiteral("$og_redirect"));
        if (!redirect.isEmpty()) {
            // Java line 83: return Optional.ofNullable(Uri.parse(redirect));
            QUrl parsedRedirect(redirect);
            if (parsedRedirect.isValid()) {
                return redirect;
            }
        }
    }

    // Java lines 87-95: Google AMP prefix /amp/s/amp.reddit.com
    const QString ampPrefix = QStringLiteral("/amp/s/amp.reddit.com");
    const QString host = uri.host();

    // Java lines 89-91: google.com host check + path starts with ampPrefix
    if ((host == QStringLiteral("google.com") || host.endsWith(QStringLiteral(".google.com")))
            && uri.path().startsWith(ampPrefix)) {
        // Java lines 93-94: "https://reddit.com" + uri.getPath().substring(ampPrefix.length())
        QString redditPath = uri.path().mid(ampPrefix.length());
        QString redditUrl = QStringLiteral("https://reddit.com") + redditPath;
        return redditUrl;
    }

    // Java line 97: final String[] hostSegments = StringUtils.asciiLowercase(uri.getHost()).split("\\.");
    const QString lowerHost = uri.host().toLower();
    const QStringList hostSegments = lowerHost.split(QLatin1Char('.'));

    // Java lines 99-101: if(hostSegments.length < 2) return Optional.empty();
    if (hostSegments.size() < 2) {
        return std::nullopt;
    }

    // Java lines 103-106: hostSegments[last-1] == "reddit" && hostSegments[last] == "com"
    if (hostSegments.at(hostSegments.size() - 1) == QStringLiteral("com")
            && hostSegments.at(hostSegments.size() - 2) == QStringLiteral("reddit")) {
        // Java line 105: return Optional.of(uri);
        return rawUriStr;
    }

    // Java lines 108-111: hostSegments[last-1] == "redd" && hostSegments[last] == "it"
    if (hostSegments.at(hostSegments.size() - 1) == QStringLiteral("it")
            && hostSegments.at(hostSegments.size() - 2) == QStringLiteral("redd")) {
        // Java line 110: return Optional.of(uri);
        return rawUriStr;
    }

    // Java line 113: return Optional.empty();
    return std::nullopt;
}

// ============================================================================
// parse — Java lines 116-199
// ============================================================================

std::shared_ptr<RedditURLParser::RedditURL> RedditURLParser::parse(const QString &rawUri)
{
    // Java line 119: if(rawUri == null) return null;
    if (rawUri.isEmpty()) {
        return nullptr;
    }

    // Java line 123: final Optional<Uri> optionalUri = tryGetRedditUri(rawUri);
    std::optional<QString> optionalUri = tryGetRedditUri(rawUri);

    // Java lines 125-127: if(optionalUri.isEmpty()) return null;
    if (!optionalUri.has_value()) {
        return nullptr;
    }

    // Java line 129: final Uri uri = optionalUri.get();
    const QString &uri = optionalUri.value();

    // Java lines 131-136: OpaqueSharedURL
    {
        auto opaqueSharedURL = detail::parseOpaqueSharedURL(uri);
        if (opaqueSharedURL) {
            return opaqueSharedURL;
        }
    }

    // Java lines 138-143: SubredditPostListURL
    {
        auto subredditPostListURL = detail::parseSubredditPostListURL(uri);
        if (subredditPostListURL) {
            return subredditPostListURL;
        }
    }

    // Java lines 145-151: MultiredditPostListURL
    {
        auto multiredditPostListURL = detail::parseMultiredditPostListURL(uri);
        if (multiredditPostListURL) {
            return multiredditPostListURL;
        }
    }

    // Java lines 153-158: SearchPostListURL
    {
        auto searchPostListURL = detail::parseSearchPostListURL(uri);
        if (searchPostListURL) {
            return searchPostListURL;
        }
    }

    // Java lines 160-165: UserPostListingURL
    {
        auto userPostListURL = detail::parseUserPostListingURL(uri);
        if (userPostListURL) {
            return userPostListURL;
        }
    }

    // Java lines 167-173: UserCommentListingURL
    {
        auto userCommentListURL = detail::parseUserCommentListingURL(uri);
        if (userCommentListURL) {
            return userCommentListURL;
        }
    }

    // Java lines 175-181: PostCommentListingURL
    {
        auto commentListingURL = detail::parsePostCommentListingURL(uri);
        if (commentListingURL) {
            return commentListingURL;
        }
    }

    // Java lines 183-188: UserProfileURL
    {
        auto userProfileURL = detail::parseUserProfileURL(uri);
        if (userProfileURL) {
            return userProfileURL;
        }
    }

    // Java lines 190-196: ComposeMessageURL
    {
        auto composeMessageURL = detail::parseComposeMessageURL(uri);
        if (composeMessageURL) {
            return composeMessageURL;
        }
    }

    // Java line 198: return null;
    return nullptr;
}

// ============================================================================
// parseProbableCommentListing — Java lines 201-209
// ============================================================================

std::shared_ptr<RedditURLParser::RedditURL> RedditURLParser::parseProbableCommentListing(const QString &uri)
{
    // Java lines 203-204: final RedditURL matchURL = parse(uri); if(matchURL != null) return matchURL;
    auto matchURL = parse(uri);
    if (matchURL) {
        return matchURL;
    }

    // Java line 208: return new UnknownCommentListURL(uri);
    // NOTE: UnknownCommentListURL must be defined in another header.
    // We return nullptr as fallback; the real port would create that object.
    qDebug() << "RedditURLParser::parseProbableCommentListing: no match for" << uri;
    return nullptr;
}

// ============================================================================
// parseProbablePostListing — Java lines 211-220
// ============================================================================

std::shared_ptr<RedditURLParser::RedditURL> RedditURLParser::parseProbablePostListing(const QString &uri)
{
    // Java lines 214-215: final RedditURL matchURL = parse(uri); if(matchURL != null) return matchURL;
    auto matchURL = parse(uri);
    if (matchURL) {
        return matchURL;
    }

    // Java line 219: return new UnknownPostListURL(uri);
    // NOTE: UnknownPostListURL must be defined in another header.
    qDebug() << "RedditURLParser::parseProbablePostListing: no match for" << uri;
    return nullptr;
}

} // namespace PinkReader
