#pragma once

#include <QObject>
#include <QStringList>
#include <memory>
#include "../core/models.hpp"
#include "post_list_model.hpp"
#include "comment_tree_model.hpp"

namespace PinkReader {

class RedditClient;
class CacheManager;
class AccountManager;
class ContentResolver;

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(PostListModel* postModel READ postModel CONSTANT)
    Q_PROPERTY(CommentTreeModel* commentModel READ commentModel CONSTANT)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString currentSubreddit READ currentSubreddit NOTIFY currentSubredditChanged)
    Q_PROPERTY(QString currentSort READ currentSort NOTIFY currentSortChanged)
    Q_PROPERTY(QStringList subscribedSubreddits READ subscribedSubreddits NOTIFY subscribedChanged)
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loginStateChanged)
    Q_PROPERTY(QString currentUser READ currentUser NOTIFY loginStateChanged)
    
public:
    explicit AppController(QObject* parent = nullptr);
    ~AppController();
    
    PostListModel* postModel() const { return m_postModel; }
    CommentTreeModel* commentModel() const { return m_commentModel; }
    bool loading() const { return m_loading; }
    QString currentSubreddit() const { return m_currentSubreddit; }
    QString currentSort() const { return m_currentSort; }
    QStringList subscribedSubreddits() const { return m_subscribed; }
    bool isLoggedIn() const { return m_loggedIn; }
    QString currentUser() const { return m_currentUser; }
    
    Q_INVOKABLE void loadFrontpage(const QString& sort = "hot");
    Q_INVOKABLE void loadSubreddit(const QString& subreddit, const QString& sort = "hot");
    Q_INVOKABLE void loadComments(const QString& postId, const QString& subreddit);
    Q_INVOKABLE void loadMore();
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void vote(const QString& fullname, int direction);
    Q_INVOKABLE void savePost(const QString& fullname);
    Q_INVOKABLE void search(const QString& query);
    Q_INVOKABLE void login();
    Q_INVOKABLE void logout();
    Q_INVOKABLE void addSubscription(const QString& subreddit);
    Q_INVOKABLE void removeSubscription(const QString& subreddit);
    Q_INVOKABLE void clearCache();
    Q_INVOKABLE qint64 cacheSize();
    
signals:
    void loadingChanged();
    void currentSubredditChanged();
    void currentSortChanged();
    void subscribedChanged();
    void loginStateChanged();
    void errorOccurred(const QString& message);
    void postSelected(const QString& postId, const QString& subreddit);
    
private:
    void initialize();
    void loadSubscriptions();
    void saveSubscriptions();
    
    RedditClient* m_client;
    CacheManager* m_cache;
    AccountManager* m_accounts;
    ContentResolver* m_contentResolver;
    PostListModel* m_postModel;
    CommentTreeModel* m_commentModel;
    
    bool m_loading = false;
    QString m_currentSubreddit;
    QString m_currentSort = "hot";
    QStringList m_subscribed;
    bool m_loggedIn = false;
    QString m_currentUser;
};

} // namespace PinkReader
