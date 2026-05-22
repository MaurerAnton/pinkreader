#include "oauth_json_strategy.hpp"
#include "../api_routes.hpp"
#include "../json_parser.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QUrlQuery>

static QString oauthBase() { return QStringLiteral("https://oauth.reddit.com"); }

namespace PinkReader {

OAuthJsonStrategy::OAuthJsonStrategy(QObject* parent)
    : ApiStrategy(parent)
    , m_nam(new QNetworkAccessManager(this))
    , m_userAgent("PinkReader/0.1 (Android; C++/Qt6)")
{
    m_nam->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

void OAuthJsonStrategy::setCredentials(const QString& clientId, const QString& redirectUri) {
    m_clientId = clientId;
    m_redirectUri = redirectUri;
}

void OAuthJsonStrategy::setTokens(const QString& accessToken, const QString& refreshToken) {
    m_accessToken = accessToken;
    m_refreshToken = refreshToken;
}

bool OAuthJsonStrategy::isAvailable() const {
    return !m_accessToken.isEmpty();
}

QNetworkRequest OAuthJsonStrategy::makeRequest(const QString& path) const {
    QUrl url(oauthBase() + path);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return req;
}

void OAuthJsonStrategy::fetchFeed(const FeedRequest& request, PostCallback callback) {
    QString path;
    if (request.subreddit.isEmpty()) {
        path = request.sort == SortOrder::Hot ? QStringLiteral("/hot") : QStringLiteral("/new");
    } else {
        path = QStringLiteral("/r/") + request.subreddit + QStringLiteral("/")
               + (request.sort == SortOrder::Hot ? QStringLiteral("hot") : QStringLiteral("new"));
    }
    
    QUrlQuery query;
    query.addQueryItem("limit", QString::number(request.limit));
    query.addQueryItem("raw_json", "1");
    if (!request.after.isEmpty())
        query.addQueryItem("after", request.after);
    
    QUrl url(oauthBase() + path);
    url.setQuery(query);
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, {}, reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        Listing listing = Listing::fromJson(doc.object());
        callback(true, listing.posts, listing.after, {});
    });
}

void OAuthJsonStrategy::fetchComments(const CommentRequest& request, CommentCallback callback) {
    QString path = QString("/r/all/comments/%1").arg(request.postId);
    QUrlQuery query;
    query.addQueryItem("limit", QString::number(request.limit));
    query.addQueryItem("depth", QString::number(request.depth));
    query.addQueryItem("sort", request.sort);
    query.addQueryItem("raw_json", "1");
    if (!request.commentId.isEmpty())
        query.addQueryItem("comment", request.commentId);
    
    QUrl url(oauthBase() + path);
    url.setQuery(query);
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
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
        // Second element contains comments
        if (arr.size() >= 2) {
            QJsonObject data = arr[1].toObject();
            QJsonObject listingObj = data["data"].toObject();
            QJsonArray children = listingObj["children"].toArray();
            for (const auto& child : children) {
                QJsonObject childData = child.toObject()["data"].toObject();
                comments.append(Comment::fromJson(childData));
            }
        }
        callback(true, comments, {});
    });
}

void OAuthJsonStrategy::fetchSubreddit(const QString& subreddit, SubredditCallback callback) {
    auto* reply = m_nam->get(makeRequest("/r/" + subreddit + "/about"));
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

void OAuthJsonStrategy::fetchMoreChildren(const QString& linkId, const QStringList& children, StringCallback callback) {
    QUrl url(oauthBase() + "/api/morechildren");
    QUrlQuery query;
    query.addQueryItem("link_id", linkId);
    query.addQueryItem("children", children.join(","));
    query.addQueryItem("api_type", "json");
    url.setQuery(query);
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
    auto* reply = m_nam->post(req, QByteArray());
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, reply->errorString());
            return;
        }
        callback(true, reply->readAll(), {});
    });
}

void OAuthJsonStrategy::vote(const QString& fullname, int direction, ApiCallback callback) {
    QUrl url(oauthBase() + "/api/vote");
    QUrlQuery query;
    query.addQueryItem("id", fullname);
    query.addQueryItem("dir", QString::number(direction));
    query.addQueryItem("rank", "2");
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    auto* reply = m_nam->post(req, query.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        callback(reply->error() == QNetworkReply::NoError, reply->errorString());
    });
}

void OAuthJsonStrategy::save(const QString& fullname, bool saveFlag, ApiCallback callback) {
    QString endpoint = saveFlag ? "/api/save" : "/api/unsave";
    QUrl url(oauthBase() + endpoint);
    QUrlQuery query;
    query.addQueryItem("id", fullname);
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    auto* reply = m_nam->post(req, query.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        callback(reply->error() == QNetworkReply::NoError, reply->errorString());
    });
}

void OAuthJsonStrategy::hide(const QString& fullname, ApiCallback callback) {
    QUrl url(oauthBase() + "/api/hide");
    QUrlQuery query;
    query.addQueryItem("id", fullname);
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    auto* reply = m_nam->post(req, query.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        callback(reply->error() == QNetworkReply::NoError, reply->errorString());
    });
}

void OAuthJsonStrategy::submitComment(const QString& parentFullname, const QString& text, ApiCallback callback) {
    QUrl url(oauthBase() + "/api/comment");
    QUrlQuery query;
    query.addQueryItem("thing_id", parentFullname);
    query.addQueryItem("text", text);
    query.addQueryItem("api_type", "json");
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    auto* reply = m_nam->post(req, query.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        callback(reply->error() == QNetworkReply::NoError, reply->errorString());
    });
}

void OAuthJsonStrategy::search(const QString& query, const QString& subreddit, PostCallback callback) {
    QString path = subreddit.isEmpty()
        ? "/search"
        : "/r/" + subreddit + "/search";
    
    QUrlQuery q;
    q.addQueryItem("q", query);
    q.addQueryItem("type", "link");
    q.addQueryItem("sort", "relevance");
    q.addQueryItem("limit", "25");
    q.addQueryItem("raw_json", "1");
    
    QUrl url(oauthBase() + path);
    url.setQuery(q);
    
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, {}, reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        Listing listing = Listing::fromJson(doc.object());
        callback(true, listing.posts, listing.after, {});
    });
}

} // namespace PinkReader
