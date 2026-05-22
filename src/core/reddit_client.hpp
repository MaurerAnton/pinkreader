#pragma once

#include <QObject>
#include <memory>
#include "models.hpp"
#include "api_strategy.hpp"
#include "cache_manager.hpp"
#include "content_resolver.hpp"

namespace PinkReader {

class StrategyChain;
class AccountManager;

class RedditClient : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    
public:
    explicit RedditClient(QObject* parent = nullptr);
    ~RedditClient();
    
    bool loading() const { return m_loading; }
    QString error() const { return m_error; }
    
    void setAccountManager(AccountManager* mgr) { m_accountManager = mgr; }
    void setCacheManager(CacheManager* cache) { m_cache = cache; }
    void setContentResolver(ContentResolver* resolver) { m_contentResolver = resolver; }
    void updateAuthToken(const QString& token);
    
    Q_INVOKABLE void fetchFrontpage(const QString& sort = "hot", const QString& after = "");
    Q_INVOKABLE void fetchSubreddit(const QString& subreddit, const QString& sort = "hot", const QString& after = "");
    Q_INVOKABLE void fetchComments(const QString& postId, const QString& subreddit, const QString& sort = "confidence");
    Q_INVOKABLE void fetchPost(const QString& postId);
    Q_INVOKABLE void vote(const QString& fullname, int direction); // 1=up, -1=down, 0=cancel
    Q_INVOKABLE void savePost(const QString& fullname);
    Q_INVOKABLE void unsavePost(const QString& fullname);
    Q_INVOKABLE void hidePost(const QString& fullname);
    Q_INVOKABLE void submitComment(const QString& parentFullname, const QString& text);
    Q_INVOKABLE void search(const QString& query, const QString& subreddit = "");
    Q_INVOKABLE void searchSubreddits(const QString& query);
    Q_INVOKABLE void fetchUserAbout(const QString& username);
    Q_INVOKABLE void fetchUserPosts(const QString& username, const QString& sort = "new");
    Q_INVOKABLE void fetchSubredditInfo(const QString& subreddit);
    Q_INVOKABLE void submitPost(const QString& kind, const QString& subreddit,
                                const QString& title, const QString& url,
                                const QString& text, const QString& flair);
    Q_INVOKABLE void fetchMultireddit(const QString& username, const QString& multiname);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void loadMore();
    
signals:
    void postsReady(const QVector<Post>& posts, const QString& after);
    void commentsReady(const QVector<Comment>& comments);
    void subredditInfoReady(const Subreddit& info);
    void submitComplete(bool success, const QString& error);
    void subredditsReady(const QVector<Subreddit>& subreddits);
    void userAboutReady(const QString& username, int linkKarma, int commentKarma, const QString& created);
    void userPostsReady(const QVector<Post>& posts, const QString& after);
    void voteComplete(const QString& fullname, int direction);
    void saveComplete(const QString& fullname, bool saved);
    void loadingChanged();
    void errorChanged();
    void errorOccurred(const QString& message);
    
private:
    void setLoading(bool loading);
    void setError(const QString& error);
    
    StrategyChain* m_strategyChain;
    CacheManager* m_cache;
    ContentResolver* m_contentResolver;
    AccountManager* m_accountManager;
    class OAuthJsonStrategy* m_oauthStrategy;
    FeedRequest m_currentRequest;
    bool m_loading = false;
    QString m_error;
};

} // namespace PinkReader
