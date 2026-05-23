#include "api_strategy.hpp"

#include <algorithm>
#include <utility>

namespace PinkReader {

StrategyChain::StrategyChain(QObject* parent) : QObject(parent) {}

void StrategyChain::addStrategy(ApiStrategy* strategy) {
    m_strategies.append(strategy);
    // Sort by priority
    std::sort(m_strategies.begin(), m_strategies.end(),
              [](ApiStrategy* a, ApiStrategy* b) { return a->priority() < b->priority(); });
}

void StrategyChain::setAccountToken(const QString& token) {
    m_token = token;
}

void StrategyChain::fetchFeed(const FeedRequest& request, PostCallback callback) {
    if (m_strategies.isEmpty()) {
        callback(false, {}, {}, "No API strategies available");
        return;
    }
    m_strategies[0]->fetchFeed(request,
                               [this, request, callback, idx = 0](bool ok, auto posts, auto after, auto err) mutable {
                                   if (ok) {
                                       callback(true, posts, after, err);
                                   } else if (idx + 1 < m_strategies.size()) {
                                       m_strategies[idx + 1]->fetchFeed(request, callback);
                                   } else {
                                       callback(false, {}, {}, err);
                                   }
                               });
}

void StrategyChain::fetchComments(const CommentRequest& request, CommentCallback callback) {
    if (m_strategies.isEmpty()) {
        callback(false, {}, "No API strategies available");
        return;
    }
    m_strategies[0]->fetchComments(request,
                                   [this, request, callback, idx = 0](bool ok, auto comments, auto err) mutable {
                                       if (ok) {
                                           callback(true, comments, err);
                                       } else if (idx + 1 < m_strategies.size()) {
                                           m_strategies[idx + 1]->fetchComments(request, callback);
                                       } else {
                                           callback(false, {}, err);
                                       }
                                   });
}

void StrategyChain::fetchSubreddit(const QString& subreddit, SubredditCallback callback) {
    if (m_strategies.isEmpty()) {
        callback(false, Subreddit{}, "No API strategies available");
        return;
    }
    m_strategies[0]->fetchSubreddit(subreddit,
                                    [this, subreddit, callback, idx = 0](bool ok, auto sr, auto err) mutable {
                                        if (ok) {
                                            callback(true, sr, err);
                                        } else if (idx + 1 < m_strategies.size()) {
                                            m_strategies[idx + 1]->fetchSubreddit(subreddit, callback);
                                        } else {
                                            callback(false, Subreddit{}, err);
                                        }
                                    });
}

void StrategyChain::vote(const QString& fullname, int direction, ApiCallback callback) {
    for (auto* s : m_strategies) {
        if (!s->requiresAuth())
            continue;
        s->vote(fullname, direction, callback);
        return;
    }
    callback(false, "No authenticated strategy available");
}

void StrategyChain::save(const QString& fullname, bool saveFlag, ApiCallback callback) {
    for (auto* s : m_strategies) {
        if (!s->requiresAuth())
            continue;
        s->save(fullname, saveFlag, callback);
        return;
    }
    callback(false, "No authenticated strategy available");
}

void StrategyChain::search(const QString& query, const QString& subreddit, PostCallback callback) {
    m_strategies[0]->search(query, subreddit, callback);
}

}  // namespace PinkReader
