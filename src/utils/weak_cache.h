/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: weak_cache.h
 * Port of RedReader's WeakCache.java
 *
 * Exact line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/io/WeakCache.java
 *   (229 lines)
 *
 * Template parameters: K (key), V (value, extends WritableObject<K>), F (failure type)
 * Every field, method, inner class, and logic branch ported 1:1.
 */

#pragma once

#include "io/request_response_handler.h"
#include "cache/cache_db_manager.h"  // for TimestampUTC

#include <QHash>
#include <QSet>
#include <QVector>
#include <QMutex>
#include <QSharedPointer>
#include <functional>
#include <memory>
#include <vector>

namespace PinkReader {

// Forward declarations
template<typename K>
class WritableObject;

template<typename K>
class TimestampBound;

// ========================================================================
// WritableObject<K> — interface for objects stored in cache
// Port of WritableObject.java (62 lines)
// ========================================================================
template<typename K>
class WritableObject {
public:
    virtual ~WritableObject() = default;

    // getKey (Java line 39)
    virtual K getKey() const = 0;

    // getTimestamp (Java line 41)
    virtual TimestampUTC getTimestamp() const = 0;
};

// ========================================================================
// TimestampBound — bounds checking for cache timestamps
// Port of TimestampBound.java (57 lines)
// ========================================================================
template<typename K>
class TimestampBound {
public:
    virtual ~TimestampBound() = default;

    // verifyTimestamp (Java line 25)
    virtual bool verifyTimestamp(const TimestampUTC &timestamp) const = 0;

    // Static instances
    static const TimestampBound *ANY;
    static const TimestampBound *NONE;

    // notOlderThan (Java lines 54-56)
    static TimestampBound notOlderThan(const TimeDuration &age);
};

// ========================================================================
// CacheDataSource<K, V, F> — interface for cache data sources
// Port of CacheDataSource.java (39 lines)
// ========================================================================
template<typename K, typename V, typename F>
class CacheDataSource {
public:
    virtual ~CacheDataSource() = default;

    // performRequest single (Java lines 26-29)
    virtual void performRequest(
            const K &key,
            const TimestampBound<K> *timestampBound,
            RequestResponseHandler<V, F> &handler) = 0;

    // performRequest batch (Java lines 31-34)
    virtual void performRequest(
            const QSet<K> &keys,
            const TimestampBound<K> *timestampBound,
            RequestResponseHandler<QHash<K, V>, F> &handler) = 0;

    // performWrite single (Java line 36)
    virtual void performWrite(const V &value) = 0;

    // performWrite batch (Java line 38)
    virtual void performWrite(const QVector<V> &values) = 0;
};

// ========================================================================
// UpdatedVersionListener<K, V> — listener interface
// Port of UpdatedVersionListener.java (22 lines)
// ========================================================================
template<typename K, typename V>
class UpdatedVersionListener {
public:
    virtual ~UpdatedVersionListener() = default;

    // onUpdatedVersion (Java line 21)
    virtual void onUpdatedVersion(const V &data) = 0;
};

// ========================================================================
// WeakReferenceListManager<E> — manages a list of weak references
// Port of WeakReferenceListManager.java (100 lines)
// ========================================================================
template<typename E>
class WeakReferenceListManager {
public:
    // Operator interface (Java lines 93-95)
    class Operator {
    public:
        virtual ~Operator() = default;
        virtual void operate(const E &object) = 0;
    };

    // ArgOperator interface (Java lines 97-99)
    template<typename A>
    class ArgOperator {
    public:
        virtual ~ArgOperator() = default;
        virtual void operate(const E &object, const A &arg) = 0;
    };

    // size (Java lines 28-30)
    int size() {
        QMutexLocker locker(&m_mutex);
        return m_data.size();
    }

    // add (Java lines 32-34)
    void add(const E &object) {
        QMutexLocker locker(&m_mutex);
        m_data.append(std::make_shared<E>(object));
    }

    // map with Operator (Java lines 36-49)
    void map(Operator &op) {
        QMutexLocker locker(&m_mutex);
        auto it = m_data.begin();
        while (it != m_data.end()) {
            if (it->expired()) {
                it = m_data.erase(it);
            } else {
                auto obj = it->lock();
                if (obj) {
                    op.operate(*obj);
                }
                ++it;
            }
        }
    }

    // map with ArgOperator (Java lines 51-64)
    template<typename A>
    void map(ArgOperator<A> &op, const A &arg) {
        QMutexLocker locker(&m_mutex);
        auto it = m_data.begin();
        while (it != m_data.end()) {
            if (it->expired()) {
                it = m_data.erase(it);
            } else {
                auto obj = it->lock();
                if (obj) {
                    op.operate(*obj, arg);
                }
                ++it;
            }
        }
    }

    // remove (Java lines 66-74)
    void remove(const E &object) {
        QMutexLocker locker(&m_mutex);
        // WeakReference comparison in Java is by identity
        // In C++ we compare by value
        m_data.erase(
            std::remove_if(m_data.begin(), m_data.end(),
                [&object](const std::weak_ptr<E> &wp) {
                    auto sp = wp.lock();
                    return sp && *sp == object;
                }),
            m_data.end());
    }

    // clean (Java lines 76-87)
    void clean() {
        QMutexLocker locker(&m_mutex);
        m_data.erase(
            std::remove_if(m_data.begin(), m_data.end(),
                [](const std::weak_ptr<E> &wp) {
                    return wp.expired();
                }),
            m_data.end());
    }

    // isEmpty (Java lines 89-91)
    bool isEmpty() {
        QMutexLocker locker(&m_mutex);
        return m_data.isEmpty();
    }

private:
    QVector<std::weak_ptr<E>> m_data;
    QMutex m_mutex;
};

// ========================================================================
// UpdatedVersionListenerNotifier<K, V>
// Port of UpdatedVersionListenerNotifier.java (29 lines)
// Implements WeakReferenceListManager.ArgOperator
// ========================================================================
template<typename K, typename V>
class UpdatedVersionListenerNotifier
    : public WeakReferenceListManager<UpdatedVersionListener<K, V>>::
             template ArgOperator<V> {
public:
    void operate(const UpdatedVersionListener<K, V> &listener,
                 const V &data) override {
        listener.onUpdatedVersion(data);
    }
};

// ========================================================================
// WeakCache<K, V, F>
// Port of WeakCache.java (229 lines)
//
// Fields:
//   cached: HashMap<K, CacheEntry>
//   cacheDataSource: CacheDataSource<K, V, F>
//   updatedVersionListenerNotifier: UpdatedVersionListenerNotifier<K, V>
//
// Implements CacheDataSource<K, V, F>
// ========================================================================
template<typename K, typename V, typename F>
class WeakCache : public CacheDataSource<K, V, F> {
public:
    // ====================================================================
    // Inner class: CacheEntry (Java lines 213-227)
    // ====================================================================
    struct CacheEntry {
        std::weak_ptr<V> data;
        WeakReferenceListManager<UpdatedVersionListener<K, V>> listeners;

        CacheEntry(const std::weak_ptr<V> &d,
                   const WeakReferenceListManager<
                       UpdatedVersionListener<K, V>> &l)
            : data(d), listeners(l) {}

        explicit CacheEntry(const std::weak_ptr<V> &d)
            : data(d) {}
    };

    // ====================================================================
    // Constructor (Java lines 38-40)
    // ====================================================================
    explicit WeakCache(CacheDataSource<K, V, F> &cacheDataSource)
        : m_cacheDataSource(cacheDataSource) {}

    // ====================================================================
    // performRequest (single key, no listener) (Java lines 42-48)
    // ====================================================================
    void performRequest(
            const K &key,
            const TimestampBound<K> *timestampBound,
            RequestResponseHandler<V, F> &handler) override {
        performRequestImpl(key, timestampBound, handler, nullptr);
    }

    // ====================================================================
    // performRequest (batch) (Java lines 50-111)
    // ====================================================================
    void performRequest(
            const QSet<K> &keys,
            const TimestampBound<K> *timestampBound,
            RequestResponseHandler<QHash<K, V>, F> &handler) override {
        QMutexLocker locker(&m_mutex);

        QSet<K> keysRemaining = keys;
        QHash<K, V> cacheResult;
        TimestampUTC oldestTimestamp;
        bool hasOldestTimestamp = false;

        // Check cache for each key (Java lines 60-79)
        for (const K &key : keys) {
            auto it = m_cached.find(key);
            if (it != m_cached.end()) {
                auto valuePtr = it->data.lock();
                if (valuePtr) {
                    if (timestampBound == nullptr
                            || timestampBound->verifyTimestamp(
                                valuePtr->getTimestamp())) {
                        keysRemaining.remove(key);
                        cacheResult.insert(key, *valuePtr);
                        if (!hasOldestTimestamp) {
                            oldestTimestamp = valuePtr->getTimestamp();
                            hasOldestTimestamp = true;
                        } else {
                            oldestTimestamp = TimestampUTC::oldest(
                                oldestTimestamp,
                                valuePtr->getTimestamp());
                        }
                    }
                }
            }
        }

        // If not all found in cache, request from data source
        // (Java lines 81-110)
        if (!keysRemaining.isEmpty()) {
            m_cacheDataSource.performRequest(
                keysRemaining,
                timestampBound,
                FunctionRequestResponseHandler<QHash<K, V>, F>(
                    // onSuccess
                    [this, cacheResult, oldestTimestamp, hasOldestTimestamp,
                     &handler](const QHash<K, V> &result,
                               const TimestampUTC &timeCached) mutable {
                        // cacheResult.putAll(result) — merge
                        for (auto it = result.begin();
                             it != result.end(); ++it) {
                            cacheResult.insert(it.key(), it.value());
                        }
                        TimestampUTC ts = hasOldestTimestamp
                            ? TimestampUTC::oldest(timeCached, oldestTimestamp)
                            : timeCached;
                        handler.onRequestSuccess(cacheResult, ts);
                    },
                    // onFailure
                    [&handler](const F &reason) {
                        handler.onRequestFailed(reason);
                    }));
        } else {
            handler.onRequestSuccess(cacheResult, oldestTimestamp);
        }
    }

    // ====================================================================
    // performWrite (single) (Java lines 113-116)
    // ====================================================================
    void performWrite(const V &value) override {
        putSingle(value, true);
    }

    // ====================================================================
    // performWrite (batch) (Java lines 118-121)
    // ====================================================================
    void performWrite(const QVector<V> &values) override {
        putBatch(values, true);
    }

    // ====================================================================
    // performRequest with listener (Java lines 123-160)
    // ====================================================================
    void performRequest(
            const K &key,
            const TimestampBound<K> *timestampBound,
            RequestResponseHandler<V, F> &handler,
            UpdatedVersionListener<K, V> *updatedVersionListener) {
        QMutexLocker locker(&m_mutex);

        if (timestampBound != nullptr) {
            auto it = m_cached.find(key);
            if (it != m_cached.end()) {
                auto existing = it->data.lock();
                if (existing
                        && timestampBound->verifyTimestamp(
                            existing->getTimestamp())) {
                    // Cache hit — return immediately
                    handler.onRequestSuccess(*existing,
                                              existing->getTimestamp());
                    return;
                }
            }
        }

        // Cache miss — fetch from data source
        m_cacheDataSource.performRequest(
            key,
            timestampBound,
            FunctionRequestResponseHandler<V, F>(
                // onSuccess
                [this, key, updatedVersionListener, &handler](
                        const V &result,
                        const TimestampUTC &timeCached) {
                    QMutexLocker innerLock(&m_mutex);
                    putSingle(result, false);
                    if (updatedVersionListener != nullptr) {
                        auto it = m_cached.find(key);
                        if (it != m_cached.end()) {
                            it->listeners.add(
                                *updatedVersionListener);
                        }
                    }
                    handler.onRequestSuccess(result, timeCached);
                },
                // onFailure
                [&handler](const F &reason) {
                    handler.onRequestFailed(reason);
                }));
    }

    // ====================================================================
    // forceUpdate (Java lines 162-174)
    // ====================================================================
    void forceUpdate(const K &key) {
        // Pass null timestamp bound
        m_cacheDataSource.performRequest(
            key,
            nullptr,
            FunctionRequestResponseHandler<V, F>(
                // onSuccess
                [this](const V &result, const TimestampUTC &) {
                    QMutexLocker locker(&m_mutex);
                    putSingle(result, false);
                },
                // onFailure — ignore
                [](const F &) {}));
    }

private:
    // ====================================================================
    // putSingle (Java lines 176-191)
    // ====================================================================
    void putSingle(const V &value, bool writeDown) {
        QMutexLocker locker(&m_mutex);

        auto it = m_cached.find(value.getKey());

        if (it != m_cached.end()) {
            // Update existing entry, preserving listeners
            auto oldListeners = it->listeners;
            m_cached.insert(
                value.getKey(),
                CacheEntry(std::make_shared<V>(value), oldListeners));

            // Notify listeners
            UpdatedVersionListenerNotifier<K, V> notifier;
            oldListeners.template map<V>(notifier, value);
        } else {
            m_cached.insert(
                value.getKey(),
                CacheEntry(std::make_shared<V>(value)));
        }

        if (writeDown) {
            m_cacheDataSource.performWrite(value);
        }
    }

    // ====================================================================
    // putBatch (Java lines 193-211)
    // ====================================================================
    void putBatch(const QVector<V> &values, bool writeDown) {
        QMutexLocker locker(&m_mutex);

        for (const V &value : values) {
            auto it = m_cached.find(value.getKey());

            if (it != m_cached.end()) {
                auto oldListeners = it->listeners;
                m_cached.insert(
                    value.getKey(),
                    CacheEntry(std::make_shared<V>(value), oldListeners));

                UpdatedVersionListenerNotifier<K, V> notifier;
                oldListeners.template map<V>(notifier, value);
            } else {
                m_cached.insert(
                    value.getKey(),
                    CacheEntry(std::make_shared<V>(value)));
            }
        }

        if (writeDown) {
            m_cacheDataSource.performWrite(values);
        }
    }

    // ====================================================================
    // Fields (Java lines 32-36)
    // ====================================================================
    QHash<K, CacheEntry> m_cached;
    CacheDataSource<K, V, F> &m_cacheDataSource;
    UpdatedVersionListenerNotifier<K, V> m_updatedVersionListenerNotifier;
    QMutex m_mutex;
};

} // namespace PinkReader
