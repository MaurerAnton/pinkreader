#include "rss_strategy.hpp"

#include "../api_routes.hpp"

#include <QNetworkReply>
#include <QRegularExpression>
#include <QXmlStreamReader>

namespace PinkReader {

RssStrategy::RssStrategy(QObject* parent) : ApiStrategy(parent), m_nam(new QNetworkAccessManager(this)) {}

void RssStrategy::fetchFeed(const FeedRequest& request, PostCallback callback) {
    QUrl url;
    if (request.subreddit.isEmpty()) {
        url = QUrl(ApiRoutes::rssFrontpage());
    } else {
        url = QUrl(ApiRoutes::rssSubreddit(request.subreddit));
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
        auto posts = parseRssFeed(reply->readAll());
        callback(true, posts, {}, {});
    });
}

QVector<Post> RssStrategy::parseRssFeed(const QByteArray& data) {
    QVector<Post> posts;
    QXmlStreamReader xml(data);

    Post current;
    bool inItem = false;
    QString currentElement;

    while (!xml.atEnd() && !xml.hasError()) {
        auto token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            currentElement = xml.name().toString();
            if (currentElement == QLatin1String("entry") || currentElement == QLatin1String("item")) {
                inItem = true;
                current = Post{};
            }
        } else if (token == QXmlStreamReader::EndElement) {
            if (xml.name().toString() == "entry" || xml.name().toString() == "item") {
                inItem = false;
                if (!current.id.isEmpty()) {
                    posts.append(current);
                }
            }
        } else if (token == QXmlStreamReader::Characters && inItem) {
            QString text = xml.text().toString();

            if (currentElement == "id" || currentElement == "guid") {
                current.id = text;
                current.fullname = "t3_" + text;
            } else if (currentElement == "title") {
                current.title = text;
            } else if (currentElement == "content" || currentElement == "description") {
                current.selftext = text;
            } else if (currentElement == "link") {
                // In RSS, link is inside entry; need to distinguish from atom:link
            } else if (currentElement == "author") {
                // /u/username
                QRegularExpression re("/u/(\\w+)");
                auto match = re.match(text);
                if (match.hasMatch()) {
                    current.author = match.captured(1);
                }
            }
        }
    }

    return posts;
}

}  // namespace PinkReader
