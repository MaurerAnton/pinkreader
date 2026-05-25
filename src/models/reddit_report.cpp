/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * File: reddit_report.cpp
 * Description: Model implementation for reddit report
 */

#include "models/reddit_report.h"
#include <QJsonDocument>
#include <QDebug>
#include <QHash>

namespace PinkReader {

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

RedditReport::RedditReport()
    : m_id()
    , m_name()
    , m_created()
    , m_createdUtc(0)
    , m_author()
    , m_body()
    , m_bodyHtml()
    , m_score(0)
    , m_ups(0)
    , m_downs(0)
    , m_upvoteRatio(0.0)
    , m_edited(false)
    , m_stickied(false)
    , m_locked(false)
    , m_over18(false)
    , m_spoiler(false)
    , m_archived(false)
    , m_distinguished()
    , m_subreddit()
    , m_subredditId()
    , m_permalink()
    , m_url()
{
}

RedditReport::RedditReport(const QJsonObject &json)
    : RedditReport()
{
    fromJson(json);
}

RedditReport::RedditReport(const QString &id)
    : RedditReport()
{
    m_id = id;
    m_name = id;
}

// ---------------------------------------------------------------------------
// Serialization
// ---------------------------------------------------------------------------

QJsonObject RedditReport::toJson() const
{
    QJsonObject obj;

    // Core identification
    if (!m_id.isEmpty())
        obj[QStringLiteral("id")] = m_id;
    if (!m_name.isEmpty())
        obj[QStringLiteral("name")] = m_name;

    // Timestamps
    if (m_createdUtc > 0)
        obj[QStringLiteral("created_utc")] = static_cast<double>(m_createdUtc);
    if (m_created.isValid())
        obj[QStringLiteral("created")] = m_created.toString(Qt::ISODate);

    // Content
    if (!m_author.isEmpty())
        obj[QStringLiteral("author")] = m_author;
    if (!m_body.isEmpty())
        obj[QStringLiteral("body")] = m_body;
    if (!m_bodyHtml.isEmpty())
        obj[QStringLiteral("body_html")] = m_bodyHtml;

    // Voting
    obj[QStringLiteral("score")] = m_score;
    obj[QStringLiteral("ups")] = m_ups;
    obj[QStringLiteral("downs")] = m_downs;
    if (m_upvoteRatio > 0)
        obj[QStringLiteral("upvote_ratio")] = m_upvoteRatio;

    // Flags
    if (m_edited)
        obj[QStringLiteral("edited")] = true;
    if (m_editedTime.isValid())
        obj[QStringLiteral("edited_time")] = m_editedTime.toString(Qt::ISODate);
    if (m_stickied)
        obj[QStringLiteral("stickied")] = true;
    if (m_locked)
        obj[QStringLiteral("locked")] = true;
    if (m_over18)
        obj[QStringLiteral("over_18")] = true;
    if (m_spoiler)
        obj[QStringLiteral("spoiler")] = true;
    if (m_archived)
        obj[QStringLiteral("archived")] = true;

    // Metadata
    if (!m_distinguished.isEmpty())
        obj[QStringLiteral("distinguished")] = m_distinguished;
    if (!m_subreddit.isEmpty())
        obj[QStringLiteral("subreddit")] = m_subreddit;
    if (!m_subredditId.isEmpty())
        obj[QStringLiteral("subreddit_id")] = m_subredditId;
    if (!m_permalink.isEmpty())
        obj[QStringLiteral("permalink")] = m_permalink;
    if (!m_url.isEmpty())
        obj[QStringLiteral("url")] = m_url;

    // Extra data
    if (!m_extraData.isEmpty())
        obj[QStringLiteral("extra_data")] = m_extraData;

    return obj;
}

bool RedditReport::fromJson(const QJsonObject &json)
{
    // Core identification
    m_id = json.value(QStringLiteral("id")).toString();
    m_name = json.value(QStringLiteral("name")).toString();

    // Timestamps
    QJsonValue createdUtcVal = json.value(QStringLiteral("created_utc"));
    if (createdUtcVal.isDouble()) {
        m_createdUtc = static_cast<qint64>(createdUtcVal.toDouble());
        m_created = QDateTime::fromSecsSinceEpoch(m_createdUtc, Qt::UTC);
    } else if (createdUtcVal.isString()) {
        bool ok;
        m_createdUtc = createdUtcVal.toString().toLongLong(&ok);
        if (ok)
            m_created = QDateTime::fromSecsSinceEpoch(m_createdUtc, Qt::UTC);
    }

    // Alternative: try "created" field
    if (!m_created.isValid()) {
        QJsonValue createdVal = json.value(QStringLiteral("created"));
        if (createdVal.isDouble()) {
            m_createdUtc = static_cast<qint64>(createdVal.toDouble());
            m_created = QDateTime::fromSecsSinceEpoch(m_createdUtc, Qt::UTC);
        }
    }

    // Content
    m_author = json.value(QStringLiteral("author")).toString();
    m_body = json.value(QStringLiteral("body")).toString();
    m_bodyHtml = json.value(QStringLiteral("body_html")).toString();

    // Also try "selftext" for posts
    if (m_body.isEmpty()) {
        m_body = json.value(QStringLiteral("selftext")).toString();
        m_bodyHtml = json.value(QStringLiteral("selftext_html")).toString();
    }

    // Voting
    m_score = json.value(QStringLiteral("score")).toInt(m_score);
    m_ups = json.value(QStringLiteral("ups")).toInt(m_ups);
    m_downs = json.value(QStringLiteral("downs")).toInt(m_downs);
    m_upvoteRatio = json.value(QStringLiteral("upvote_ratio")).toDouble(m_upvoteRatio);

    // Flags
    QJsonValue editedVal = json.value(QStringLiteral("edited"));
    if (editedVal.isBool()) {
        m_edited = editedVal.toBool();
    } else if (editedVal.isDouble()) {
        // Reddit returns edited as a timestamp (epoch) or false
        double editedEpoch = editedVal.toDouble();
        m_edited = (editedEpoch > 0);
        if (m_edited)
            m_editedTime = QDateTime::fromSecsSinceEpoch(
                static_cast<qint64>(editedEpoch), Qt::UTC);
    }

    m_stickied = json.value(QStringLiteral("stickied")).toBool(false);
    m_locked = json.value(QStringLiteral("locked")).toBool(false);
    m_over18 = json.value(QStringLiteral("over_18")).toBool(false);
    m_spoiler = json.value(QStringLiteral("spoiler")).toBool(false);
    m_archived = json.value(QStringLiteral("archived")).toBool(false);

    // Metadata
    m_distinguished = json.value(QStringLiteral("distinguished")).toString();
    m_subreddit = json.value(QStringLiteral("subreddit")).toString();
    m_subredditId = json.value(QStringLiteral("subreddit_id")).toString();
    m_permalink = json.value(QStringLiteral("permalink")).toString();
    m_url = json.value(QStringLiteral("url")).toString();

    // Extra data
    QJsonValue extraVal = json.value(QStringLiteral("extra_data"));
    if (extraVal.isObject())
        m_extraData = extraVal.toObject();

    return isValid();
}

// ---------------------------------------------------------------------------
// Validation
// ---------------------------------------------------------------------------

bool RedditReport::isValid() const
{
    return !m_id.isEmpty();
}

// ---------------------------------------------------------------------------
// Utility
// ---------------------------------------------------------------------------

QString RedditReport::toString() const
{
    return QStringLiteral("RedditReport(id=%1, author=%2, score=%3)")
        .arg(m_id, m_author)
        .arg(m_score);
}

// ---------------------------------------------------------------------------
// Comparison Operators
// ---------------------------------------------------------------------------

bool RedditReport::operator==(const RedditReport &other) const
{
    return m_id == other.m_id;
}

bool RedditReport::operator!=(const RedditReport &other) const
{
    return !(*this == other);
}

// ---------------------------------------------------------------------------
// Hash Support
// ---------------------------------------------------------------------------

uint qHash(const RedditReport &key, uint seed)
{
    return qHash(key.m_id, seed);
}

} // namespace PinkReader
