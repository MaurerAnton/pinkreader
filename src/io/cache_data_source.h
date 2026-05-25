/*
 * PinkReader - GPLv3
 * File: cache_data_source.h
 * Exact port of CacheDataSource.java (39 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/CacheDataSource.java
 *
 * Original: public interface CacheDataSource<K, V, F>
 * Methods:
 *   void performRequest(K key, TimestampBound timestampBound, RequestResponseHandler<V, F> handler);
 *   void performRequest(Collection<K> keys, TimestampBound timestampBound,
 *                       RequestResponseHandler<HashMap<K, V>, F> handler);
 *   void performWrite(V value);
 *   void performWrite(Collection<V> values);
 *
 * Every method signature ported exactly.
 */

#pragma once

#include <vector>
#include <unordered_map>
#include "request_response_handler.h"
#include "../common/timestamp_bound.h"  // Port of common.TimestampBound

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.CacheDataSource
 *
 * Interface for cache data sources. Template parameters:
 *   K - key type
 *   V - value type
 *   F - failure reason type
 */
template<typename K, typename V, typename F>
class CacheDataSource {
public:
    virtual ~CacheDataSource() = default;

    /**
     * Port of: void performRequest(K key, final TimestampBound timestampBound,
     *                              RequestResponseHandler<V, F> handler);
     */
    virtual void performRequest(
            const K &key,
            const TimestampBound &timestampBound,
            RequestResponseHandler<V, F> &handler) = 0;

    /**
     * Port of: void performRequest(Collection<K> keys,
     *     final TimestampBound timestampBound,
     *     RequestResponseHandler<HashMap<K, V>, F> handler);
     */
    virtual void performRequest(
            const std::vector<K> &keys,
            const TimestampBound &timestampBound,
            RequestResponseHandler<std::unordered_map<K, V>, F> &handler) = 0;

    /**
     * Port of: void performWrite(V value);
     */
    virtual void performWrite(const V &value) = 0;

    /**
     * Port of: void performWrite(Collection<V> values);
     */
    virtual void performWrite(const std::vector<V> &values) = 0;
};

} // namespace PinkReader
