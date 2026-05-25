/*
 * PinkReader - GPLv3
 * File: weak_cache.h
 * Exact port of WeakCache.java (229 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/WeakCache.java
 *
 * Original: public final class WeakCache<K, V extends WritableObject<K>, F>
 *     implements CacheDataSource<K, V, F>
 *
 * Same as PermanentCache but uses WeakReference<V> for stored values.
 * Every field, method, inner class, and logic branch ported exactly.
 */

#pragma once

#include "cache_data_source.h"
#include "request_response_handler.h"
#include "updated_version_listener.h"
#include "updated_version_listener_notifier.h"
#include "../common/collections/weak_ref_list_manager.h"
#include "../utils/reddit_time.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <mutex>

namespace PinkReader {

template<typename K, typename V, typename F>
class WeakCache : public CacheDataSource<K, V, F> {
public:
    // ========================================================================
    // Constructor — Java line 38-39
    // Port of: public WeakCache(final CacheDataSource<K, V, F> cacheDataSource)
    // ========================================================================
    explicit WeakCache(CacheDataSource<K, V, F> &cacheDataSource)
        : m_cacheDataSource(cacheDataSource) {}

    // ========================================================================
    // performRequest(K key, ...) — Java lines 42-48
    // ========================================================================
    void performRequest(
            const K &key,
            const TimestampBound &timestampBound,
            RequestResponseHandler<V, F> &handler) override {
        performRequest(key, timestampBound, handler, nullptr);
    }

    // ========================================================================
    // performRequest(Collection<K> keys, ...) — Java lines 50-111
    // ========================================================================
    void performRequest(
            const std::vector<K> &keys,
            const TimestampBound &timestampBound,
            RequestResponseHandler<std::unordered_map<K, V>, F> &handler) override {

        std::unordered_set<K> keysRemaining(keys.begin(), keys.end());
        std::unordered_map<K, V> cacheResult;
        cacheResult.reserve(keys.size());
        TimestampUTC *oldestTimestamp = nullptr;
        TimestampUTC oldestStamp;

        std::lock_guard<std::mutex> lock(m_mutex);

        // Port of: for(final K key : keys)
        for (const K &key : keys) {
            auto entryIt = m_cached.find(key);
            if (entryIt != m_cached.end()) {
                // Port of: final V value = entry.data.get();
                auto sp = entryIt->second.data.lock();
                if (sp) {
                    const V &value = *sp;
                    // Port of: if(timestampBound.verifyTimestamp(value.getTimestamp()))
                    if (timestampBound.verifyTimestamp(value.getTimestamp())) {
                        keysRemaining.erase(key);
                        cacheResult[key] = value;
                        if (!oldestTimestamp) {
                            oldestStamp = value.getTimestamp();
                            oldestTimestamp = &oldestStamp;
                        } else {
                            oldestStamp = TimestampUTC::oldest(
                                oldestStamp, value.getTimestamp());
                        }
                    }
                }
            }
        }

        if (!keysRemaining.empty()) {
            TimestampUTC outerOldestTimestamp = oldestStamp;
            std::vector<K> remainingVec(keysRemaining.begin(), keysRemaining.end());

            struct BulkHandler : public RequestResponseHandler<std::unordered_map<K, V>, F> {
                RequestResponseHandler<std::unordered_map<K, V>, F> *origHandler;
                std::unordered_map<K, V> &cacheRes;
                TimestampUTC outerOldest;

                BulkHandler(RequestResponseHandler<std::unordered_map<K, V>, F> *h,
                            std::unordered_map<K, V> &cr,
                            const TimestampUTC &oot)
                    : origHandler(h), cacheRes(cr), outerOldest(oot) {}

                void onRequestFailed(const F &failureReason) override {
                    origHandler->onRequestFailed(failureReason);
                }

                void onRequestSuccess(const std::unordered_map<K, V> &result,
                                      const TimestampUTC &timeCached) override {
                    cacheRes.insert(result.begin(), result.end());
                    TimestampUTC timestamp = TimestampUTC::oldest(timeCached, outerOldest);
                    origHandler->onRequestSuccess(cacheRes, timestamp);
                }
            };

            BulkHandler bulkHandler(&handler, cacheResult, outerOldestTimestamp);
            m_cacheDataSource.performRequest(remainingVec, timestampBound, bulkHandler);

        } else {
            handler.onRequestSuccess(cacheResult, oldestStamp);
        }
    }

    // ========================================================================
    // performWrite — Java lines 112-121
    // ========================================================================
    void performWrite(const V &value) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        put(value, true);
    }

    void performWrite(const std::vector<V> &values) override {
        std::lock_guard<std::mutex> lock(m_mutex);
        put(values, true);
    }

    // ========================================================================
    // performRequest with UpdatedVersionListener — Java lines 123-159
    // ========================================================================
    void performRequest(
            const K &key,
            const TimestampBound &timestampBound,
            RequestResponseHandler<V, F> &handler,
            UpdatedVersionListener<K, V> *updatedVersionListener) {

        std::lock_guard<std::mutex> lock(m_mutex);

        // Port of: if(timestampBound != null)
        {
            auto existingEntry = m_cached.find(key);
            if (existingEntry != m_cached.end()) {
                // Port of: final V existing = existingEntry.data.get();
                auto sp = existingEntry->second.data.lock();
                if (sp) {
                    const V &existing = *sp;
                    if (timestampBound.verifyTimestamp(existing.getTimestamp())) {
                        handler.onRequestSuccess(existing, existing.getTimestamp());
                        return;
                    }
                }
            }
        }

        auto *pThis = this;
        auto *pHandler = &handler;
        auto *pListener = updatedVersionListener;
        K keyCopy = key;

        struct SingleHandler : public RequestResponseHandler<V, F> {
            WeakCache *cache;
            RequestResponseHandler<V, F> *origHandler;
            UpdatedVersionListener<K, V> *listener;
            K key;

            SingleHandler(WeakCache *c, RequestResponseHandler<V, F> *h,
                          UpdatedVersionListener<K, V> *l, const K &k)
                : cache(c), origHandler(h), listener(l), key(k) {}

            void onRequestFailed(const F &failureReason) override {
                origHandler->onRequestFailed(failureReason);
            }

            void onRequestSuccess(const V &result, const TimestampUTC &timeCached) override {
                std::lock_guard<std::mutex> lock(cache->m_mutex);
                cache->put(result, false);
                if (listener) {
                    auto entryIt = cache->m_cached.find(key);
                    if (entryIt != cache->m_cached.end()) {
                        entryIt->second.listeners.add(
                            std::shared_ptr<UpdatedVersionListener<K, V>>(
                                listener, [](UpdatedVersionListener<K, V> *){}));
                    }
                }
                origHandler->onRequestSuccess(result, timeCached);
            }
        };

        SingleHandler singleHandler(pThis, pHandler, pListener, keyCopy);
        m_cacheDataSource.performRequest(keyCopy, timestampBound, singleHandler);
    }

    // ========================================================================
    // forceUpdate — Java lines 161-173
    // ========================================================================
    void forceUpdate(const K &key) {
        auto *pThis = this;

        struct ForceHandler : public RequestResponseHandler<V, F> {
            WeakCache *cache;
            explicit ForceHandler(WeakCache *c) : cache(c) {}

            void onRequestFailed(const F & /*failureReason*/) override {}

            void onRequestSuccess(const V &result,
                                  const TimestampUTC & /*timeCached*/) override {
                std::lock_guard<std::mutex> lock(cache->m_mutex);
                cache->put(result, false);
            }
        };

        ForceHandler forceHandler(pThis);
        m_cacheDataSource.performRequest(key, TimestampBound::ANY(), forceHandler);
    }

private:
    // ========================================================================
    // put — Java lines 176-211
    // ========================================================================
    void put(const V &value, bool writeDown) {
        K key = value.getKey();
        auto oldEntryIt = m_cached.find(key);

        // Port of: if(oldEntry != null)
        if (oldEntryIt != m_cached.end()) {
            // Port of: cached.put(value.getKey(),
            //     new CacheEntry(new WeakReference<>(value), oldEntry.listeners));
            m_cached[key] = CacheEntry(
                std::make_shared<V>(value),
                oldEntryIt->second.listeners);
            // Port of: oldEntry.listeners.map(updatedVersionListenerNotifier, value);
            oldEntryIt->second.listeners.map(
                m_updatedVersionListenerNotifier,
                const_cast<V &>(value));
        } else {
            // Port of: cached.put(value.getKey(),
            //     new CacheEntry(new WeakReference<>(value)));
            m_cached[key] = CacheEntry(std::make_shared<V>(value));
        }

        // Port of: if(writeDown) { cacheDataSource.performWrite(value); }
        if (writeDown) {
            m_cacheDataSource.performWrite(value);
        }
    }

    // Port of: put(Collection<V>, boolean) — Java lines 193-211
    void put(const std::vector<V> &values, bool writeDown) {
        for (const V &value : values) {
            K key = value.getKey();
            auto oldEntryIt = m_cached.find(key);

            if (oldEntryIt != m_cached.end()) {
                m_cached[key] = CacheEntry(
                    std::make_shared<V>(value),
                    oldEntryIt->second.listeners);
                oldEntryIt->second.listeners.map(
                    m_updatedVersionListenerNotifier,
                    const_cast<V &>(value));
            } else {
                m_cached[key] = CacheEntry(std::make_shared<V>(value));
            }
        }

        if (writeDown) {
            m_cacheDataSource.performWrite(values);
        }
    }

    // ========================================================================
    // Inner class: CacheEntry — Java lines 213-227
    // ========================================================================
    struct CacheEntry {
        // Port of: public final WeakReference<V> data;
        std::weak_ptr<V> data;

        // Port of: public final WeakReferenceListManager<UpdatedVersionListener<K, V>> listeners;
        WeakReferenceListManager<UpdatedVersionListener<K, V>> listeners;

        // Port of: private CacheEntry(final WeakReference<V> data,
        //     final WeakReferenceListManager<...> listeners)
        CacheEntry(const std::shared_ptr<V> &data_,
                   const WeakReferenceListManager<UpdatedVersionListener<K, V>> &listeners_)
            : data(data_), listeners(listeners_) {}

        // Port of: private CacheEntry(final WeakReference<V> data)
        //   this(data, new WeakReferenceListManager<UpdatedVersionListener<K, V>>());
        explicit CacheEntry(const std::shared_ptr<V> &data_)
            : data(data_), listeners() {}
    };

    // ========================================================================
    // Fields — exact ports
    // ========================================================================
    std::unordered_map<K, CacheEntry> m_cached;
    CacheDataSource<K, V, F> &m_cacheDataSource;
    UpdatedVersionListenerNotifier<K, V> m_updatedVersionListenerNotifier;
    std::mutex m_mutex;
};

} // namespace PinkReader
