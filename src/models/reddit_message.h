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
 * File: reddit_message.h
 * Description: Model class for reddit message
 */

#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QVector>
#include <QUrl>
#include <QVariant>
#include <QMetaType>
#include <QSharedPointer>
#include <cstdint>
#include <optional>

namespace PinkReader {

/**
 * @brief Data model class for reddit message
 *
 * Represents a reddit message entity from the Reddit API.
 * This class provides JSON deserialization, property accessors,
 * and data validation for all fields returned by the Reddit API
 * for this entity type.
 *
 * The class is designed to be:
 * - Copyable and movable for efficient list operations
 * - JSON-serializable for caching to disk
 * - Qt-compatible (uses QString, QDateTime, etc.)
 * - Thread-safe for read access (const methods)
 *
 * @note All Reddit API field names are preserved as documented in the
 *       official Reddit API documentation.
 * @see https://www.reddit.com/dev/api/
 */
class RedditMessage
{
public:
    /** @brief Default constructor - creates an empty instance */
    RedditMessage();

    /**
     * @brief Construct from a JSON object
     * @param json The JSON object from the Reddit API response
     *
     * Parses all known fields from the JSON object. Unknown fields
     * are silently ignored. Missing optional fields are set to
     * their default values.
     */
    explicit RedditMessage(const QJsonObject &json);

    /**
     * @brief Construct with minimal required fields
     * @param id The unique identifier for this entity
     *
     * Creates a minimal valid instance with just the required
     * identifier field. All other fields are set to defaults.
     */
    explicit RedditMessage(const QString &id);

    /** @brief Copy constructor */
    RedditMessage(const RedditMessage &other) = default;

    /** @brief Move constructor */
    RedditMessage(RedditMessage &&other) noexcept = default;

    /** @brief Copy assignment operator */
    RedditMessage &operator=(const RedditMessage &other) = default;

    /** @brief Move assignment operator */
    RedditMessage &operator=(RedditMessage &&other) noexcept = default;

    /** @brief Destructor */
    ~RedditMessage() = default;

    // -----------------------------------------------------------------------
    // Core Property Accessors
    // -----------------------------------------------------------------------

    /**
     * @brief Get the unique identifier
     * @return The Reddit thing ID (e.g., "t3_abc123" for posts, "t1_xyz789" for comments)
     */
    QString id() const { return m_id; }

    /**
     * @brief Set the unique identifier
     * @param id The Reddit thing ID
     */
    void setId(const QString &id) { m_id = id; }

    /**
     * @brief Get the full name (same as ID for most types)
     * @return The full Reddit thing name
     */
    QString name() const { return m_name; }

    /**
     * @brief Set the full name
     * @param name The full Reddit thing name
     */
    void setName(const QString &name) { m_name = name; }

    /**
     * @brief Get the creation timestamp
     * @return UTC timestamp when this entity was created
     */
    QDateTime created() const { return m_created; }

    /**
     * @brief Set the creation timestamp
     * @param created UTC creation timestamp
     */
    void setCreated(const QDateTime &created) { m_created = created; }

    /**
     * @brief Get the creation time as Unix timestamp
     * @return Unix timestamp (seconds since epoch)
     */
    qint64 createdUtc() const { return m_createdUtc; }

    /**
     * @brief Set the creation time as Unix timestamp
     * @param utc Unix timestamp (seconds since epoch)
     */
    void setCreatedUtc(qint64 utc) { m_createdUtc = utc; }

    // -----------------------------------------------------------------------
    // Additional Fields (type-specific)
    // -----------------------------------------------------------------------

    /**
     * @brief Get the author username
     * @return Username of the author (empty if anonymous/deleted)
     */
    QString author() const { return m_author; }

    /**
     * @brief Set the author username
     * @param author Username of the author
     */
    void setAuthor(const QString &author) { m_author = author; }

    /**
     * @brief Get the body/text content
     * @return The body text or self-text content
     */
    QString body() const { return m_body; }

    /**
     * @brief Set the body/text content
     * @param body The body text content
     */
    void setBody(const QString &body) { m_body = body; }

    /**
     * @brief Get the body HTML (if available)
     * @return HTML formatted body text
     */
    QString bodyHtml() const { return m_bodyHtml; }

    /**
     * @brief Set the body HTML
     * @param html HTML formatted body text
     */
    void setBodyHtml(const QString &html) { m_bodyHtml = html; }

    /**
     * @brief Get the score (upvotes - downvotes)
     * @return Net score value
     */
    int score() const { return m_score; }

    /**
     * @brief Set the score
     * @param score Net score value
     */
    void setScore(int score) { m_score = score; }

    /**
     * @brief Get the number of upvotes
     * @return Upvote count
     */
    int ups() const { return m_ups; }

    /**
     * @brief Set the upvote count
     * @param ups Upvote count
     */
    void setUps(int ups) { m_ups = ups; }

    /**
     * @brief Get the number of downvotes
     * @return Downvote count
     */
    int downs() const { return m_downs; }

    /**
     * @brief Set the downvote count
     * @param downs Downvote count
     */
    void setDowns(int downs) { m_downs = downs; }

    /**
     * @brief Get the upvote ratio (0.0 to 1.0)
     * @return Upvote ratio percentage
     */
    double upvoteRatio() const { return m_upvoteRatio; }

    /**
     * @brief Set the upvote ratio
     * @param ratio Upvote ratio (0.0 to 1.0)
     */
    void setUpvoteRatio(double ratio) { m_upvoteRatio = ratio; }

    /**
     * @brief Check if the entity is edited
     * @return true if edited after initial creation
     */
    bool isEdited() const { return m_edited; }

    /**
     * @brief Set the edited flag
     * @param edited Whether the entity was edited
     */
    void setEdited(bool edited) { m_edited = edited; }

    /**
     * @brief Get the edit timestamp (if edited)
     * @return Edit timestamp or invalid QDateTime if never edited
     */
    QDateTime editedTime() const { return m_editedTime; }

    /**
     * @brief Set the edit timestamp
     * @param time Edit timestamp
     */
    void setEditedTime(const QDateTime &time) { m_editedTime = time; }

    /**
     * @brief Check if the entity is stickied/pinned
     * @return true if stickied
     */
    bool isStickied() const { return m_stickied; }

    /**
     * @brief Set the stickied flag
     * @param stickied Whether the entity is stickied
     */
    void setStickied(bool stickied) { m_stickied = stickied; }

    /**
     * @brief Check if the entity is locked (no new comments)
     * @return true if locked
     */
    bool isLocked() const { return m_locked; }

    /**
     * @brief Set the locked flag
     * @param locked Whether the entity is locked
     */
    void setLocked(bool locked) { m_locked = locked; }

    /**
     * @brief Check if this is NSFW content
     * @return true if marked as NSFW/over_18
     */
    bool isNsfw() const { return m_over18; }

    /**
     * @brief Set the NSFW flag
     * @param over18 Whether the content is 18+
     */
    void setOver18(bool over18) { m_over18 = over18; }

    /**
     * @brief Check if this has a spoiler tag
     * @return true if marked as spoiler
     */
    bool isSpoiler() const { return m_spoiler; }

    /**
     * @brief Set the spoiler flag
     * @param spoiler Whether this has a spoiler tag
     */
    void setSpoiler(bool spoiler) { m_spoiler = spoiler; }

    /**
     * @brief Check if the entity is archived (>6 months old)
     * @return true if archived
     */
    bool isArchived() const { return m_archived; }

    /**
     * @brief Set the archived flag
     * @param archived Whether the entity is archived
     */
    void setArchived(bool archived) { m_archived = archived; }

    /**
     * @brief Get the distinguished status
     * @return Distinguished type string (moderator, admin, special, or empty)
     */
    QString distinguished() const { return m_distinguished; }

    /**
     * @brief Set the distinguished status
     * @param distinguished Distinguished type string
     */
    void setDistinguished(const QString &distinguished) { m_distinguished = distinguished; }

    /**
     * @brief Get the subreddit name
     * @return Subreddit name (without r/ prefix)
     */
    QString subreddit() const { return m_subreddit; }

    /**
     * @brief Set the subreddit name
     * @param subreddit Subreddit name
     */
    void setSubreddit(const QString &subreddit) { m_subreddit = subreddit; }

    /**
     * @brief Get the subreddit ID
     * @return Subreddit thing ID
     */
    QString subredditId() const { return m_subredditId; }

    /**
     * @brief Set the subreddit ID
     * @param id Subreddit thing ID
     */
    void setSubredditId(const QString &id) { m_subredditId = id; }

    /**
     * @brief Get the permalink URL
     * @return Permalink to this entity on Reddit
     */
    QString permalink() const { return m_permalink; }

    /**
     * @brief Set the permalink
     * @param permalink Permalink URL
     */
    void setPermalink(const QString &permalink) { m_permalink = permalink; }

    /**
     * @brief Get the URL associated with this entity
     * @return URL string (post link, image URL, etc.)
     */
    QString url() const { return m_url; }

    /**
     * @brief Set the URL
     * @param url URL string
     */
    void setUrl(const QString &url) { m_url = url; }

    // -----------------------------------------------------------------------
    // Serialization
    // -----------------------------------------------------------------------

    /**
     * @brief Serialize to a JSON object
     * @return QJsonObject representation of this entity
     *
     * Used for caching to disk via the CacheManager.
     */
    QJsonObject toJson() const;

    /**
     * @brief Deserialize from a JSON object
     * @param json The JSON object to parse
     * @return true if parsing was successful
     *
     * Parses all known fields and validates required fields.
     */
    bool fromJson(const QJsonObject &json);

    /**
     * @brief Check if this entity contains valid data
     * @return true if all required fields are present and valid
     */
    bool isValid() const;

    /**
     * @brief Get a human-readable summary string
     * @return Summary string for debugging
     */
    QString toString() const;

    /**
     * @brief Equality comparison
     * @param other The other entity to compare
     * @return true if both entities have the same ID
     */
    bool operator==(const RedditMessage &other) const;

    /**
     * @brief Inequality comparison
     * @param other The other entity to compare
     * @return true if entities have different IDs
     */
    bool operator!=(const RedditMessage &other) const;

    // -----------------------------------------------------------------------
    // Hash Support
    // -----------------------------------------------------------------------

    /**
     * @brief Hash function for use in QHash/QSet
     * @param key The key to hash
     * @param seed Seed value
     * @return Hash value
     */
    friend uint qHash(const RedditMessage &key, uint seed = 0);

private:
    // Core identification
    QString m_id;
    QString m_name;

    // Timestamps
    QDateTime m_created;
    qint64 m_createdUtc = 0;

    // Content
    QString m_author;
    QString m_body;
    QString m_bodyHtml;

    // Voting
    int m_score = 0;
    int m_ups = 0;
    int m_downs = 0;
    double m_upvoteRatio = 0.0;

    // Flags
    bool m_edited = false;
    QDateTime m_editedTime;
    bool m_stickied = false;
    bool m_locked = false;
    bool m_over18 = false;
    bool m_spoiler = false;
    bool m_archived = false;

    // Metadata
    QString m_distinguished;
    QString m_subreddit;
    QString m_subredditId;
    QString m_permalink;
    QString m_url;

    // Additional data (type-specific extras stored as JSON)
    QJsonObject m_extraData;
};

} // namespace PinkReader

// Register with Qt's meta-type system for use in signals/slots and QVariant
Q_DECLARE_METATYPE(PinkReader::RedditMessage)
Q_DECLARE_METATYPE(QSharedPointer<PinkReader::RedditMessage>)
