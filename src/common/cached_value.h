// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/CachedValue.java (lines 1-74)
// Ported from Java to C++ template

#pragma once

#include <functional>
#include <atomic>
#include <memory>
#include <chrono>

namespace PinkReader {

// CachedValue.java line 25: public class CachedValue<E>
template<typename E>
class CachedValue {
public:
    // line 27-44: private static class CacheEntry<E>
    struct CacheEntry {
        E mValue;
        long long mLastUpdateMs;

        CacheEntry(E value, long long lastUpdateMs)
            : mValue(std::move(value))
            , mLastUpdateMs(lastUpdateMs) {}

        // line 37-39: getValue()
        const E& getValue() const { return mValue; }

        // line 41-43: getLastUpdateMs()
        long long getLastUpdateMs() const { return mLastUpdateMs; }
    };

    // line 46-47: private final fields
    // line 51-57: constructor
    CachedValue(std::function<E()> factory, long long maxAgeMs)
        : mFactory(std::move(factory))
        , mMaxAgeMs(maxAgeMs)
        , mEntry(nullptr) {}

    // line 59-73: public E get()
    E get() {
        // line 62: final long timeNow = SystemClock.uptimeMillis();
        auto timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();

        // line 64: final CacheEntry<E> entry = mEntry.get();
        CacheEntry* entry = mEntry.load(std::memory_order_acquire);

        // line 66-68: if(entry != null && timeNow - entry.getLastUpdateMs() < mMaxAgeMs)
        if (entry != nullptr && timeNow - entry->getLastUpdateMs() < mMaxAgeMs) {
            return entry->getValue();
        }

        // line 70: final E newValue = mFactory.create();
        E newValue = mFactory();

        // line 71: mEntry.set(new CacheEntry<>(newValue, timeNow));
        auto newEntry = std::make_unique<CacheEntry>(std::move(newValue), timeNow);
        mEntry.store(newEntry.release(), std::memory_order_release);

        // line 72: return newValue;
        return newValue;
    }

private:
    // line 46: @NonNull private final GenericFactory<E, RuntimeException> mFactory;
    std::function<E()> mFactory;

    // line 47: private final long mMaxAgeMs;
    long long mMaxAgeMs;

    // line 49: private final AtomicReference<CacheEntry<E>> mEntry = new AtomicReference<>(null);
    std::atomic<CacheEntry*> mEntry;
};

} // namespace PinkReader
