/*
 * PinkReader - GPLv3
 * File: threaded_raw_object_db.h
 * Exact port of ThreadedRawObjectDB.java (281 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/ThreadedRawObjectDB.java
 *
 * Original: public class ThreadedRawObjectDB<K, V extends WritableObject<K>, F>
 *     implements CacheDataSource<K, V, F>
 *
 * Every field, method, inner class, and logic branch ported exactly.
 * All implementation in header (template class).
 */

#pragma once

#include "cache_data_source.h"
#include "request_response_handler.h"
#include "../common/triggerable_thread.h"
#include "../common/timestamp_bound.h"
#include "../utils/reddit_time.h"
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>

// Forward declaration
namespace PinkReader {
template<typename K, typename V> class RawObjectDB;
}

namespace PinkReader {

template<typename K, typename V, typename F>
class ThreadedRawObjectDB : public CacheDataSource<K, V, F> {
public:
    // ========================================================================
    // Constructor — Java lines 46-51
    // ========================================================================
    ThreadedRawObjectDB(RawObjectDB<K, V> &db, CacheDataSource<K, V, F> &alternateSource)
        : m_db(db), m_alternateSource(alternateSource),
          m_writeThread([this]() { doWrite(); }, 1500),
          m_readThread([this]() { doRead(); }, 0) {}

    // ========================================================================
    // CacheDataSource interface
    // ========================================================================

    // Port of: performRequest(K, ...) — Java lines 76-82
    void performRequest(
            const K &key,
            const TimestampBound &timestampBound,
            RequestResponseHandler<V, F> &handler) override {

        std::lock_guard<std::mutex> lock(m_readMutex);
        // Port of: toRead.offer(new SingleReadOperation(timestampBound, handler, key));
        m_toRead.push(std::make_unique<SingleReadOperation>(
            timestampBound, handler, key, *this));
        // Port of: readThread.trigger();
        m_readThread.trigger();
    }

    // Port of: performRequest(Collection<K>, ...) — Java lines 85-91
    void performRequest(
            const std::vector<K> &keys,
            const TimestampBound &timestampBound,
            RequestResponseHandler<std::unordered_map<K, V>, F> &handler) override {

        std::lock_guard<std::mutex> lock(m_readMutex);
        m_toRead.push(std::make_unique<BulkReadOperation>(
            timestampBound, handler, keys, *this));
        m_readThread.trigger();
    }

    // Port of: performWrite(V) — Java lines 94-101
    void performWrite(const V &value) override {
        {
            std::lock_guard<std::mutex> lock(m_writeMutex);
            m_toWrite[value.getKey()] = value;
        }
        m_writeThread.trigger();
    }

    // Port of: performWrite(Collection<V>) — Java lines 104-113
    void performWrite(const std::vector<V> &values) override {
        {
            std::lock_guard<std::mutex> lock(m_writeMutex);
            for (const V &value : values) {
                m_toWrite[value.getKey()] = value;
            }
        }
        m_writeThread.trigger();
    }

private:
    // ========================================================================
    // doWrite — Java lines 53-65
    // ========================================================================
    void doWrite() {
        std::lock_guard<std::mutex> ioLock(m_ioLock);

        std::vector<V> values;

        {
            std::lock_guard<std::mutex> lock(m_writeMutex);
            values.reserve(m_toWrite.size());
            for (auto &pair : m_toWrite) {
                values.push_back(pair.second);
            }
            m_toWrite.clear();
        }

        // Port of: db.putAll(values);
        m_db.putAll(values);
    }

    // ========================================================================
    // doRead — Java lines 67-73
    // ========================================================================
    void doRead() {
        std::lock_guard<std::mutex> ioLock(m_ioLock);

        while (true) {
            std::unique_ptr<ReadOperation> op;
            {
                std::lock_guard<std::mutex> lock(m_readMutex);
                if (m_toRead.empty()) break;
                op = std::move(m_toRead.front());
                m_toRead.pop();
            }
            if (op) {
                op->run();
            }
        }
    }

    // ========================================================================
    // ReadOperation base — Java lines 271-280
    // (TimestamBound is abstract in Java; we use a unique_ptr in C++)
    // ========================================================================
    struct ReadOperation {
        virtual ~ReadOperation() = default;
        virtual void run() = 0;
    };

    // ========================================================================
    // BulkReadOperation — Java lines 115-213
    // ========================================================================
    struct BulkReadOperation : public ReadOperation {
        std::vector<K> keys;
        RequestResponseHandler<std::unordered_map<K, V>, F> &responseHandler;
        ThreadedRawObjectDB &owner;
        // Store as a copy of the concrete subclass (MoreRecentThanBound typically)
        std::unique_ptr<TimestampBound> timestampBound;

        BulkReadOperation(
                const TimestampBound &tb,
                RequestResponseHandler<std::unordered_map<K, V>, F> &responseHandler_,
                const std::vector<K> &keys_,
                ThreadedRawObjectDB &owner_)
            : keys(keys_),
              responseHandler(responseHandler_),
              owner(owner_),
              // Clone via virtual copy constructor pattern
              timestampBound(tb.clone()) {}

        void run() override {
            std::unordered_map<K, V> existingResult;
            existingResult.reserve(keys.size());
            bool hasOldest = false;
            TimestampUTC oldestStamp;

            // Port of: synchronized(toWrite)
            {
                std::lock_guard<std::mutex> lock(owner.m_writeMutex);

                auto iter = keys.begin();
                while (iter != keys.end()) {
                    const K &key = *iter;
                    auto writeResult = owner.m_toWrite.find(key);
                    if (writeResult != owner.m_toWrite.end()
                        && timestampBound->verifyTimestamp(writeResult->second.getTimestamp())) {
                        existingResult[key] = writeResult->second;
                        if (!hasOldest) {
                            oldestStamp = writeResult->second.getTimestamp();
                            hasOldest = true;
                        } else {
                            oldestStamp = TimestampUTC::oldest(
                                oldestStamp, writeResult->second.getTimestamp());
                        }
                        iter = keys.erase(iter);
                    } else {
                        ++iter;
                    }
                }
            }

            if (keys.empty()) {
                responseHandler.onRequestSuccess(existingResult, oldestStamp);
                return;
            }

            {
                auto iter = keys.begin();
                while (iter != keys.end()) {
                    const K &key = *iter;
                    V dbResult = owner.m_db.getById(key);
                    if (dbResult.getKey() == key
                        && timestampBound->verifyTimestamp(dbResult.getTimestamp())) {
                        existingResult[key] = dbResult;
                        if (!hasOldest) {
                            oldestStamp = dbResult.getTimestamp();
                            hasOldest = true;
                        } else {
                            oldestStamp = TimestampUTC::oldest(
                                oldestStamp, dbResult.getTimestamp());
                        }
                        iter = keys.erase(iter);
                    } else {
                        ++iter;
                    }
                }
            }

            if (keys.empty()) {
                responseHandler.onRequestSuccess(existingResult, oldestStamp);
                return;
            }

            TimestampUTC outerOldestTimestamp = oldestStamp;

            struct AltHandler : public RequestResponseHandler<std::unordered_map<K, V>, F> {
                RequestResponseHandler<std::unordered_map<K, V>, F> &origHandler;
                std::unordered_map<K, V> &existingRes;
                TimestampUTC outerOldest;
                ThreadedRawObjectDB &ownerRef;

                AltHandler(RequestResponseHandler<std::unordered_map<K, V>, F> &h,
                           std::unordered_map<K, V> &er,
                           const TimestampUTC &oot,
                           ThreadedRawObjectDB &o)
                    : origHandler(h), existingRes(er), outerOldest(oot), ownerRef(o) {}

                void onRequestFailed(const F &failureReason) override {
                    origHandler.onRequestFailed(failureReason);
                }

                void onRequestSuccess(const std::unordered_map<K, V> &result,
                                      const TimestampUTC &timeCached) override {
                    TimestampUTC timestamp = TimestampUTC::oldest(outerOldest, timeCached);

                    std::vector<V> writeVals;
                    writeVals.reserve(result.size());
                    for (auto &pair : result) {
                        writeVals.push_back(pair.second);
                    }
                    ownerRef.performWrite(writeVals);

                    existingRes.insert(result.begin(), result.end());
                    origHandler.onRequestSuccess(existingRes, timestamp);
                }
            };

            AltHandler altHandler(responseHandler, existingResult,
                                  outerOldestTimestamp, owner);
            owner.m_alternateSource.performRequest(keys, *timestampBound, altHandler);
        }
    };

    // ========================================================================
    // SingleReadOperation — Java lines 216-268
    // ========================================================================
    struct SingleReadOperation : public ReadOperation {
        K key;
        RequestResponseHandler<V, F> &responseHandler;
        ThreadedRawObjectDB &owner;
        std::unique_ptr<TimestampBound> timestampBound;

        SingleReadOperation(
                const TimestampBound &tb,
                RequestResponseHandler<V, F> &responseHandler_,
                const K &key_,
                ThreadedRawObjectDB &owner_)
            : key(key_),
              responseHandler(responseHandler_),
              owner(owner_),
              timestampBound(tb.clone()) {}

        void run() override {
            {
                std::lock_guard<std::mutex> lock(owner.m_writeMutex);
                auto writeResult = owner.m_toWrite.find(key);
                if (writeResult != owner.m_toWrite.end()
                    && timestampBound->verifyTimestamp(writeResult->second.getTimestamp())) {
                    responseHandler.onRequestSuccess(
                        writeResult->second, writeResult->second.getTimestamp());
                    return;
                }
            }

            V dbResult = owner.m_db.getById(key);
            if (dbResult.getKey() == key
                && timestampBound->verifyTimestamp(dbResult.getTimestamp())) {
                responseHandler.onRequestSuccess(dbResult, dbResult.getTimestamp());
                return;
            }

            struct AltHandler : public RequestResponseHandler<V, F> {
                RequestResponseHandler<V, F> &origHandler;
                ThreadedRawObjectDB &ownerRef;

                AltHandler(RequestResponseHandler<V, F> &h, ThreadedRawObjectDB &o)
                    : origHandler(h), ownerRef(o) {}

                void onRequestFailed(const F &failureReason) override {
                    origHandler.onRequestFailed(failureReason);
                }

                void onRequestSuccess(const V &result,
                                      const TimestampUTC &timeCached) override {
                    ownerRef.performWrite(result);
                    origHandler.onRequestSuccess(result, timeCached);
                }
            };

            AltHandler altHandler(responseHandler, owner);
            owner.m_alternateSource.performRequest(key, *timestampBound, altHandler);
        }
    };

    // ========================================================================
    // Fields — exact ports of Java fields
    // ========================================================================

    // Port of: private final TriggerableThread writeThread = new TriggerableThread(this::doWrite, 1500);
    TriggerableThread m_writeThread;

    // Port of: private final TriggerableThread readThread = new TriggerableThread(this::doRead, 0);
    TriggerableThread m_readThread;

    // Port of: private final HashMap<K, V> toWrite = new HashMap<>();
    std::unordered_map<K, V> m_toWrite;
    std::mutex m_writeMutex;

    // Port of: private final LinkedBlockingQueue<ReadOperation> toRead
    std::queue<std::unique_ptr<ReadOperation>> m_toRead;
    std::mutex m_readMutex;

    // Port of: private final Object ioLock = new Object();
    std::mutex m_ioLock;

    // Port of: private final RawObjectDB<K, V> db;
    RawObjectDB<K, V> &m_db;

    // Port of: private final CacheDataSource<K, V, F> alternateSource;
    CacheDataSource<K, V, F> &m_alternateSource;
};

} // namespace PinkReader
