// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/CachedHash.kt (lines 1-44)
// Ported from Kotlin class to C++ template

#pragma once

#include <string>
#include <functional>
#include <atomic>
#include <memory>
#include <optional>

namespace PinkReader {

// CachedHash.kt line 22: class CachedStringHash(private val data: () -> String)
// Original: computes SHA-256 hash, caching result based on input data
class CachedStringHash {
public:
    // line 24: private class CacheEntry(val key: String, val value: String)
    struct CacheEntry {
        std::string key;
        std::string value;

        CacheEntry(std::string k, std::string v)
            : key(std::move(k))
            , value(std::move(v)) {}
    };

    // line 22: private val data: () -> String
    explicit CachedStringHash(std::function<std::string()> dataProvider)
        : mData(std::move(dataProvider)) {}

    // line 31-43: val hash: String get()
    std::string hash() {
        const std::string data = mData();

        // line 29: private var cache = AtomicReference<CacheEntry?>()
        auto* entry = mCache.load(std::memory_order_acquire);

        // line 35: if (data == this?.key) return value
        if (entry != nullptr && data == entry->key) {
            return entry->value;
        }

        // line 39: val result = General.sha256(data)
        std::string result = sha256(data);

        // line 41: cache.set(CacheEntry(data, result))
        auto newEntry = std::make_unique<CacheEntry>(data, result);
        mCache.store(newEntry.release(), std::memory_order_release);

        return result;
    }

private:
    std::function<std::string()> mData;
    // line 29: private var cache = AtomicReference<CacheEntry?>()
    std::atomic<CacheEntry*> mCache{nullptr};

    static std::string sha256(const std::string& data);
};

} // namespace PinkReader
