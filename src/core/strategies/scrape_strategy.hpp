#pragma once

#include "../api_strategy.hpp"
#include <QNetworkAccessManager>

namespace PinkReader {

class ScrapeStrategy : public ApiStrategy {
    Q_OBJECT
public:
    explicit ScrapeStrategy(QObject* parent = nullptr);
    
    QString name() const override { return "HTML Scraping (old.reddit.com)"; }
    int priority() const override { return 50; }
    bool requiresAuth() const override { return false; }
    
    void fetchFeed(const FeedRequest& request, PostCallback callback) override;
    void fetchComments(const CommentRequest& request, CommentCallback callback) override;
    void fetchSubreddit(const QString& subreddit, SubredditCallback callback) override;
    void fetchMoreChildren(const QString&, const QStringList&, StringCallback callback) override {
        callback(false, {}, "Not supported via scraping");
    }
    void vote(const QString&, int, ApiCallback callback) override {
        callback(false, "Voting not available via scraping");
    }
    void save(const QString&, bool, ApiCallback callback) override {
        callback(false, "Save not available via scraping");
    }
    void hide(const QString&, ApiCallback callback) override {
        callback(false, "Hide not available via scraping");
    }
    void submitComment(const QString&, const QString&, ApiCallback callback) override {
        callback(false, "Commenting not available via scraping");
    }
    void search(const QString& query, const QString& subreddit, PostCallback callback) override {
        Q_UNUSED(query) Q_UNUSED(subreddit) callback(false, {}, {}, "Search not available via scraping");
    }
    
private:
    QVector<Post> parseOldRedditHtml(const QString& html);
    Post parsePostEntry(const QString& entryHtml);
    
    QNetworkAccessManager* m_nam;
};

} // namespace PinkReader
