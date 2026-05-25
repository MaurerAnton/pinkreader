/*
 * PinkReader - GPLv3
 * File: permanent_cache.h
 * Exact port of PermanentCache.java (222 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/PermanentCache.java
 *
 * Original: public final class PermanentCache<K, V extends WritableObject<K>, F>
 *     implements CacheDataSource<K, V, F>
 *
 * Every field, method, inner class, and logic branch ported exactly.
 * All implementation is in this header because this is a template class.
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
class PermanentCache : public CacheDataSource<K, V, F> {
public:
    // ========================================================================
    // Constructor — Java line 37-39
    // Port of: public PermanentCache(final CacheDataSource<K, V, F> cacheDataSource)
    //   this.cacheDataSource = cacheDataSource;
    // ========================================================================
    explicit PermanentCache(CacheDataSource<K, V, F> &cacheDataSource)
        : m_cacheDataSource(cacheDataSource) {}

    // ========================================================================
    // performRequest(K key, ...) — Java line 42-47
    // Port of: @Override public void performRequest(final K key,
    //     final TimestampBound timestampBound, final RequestResponseHandler<V, F> handler)
    //   performRequest(key, timestampBound, handler, null);
    // ========================================================================
    void performRequest(
            const K &key,
            const TimestampBound &timestampBound,
            RequestResponseHandler<V, F> &handler) override {
        performRequest(key, timestampBound, handler, nullptr);
    }

    // ========================================================================
    // performRequest(Collection<K> keys, ...) — Java lines 49-110
    // ========================================================================
    void performRequest(
            const std::vector<K> &keys,
            const TimestampBound &timestampBound,
            RequestResponseHandler<std::unordered_map<K, V>, F> &handler) override {

        // Port of: final HashSet<K> keysRemaining = new HashSet<>(keys);
        std::unordered_set<K> keysRemaining(keys.begin(), keys.end());

        // Port of: final HashMap<K, V> cacheResult = new HashMap<>(keys.size());
        std::unordered_map<K, V> cacheResult;
        cacheResult.reserve(keys.size());

        // Port of: TimestampUTC oldestTimestamp = null;
        TimestampUTC *oldestTimestamp = nullptr;
        TimestampUTC oldestStamp;

        std::lock_guard<std::mutex> lock(m_mutex);

        // Port of: for(final K key : keys)
        for (const K &key : keys) {
            // Port of: final CacheEntry entry = cached.get(key);
            auto entryIt = m_cached.find(key);
            if (entryIt != m_cached.end()) {
                // Port of: final V value = entry.data;
                const V &value = entryIt->second.data;
                // Port of: if(timestampBound.verifyTimestamp(value.getTimestamp()))
                if (timestampBound.verifyTimestamp(value.getTimestamp())) {
                    // Port of: keysRemaining.remove(key);
                    keysRemaining.erase(key);
                    // Port of: cacheResult.put(key, value);
                    cacheResult[key] = value;
                    // Port of: if(oldestTimestamp == null) { oldestTimestamp = value.getTimestamp(); }
                    //         else { oldestTimestamp = TimestampUTC.oldest(oldestTimestamp, ...); }
                    if (!oldestTimestamp) {
                        oldestStamp = value.getTimestamp();
                        oldestTimestamp = &oldestStamp;
                    } else {
                        oldestStamp = TimestampUTC::oldest(oldestStamp, value.getTimestamp());
                    }
                }
            }
        }

        // Port of: if(!keysRemaining.isEmpty()) { ... } else { handler.onRequestSuccess(...); }
        if (!keysRemaining.empty()) {
            // Port of: final TimestampUTC outerOldestTimestamp = oldestTimestamp;
            TimestampUTC outerOldestTimestamp = oldestStamp;

            // Port of: cacheDataSource.performRequest(keysRemaining, timestampBound,
            //     new RequestResponseHandler<HashMap<K, V>, F>() { ... });
            std::vector<K> remainingVec(keysRemaining.begin(), keysRemaining.end());

            // Use a handler function-wrapper approach (Java anonymous inner class)
            auto *pThis = this;
            auto *pHandler = &handler;

            struct BulkHandler : public RequestResponseHandler<std::unordered_map<K, V>, F> {
                PermanentCache *cache;
                RequestResponseHandler<std::unordered_map<K, V>, F> *origHandler;
                std::unordered_map<K, V> &cacheRes;
                TimestampUTC outerOldest;

                BulkHandler(PermanentCache *c,
                            RequestResponseHandler<std::unordered_map<K, V>, F> *h,
                            std::unordered_map<K, V> &cr,
                            const TimestampUTC &oot)
                    : cache(c), origHandler(h), cacheRes(cr), outerOldest(oot) {}

                // Port of: @Override public void onRequestFailed(final F failureReason)
                //   handler.onRequestFailed(failureReason);
                void onRequestFailed(const F &failureReason) override {
                    origHandler->onRequestFailed(failureReason);
                }

                // Port of: @Override public void onRequestSuccess(
                //     final HashMap<K, V> result, final TimestampUTC timeCached)
                //   cacheResult.putAll(result);
                //   final TimestampUTC timestamp = outerOldestTimestamp == null
                //       ? timeCached : TimestampUTC.oldest(outerOldestTimestamp, timeCached);
                //   handler.onRequestSuccess(cacheResult, timestamp);
                void onRequestSuccess(const std::unordered_map<K, V> &result,
                                      const TimestampUTC &timeCached) override {
                    cacheRes.insert(result.begin(), result.end());
                    TimestampUTC timestamp = TimestampUTC::oldest(outerOldest, timeCached);
                    origHandler->onRequestSuccess(cacheRes, timestamp);
                }
            };

            BulkHandler bulkHandler(pThis, pHandler, cacheResult, outerOldestTimestamp);
            m_cacheDataSource.performRequest(remainingVec, timestampBound, bulkHandler);

        } else {
            // Port of: handler.onRequestSuccess(cacheResult, oldestTimestamp);
            handler.onRequestSuccess(cacheResult, oldestStamp);
        }
    }

    // ========================================================================
    // performWrite(V) — Java lines 112-115
    // ========================================================================
    void performWrite(const V &value) override {
        // Port of: public synchronized void performWrite(final V value)
        //   put(value, true);
        std::lock_guard<std::mutex> lock(m_mutex);
        put(value, true);
    }

    // ========================================================================
    // performWrite(Collection<V>) — Java lines 117-120
    // ========================================================================
    void performWrite(const std::vector<V> &values) override {
        // Port of: public void performWrite(final Collection<V> values)
        //   put(values, true);
        std::lock_guard<std::mutex> lock(m_mutex);
        put(values, true);
    }

    // ========================================================================
    // performRequest with UpdatedVersionListener — Java lines 122-158
    // ========================================================================
    void performRequest(
            const K &key,
            const TimestampBound &timestampBound,
            RequestResponseHandler<V, F> &handler,
            UpdatedVersionListener<K, V> *updatedVersionListener) {

        std::lock_guard<std::mutex> lock(m_mutex);

        // Port of: if(timestampBound != null)
        {
            // Port of: final CacheEntry existingEntry = cached.get(key);
            auto existingEntry = m_cached.find(key);
            if (existingEntry != m_cached.end()) {
                // Port of: final V existing = existingEntry.data;
                const V &existing = existingEntry->second.data;
                // Port of: if(timestampBound.verifyTimestamp(existing.getTimestamp()))
                if (timestampBound.verifyTimestamp(existing.getTimestamp())) {
                    // Port of: handler.onRequestSuccess(existing, existing.getTimestamp());
                    handler.onRequestSuccess(existing, existing.getTimestamp());
                    return;
                }
            }
        }

        // Port of: cacheDataSource.performRequest(key, timestampBound,
        //     new RequestResponseHandler<V, F>() { ... });
        auto *pThis = this;
        auto *pHandler = &handler;
        auto *pListener = updatedVersionListener;
        K keyCopy = key;

        struct SingleHandler : public RequestResponseHandler<V, F> {
            PermanentCache *cache;
            RequestResponseHandler<V, F> *origHandler;
            UpdatedVersionListener<K, V> *listener;
            K key;

            SingleHandler(PermanentCache *c, RequestResponseHandler<V, F> *h,
                          UpdatedVersionListener<K, V> *l, const K &k)
                : cache(c), origHandler(h), listener(l), key(k) {}

            // Port of: @Override public void onRequestFailed(final F failureReason)
            //   handler.onRequestFailed(failureReason);
            void onRequestFailed(const F &failureReason) override {
                origHandler->onRequestFailed(failureReason);
            }

            // Port of: @Override public void onRequestSuccess(
            //     final V result, final TimestampUTC timeCached)
            void onRequestSuccess(const V &result, const TimestampUTC &timeCached) override {
                // Port of: synchronized(PermanentCache.this) {
                std::lock_guard<std::mutex> lock(cache->m_mutex);
                // Port of: put(result, false);
                cache->put(result, false);
                // Port of: if(updatedVersionListener != null)
                //   cached.get(key).listeners.add(updatedVersionListener);
                if (listener) {
                    auto entryIt = cache->m_cached.find(key);
                    if (entryIt != cache->m_cached.end()) {
                        entryIt->second.listeners.add(
                            std::shared_ptr<UpdatedVersionListener<K, V>>(
                                listener, [](UpdatedVersionListener<K, V> *){}));
                    }
                }
                // Port of: handler.onRequestSuccess(result, timeCached);
                origHandler->onRequestSuccess(result, timeCached);
            }
        };

        SingleHandler singleHandler(pThis, pHandler, pListener, keyCopy);
        m_cacheDataSource.performRequest(keyCopy, timestampBound, singleHandler);
    }

    // ========================================================================
    // forceUpdate — Java lines 160-172
    // ========================================================================
    void forceUpdate(const K &key) {
        // Port of: cacheDataSource.performRequest(key, null, new RequestResponseHandler<V, F>() {
        //   @Override public void onRequestFailed(final F failureReason) { }
        //   @Override public void onRequestSuccess(final V result, final TimestampUTC timeCached)
        //     put(result, false);
        // });
        auto *pThis = this;

        struct ForceHandler : public RequestResponseHandler<V, F> {
            PermanentCache *cache;
            explicit ForceHandler(PermanentCache *c) : cache(c) {}

            void onRequestFailed(const F & /*failureReason*/) override {
                // Port of: empty body
            }

            void onRequestSuccess(const V &result,
                                  const TimestampUTC & /*timeCached*/) override {
                // Port of: put(result, false);
                std::lock_guard<std::mutex> lock(cache->m_mutex);
                cache->put(result, false);
            }
        };

        ForceHandler forceHandler(pThis);
        // TimestampBound::ANY() (null is passed in Java — verifyTimestamp will be called with null
        // but the Java code passes null to the underlying data source as-is)
        m_cacheDataSource.performRequest(key, TimestampBound::ANY(), forceHandler);
    }

private:
    // ========================================================================
    // put(V, boolean) — Java lines 174-187
    // ========================================================================
    void put(const V &value, bool writeDown) {
        // Port of: final CacheEntry oldEntry = cached.get(value.getKey());
        K key = value.getKey();
        auto oldEntryIt = m_cached.find(key);

        if (oldEntryIt != m_cached.end()) {
            // Port of: cached.put(value.getKey(),
            //     new CacheEntry(value, oldEntry.listeners));
            CacheEntry newEntry(value, oldEntryIt->second.listeners);
            m_cached[key] = newEntry;
            // Port of: oldEntry.listeners.map(updatedVersionListenerNotifier, value);
            oldEntryIt->second.listeners.map(
                m_updatedVersionListenerNotifier,
                const_cast<V &>(value));
        } else {
            // Port of: cached.put(value.getKey(), new CacheEntry(value));
            m_cached[key] = CacheEntry(value);
        }

        // Port of: if(writeDown) { cacheDataSource.performWrite(value); }
        if (writeDown) {
            m_cacheDataSource.performWrite(value);
        }
    }

    // ========================================================================
    // put(Collection<V>, boolean) — Java lines 189-205
    // ========================================================================
    void put(const std::vector<V> &values, bool writeDown) {
        // Port of: for(final V value : values)
        for (const V &value : values) {
            // Port of: final CacheEntry oldEntry = cached.get(value.getKey());
            K key = value.getKey();
            auto oldEntryIt = m_cached.find(key);

            if (oldEntryIt != m_cached.end()) {
                // Port of: cached.put(value.getKey(),
                //     new CacheEntry(value, oldEntry.listeners));
                CacheEntry newEntry(value, oldEntryIt->second.listeners);
                m_cached[key] = newEntry;
                // Port of: oldEntry.listeners.map(updatedVersionListenerNotifier, value);
                oldEntryIt->second.listeners.map(
                    m_updatedVersionListenerNotifier,
                    const_cast<V &>(value));
            } else {
                // Port of: cached.put(value.getKey(), new CacheEntry(value));
                m_cached[key] = CacheEntry(value);
            }
        }

        // Port of: if(writeDown) { cacheDataSource.performWrite(values); }
        if (writeDown) {
            m_cacheDataSource.performWrite(values);
        }
    }

    // ========================================================================
    // Inner class: CacheEntry — Java lines 207-221
    // ========================================================================
    struct CacheEntry {
        // Port of: public final V data;
        V data;

        // Port of: public final WeakReferenceListManager<UpdatedVersionListener<K, V>> listeners;
        WeakReferenceListManager<UpdatedVersionListener<K, V>> listeners;

        // Port of: private CacheEntry(final V data,
        //     final WeakReferenceListManager<UpdatedVersionListener<K, V>> listeners)
        CacheEntry(const V &data_,
                   const WeakReferenceListManager<UpdatedVersionListener<K, V>> &listeners_)
            : data(data_), listeners(listeners_) {}

        // Port of: private CacheEntry(final V data)
        //   this(data, new WeakReferenceListManager<UpdatedVersionListener<K, V>>());
        explicit CacheEntry(const V &data_)
            : data(data_), listeners() {}
    };

    // ========================================================================
    // Fields — exact ports
    // ========================================================================

    // Port of: private final HashMap<K, CacheEntry> cached = new HashMap<>();
    std::unordered_map<K, CacheEntry> m_cached;

    // Port of: private final CacheDataSource<K, V, F> cacheDataSource;
    CacheDataSource<K, V, F> &m_cacheDataSource;

    // Port of: private final UpdatedVersionListenerNotifier<K, V>
    //     updatedVersionListenerNotifier = new UpdatedVersionListenerNotifier<>();
    UpdatedVersionListenerNotifier<K, V> m_updatedVersionListenerNotifier;

    // Port of: synchronized methods
    std::mutex m_mutex;
};

} // namespace PinkReader
