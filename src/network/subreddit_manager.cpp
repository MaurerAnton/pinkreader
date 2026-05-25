/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: subreddit_manager.cpp - Implementation port of RedditSubredditManager.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/RedditSubredditManager.java
 */

#include "network/subreddit_manager.h"

#include <QDebug>
#include <QCryptographicHash>

#include <stdexcept>

// These includes will be used when the corresponding types are ported:
// #include "models/reddit_subreddit.h"
// #include "models/subreddit_canonical_id.h"  (SubredditCanonicalId)
// #include "accounts/account.h"
// #include "io/raw_object_db.h"
// #include "io/threaded_raw_object_db.h"
// #include "io/weak_cache.h"
// #include "network/reddit_api_individual_subreddit_data_requester.h"
// #include "common/timestamp_bound.h"
// #include "common/timestamp_utc.h"
// #include "common/rr_error.h"
// #include "common/general.h"

namespace PinkReader {

// ============================================================================
// Static members (Java lines 62-63)
// ============================================================================

std::unique_ptr<RedditSubredditManager> RedditSubredditManager::s_singleton;
std::mutex RedditSubredditManager::s_singletonMutex;
std::optional<Account> RedditSubredditManager::s_singletonUser;

// ============================================================================
// Singleton getInstance (Java lines 68-78)
// ============================================================================

RedditSubredditManager &RedditSubredditManager::getInstance(const Account &user) {
    // Java: if(singleton == null || !user.equals(singletonUser)) {
    //         singletonUser = user;
    //         singleton = new RedditSubredditManager(context, user);
    //       }
    //       return singleton;

    std::lock_guard<std::mutex> lock(s_singletonMutex);

    bool needsNew = false;
    if (!s_singleton) {
        needsNew = true;
    } else if (!s_singletonUser.has_value() || !(user == s_singletonUser.value())) {
        needsNew = true;
    }

    if (needsNew) {
        s_singletonUser = user;
        s_singleton.reset(new RedditSubredditManager(user));
    }

    return *s_singleton;
}

// ============================================================================
// Constructor (Java lines 80-97)
// ============================================================================

RedditSubredditManager::RedditSubredditManager(const Account &user) {
    // Java:
    //   final RawObjectDB<SubredditCanonicalId, RedditSubreddit> subredditDb
    //       = new RawObjectDB<>(context, getDbFilename("subreddits", user), RedditSubreddit.class);
    //
    //   final ThreadedRawObjectDB<SubredditCanonicalId, RedditSubreddit, RRError> subredditDbWrapper
    //       = new ThreadedRawObjectDB<>(subredditDb, new RedditAPIIndividualSubredditDataRequester(context, user));
    //
    //   subredditCache = new WeakCache<>(subredditDbWrapper);

    const QString dbFilename = getDbFilename(QStringLiteral("subreddits"), user);

    qDebug() << "RedditSubredditManager: initializing with DB:" << dbFilename;

    // TODO: When RawObjectDB, ThreadedRawObjectDB, WeakCache, and
    // RedditAPIIndividualSubredditDataRequester are ported:
    //
    // auto subredditDb = std::make_unique<RawObjectDB<SubredditCanonicalId, RedditSubreddit>>(
    //     dbFilename);
    //
    // auto requester = std::make_unique<RedditAPIIndividualSubredditDataRequester>(user);
    //
    // auto subredditDbWrapper = std::make_unique<
    //     ThreadedRawObjectDB<SubredditCanonicalId, RedditSubreddit, RRError>>(
    //     std::move(subredditDb), std::move(requester));
    //
    // m_subredditCache = new WeakCache<SubredditCanonicalId, RedditSubreddit, RRError>(
    //     std::move(subredditDbWrapper));
}

// ============================================================================
// Destructor
// ============================================================================

RedditSubredditManager::~RedditSubredditManager() {
    // Clean up the placeholder void* if it was allocated
    // In the real impl, the unique_ptr will handle this automatically
    if (m_subredditCache) {
        // delete static_cast<WeakCache<SubredditCanonicalId, RedditSubreddit, RRError>*>(m_subredditCache);
        m_subredditCache = nullptr;
    }
}

// ============================================================================
// offerRawSubredditData (Java lines 40-44)
// ============================================================================

void RedditSubredditManager::offerRawSubredditData(
        const std::vector<RedditSubreddit> &toWrite,
        const TimestampUTC &timestamp) {
    // Java: subredditCache.performWrite(toWrite);
    // TODO: when WeakCache is ported:
    //   static_cast<WeakCache<SubredditCanonicalId, RedditSubreddit, RRError>*>(
    //       m_subredditCache)->performWrite(toWrite);
    Q_UNUSED(toWrite);
    Q_UNUSED(timestamp);
}

// ============================================================================
// getSubreddit (Java lines 103-115)
// ============================================================================

void RedditSubredditManager::getSubreddit(
        const SubredditCanonicalId &subredditCanonicalId,
        const TimestampBound &timestampBound,
        RequestResponseHandler<RedditSubreddit, RRError> &handler,
        UpdatedVersionListener<SubredditCanonicalId, RedditSubreddit> *updatedVersionListener) {
    // Java:
    //   subredditCache.performRequest(
    //       subredditCanonicalId, timestampBound, handler, updatedVersionListener);

    // TODO: when WeakCache is ported:
    //   static_cast<WeakCache<SubredditCanonicalId, RedditSubreddit, RRError>*>(
    //       m_subredditCache)->performRequest(
    //           subredditCanonicalId, timestampBound, handler, updatedVersionListener);

    Q_UNUSED(subredditCanonicalId);
    Q_UNUSED(timestampBound);
    Q_UNUSED(handler);
    Q_UNUSED(updatedVersionListener);
}

// ============================================================================
// getSubreddits (Java lines 117-125)
// ============================================================================

void RedditSubredditManager::getSubreddits(
        const std::vector<SubredditCanonicalId> &ids,
        const TimestampBound &timestampBound,
        RequestResponseHandler<std::map<SubredditCanonicalId, RedditSubreddit>, RRError> &handler) {
    // Java:
    //   subredditCache.performRequest(ids, timestampBound, handler);

    // TODO: when WeakCache is ported:
    //   static_cast<WeakCache<SubredditCanonicalId, RedditSubreddit, RRError>*>(
    //       m_subredditCache)->performRequest(ids, timestampBound, handler);

    Q_UNUSED(ids);
    Q_UNUSED(timestampBound);
    Q_UNUSED(handler);
}

// ============================================================================
// getDbFilename (Java lines 99-101)
// ============================================================================

QString RedditSubredditManager::getDbFilename(const QString &type, const Account &user) {
    // Java:
    //   return General.sha1(user.username.getBytes()) + "_" + type + "_subreddits.db";

    const QString username = user.username();

    // SHA-1 hash of username, matching Java's General.sha1()
    const QByteArray hash = QCryptographicHash::hash(
            username.toUtf8(),
            QCryptographicHash::Sha1);

    const QString hexHash = QString::fromLatin1(hash.toHex());

    return hexHash + QStringLiteral("_") + type + QStringLiteral("_subreddits.db");
}

} // namespace PinkReader
