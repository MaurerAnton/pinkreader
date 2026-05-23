#pragma once

#include "../api_strategy.hpp"

#include <QNetworkAccessManager>

namespace PinkReader {

class RssStrategy : public ApiStrategy {
    Q_OBJECT
public:
    explicit RssStrategy(QObject* parent = nullptr);

    QString name() const override { return "RSS Feeds"; }
    int priority() const override { return 30; }
    bool requiresAuth() const override { return false; }

    void fetchFeed(const FeedRequest& request, PostCallback callback) override;
    void fetchComments(const CommentRequest& request, CommentCallback callback) override {
        Q_UNUSED(request)
        callback(false, {}, "RSS does not support comments");
    }
    void fetchSubreddit(const QString& subreddit, SubredditCallback callback) override {
        Q_UNUSED(subreddit)
        callback(false, Subreddit{}, "RSS does not support subreddit info");
    }
    void fetchMoreChildren(const QString&, const QStringList&, StringCallback callback) override {
        callback(false, {}, "Not supported via RSS");
    }
    void vote(const QString&, int, ApiCallback callback) override { callback(false, "Voting not available via RSS"); }
    void save(const QString&, bool, ApiCallback callback) override { callback(false, "Save not available via RSS"); }
    void hide(const QString&, ApiCallback callback) override { callback(false, "Hide not available via RSS"); }
    void submitComment(const QString&, const QString&, ApiCallback callback) override {
        callback(false, "Commenting not available via RSS");
    }
    void search(const QString& query, const QString& subreddit, PostCallback callback) override {
        Q_UNUSED(query)
        Q_UNUSED(subreddit)
        callback(false, {}, {}, "Search not available via RSS");
    }

private:
    QVector<Post> parseRssFeed(const QByteArray& data);
    QNetworkAccessManager* m_nam;
};

}  // namespace PinkReader
