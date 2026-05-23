#pragma once

#include "../api_strategy.hpp"

#include <QNetworkAccessManager>

namespace PinkReader {

class AnonymousJsonStrategy : public ApiStrategy {
    Q_OBJECT
public:
    explicit AnonymousJsonStrategy(QObject* parent = nullptr);

    QString name() const override { return "Anonymous JSON API (old.reddit.com)"; }
    int priority() const override { return 20; }
    bool requiresAuth() const override { return false; }

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
    QNetworkRequest makeRequest(const QUrl& url) const;
    Listing parseListing(const QJsonDocument& doc) const;

    QNetworkAccessManager* m_nam;
    QString m_userAgent;
    QString m_modhash;  // CSRF token from cookie
    QString m_cookie;   // reddit_session cookie
};

}  // namespace PinkReader
