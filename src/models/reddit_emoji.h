/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_emoji.h - Subreddit emoji/flair model
 */

#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QUrl>
#include <QVariant>
#include <cstdint>

namespace PinkReader {

/**
 * @brief Data model for Subreddit emoji/flair model
 *
 * @see https://www.reddit.com/dev/api/
 */
class RedditEmoji
{
public:
    RedditEmoji();
    explicit RedditEmoji(const QJsonObject &json);
    explicit RedditEmoji(const QString &id);
    RedditEmoji(const RedditEmoji &other) = default;
    RedditEmoji(RedditEmoji &&other) noexcept = default;
    RedditEmoji& operator=(const RedditEmoji &other) = default;
    RedditEmoji& operator=(RedditEmoji &&other) noexcept = default;
    ~RedditEmoji() = default;

    QString id() const { return m_id; }
    void setId(const QString &v) { m_id = v; }
    QString name() const { return m_name; }
    void setName(const QString &v) { m_name = v; }

    QDateTime createdAt() const { return m_createdAt; }
    void setCreatedAt(const QDateTime &v) { m_createdAt = v; }
    qint64 createdUtc() const { return m_createdUtc; }
    void setCreatedUtc(qint64 v) { m_createdUtc = v; }
    QDateTime updatedAt() const { return m_updatedAt; }
    void setUpdatedAt(const QDateTime &v) { m_updatedAt = v; }

    QString title() const { return m_title; }
    void setTitle(const QString &v) { m_title = v; }
    QString description() const { return m_description; }
    void setDescription(const QString &v) { m_description = v; }
    QString body() const { return m_body; }
    void setBody(const QString &v) { m_body = v; }
    QString author() const { return m_author; }
    void setAuthor(const QString &v) { m_author = v; }
    QString authorId() const { return m_authorId; }
    void setAuthorId(const QString &v) { m_authorId = v; }

    bool isActive() const { return m_isActive; }
    void setIsActive(bool v) { m_isActive = v; }
    bool isPinned() const { return m_isPinned; }
    void setIsPinned(bool v) { m_isPinned = v; }
    bool isLocked() const { return m_isLocked; }
    void setIsLocked(bool v) { m_isLocked = v; }
    bool isArchived() const { return m_isArchived; }
    void setIsArchived(bool v) { m_isArchived = v; }
    bool isNsfw() const { return m_isNsfw; }
    void setIsNsfw(bool v) { m_isNsfw = v; }
    bool isSpoiler() const { return m_isSpoiler; }
    void setIsSpoiler(bool v) { m_isSpoiler = v; }

    int totalCount() const { return m_totalCount; }
    void setTotalCount(int v) { m_totalCount = v; }
    int upvoteCount() const { return m_upvoteCount; }
    void setUpvoteCount(int v) { m_upvoteCount = v; }
    int downvoteCount() const { return m_downvoteCount; }
    void setDownvoteCount(int v) { m_downvoteCount = v; }
    int commentCount() const { return m_commentCount; }
    void setCommentCount(int v) { m_commentCount = v; }

    QUrl url() const { return m_url; }
    void setUrl(const QUrl &v) { m_url = v; }
    QUrl thumbnailUrl() const { return m_thumbnailUrl; }
    void setThumbnailUrl(const QUrl &v) { m_thumbnailUrl = v; }
    QUrl permalink() const { return m_permalink; }
    void setPermalink(const QUrl &v) { m_permalink = v; }

    QJsonObject toJson() const;
    bool fromJson(const QJsonObject &json);
    bool isValid() const;
    QString toString() const;

    bool operator==(const RedditEmoji &other) const;
    bool operator!=(const RedditEmoji &other) const;
    friend uint qHash(const RedditEmoji &key, uint seed = 0);

private:
    QString m_id;
    QString m_name;
    QDateTime m_createdAt;
    qint64 m_createdUtc = 0;
    QDateTime m_updatedAt;
    QString m_title;
    QString m_description;
    QString m_body;
    QString m_author;
    QString m_authorId;
    bool m_isActive = true;
    bool m_isPinned = false;
    bool m_isLocked = false;
    bool m_isArchived = false;
    bool m_isNsfw = false;
    bool m_isSpoiler = false;
    int m_totalCount = 0;
    int m_upvoteCount = 0;
    int m_downvoteCount = 0;
    int m_commentCount = 0;
    QUrl m_url;
    QUrl m_thumbnailUrl;
    QUrl m_permalink;
    QJsonObject m_extraData;
};

} // namespace PinkReader

Q_DECLARE_METATYPE(PinkReader::RedditEmoji)