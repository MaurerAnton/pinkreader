#pragma once

#include "models.hpp"

#include <QNetworkReply>
#include <QObject>
#include <functional>

namespace PinkReader {

using ApiCallback = std::function<void(bool success, const QString& error)>;
using ListingCallback = std::function<void(bool success, const Listing& listing, const QString& error)>;
using PostCallback =
    std::function<void(bool success, const QVector<Post>& posts, const QString& after, const QString& error)>;
using CommentCallback = std::function<void(bool success, const QVector<Comment>& comments, const QString& error)>;
using SubredditCallback = std::function<void(bool success, const Subreddit& subreddit, const QString& error)>;
using StringCallback = std::function<void(bool success, const QString& data, const QString& error)>;

class ApiStrategy : public QObject {
    Q_OBJECT
public:
    explicit ApiStrategy(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~ApiStrategy() = default;

    virtual QString name() const = 0;
    virtual int priority() const = 0;  // lower = try first
    virtual bool requiresAuth() const = 0;
    virtual bool isAvailable() const { return true; }

    virtual void fetchFeed(const FeedRequest& request, PostCallback callback) = 0;
    virtual void fetchComments(const CommentRequest& request, CommentCallback callback) = 0;
    virtual void fetchSubreddit(const QString& subreddit, SubredditCallback callback) = 0;
    virtual void fetchMoreChildren(const QString& linkId, const QStringList& children, StringCallback callback) = 0;
    virtual void vote(const QString& fullname, int direction, ApiCallback callback) = 0;
    virtual void save(const QString& fullname, bool save, ApiCallback callback) = 0;
    virtual void hide(const QString& fullname, ApiCallback callback) = 0;
    virtual void submitComment(const QString& parentFullname, const QString& text, ApiCallback callback) = 0;
    virtual void search(const QString& query, const QString& subreddit, PostCallback callback) = 0;

signals:
    void rateLimited(int retryAfterSeconds);
    void networkError(const QString& message);
};

class StrategyChain : public QObject {
    Q_OBJECT
public:
    explicit StrategyChain(QObject* parent = nullptr);

    void addStrategy(ApiStrategy* strategy);
    void setAccountToken(const QString& token);

    void fetchFeed(const FeedRequest& request, PostCallback callback);
    void fetchComments(const CommentRequest& request, CommentCallback callback);
    void fetchSubreddit(const QString& subreddit, SubredditCallback callback);
    void vote(const QString& fullname, int direction, ApiCallback callback);
    void save(const QString& fullname, bool save, ApiCallback callback);
    void search(const QString& query, const QString& subreddit, PostCallback callback);

private:
    void tryNextStrategy(int index, auto callback, auto... args);
    QVector<ApiStrategy*> m_strategies;
    QString m_token;
    int m_activeAttempts = 0;
};

}  // namespace PinkReader
