#pragma once

#include "../api_strategy.hpp"
#include <QNetworkAccessManager>

namespace PinkReader {

class OAuthJsonStrategy : public ApiStrategy {
    Q_OBJECT
public:
    explicit OAuthJsonStrategy(QObject* parent = nullptr);
    
    QString name() const override { return "OAuth JSON API"; }
    int priority() const override { return 10; }
    bool requiresAuth() const override { return true; }
    bool isAvailable() const override;
    
    void setCredentials(const QString& clientId, const QString& redirectUri);
    void setTokens(const QString& accessToken, const QString& refreshToken);
    QString accessToken() const { return m_accessToken; }
    
    void fetchFeed(const FeedRequest& request, PostCallback callback) override;
    void fetchComments(const CommentRequest& request, CommentCallback callback) override;
    void fetchSubreddit(const QString& subreddit, SubredditCallback callback) override;
    void fetchMoreChildren(const QString& linkId, const QStringList& children, StringCallback callback) override;
    void vote(const QString& fullname, int direction, ApiCallback callback) override;
    void save(const QString& fullname, bool save, ApiCallback callback) override;
    void hide(const QString& fullname, ApiCallback callback) override;
    void submitComment(const QString& parentFullname, const QString& text, ApiCallback callback) override;
    void search(const QString& query, const QString& subreddit, PostCallback callback) override;
    
private:
    QNetworkRequest makeRequest(const QString& path) const;
    void handleReply(QNetworkReply* reply, std::function<void(const QJsonDocument&)> onSuccess, ApiCallback onError);
    
    QNetworkAccessManager* m_nam;
    QString m_accessToken;
    QString m_refreshToken;
    QString m_clientId;
    QString m_redirectUri;
    QString m_userAgent;
};

} // namespace PinkReader
