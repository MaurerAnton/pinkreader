/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: reddit_thing.h
 * Port of RedReader's RedditThing.java
 *
 * Exact line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/things/RedditThing.java
 *
 * Every field, method, inner class, enum, and constant ported 1:1.
 */

#pragma once

#include <QString>
#include <QHash>
#include <stdexcept>
#include <memory>

namespace PinkReader {

// Forward declarations
class RedditSubreddit;
class RedditUser;
class JsonObject;

/**
 * RedditThing — wrapper around a Reddit API "thing".
 * Port of RedditThing.java (78 lines).
 *
 * Fields:
 *   - KIND_USER = "t2" (static constant)
 *   - kind: String
 *   - data: JsonObject
 *
 * Inner enum:
 *   - Kind { POST, USER, COMMENT, MESSAGE, SUBREDDIT, MORE_COMMENTS, LISTING }
 *
 * Methods:
 *   - getKind()
 *   - asSubreddit()
 *   - asUser()
 */
class RedditThing {
public:
    // ========================================================================
    // KIND_USER constant (Java line 29)
    // ========================================================================
    static const QString KIND_USER;

    // ========================================================================
    // Kind enum (Java lines 31-33)
    // ========================================================================
    enum class Kind {
        POST,           // ordinal 0
        USER,           // ordinal 1
        COMMENT,        // ordinal 2
        MESSAGE,        // ordinal 3
        SUBREDDIT,      // ordinal 4
        MORE_COMMENTS,  // ordinal 5
        LISTING         // ordinal 6
    };

    // ========================================================================
    // Fields (Java lines 48-49)
    // ========================================================================
    QString kind;
    // JsonObject data — we use a shared pointer to a QJsonObject wrapper
    std::shared_ptr<JsonObject> data;

    // ========================================================================
    // Construction
    // ========================================================================
    RedditThing() = default;

    // ========================================================================
    // getKind (Java lines 51-61)
    // Returns the Kind enum matching this.kind string.
    // Throws std::runtime_error if kind is unknown.
    // ========================================================================
    Kind getKind() const;

    // ========================================================================
    // asSubreddit (Java lines 63-69)
    // Throws on deserialization failure.
    // ========================================================================
    RedditSubreddit asSubreddit() const;

    // ========================================================================
    // asUser (Java lines 71-77)
    // Throws on deserialization failure.
    // ========================================================================
    RedditUser asUser() const;

private:
    // ========================================================================
    // kinds map (Java static initializer, lines 35-46)
    // Maps kind string -> Kind enum
    // ========================================================================
    static QHash<QString, Kind> s_kinds;
    static QHash<QString, Kind> initKinds();
};

} // namespace PinkReader
