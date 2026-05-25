/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_widget.cpp - Implementation of Subreddit widget data model
 */

#include "models/reddit_widget.h"
#include <QJsonDocument>
#include <QDebug>
#include <QHash>
#include <QRegularExpression>

namespace PinkReader {

RedditWidget::RedditWidget()
    : m_id()
    , m_name()
    , m_createdAt()
    , m_createdUtc(0)
    , m_updatedAt()
    , m_title()
    , m_description()
    , m_body()
    , m_author()
    , m_authorId()
{
}

RedditWidget::RedditWidget(const QJsonObject &json)
    : RedditWidget()
{
    fromJson(json);
}

RedditWidget::RedditWidget(const QString &id)
    : RedditWidget()
{
    m_id = id;
    m_name = id;
}

QJsonObject RedditWidget::toJson() const
{
    QJsonObject obj;
    if (!m_id.isEmpty()) obj[QStringLiteral("id")] = m_id;
    if (!m_name.isEmpty()) obj[QStringLiteral("name")] = m_name;
    if (m_createdUtc > 0) obj[QStringLiteral("created_utc")] = static_cast<double>(m_createdUtc);
    if (m_createdAt.isValid()) obj[QStringLiteral("created_at")] = m_createdAt.toString(Qt::ISODate);
    if (m_updatedAt.isValid()) obj[QStringLiteral("updated_at")] = m_updatedAt.toString(Qt::ISODate);
    if (!m_title.isEmpty()) obj[QStringLiteral("title")] = m_title;
    if (!m_description.isEmpty()) obj[QStringLiteral("description")] = m_description;
    if (!m_body.isEmpty()) obj[QStringLiteral("body")] = m_body;
    if (!m_author.isEmpty()) obj[QStringLiteral("author")] = m_author;
    if (!m_authorId.isEmpty()) obj[QStringLiteral("author_id")] = m_authorId;
    obj[QStringLiteral("isactive")] = m_isActive;
    obj[QStringLiteral("ispinned")] = m_isPinned;
    obj[QStringLiteral("islocked")] = m_isLocked;
    obj[QStringLiteral("isarchived")] = m_isArchived;
    obj[QStringLiteral("over_18")] = m_isNsfw;
    obj[QStringLiteral("spoiler")] = m_isSpoiler;
    obj[QStringLiteral("total_count")] = m_totalCount;
    obj[QStringLiteral("upvote_count")] = m_upvoteCount;
    obj[QStringLiteral("downvote_count")] = m_downvoteCount;
    obj[QStringLiteral("comment_count")] = m_commentCount;
    if (m_url.isValid()) obj[QStringLiteral("url")] = m_url.toString();
    if (m_thumbnailUrl.isValid()) obj[QStringLiteral("thumbnail_url")] = m_thumbnailUrl.toString();
    if (m_permalink.isValid()) obj[QStringLiteral("permalink")] = m_permalink.toString();
    if (!m_extraData.isEmpty()) obj[QStringLiteral("extra_data")] = m_extraData;
    return obj;
}

bool RedditWidget::fromJson(const QJsonObject &json)
{
    m_id = json.value(QStringLiteral("id")).toString();
    m_name = json.value(QStringLiteral("name")).toString();
    QJsonValue cv = json.value(QStringLiteral("created_utc"));
    if (cv.isDouble()) {
        m_createdUtc = static_cast<qint64>(cv.toDouble());
        m_createdAt = QDateTime::fromSecsSinceEpoch(m_createdUtc, Qt::UTC);
    }
    QJsonValue uv = json.value(QStringLiteral("updated_utc"));
    if (uv.isDouble())
        m_updatedAt = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(uv.toDouble()), Qt::UTC);
    m_title = json.value(QStringLiteral("title")).toString();
    m_description = json.value(QStringLiteral("description")).toString();
    m_body = json.value(QStringLiteral("body")).toString();
    if (m_body.isEmpty()) m_body = json.value(QStringLiteral("body_html")).toString();
    m_author = json.value(QStringLiteral("author")).toString();
    m_authorId = json.value(QStringLiteral("author_fullname")).toString();
    if (m_authorId.isEmpty()) m_authorId = json.value(QStringLiteral("author_id")).toString();
    m_isActive = json.value(QStringLiteral("isactive")).toBool(true);
    m_isPinned = json.value(QStringLiteral("ispinned")).toBool(false);
    m_isLocked = json.value(QStringLiteral("locked")).toBool(false);
    m_isArchived = json.value(QStringLiteral("archived")).toBool(false);
    m_isNsfw = json.value(QStringLiteral("over_18")).toBool(false);
    m_isSpoiler = json.value(QStringLiteral("spoiler")).toBool(false);
    auto gi = [&json](const QString &k, int d = 0) {
        QJsonValue v = json.value(k);
        if (v.isDouble()) return v.toInt();
        if (v.isString()) { bool ok; int r = v.toString().toInt(&ok); return ok ? r : d; }
        return d;
    };
    m_totalCount = gi(QStringLiteral("total_count"));
    m_upvoteCount = gi(QStringLiteral("ups"));
    m_downvoteCount = gi(QStringLiteral("downs"));
    m_commentCount = gi(QStringLiteral("num_comments"));
    QString u = json.value(QStringLiteral("url")).toString();
    if (!u.isEmpty()) m_url = QUrl(u);
    QString t = json.value(QStringLiteral("thumbnail")).toString();
    if (!t.isEmpty() && t != QStringLiteral("self") && t != QStringLiteral("nsfw") && t != QStringLiteral("default"))
        m_thumbnailUrl = QUrl(t);
    QString p = json.value(QStringLiteral("permalink")).toString();
    if (!p.isEmpty()) m_permalink = QUrl(QStringLiteral("https://www.reddit.com") + p);
    QJsonValue ev = json.value(QStringLiteral("extra_data"));
    if (ev.isObject()) m_extraData = ev.toObject();
    return isValid();
}

bool RedditWidget::isValid() const
{
    if (m_id.isEmpty()) return false;
    if (m_id.length() > 50) return false;
    if (m_url.isValid() && !m_url.scheme().isEmpty()) {
        QString s = m_url.scheme().toLower();
        if (s != QStringLiteral("http") && s != QStringLiteral("https"))
            qDebug() << "[RedditWidget] Non-HTTP URL scheme:" << s;
    }
    if (m_totalCount < 0 || m_upvoteCount < 0 || m_downvoteCount < 0) {
        qDebug() << "[RedditWidget] Negative count detected";
        return false;
    }
    if (m_createdUtc > 0 && m_createdUtc < 1114905600)
        qDebug() << "[RedditWidget] Suspicious timestamp:" << m_createdUtc;
    return true;
}

QString RedditWidget::toString() const
{
    return QStringLiteral("RedditWidget(id=%1, title=%2)")
        .arg(m_id.isEmpty() ? QStringLiteral("(none)") : m_id,
             m_title.isEmpty() ? QStringLiteral("(none)") : m_title);
}

bool RedditWidget::operator==(const RedditWidget &other) const
{ return m_id == other.m_id; }

bool RedditWidget::operator!=(const RedditWidget &other) const
{ return !(*this == other); }

uint qHash(const RedditWidget &key, uint seed)
{ return qHash(key.m_id, seed); }

} // namespace PinkReader