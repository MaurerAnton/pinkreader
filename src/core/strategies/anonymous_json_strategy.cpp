#include "anonymous_json_strategy.hpp"

#include "../api_routes.hpp"
#include "../json_parser.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QUrlQuery>

namespace PinkReader {

AnonymousJsonStrategy::AnonymousJsonStrategy(QObject* parent)
    : ApiStrategy(parent), m_nam(new QNetworkAccessManager(this)), m_userAgent("PinkReader/0.1 (Android; C++/Qt6)") {
    m_nam->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    auto* jar = new QNetworkCookieJar(m_nam);
    m_nam->setCookieJar(jar);
}

QNetworkRequest AnonymousJsonStrategy::makeRequest(const QUrl& url) const {
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", m_userAgent.toUtf8());
    req.setRawHeader("Accept", "application/json");
    if (!m_cookie.isEmpty()) {
        req.setRawHeader("Cookie", m_cookie.toUtf8());
    }
    if (!m_modhash.isEmpty()) {
        req.setRawHeader("X-Modhash", m_modhash.toUtf8());
    }
    return req;
}

// Parse API route path based on sort
static QString sortPath(SortOrder sort) {
    switch (sort) {
        case SortOrder::Hot:
            return "hot";
        case SortOrder::New:
            return "new";
        case SortOrder::Rising:
            return "rising";
        case SortOrder::Top:
            return "top";
        case SortOrder::Controversial:
            return "controversial";
        case SortOrder::Best:
            return "best";
    }
    return "hot";
}

void AnonymousJsonStrategy::fetchFeed(const FeedRequest& request, PostCallback callback) {
    QUrl url;
    if (request.subreddit.isEmpty()) {
        url = QUrl(QString("%1/%2.json").arg(ApiRoutes::ANON_BASE).arg(sortPath(request.sort)));
    } else {
        url = QUrl(
            QString("%1/r/%2/%3.json").arg(ApiRoutes::ANON_BASE).arg(request.subreddit).arg(sortPath(request.sort)));
    }

    QUrlQuery query;
    query.addQueryItem("limit", QString::number(request.limit));
    query.addQueryItem("raw_json", "1");
    if (!request.after.isEmpty())
        query.addQueryItem("after", request.after);
    url.setQuery(query);

    auto* reply = m_nam->get(makeRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (status == 429) {
                emit rateLimited(60);
            }
            callback(false, {}, {}, reply->errorString());
            return;
        }

        // Store cookies for auth persistence
        for (const auto& header : reply->rawHeaderPairs()) {
            if (header.first.toLower() == "set-cookie") {
                // Parse reddit_session cookie
                QRegularExpression re("reddit_session=([^;]+)");
                auto match = re.match(header.second);
                if (match.hasMatch()) {
                    m_cookie = "reddit_session=" + match.captured(1);
                }
            }
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        Listing listing = parseListing(doc);
        callback(true, listing.posts, listing.after, {});
    });
}

Listing AnonymousJsonStrategy::parseListing(const QJsonDocument& doc) const {
    Listing listing;
    QJsonObject root = doc.object();
    listing.kind = root["kind"].toString();

    QJsonObject data = root["data"].toObject();
    listing.before = data["before"].toString();
    listing.after = data["after"].toString();
    listing.count = data["dist"].toInt();

    QJsonArray children = data["children"].toArray();
    for (const auto& child : children) {
        QJsonObject childObj = child.toObject();
        QString kind = childObj["kind"].toString();
        QJsonObject childData = childObj["data"].toObject();

        if (kind == "t3") {
            listing.posts.append(Post::fromJson(childData));
        } else if (kind == "t1") {
            listing.comments.append(Comment::fromJson(childData));
        } else if (kind == "t5") {
            listing.subreddits.append(Subreddit::fromJson(childData));
        }
    }

    return listing;
}

void AnonymousJsonStrategy::fetchComments(const CommentRequest& request, CommentCallback callback) {
    // Extract subreddit from post context or use "all"
    QString subreddit = "all";

    QUrl url = QUrl(QString("%1/comments/%2.json").arg(ApiRoutes::ANON_BASE).arg(request.postId));

    QUrlQuery query;
    query.addQueryItem("limit", QString::number(request.limit));
    query.addQueryItem("depth", QString::number(request.depth));
    query.addQueryItem("sort", request.sort);
    query.addQueryItem("raw_json", "1");
    if (!request.commentId.isEmpty()) {
        query.addQueryItem("comment", request.commentId);
        query.addQueryItem("context", "3");
    }
    url.setQuery(query);

    auto* reply = m_nam->get(makeRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonArray arr = doc.array();
        QVector<Comment> comments;

        if (arr.size() >= 2) {
            QJsonObject listingObj = arr[1].toObject();
            QJsonObject listingData = listingObj["data"].toObject();
            QJsonArray children = listingData["children"].toArray();
            for (const auto& child : children) {
                QJsonObject childObj = child.toObject();
                if (childObj["kind"].toString() == "t1") {
                    comments.append(Comment::fromJson(childObj["data"].toObject()));
                }
            }
        }

        callback(true, comments, {});
    });
}

void AnonymousJsonStrategy::fetchSubreddit(const QString& subreddit, SubredditCallback callback) {
    QUrl url = QUrl(ApiRoutes::subredditAbout(subreddit));
    auto* reply = m_nam->get(makeRequest(url));
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

void AnonymousJsonStrategy::fetchMoreChildren(const QString& linkId, const QStringList& children,
                                              StringCallback callback) {
    QUrl url(ApiRoutes::ANON_BASE + QString("/api/morechildren.json"));
    QUrlQuery query;
    query.addQueryItem("link_id", linkId);
    query.addQueryItem("children", children.join(","));
    query.addQueryItem("api_type", "json");
    url.setQuery(query);

    QNetworkRequest req = makeRequest(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    auto* reply = m_nam->post(req, query.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        callback(reply->error() == QNetworkReply::NoError, reply->readAll(), reply->errorString());
    });
}

void AnonymousJsonStrategy::vote(const QString& fullname, int direction, ApiCallback callback) {
    callback(false, "Voting requires authentication. Please log in.");
    Q_UNUSED(fullname)
    Q_UNUSED(direction)
}

void AnonymousJsonStrategy::save(const QString& fullname, bool saveFlag, ApiCallback callback) {
    callback(false, "Save requires authentication. Please log in.");
    Q_UNUSED(fullname)
    Q_UNUSED(saveFlag)
}

void AnonymousJsonStrategy::hide(const QString& fullname, ApiCallback callback) {
    callback(false, "Hide requires authentication. Please log in.");
    Q_UNUSED(fullname)
}

void AnonymousJsonStrategy::submitComment(const QString& parentFullname, const QString& text, ApiCallback callback) {
    callback(false, "Commenting requires authentication. Please log in.");
    Q_UNUSED(parentFullname)
    Q_UNUSED(text)
}

void AnonymousJsonStrategy::search(const QString& query, const QString& subreddit, PostCallback callback) {
    QUrl url = QUrl(ApiRoutes::search(query, subreddit));
    auto* reply = m_nam->get(makeRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, {}, reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        Listing listing = parseListing(doc);
        callback(true, listing.posts, listing.after, {});
    });
}

}  // namespace PinkReader
