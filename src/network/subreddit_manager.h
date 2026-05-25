/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_manager.h - Port of RedReader's RedditSubredditManager.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/RedditSubredditManager.java
 *
 * Every field, method, enum, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QSharedPointer>

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include <set>

namespace PinkReader {

// Forward declarations for types used by RedditSubredditManager
class Account;                       // Port of RedditAccount
class SubredditCanonicalId;
class RedditSubreddit;
class RRError;
class TimestampBound;
class TimestampUTC;

// IO layer types (not yet fully ported — forward-declared)
template<typename K, typename V>
class RawObjectDB;

template<typename K, typename V, typename E>
class ThreadedRawObjectDB;

template<typename K, typename V, typename E>
class WeakCache;

// API requester
class RedditAPIIndividualSubredditDataRequester;

// Handler types
template<typename V, typename E>
class RequestResponseHandler;

template<typename K, typename V>
class UpdatedVersionListener;

// ============================================================================
// RedditSubredditManager — port of Java class (Java line 38)
// ============================================================================

class RedditSubredditManager {
public:
    // ========================================================================
    // SubredditListType enum — port of Java enum (Java lines 54-60)
    // ========================================================================

    enum class SubredditListType {
        SUBSCRIBED,
        MODERATED,
        MULTIREDDITS,
        MOST_POPULAR,
        DEFAULTS
    };

    // ========================================================================
    // Singleton (Java lines 62, 68-78)
    // ========================================================================

    static RedditSubredditManager &getInstance(const Account &user);

    // ========================================================================
    // offerRawSubredditData (Java lines 40-44)
    // ========================================================================

    void offerRawSubredditData(
            const std::vector<RedditSubreddit> &toWrite,
            const TimestampUTC &timestamp);

    // ========================================================================
    // getSubreddit (Java lines 103-115)
    // ========================================================================

    void getSubreddit(
            const SubredditCanonicalId &subredditCanonicalId,
            const TimestampBound &timestampBound,
            RequestResponseHandler<RedditSubreddit, RRError> &handler,
            UpdatedVersionListener<SubredditCanonicalId, RedditSubreddit> *updatedVersionListener);

    // ========================================================================
    // getSubreddits (Java lines 117-125)
    // ========================================================================

    void getSubreddits(
            const std::vector<SubredditCanonicalId> &ids,
            const TimestampBound &timestampBound,
            RequestResponseHandler<std::map<SubredditCanonicalId, RedditSubreddit>, RRError> &handler);

    // ========================================================================
    // Destructor
    // ========================================================================

    ~RedditSubredditManager();

private:
    // ---- Singleton fields (Java lines 62-63) ----
    static std::unique_ptr<RedditSubredditManager> s_singleton;
    static std::mutex s_singletonMutex;
    static std::optional<Account> s_singletonUser;

    // ---- Constructor (Java lines 80-97) ----
    explicit RedditSubredditManager(const Account &user);

    // ---- subredditCache (Java lines 65-66) ----
    // WeakCache<SubredditCanonicalId, RedditSubreddit, RRError>
    // Stored as void* to avoid including full WeakCache template until it's ported
    // In the real implementation, this would be:
    //   std::unique_ptr<WeakCache<SubredditCanonicalId, RedditSubreddit, RRError>> m_subredditCache;
    void *m_subredditCache = nullptr;  // Placeholder until WeakCache is ported

    // ---- getDbFilename (Java lines 99-101) ----
    static QString getDbFilename(const QString &type, const Account &user);
};

// ============================================================================
// RequestResponseHandler — port of Java interface
// ============================================================================

template<typename V, typename E>
class RequestResponseHandler {
public:
    virtual ~RequestResponseHandler() = default;

    virtual void onRequestSuccess(const V &result, const TimestampUTC &timestamp) = 0;
    virtual void onRequestFailed(const E &error) = 0;
};

// ============================================================================
// UpdatedVersionListener — port of Java interface
// ============================================================================

template<typename K, typename V>
class UpdatedVersionListener {
public:
    virtual ~UpdatedVersionListener() = default;

    virtual void onUpdatedVersion(const K &key, const V &value) = 0;
};

} // namespace PinkReader
