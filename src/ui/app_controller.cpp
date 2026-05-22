#include "app_controller.hpp"
#include "post_list_model.hpp"
#include "comment_tree_model.hpp"
#include "../core/reddit_client.hpp"
#include "../core/cache_manager.hpp"
#include "../core/account_manager.hpp"
#include "../core/content_resolver.hpp"
#include "../core/image_cache.hpp"
#include "../core/media_loader.hpp"
#include "../core/oauth_flow.hpp"
#include "../core/offline_detector.hpp"
#include "../core/offline_queue.hpp"
#include "../core/theme_manager.hpp"
#include "../core/markdown_parser.hpp"

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

namespace PinkReader {

AppController::AppController(QObject* parent)
    : QObject(parent)
    , m_postModel(new PostListModel(this))
    , m_commentModel(new CommentTreeModel(this))
    , m_oauth(nullptr)
    , m_offlineDetector(nullptr)
    , m_offlineQueue(nullptr)
    , m_theme(nullptr)
    , m_markdown(nullptr)
    , m_subredditModel(nullptr)
{
    initialize();
}

AppController::~AppController() = default;

void AppController::initialize() {
    // Setup cache
    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                        + "/cache.db";
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    
    m_cache = new CacheManager(cachePath, this);
    m_cache->initialize();
    
    // Setup account manager
    m_accounts = new AccountManager(this);
    
    // Setup content resolver
    m_contentResolver = new ContentResolver(this);

    // Setup theme
    m_theme = new ThemeManager(this);

    // Setup markdown parser
    m_markdown = new MarkdownParser(this);

    // Setup subreddit search model
    m_subredditModel = new SubredditListModel(this);

    // Setup image cache
    QString imageCachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                             + "/images";
    m_imageCache = new ImageCache(imageCachePath, this);

    // Setup media loader
    m_mediaLoader = new MediaLoader(this);

    // Setup OAuth flow
    m_oauth = new OAuthFlow(this);
    connect(m_oauth, &OAuthFlow::authUrlReady, this, [this]() {
        m_authUrl = m_oauth->authUrl();
        emit authUrlReady();
    });
    connect(m_oauth, &OAuthFlow::authSuccess, this, [this](const QString& accessToken,
                                                          const QString& refreshToken,
                                                          int expiresIn) {
        m_client->updateAuthToken(accessToken);
        Account acc;
        acc.username = "user"; // Will be updated after fetching identity
        acc.accessToken = accessToken;
        acc.refreshToken = refreshToken;
        acc.expiresIn = expiresIn;
        acc.isAnonymous = false;
        m_accounts->addAccount(acc);
        m_loggedIn = true;
        // Fetch username
        // For now, set a placeholder
        m_currentUser = "logged_in";
        emit loginStateChanged();
    });
    connect(m_oauth, &OAuthFlow::authError, this, [this](const QString& err) {
        emit errorOccurred("Login failed: " + err);
    });

    // Setup offline detector
    m_offlineDetector = new OfflineDetector(this);
    connect(m_offlineDetector, &OfflineDetector::onlineChanged, this, [this](bool online) {
        m_isOffline = !online;
        emit offlineChanged();
        if (online) {
            // Flush pending actions when back online
            if (m_offlineQueue) m_offlineQueue->flush();
            // Refresh feed
            refresh();
        }
    });

    // Setup offline queue
    m_offlineQueue = new OfflineQueue(this);
    m_offlineQueue->setActionHandler([this](const PendingAction& action, auto done) {
        switch (action.type) {
        case PendingAction::Vote:
            m_client->vote(action.fullname, action.direction);
            break;
        case PendingAction::Save:
            action.direction ? m_client->savePost(action.fullname)
                             : m_client->unsavePost(action.fullname);
            break;
        case PendingAction::Hide:
            m_client->hidePost(action.fullname);
            break;
        default:
            break;
        }
        done(true);
    });
    connect(m_offlineQueue, &OfflineQueue::queueChanged, this, [this]() {
        m_pendingActions = m_offlineQueue->pendingCount();
        emit pendingActionsChanged();
    });

    // Setup API client
    m_client = new RedditClient(this);
    m_client->setCacheManager(m_cache);
    m_client->setAccountManager(m_accounts);
    m_client->setContentResolver(m_contentResolver);
    
    // Connect signals
    connect(m_client, &RedditClient::postsReady, this, [this](const QVector<Post>& posts, const QString& after) {
        m_postModel->setPosts(posts, after);
        m_loading = false;
        emit loadingChanged();
    });
    
    connect(m_client, &RedditClient::commentsReady, this, [this](const QVector<Comment>& comments) {
        m_commentModel->setComments(comments);
        m_loading = false;
        emit loadingChanged();
    });
    
    connect(m_client, &RedditClient::errorOccurred, this, [this](const QString& err) {
        m_loading = false;
        emit loadingChanged();
        emit errorOccurred(err);
    });
    
    connect(m_client, &RedditClient::loadingChanged, this, [this]() {
        m_loading = m_client->loading();
        emit loadingChanged();
    });

    connect(m_client, &RedditClient::subredditsReady, this, [this](const QVector<Subreddit>& subs) {
        m_subredditModel->setSubreddits(subs);
        m_loading = false;
        emit loadingChanged();
    });

    connect(m_accounts, &AccountManager::accountsChanged, this, [this]() {
        auto* acc = m_accounts->activeAccount();
        m_loggedIn = acc && !acc->isAnonymous;
        m_currentUser = m_loggedIn ? acc->username : QString{};
        emit loginStateChanged();
    });
    
    loadSubscriptions();
}

void AppController::loadFrontpage(const QString& sort) {
    m_loading = true;
    emit loadingChanged();
    m_currentSubreddit.clear();
    m_currentSort = sort;
    emit currentSubredditChanged();

    if (m_isOffline) {
        auto cached = m_cache->getCachedPosts("_frontpage_" + sort);
        if (cached) {
            m_postModel->setPosts(*cached, {});
        }
        m_loading = false;
        emit loadingChanged();
        return;
    }
    m_client->fetchFrontpage(sort);
}

void AppController::loadSubreddit(const QString& subreddit, const QString& sort) {
    m_loading = true;
    emit loadingChanged();
    m_currentSubreddit = subreddit;
    m_currentSort = sort;
    emit currentSubredditChanged();
    emit currentSortChanged();

    if (m_isOffline) {
        auto cached = m_cache->getCachedPosts(subreddit + "_" + sort);
        if (cached) {
            m_postModel->setPosts(*cached, {});
        }
        m_loading = false;
        emit loadingChanged();
        return;
    }
    m_client->fetchSubreddit(subreddit, sort);
}

void AppController::loadComments(const QString& postId, const QString& subreddit) {
    m_loading = true;
    emit loadingChanged();
    m_client->fetchComments(postId, subreddit);
}

void AppController::loadMore() {
    if (m_loading) return;
    m_loading = true;
    emit loadingChanged();
    m_client->loadMore();
}

void AppController::refresh() {
    m_client->refresh();
}

void AppController::vote(const QString& fullname, int direction) {
    if (m_isOffline) {
        m_offlineQueue->enqueueVote(fullname, direction);
        emit errorOccurred("Vote queued for when online");
        return;
    }
    m_client->vote(fullname, direction);
}

void AppController::savePost(const QString& fullname) {
    if (m_isOffline) {
        m_offlineQueue->enqueueSave(fullname, true);
        emit errorOccurred("Save queued for when online");
        return;
    }
    m_client->savePost(fullname);
}

void AppController::submitComment(const QString& parentFullname, const QString& text) {
    if (!m_loggedIn) {
        emit errorOccurred("Log in to comment");
        return;
    }
    if (text.trimmed().isEmpty()) return;
    m_client->submitComment(parentFullname, text);
}

void AppController::search(const QString& query) {
    m_loading = true;
    emit loadingChanged();
    m_client->search(query, m_currentSubreddit);
}

void AppController::searchSubreddits(const QString& query, const QString& sort) {
    m_loading = true;
    m_subredditModel->setSortBy(sort);
    emit loadingChanged();
    m_client->searchSubreddits(query);
}

void AppController::retryOffline() {
    m_offlineDetector->checkNow();
    if (!m_isOffline) {
        refresh();
    }
}

void AppController::login() {
    if (m_oauth) {
        m_oauth->startAuth();
    }
}

void AppController::logout() {
    m_accounts->removeAccount(m_currentUser);
    m_client->updateAuthToken({});
    m_loggedIn = false;
    m_currentUser.clear();
    emit loginStateChanged();
}

void AppController::handleOAuthRedirect(const QString& url) {
    if (m_oauth) {
        m_oauth->handleRedirect(QUrl(url));
    }
}

void AppController::addSubscription(const QString& subreddit) {
    if (!subreddit.isEmpty() && !m_subscribed.contains(subreddit)) {
        m_subscribed.append(subreddit);
        m_subscribed.sort();
        saveSubscriptions();
        emit subscribedChanged();
    }
}

void AppController::removeSubscription(const QString& subreddit) {
    m_subscribed.removeAll(subreddit);
    saveSubscriptions();
    emit subscribedChanged();
}

void AppController::clearCache() {
    if (m_cache) m_cache->clearAll();
}

qint64 AppController::cacheSize() {
    return m_cache ? m_cache->cacheSize() : 0;
}

void AppController::loadSubscriptions() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                   + "/subscriptions.json";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    
    for (const auto& val : doc.array()) {
        m_subscribed.append(val.toString());
    }
}

void AppController::saveSubscriptions() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                   + "/subscriptions.json";
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    
    QJsonArray arr;
    for (const auto& s : m_subscribed) arr.append(s);
    
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(arr).toJson());
        file.close();
    }
}

} // namespace PinkReader
