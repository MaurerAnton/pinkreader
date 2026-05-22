#pragma once

#include <QObject>
#include <QStringList>
#include <QSet>
#include <QVariantMap>
#include <memory>
#include "../core/models.hpp"
#include "post_list_model.hpp"
#include "comment_tree_model.hpp"
#include "subreddit_list_model.hpp"

namespace PinkReader {

class RedditClient;
class CacheManager;
class AccountManager;
class ContentResolver;
class ImageCache;
class MediaLoader;
class OAuthFlow;
class OfflineDetector;
class OfflineQueue;
class ThemeManager;
class MarkdownParser;

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
    Q_PROPERTY(bool isOffline READ isOffline NOTIFY offlineChanged)
    Q_PROPERTY(int pendingActions READ pendingActions NOTIFY pendingActionsChanged)
    Q_PROPERTY(ImageCache* imageCache READ imageCache CONSTANT)
    Q_PROPERTY(MediaLoader* mediaLoader READ mediaLoader CONSTANT)
    Q_PROPERTY(OAuthFlow* oauth READ oauth CONSTANT)
    Q_PROPERTY(ThemeManager* theme READ theme CONSTANT)
    Q_PROPERTY(MarkdownParser* markdown READ markdown CONSTANT)
    Q_PROPERTY(SubredditListModel* subredditModel READ subredditModel CONSTANT)
    Q_PROPERTY(QString authUrl READ authUrl NOTIFY authUrlReady)
    Q_PROPERTY(QString profileUser READ profileUser NOTIFY profileUserChanged)
    Q_PROPERTY(int profileLinkKarma READ profileLinkKarma NOTIFY profileUserChanged)
    Q_PROPERTY(int profileCommentKarma READ profileCommentKarma NOTIFY profileUserChanged)
    Q_PROPERTY(QString profileCreated READ profileCreated NOTIFY profileUserChanged)
    Q_PROPERTY(bool showNsfw READ showNsfw WRITE setShowNsfw NOTIFY contentSettingsChanged)
    Q_PROPERTY(bool autoHideRead READ autoHideRead WRITE setAutoHideRead NOTIFY contentSettingsChanged)
    Q_PROPERTY(QStringList readPostIds READ readPostIds NOTIFY contentSettingsChanged)
    Q_PROPERTY(QVariantMap subredditInfo READ subredditInfo NOTIFY subredditInfoChanged)
    
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
    Q_INVOKABLE void submitComment(const QString& parentFullname, const QString& text);
    Q_INVOKABLE void search(const QString& query);
    Q_INVOKABLE void searchSubreddits(const QString& query, const QString& sort = "relevance");
    Q_INVOKABLE void fetchUserAbout(const QString& username);
    Q_INVOKABLE void fetchUserPosts(const QString& username, const QString& sort = "new");
    Q_INVOKABLE void login();
    Q_INVOKABLE void logout();
    Q_INVOKABLE void addSubscription(const QString& subreddit);
    Q_INVOKABLE void removeSubscription(const QString& subreddit);
    Q_INVOKABLE void clearCache();
    Q_INVOKABLE qint64 cacheSize();
    Q_INVOKABLE void handleOAuthRedirect(const QString& url);
    Q_INVOKABLE void retryOffline();
    Q_INVOKABLE void markPostRead(const QString& postId);
    Q_INVOKABLE void hidePost(const QString& postId);
    Q_INVOKABLE bool isPostRead(const QString& postId) const;
    Q_INVOKABLE void copyToClipboard(const QString& text);
    Q_INVOKABLE void shareUrl(const QString& url, const QString& title = {});

    ImageCache* imageCache() const { return m_imageCache; }
    MediaLoader* mediaLoader() const { return m_mediaLoader; }
    OAuthFlow* oauth() const { return m_oauth; }
    QString authUrl() const { return m_authUrl; }
    bool isOffline() const { return m_isOffline; }
    int pendingActions() const { return m_pendingActions; }
    ThemeManager* theme() const { return m_theme; }
    MarkdownParser* markdown() const { return m_markdown; }
    SubredditListModel* subredditModel() const { return m_subredditModel; }
    QString profileUser() const { return m_profileUser; }
    int profileLinkKarma() const { return m_profileLinkKarma; }
    int profileCommentKarma() const { return m_profileCommentKarma; }
    QString profileCreated() const { return m_profileCreated; }
    QVariantMap subredditInfo() const { return m_subredditInfo; }
    bool showNsfw() const { return m_showNsfw; }
    void setShowNsfw(bool v);
    bool autoHideRead() const { return m_autoHideRead; }
    void setAutoHideRead(bool v);
    QStringList readPostIds() const;

signals:
    void loadingChanged();
    void currentSubredditChanged();
    void currentSortChanged();
    void subscribedChanged();
    void loginStateChanged();
    void errorOccurred(const QString& message);
    void postSelected(const QString& postId, const QString& subreddit);
    void authUrlReady();
    void offlineChanged();
    void pendingActionsChanged();
    void profileUserChanged();
    void contentSettingsChanged();
    void subredditInfoChanged();
    
private:
    void initialize();
    void loadSubscriptions();
    void saveSubscriptions();
    
    RedditClient* m_client;
    CacheManager* m_cache;
    AccountManager* m_accounts;
    ContentResolver* m_contentResolver;
    ImageCache* m_imageCache;
    MediaLoader* m_mediaLoader;
    OAuthFlow* m_oauth;
    OfflineDetector* m_offlineDetector;
    OfflineQueue* m_offlineQueue;
    ThemeManager* m_theme;
    MarkdownParser* m_markdown;
    SubredditListModel* m_subredditModel;
    PostListModel* m_postModel;
    CommentTreeModel* m_commentModel;
    
    bool m_loading = false;
    QString m_currentSubreddit;
    QString m_currentSort = "hot";
    QStringList m_subscribed;
    bool m_loggedIn = false;
    QString m_currentUser;
    QString m_authUrl;
    bool m_isOffline = false;
    int m_pendingActions = 0;
    QString m_profileUser;
    int m_profileLinkKarma = 0;
    int m_profileCommentKarma = 0;
    QString m_profileCreated;
    bool m_showNsfw = true;
    bool m_autoHideRead = false;
    QSet<QString> m_readPostIds;
    QVariantMap m_subredditInfo;
};

} // namespace PinkReader
