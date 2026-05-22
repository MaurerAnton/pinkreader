#include "reddit_client.hpp"
#include "api_strategy.hpp"
#include "strategies/oauth_json_strategy.hpp"
#include "strategies/anonymous_json_strategy.hpp"
#include "strategies/rss_strategy.hpp"
#include "strategies/alt_frontend_strategy.hpp"
#include "strategies/scrape_strategy.hpp"
#include "cache_manager.hpp"
#include "account_manager.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace PinkReader {

RedditClient::RedditClient(QObject* parent)
    : QObject(parent)
    , m_strategyChain(new StrategyChain(this))
    , m_cache(nullptr)
    , m_contentResolver(nullptr)
    , m_accountManager(nullptr)
{
    // Build strategy chain: try authenticated first, then fall through
    auto* oauth = new OAuthJsonStrategy(this);
    m_oauthStrategy = oauth;
    auto* anon = new AnonymousJsonStrategy(this);
    auto* rss = new RssStrategy(this);
    auto* alt = new AltFrontendStrategy(this);
    auto* scrape = new ScrapeStrategy(this);
    
    m_strategyChain->addStrategy(oauth);
    m_strategyChain->addStrategy(anon);
    m_strategyChain->addStrategy(rss);
    m_strategyChain->addStrategy(alt);
    m_strategyChain->addStrategy(scrape);
    
    // Connect signals
    for (auto* s : {static_cast<ApiStrategy*>(oauth), static_cast<ApiStrategy*>(anon), 
                     static_cast<ApiStrategy*>(rss), static_cast<ApiStrategy*>(alt)}) {
        connect(s, &ApiStrategy::networkError, this, &RedditClient::errorOccurred);
        connect(s, &ApiStrategy::rateLimited, this, [this](int seconds) {
            setError(QString("Rate limited. Retry in %1 seconds.").arg(seconds));
        });
    }
}

RedditClient::~RedditClient() = default;

void RedditClient::setLoading(bool loading) {
    if (m_loading != loading) {
        m_loading = loading;
        emit loadingChanged();
    }
}

void RedditClient::setError(const QString& error) {
    m_error = error;
    emit errorChanged();
}

void RedditClient::fetchFrontpage(const QString& sort, const QString& after) {
    setLoading(true);
    setError({});
    
    FeedRequest req;
    req.subreddit.clear();
    if (sort == "new") req.sort = SortOrder::New;
    else if (sort == "top") req.sort = SortOrder::Top;
    else if (sort == "rising") req.sort = SortOrder::Rising;
    else if (sort == "controversial") req.sort = SortOrder::Controversial;
    else if (sort == "best") req.sort = SortOrder::Best;
    else req.sort = SortOrder::Hot;
    req.after = after;
    
    m_currentRequest = req;
    
    // Check cache first
    if (m_cache && m_cache->cacheMode() == CacheManager::CacheMode::CacheFirst) {
        QString cacheKey = "_frontpage_" + sort;
        auto cached = m_cache->getCachedPosts(cacheKey);
        if (cached) {
            setLoading(false);
            emit postsReady(*cached, {});
            return;
        }
    }
    
    m_strategyChain->fetchFeed(req, [this](bool ok, auto posts, auto afterStr, auto err) {
        setLoading(false);
        if (ok) {
            if (m_cache) {
                QString cacheKey = "_frontpage_";
                m_cache->cachePosts(cacheKey, posts);
            }
            emit postsReady(posts, afterStr);
        } else {
            setError(err);
            emit errorOccurred(err);
        }
    });
}

void RedditClient::fetchSubreddit(const QString& subreddit, const QString& sort, const QString& after) {
    setLoading(true);
    setError({});
    
    FeedRequest req;
    req.subreddit = subreddit;
    if (sort == "new") req.sort = SortOrder::New;
    else if (sort == "top") req.sort = SortOrder::Top;
    else if (sort == "rising") req.sort = SortOrder::Rising;
    else if (sort == "controversial") req.sort = SortOrder::Controversial;
    else req.sort = SortOrder::Hot;
    req.after = after;
    
    m_currentRequest = req;
    
    m_strategyChain->fetchFeed(req, [this, subreddit](bool ok, auto posts, auto afterStr, auto err) {
        setLoading(false);
        if (ok) {
            if (m_cache) {
                m_cache->cachePosts(subreddit, posts);
            }
            emit postsReady(posts, afterStr);
        } else {
            setError(err);
            emit errorOccurred(err);
        }
    });
}

void RedditClient::fetchComments(const QString& postId, const QString& subreddit, const QString& sort) {
    setLoading(true);
    
    CommentRequest req;
    req.postId = postId;
    req.sort = sort;
    
    m_strategyChain->fetchComments(req, [this, postId](bool ok, auto comments, auto err) {
        setLoading(false);
        if (ok) {
            if (m_cache) {
                m_cache->cacheComments(postId, comments);
            }
            emit commentsReady(comments);
        } else {
            setError(err);
            emit errorOccurred(err);
        }
    });
}

void RedditClient::fetchPost(const QString& postId) {
    fetchComments(postId, "all", "confidence");
}

void RedditClient::vote(const QString& fullname, int direction) {
    m_strategyChain->vote(fullname, direction, [this, fullname, direction](bool ok, auto err) {
        if (ok) {
            emit voteComplete(fullname, direction);
        } else {
            setError(err);
            emit errorOccurred(err);
        }
    });
}

void RedditClient::savePost(const QString& fullname) {
    m_strategyChain->save(fullname, true, [this, fullname](bool ok, auto err) {
        if (ok) {
            emit saveComplete(fullname, true);
        } else {
            emit errorOccurred(err);
        }
    });
}

void RedditClient::unsavePost(const QString& fullname) {
    m_strategyChain->save(fullname, false, [this, fullname](bool ok, auto err) {
        if (ok) {
            emit saveComplete(fullname, false);
        } else {
            emit errorOccurred(err);
        }
    });
}

void RedditClient::hidePost(const QString& fullname) {
    if (m_oauthStrategy && m_oauthStrategy->isAvailable()) {
        m_oauthStrategy->hide(fullname, [this](bool ok, auto err) {
            if (!ok) emit errorOccurred(err);
        });
        return;
    }
    emit errorOccurred("Hide requires authentication");
}

void RedditClient::submitComment(const QString& parentFullname, const QString& text) {
    m_strategyChain->vote(parentFullname, 0, [=](bool, auto) {}); // dummy
    emit errorOccurred("Comment submission not yet implemented");
}

void RedditClient::search(const QString& query, const QString& subreddit) {
    setLoading(true);
    m_strategyChain->search(query, subreddit, [this](bool ok, auto posts, auto after, auto err) {
        setLoading(false);
        if (ok) {
            emit postsReady(posts, after);
        } else {
            setError(err);
            emit errorOccurred(err);
        }
    });
}

void RedditClient::searchSubreddits(const QString& query) {
    setLoading(true);
    QUrl url("https://www.reddit.com/subreddits/search.json");
    QUrlQuery q;
    q.addQueryItem("q", query);
    q.addQueryItem("limit", "50");
    q.addQueryItem("include_over_18", "true");
    url.setQuery(q);

    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");

    auto* nam = new QNetworkAccessManager(this);
    auto* reply = nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, nam]() {
        reply->deleteLater();
        nam->deleteLater();
        setLoading(false);

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = doc.object()["data"].toObject();
        QJsonArray children = data["children"].toArray();

        QVector<Subreddit> results;
        for (const auto& child : children) {
            QJsonObject childData = child.toObject()["data"].toObject();
            results.append(Subreddit::fromJson(childData));
        }
        emit subredditsReady(results);
    });
}

void RedditClient::fetchUserAbout(const QString& username) {
    setLoading(true);
    QUrl url("https://www.reddit.com/user/" + username + "/about.json");

    auto* nam = new QNetworkAccessManager(this);
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");

    auto* reply = nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, nam, username]() {
        reply->deleteLater();
        nam->deleteLater();
        setLoading(false);

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = doc.object()["data"].toObject();
        emit userAboutReady(
            username,
            data["link_karma"].toInt(),
            data["comment_karma"].toInt(),
            QDateTime::fromSecsSinceEpoch(
                static_cast<qint64>(data["created_utc"].toDouble()), Qt::UTC
            ).toString("yyyy-MM-dd")
        );
    });
}

void RedditClient::fetchUserPosts(const QString& username, const QString& sort) {
    setLoading(true);
    QUrl url("https://www.reddit.com/user/" + username + "/submitted.json");
    QUrlQuery q;
    q.addQueryItem("sort", sort);
    q.addQueryItem("limit", "25");
    url.setQuery(q);

    auto* nam = new QNetworkAccessManager(this);
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");

    auto* reply = nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, nam]() {
        reply->deleteLater();
        nam->deleteLater();
        setLoading(false);

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject data = doc.object()["data"].toObject();
        QJsonArray children = data["children"].toArray();

        QVector<Post> posts;
        for (const auto& child : children) {
            QJsonObject childData = child.toObject()["data"].toObject();
            posts.append(Post::fromJson(childData));
        }
        emit userPostsReady(posts, data["after"].toString());
    });
}

void RedditClient::fetchSubredditInfo(const QString& subreddit) {
    QUrl url("https://www.reddit.com/r/" + subreddit + "/about.json");

    auto* nam = new QNetworkAccessManager(this);
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");

    auto* reply = nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, nam]() {
        reply->deleteLater();
        nam->deleteLater();

        if (reply->error() != QNetworkReply::NoError) return;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        Subreddit sr = Subreddit::fromJson(doc.object()["data"].toObject());
        emit subredditInfoReady(sr);
    });
}

void RedditClient::refresh() {
    m_currentRequest.after.clear();
    fetchFrontpage(
        m_currentRequest.sort == SortOrder::Hot ? "hot" :
        m_currentRequest.sort == SortOrder::New ? "new" : "hot"
    );
}

void RedditClient::loadMore() {
    if (m_currentRequest.subreddit.isEmpty()) {
        fetchFrontpage("hot", m_currentRequest.after);
    } else {
        fetchSubreddit(m_currentRequest.subreddit, "hot", m_currentRequest.after);
    }
}

void RedditClient::updateAuthToken(const QString& token) {
    if (m_oauthStrategy) {
        m_oauthStrategy->setTokens(token, {});
    }
}

} // namespace PinkReader
