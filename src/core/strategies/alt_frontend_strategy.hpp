#pragma once

#include "../api_strategy.hpp"
#include <QNetworkAccessManager>

namespace PinkReader {

class AltFrontendStrategy : public ApiStrategy {
    Q_OBJECT
public:
    explicit AltFrontendStrategy(QObject* parent = nullptr);
    
    QString name() const override { return "Alternative Frontends (Teddi/Libreddit)"; }
    int priority() const override { return 40; }
    bool requiresAuth() const override { return false; }
    bool isAvailable() const override;
    
    void fetchFeed(const FeedRequest& request, PostCallback callback) override;
    void fetchComments(const CommentRequest& request, CommentCallback callback) override;
    void fetchSubreddit(const QString& subreddit, SubredditCallback callback) override;
    void fetchMoreChildren(const QString&, const QStringList&, StringCallback callback) override {
        callback(false, {}, "Not supported via alt frontends");
    }
    void vote(const QString&, int, ApiCallback callback) override {
        callback(false, "Voting not available via alt frontends");
    }
    void save(const QString&, bool, ApiCallback callback) override {
        callback(false, "Save not available via alt frontends");
    }
    void hide(const QString&, ApiCallback callback) override {
        callback(false, "Hide not available via alt frontends");
    }
    void submitComment(const QString&, const QString&, ApiCallback callback) override {
        callback(false, "Commenting not available via alt frontends");
    }
    void search(const QString& query, const QString& subreddit, PostCallback callback) override;
    
private:
    QString pickInstance();
    QVector<Post> parseAltResponse(const QJsonDocument& doc);
    void probeInstance(const QString& url, std::function<void(bool)> callback);
    
    QNetworkAccessManager* m_nam;
    QString m_currentInstance;
    QStringList m_availableInstances;
    bool m_probed = false;
};

} // namespace PinkReader
