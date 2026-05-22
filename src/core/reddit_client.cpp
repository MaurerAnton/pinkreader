#include "reddit_client.hpp"
#include "api_strategy.hpp"
#include "strategies/oauth_json_strategy.hpp"
#include "strategies/anonymous_json_strategy.hpp"
#include "strategies/rss_strategy.hpp"
#include "strategies/alt_frontend_strategy.hpp"
#include "strategies/scrape_strategy.hpp"
#include "cache_manager.hpp"
#include "account_manager.hpp"

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
    // Only available via OAuth
    for (auto* s : {static_cast<ApiStrategy*>(findChild<OAuthJsonStrategy*>())}) {
        if (s) {
            s->hide(fullname, [this](bool ok, auto err) {
                if (!ok) emit errorOccurred(err);
            });
            return;
        }
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

} // namespace PinkReader
