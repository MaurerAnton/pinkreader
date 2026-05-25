/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: RedditParsedComment.java
 * File: reddit_parsed_comment.h - Parsed comment with body and flair
 */

#pragma once

#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <memory>
#include <vector>
#include <functional>

// Forward declarations for types not yet fully ported
namespace PinkReader {
class RedditComment;
class BetterSSB;
class BodyElement;
class RedditIdAndType;
class AppCompatActivity;
class CacheManager;
class CacheRequest;
class CacheRequestCallbacks;

/**
 * @brief Flair emote data from Reddit API
 * Mirrors MaybeParseError<RedditComment.FlairEmoteData>
 */
class FlairEmoteData {
public:
    FlairEmoteData() = default;
    FlairEmoteData(const QString &e, const QString &a, const QString &u)
        : m_e(e), m_a(a), m_u(u) {}

    QString getE() const { return m_e; }
    QString getA() const { return m_a; }
    QString getU() const { return m_u; }

private:
    QString m_e; // object type (e.g., "emoji")
    QString m_a; // placeholder text
    QString m_u; // URL
};

/**
 * @brief MaybeParseError wrapper: Ok variant with a value
 */
template<typename T>
class MaybeParseOk {
public:
    explicit MaybeParseOk(const T &value) : m_value(value) {}
    explicit MaybeParseOk(T &&value) : m_value(std::move(value)) {}
    T getValue() const { return m_value; }
private:
    T m_value;
};

/**
 * @brief MaybeParseError variant type (Ok | Error)
 */
template<typename T>
class MaybeParseError {
public:
    using Ok = MaybeParseOk<T>;

    static MaybeParseError ok(const T &value) {
        MaybeParseError result;
        result.m_isOk = true;
        result.m_value = value;
        return result;
    }

    static MaybeParseError error(const QString &msg) {
        MaybeParseError result;
        result.m_isOk = false;
        result.m_errorMsg = msg;
        return result;
    }

    bool isOk() const { return m_isOk; }
    T getValue() const { return m_value; }
    QString getError() const { return m_errorMsg; }

private:
    bool m_isOk = false;
    T m_value{};
    QString m_errorMsg;
};

/**
 * @brief Port of RedditParsedComment.java
 *
 * Parsed Reddit comment with body text and flair.
 * Implements RedditThingWithIdAndType.
 */
class RedditParsedComment {
public:
    /**
     * @brief Construct from raw RedditComment
     * @param comment The raw Reddit comment
     * @param activity The app activity context
     */
    RedditParsedComment(
        QSharedPointer<RedditComment> comment,
        QObject *activity);

    ~RedditParsedComment() = default;

    // Disallow copy, allow move
    RedditParsedComment(const RedditParsedComment &) = delete;
    RedditParsedComment &operator=(const RedditParsedComment &) = delete;
    RedditParsedComment(RedditParsedComment &&) = default;
    RedditParsedComment &operator=(RedditParsedComment &&) = default;

    // --- Getters ---

    /** @return Parsed body elements (non-null) */
    const BodyElement &getBody() const { return *m_body; }

    /** @return Flair text with formatting, may be null */
    QSharedPointer<BetterSSB> getFlair() const { return m_flair; }

    /** @return Comment ID alone (e.g., "abc123") */
    QString getIdAlone() const;

    /** @return Combined Reddit ID and type */
    RedditIdAndType getIdAndType() const;

    /** @return Raw underlying RedditComment */
    QSharedPointer<RedditComment> getRawComment() const { return m_src; }

private:
    /**
     * Download and insert flair emotes into mFlair.
     * Mirrors: private void getFlairEmotes(...)
     */
    void getFlairEmotes(
        const std::vector<MaybeParseError<FlairEmoteData>> &flairRichtext,
        QObject *activity);

    QSharedPointer<RedditComment> m_src;
    QSharedPointer<BodyElement> m_body;
    QSharedPointer<BetterSSB> m_flair;
};

} // namespace PinkReader
