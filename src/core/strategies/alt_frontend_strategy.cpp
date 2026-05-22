#include "alt_frontend_strategy.hpp"
#include "../api_routes.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QTimer>
#include <QRandomGenerator>

namespace PinkReader {

AltFrontendStrategy::AltFrontendStrategy(QObject* parent)
    : ApiStrategy(parent)
    , m_nam(new QNetworkAccessManager(this))
{
}

bool AltFrontendStrategy::isAvailable() const {
    return !m_availableInstances.isEmpty() || !m_probed;
}

QString AltFrontendStrategy::pickInstance() {
    if (!m_availableInstances.isEmpty()) {
        int idx = QRandomGenerator::global()->bounded(m_availableInstances.size());
        return m_availableInstances[idx];
    }
    // Fallback to a random known instance
    int idx = QRandomGenerator::global()->bounded(ApiRoutes::ALT_FRONTENDS.size());
    return ApiRoutes::ALT_FRONTENDS[idx];
}

void AltFrontendStrategy::probeInstance(const QString& url, std::function<void(bool)> callback) {
    QNetworkRequest req(QUrl(url + "/api/v1/info"));
    req.setRawHeader("User-Agent", "PinkReader/0.1");
    req.setTransferTimeout(5000);
    
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, url, this, callback]() {
        reply->deleteLater();
        bool ok = reply->error() == QNetworkReply::NoError;
        if (ok) {
            if (!m_availableInstances.contains(url))
                m_availableInstances.append(url);
        }
        if (callback) callback(ok);
    });
}

void AltFrontendStrategy::fetchFeed(const FeedRequest& request, PostCallback callback) {
    QString instance = pickInstance();
    QString path = request.subreddit.isEmpty() ? "/" : "/r/" + request.subreddit;
    
    QString sortStr;
    switch (request.sort) {
    case SortOrder::New:          sortStr = "new"; break;
    case SortOrder::Top:          sortStr = "top"; break;
    case SortOrder::Rising:       sortStr = "rising"; break;
    case SortOrder::Controversial: sortStr = "controversial"; break;
    default:                      sortStr = "hot"; break;
    }
    
    QUrl url(instance + path + "/" + sortStr + ".json");
    if (!request.after.isEmpty()) {
        QUrlQuery q;
        q.addQueryItem("after", request.after);
        url.setQuery(q);
    }
    
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");
    
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, {}, reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        auto posts = parseAltResponse(doc);
        callback(true, posts, {}, {});
    });
}

void AltFrontendStrategy::fetchComments(const CommentRequest& request, CommentCallback callback) {
    QString instance = pickInstance();
    // Most alt frontends use the same comment API as Reddit
    QUrl url(instance + "/comments/" + request.postId + ".json");
    
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");
    
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonArray arr = doc.array();
        QVector<Comment> comments;
        if (arr.size() >= 2) {
            QJsonObject listingData = arr[1].toObject()["data"].toObject();
            QJsonArray children = listingData["children"].toArray();
            for (const auto& child : children) {
                comments.append(Comment::fromJson(child.toObject()["data"].toObject()));
            }
        }
        callback(true, comments, {});
    });
}

void AltFrontendStrategy::fetchSubreddit(const QString& subreddit, SubredditCallback callback) {
    QString instance = pickInstance();
    QUrl url(instance + "/r/" + subreddit + "/about.json");
    
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");
    
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, Subreddit{}, reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        Subreddit sr = Subreddit::fromJson(doc.object()["data"].toObject());
        callback(true, sr, {});
    });
}

void AltFrontendStrategy::search(const QString& query, const QString& subreddit, PostCallback callback) {
    QString instance = pickInstance();
    QString path = subreddit.isEmpty()
        ? "/search.json?q=" + query
        : "/r/" + subreddit + "/search.json?q=" + query;
    path += "&type=link";
    
    QNetworkRequest req(QUrl(instance + path));
    req.setRawHeader("User-Agent", "PinkReader/0.1");
    
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, {}, reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        auto posts = parseAltResponse(doc);
        callback(true, posts, {}, {});
    });
}

QVector<Post> AltFrontendStrategy::parseAltResponse(const QJsonDocument& doc) {
    QVector<Post> posts;
    QJsonObject root = doc.object();
    QJsonObject data = root["data"].toObject();
    QJsonArray children = data["children"].toArray();
    
    for (const auto& child : children) {
        QJsonObject childObj = child.toObject();
        if (childObj["kind"].toString() == "t3") {
            posts.append(Post::fromJson(childObj["data"].toObject()));
        }
    }
    
    return posts;
}

} // namespace PinkReader
