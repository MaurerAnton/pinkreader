#include "scrape_strategy.hpp"
#include "../api_routes.hpp"

#include <QNetworkReply>
#include <QRegularExpression>

namespace PinkReader {

static QString anonBase() { return QStringLiteral("https://old.reddit.com"); }

ScrapeStrategy::ScrapeStrategy(QObject* parent)
    : ApiStrategy(parent)
    , m_nam(new QNetworkAccessManager(this))
{
}

void ScrapeStrategy::fetchFeed(const FeedRequest& request, PostCallback callback) {
    QString path = request.subreddit.isEmpty()
        ? QStringLiteral("/")
        : QStringLiteral("/r/") + request.subreddit + QStringLiteral("/");

    QString sortStr;
    switch (request.sort) {
    case SortOrder::New:          sortStr = QStringLiteral("new/"); break;
    case SortOrder::Top:          sortStr = QStringLiteral("top/"); break;
    case SortOrder::Rising:       sortStr = QStringLiteral("rising/"); break;
    case SortOrder::Controversial: sortStr = QStringLiteral("controversial/"); break;
    default:                      sortStr.clear(); break;
    }

    QUrl url(anonBase() + path + sortStr);
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1 (like RedReader)");
    req.setRawHeader("Accept", "text/html");

    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, {}, reply->errorString());
            return;
        }
        QString html = QString::fromUtf8(reply->readAll());
        auto posts = parseOldRedditHtml(html);
        callback(true, posts, {}, {});
    });
}

void ScrapeStrategy::fetchComments(const CommentRequest& request, CommentCallback callback) {
    QUrl url(anonBase() + QStringLiteral("/comments/%1/").arg(request.postId));

    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1 (like RedReader)");
    req.setRawHeader("Accept", "text/html");

    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, {}, reply->errorString());
            return;
        }
        callback(true, {}, {});
    });
}

void ScrapeStrategy::fetchSubreddit(const QString& subreddit, SubredditCallback callback) {
    QUrl url(anonBase() + QStringLiteral("/r/") + subreddit + QStringLiteral("/"));
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "PinkReader/0.1");
    req.setRawHeader("Accept", "text/html");

    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, subreddit, callback]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            callback(false, Subreddit{}, reply->errorString());
            return;
        }
        Subreddit sr;
        sr.name = subreddit;

        QString html = QString::fromUtf8(reply->readAll());
        QRegularExpression titleRe(QStringLiteral("<title>(.*?)</title>"));
        auto match = titleRe.match(html);
        if (match.hasMatch()) {
            sr.title = match.captured(1);
        }

        QRegularExpression descRe(QStringLiteral(R"(<div class="md"[^>]*>(.*?)</div>)"));
        match = descRe.match(html);
        if (match.hasMatch()) {
            sr.description = match.captured(1);
        }

        QRegularExpression subsRe(QStringLiteral(R"((\d+[\d,]*)\s*readers)"));
        match = subsRe.match(html);
        if (match.hasMatch()) {
            sr.subscriberCount = match.captured(1).remove(',').toInt();
        }

        callback(true, sr, {});
    });
}

QVector<Post> ScrapeStrategy::parseOldRedditHtml(const QString& html) {
    Q_UNUSED(html)
    QVector<Post> posts;
    return posts;
}

Post ScrapeStrategy::parsePostEntry(const QString& entryHtml) {
    Q_UNUSED(entryHtml)
    return Post{};
}

} // namespace PinkReader
