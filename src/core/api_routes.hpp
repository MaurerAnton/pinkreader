#pragma once

#include <QMap>
#include <QString>
#include <QStringList>

namespace PinkReader {
namespace ApiRoutes {

// Reddit OAuth endpoints
constexpr auto OAUTH_BASE = "https://oauth.reddit.com";
constexpr auto OAUTH_TOKEN = "https://www.reddit.com/api/v1/access_token";
constexpr auto OAUTH_AUTHORIZE = "https://www.reddit.com/api/v1/authorize";

// Anonymous JSON endpoints
constexpr auto ANON_BASE = "https://old.reddit.com";
constexpr auto ANON_JSON_SUFFIX = ".json";

// RSS endpoints
constexpr auto RSS_BASE = "https://www.reddit.com";
constexpr auto RSS_SUFFIX = ".rss";

// Alternative privacy frontends (FOSS proxies)
// These bypass rate limits and don't require API keys
const inline QStringList ALT_FRONTENDS = {
    "https://teddit.net",
    "https://libreddit.spike.codes",
    "https://libreddit.kavin.rocks",
    "https://libredd.it",
    "https://r.betweentwobra.in",
    "https://libreddit.foss.wtf",
    "https://safereddit.com",
    "https://lr.mint.lgbt",
    "https://reddit.artemislena.eu",
    "https://redlib.kylrth.com",
    "https://redlib.matthew.science",
    "https://redlib.funami.tech",
    "https://redlib.nadeko.net",
    "https://redlib.perennialte.ch",
    "https://redlib.tux.pizza",
    "https://redlib.xn--cookie-dsa.limo",
    "https://rl.bloat.cat",
};

// Old Reddit mobile (can scrape if needed)
constexpr auto OLD_MOBILE_BASE = "https://old.reddit.com/.mobile";

// oEmbed for media preview
constexpr auto OEMBED_URL = "https://www.reddit.com/oembed";

// Endpoint builders
inline QString frontpage(SortOrder sort, TimeRange range, const QString& after = {}, int limit = 25) {
    Q_UNUSED(range)
    QString t;
    switch (sort) {
        case SortOrder::Hot:
            t = "hot";
            break;
        case SortOrder::New:
            t = "new";
            break;
        case SortOrder::Rising:
            t = "rising";
            break;
        case SortOrder::Top:
            t = "top";
            break;
        case SortOrder::Controversial:
            t = "controversial";
            break;
        case SortOrder::Best:
            t = "best";
            break;
    }
    return QString("%1/%2/%3").arg(ApiRoutes::ANON_BASE).arg(t).arg(ApiRoutes::ANON_JSON_SUFFIX);
}

inline QString subredditPosts(const QString& subreddit, SortOrder sort, const QString& after = {}, int limit = 25) {
    QString t;
    switch (sort) {
        case SortOrder::Hot:
            t = "hot";
            break;
        case SortOrder::New:
            t = "new";
            break;
        case SortOrder::Rising:
            t = "rising";
            break;
        case SortOrder::Top:
            t = "top";
            break;
        case SortOrder::Controversial:
            t = "controversial";
            break;
        case SortOrder::Best:
            t = "best";
            break;
    }
    return QString("%1/r/%2/%3%4").arg(ApiRoutes::ANON_BASE).arg(subreddit).arg(t).arg(ApiRoutes::ANON_JSON_SUFFIX);
}

inline QString comments(const QString& subreddit, const QString& postId, const QString& commentId = {},
                        int limit = 200) {
    QString url = QString("%1/r/%2/comments/%3%4")
                      .arg(ApiRoutes::ANON_BASE)
                      .arg(subreddit)
                      .arg(postId)
                      .arg(ApiRoutes::ANON_JSON_SUFFIX);
    if (!commentId.isEmpty())
        url += QString("?comment=%1").arg(commentId);
    return url;
}

inline QString subredditAbout(const QString& subreddit) {
    return QString("%1/r/%2/about%3").arg(ApiRoutes::ANON_BASE).arg(subreddit).arg(ApiRoutes::ANON_JSON_SUFFIX);
}

inline QString search(const QString& query, const QString& subreddit = {}, SortOrder sort = SortOrder::Relevance,
                      const QString& after = {}, int limit = 25) {
    QString base =
        subreddit.isEmpty()
            ? QString("%1/search%2").arg(ApiRoutes::ANON_BASE).arg(ApiRoutes::ANON_JSON_SUFFIX)
            : QString("%1/r/%2/search%3").arg(ApiRoutes::ANON_BASE).arg(subreddit).arg(ApiRoutes::ANON_JSON_SUFFIX);
    QString params = QString("?q=%1&limit=%2").arg(query).arg(limit);
    if (!after.isEmpty())
        params += "&after=" + after;
    switch (sort) {
        case SortOrder::New:
            params += "&sort=new";
            break;
        case SortOrder::Top:
            params += "&sort=top";
            break;
        case SortOrder::Hot:
            params += "&sort=hot";
            break;
        default:
            params += "&sort=relevance";
            break;
    }
    return base + params;
}

inline QString userAbout(const QString& username) {
    return QString("%1/user/%2/about%3").arg(ApiRoutes::ANON_BASE).arg(username).arg(ApiRoutes::ANON_JSON_SUFFIX);
}

inline QString userPosts(const QString& username, SortOrder sort = SortOrder::New, const QString& after = {},
                         int limit = 25) {
    return QString("%1/user/%2%3").arg(ApiRoutes::ANON_BASE).arg(username).arg(ApiRoutes::ANON_JSON_SUFFIX);
}

inline QString subredditsDefault() {
    return QString("%1/subreddits/default%2").arg(ApiRoutes::ANON_BASE).arg(ApiRoutes::ANON_JSON_SUFFIX);
}

inline QString subredditsPopular() {
    return QString("%1/subreddits/popular%2").arg(ApiRoutes::ANON_BASE).arg(ApiRoutes::ANON_JSON_SUFFIX);
}

// RSS builders
inline QString rssFrontpage() {
    return QString("%1/%2").arg(ApiRoutes::RSS_BASE).arg(ApiRoutes::RSS_SUFFIX);
}

inline QString rssSubreddit(const QString& subreddit) {
    return QString("%1/r/%2/%3").arg(ApiRoutes::RSS_BASE).arg(subreddit).arg(ApiRoutes::RSS_SUFFIX);
}

// Alternative frontend
inline QString altFeed(const QString& instance, const QString& subreddit = {}, SortOrder sort = SortOrder::Hot) {
    QString path = subreddit.isEmpty() ? "/" : QString("/r/%1").arg(subreddit);
    QString t;
    switch (sort) {
        case SortOrder::New:
            t = "new";
            break;
        case SortOrder::Top:
            t = "top";
            break;
        case SortOrder::Rising:
            t = "rising";
            break;
        case SortOrder::Controversial:
            t = "controversial";
            break;
        default:
            t = "hot";
            break;
    }
    return QString("%1%2/%3.json").arg(instance).arg(path).arg(t);
}

}  // namespace ApiRoutes
}  // namespace PinkReader
