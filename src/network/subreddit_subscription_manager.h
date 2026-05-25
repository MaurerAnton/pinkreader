/*
 * PinkReader - GPLv3
 * File: subreddit_subscription_manager.h
 * Exact port of RedditSubredditSubscriptionManager.java (466 lines)
 *
 * Ports every line: singleton pattern, listener management, subscribe/unsubscribe,
 * pending state tracking, DB-backed subscription storage, API integration.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QHash>
#include <memory>
#include <functional>
#include <mutex>
#include <vector>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class SubredditCanonicalId;
class WritableHashSet;
class TimestampUTC;
class TimeDuration;
class TimestampBound;
class RRError;
template<typename T, typename E> class RequestResponseHandler;
template<typename K, typename V> class RawObjectDB;

// Java line 83: private static RawObjectDB<String, WritableHashSet> db

/**
 * @brief Port of RedditSubredditSubscriptionManager
 *
 * Inner classes:
 *   ListenerContext (Java lines 55-68)
 *   SubredditActionResponseHandler (Java lines 364-418)
 *   SubredditSubscriptionChangeType enum (Java lines 431-435)
 *   SubredditSubscriptionStateChangeNotifier (Java lines 437-465)
 *
 * Interface:
 *   SubredditSubscriptionStateChangeListener (Java lines 420-429)
 */
class SubredditSubscriptionManager : public QObject {
    Q_OBJECT

public:
    // ---- SubredditSubscriptionState enum (not explicitly in Java, derived from method return values) ----
    enum class SubredditSubscriptionState {
        SUBSCRIBING,
        UNSUBSCRIBING,
        SUBSCRIBED,
        NOT_SUBSCRIBED
    };

    // ========================================================================
    // SubredditSubscriptionStateChangeListener — Java lines 420-429
    // ========================================================================
    class SubscriptionStateChangeListener {
    public:
        virtual ~SubscriptionStateChangeListener() = default;

        // Java line 421-422
        virtual void onSubredditSubscriptionListUpdated(
                SubredditSubscriptionManager *manager) = 0;

        // Java line 424-425
        virtual void onSubredditSubscriptionAttempted(
                SubredditSubscriptionManager *manager) = 0;

        // Java line 427-428
        virtual void onSubredditUnsubscriptionAttempted(
                SubredditSubscriptionManager *manager) = 0;
    };

    // ========================================================================
    // ListenerContext — Java lines 55-68
    // ========================================================================
    class ListenerContext {
    public:
        // Java lines 57-61: private constructor taking listener
        explicit ListenerContext(SubscriptionStateChangeListener *listener,
                                 SubredditSubscriptionManager *parent);

        // Java lines 63-67: removeListener()
        void removeListener();

    private:
        SubscriptionStateChangeListener *m_listener;
        SubredditSubscriptionManager *m_parent;
    };

    // ========================================================================
    // Static singleton — Java lines 78-116
    // ========================================================================

    // Java lines 94-116: getSingleton(Context, RedditAccount)
    static SubredditSubscriptionManager *getSingleton(
            RedditAccount *account);

    // ========================================================================
    // Public API
    // ========================================================================

    // Java lines 130-135: addListener
    ListenerContext *addListener(SubscriptionStateChangeListener *listener);

    // Java lines 137-139: areSubscriptionsReady
    bool areSubscriptionsReady();

    // Java lines 141-158: getSubscriptionState
    SubredditSubscriptionState getSubscriptionState(const SubredditCanonicalId &id);

    // Java lines 229-239: getSubscriptionList
    std::vector<SubredditCanonicalId> getSubscriptionList();

    // Java lines 275-326: triggerUpdate
    void triggerUpdate(
            RequestResponseHandler<QHash<SubredditCanonicalId, bool>, RRError> *handler,
            const TimestampBound &timestampBound);

    // Java lines 271-273: triggerUpdateIfNotReady (no-arg)
    void triggerUpdateIfNotReady();

    // Java lines 241-269: triggerUpdateIfNotReady with onFailure
    void triggerUpdateIfNotReady(std::function<void(const RRError &)> onFailure);

    // Java lines 328-344: subscribe
    void subscribe(const SubredditCanonicalId &id);

    // Java lines 346-362: unsubscribe
    void unsubscribe(const SubredditCanonicalId &id);

signals:
    // Signals for listeners
    void subscriptionListUpdated();
    void subscriptionAttempted();
    void unsubscriptionAttempted();

private:
    // ========================================================================
    // Private constructor — Java lines 118-128
    // ========================================================================
    explicit SubredditSubscriptionManager(RedditAccount *user, QObject *parent = nullptr);

    // ========================================================================
    // SubredditSubscriptionChangeType — Java lines 431-435 (private enum)
    // ========================================================================
    enum class SubscriptionChangeType {
        LIST_UPDATED,
        SUBSCRIPTION_ATTEMPTED,
        UNSUBSCRIPTION_ATTEMPTED
    };

    // ========================================================================
    // SubredditSubscriptionStateChangeNotifier — Java lines 437-465
    // ========================================================================
    class StateChangeNotifier {
    public:
        StateChangeNotifier(SubredditSubscriptionManager *manager);

        // Java lines 442-464: operate(listener, changeType)
        void operate(SubscriptionStateChangeListener *listener,
                     SubscriptionChangeType changeType);

    private:
        SubredditSubscriptionManager *m_manager;
    };

    // ========================================================================
    // SubredditActionResponseHandler — Java lines 364-418
    // ========================================================================
    class SubredditActionResponseHandler : public QObject {
        Q_OBJECT
    public:
        enum Action { SUBSCRIBE, UNSUBSCRIBE };
        SubredditActionResponseHandler(Action action,
                                        const SubredditCanonicalId &canonicalName,
                                        SubredditSubscriptionManager *parent);

    protected:
        void onSuccess();
        void onCallbackException(const std::exception &t);
        void onFailure(const RRError &error);

    private:
        Action m_action;
        SubredditCanonicalId m_canonicalName;
        SubredditSubscriptionManager *m_parent;
    };

    // ========================================================================
    // Private methods — Java lines 160-227
    // ========================================================================

    // Java lines 160-163: onSubscriptionAttempt
    void onSubscriptionAttempt(const SubredditCanonicalId &id);

    // Java lines 165-168: onUnsubscriptionAttempt
    void onUnsubscriptionAttempt(const SubredditCanonicalId &id);

    // Java lines 170-174: onSubscriptionChangeAttemptFailed
    void onSubscriptionChangeAttemptFailed(const SubredditCanonicalId &id);

    // Java lines 176-185: onSubscriptionAttemptSuccess
    void onSubscriptionAttemptSuccess(const SubredditCanonicalId &id);

    // Java lines 187-196: onUnsubscriptionAttemptSuccess
    void onUnsubscriptionAttemptSuccess(const SubredditCanonicalId &id);

    // Java lines 198-203: addToHistory (static)
    static void addToHistory(RedditAccount *account,
                             const std::vector<SubredditCanonicalId> &newSubscriptions);

    // Java lines 205-227: onNewSubscriptionListReceived
    void onNewSubscriptionListReceived(
            const QHash<SubredditCanonicalId, bool> &newSubscriptions,
            const TimestampUTC &timestamp);

    // ========================================================================
    // Fields — Java lines 70-92
    // ========================================================================

    // Java line 70: TAG = "SubscriptionManager"
    static const QString TAG;

    // Java line 72-73: notifier
    StateChangeNotifier m_notifier;

    // Java lines 74-76: listeners (WeakReferenceListManager in Java)
    std::vector<SubscriptionStateChangeListener *> m_listeners;

    // Java lines 78-79: singleton + singletonAccount (handled statically)
    static SubredditSubscriptionManager *s_singleton;
    static RedditAccount *s_singletonAccount;

    // Java line 81: user
    RedditAccount *m_user;

    // Java line 84: static db
    static RawObjectDB<QString, WritableHashSet> *s_db;

    // Java lines 86-90:
    WritableHashSet *m_subscriptions = nullptr; // Java line 86: @Nullable
    QHash<SubredditCanonicalId, bool> m_pendingSubscriptions;  // Java line 87-88
    QHash<SubredditCanonicalId, bool> m_pendingUnsubscriptions; // Java line 89-90

    // Java line 92: TimestampUTC.ZERO
    TimestampUTC m_lastUpdateRequestTime;

    // Synchronization
    mutable std::recursive_mutex m_mutex;
};

} // namespace PinkReader
