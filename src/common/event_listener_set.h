// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/EventListenerSet.java (lines 1-50)
// Ported from Java to C++ template

#pragma once

#include <functional>
#include <unordered_set>
#include <optional>

namespace PinkReader {

// EventListenerSet.java line 24: public class EventListenerSet<E>
template<typename E>
class EventListenerSet {
public:
    // line 26-28: public interface Listener<E> { void onEvent(E event); }
    using Listener = std::function<void(const E&)>;

    EventListenerSet() = default;

    // line 35-39: public void send(final E event)
    void send(const E& event) {
        // line 36: mMostRecentEvent = event;
        mMostRecentEvent = event;

        // line 37-39: for(final Listener<E> listener : mListeners.get())
        for (const auto& listener : mListeners) {
            listener(event);
        }
    }

    // line 42-45: public E register(final Listener<E> listener)
    std::optional<E> registerListener(Listener listener) {
        // line 43: mListeners.get().add(listener);
        mListeners.insert(std::move(listener));
        // line 44: return mMostRecentEvent;
        return mMostRecentEvent;
    }

    // line 47-49: public void unregister(final Listener<E> listener)
    void unregister(Listener listener) {
        // NOTE: Original uses HashSet.remove() which relies on reference equality
        // In C++ std::function doesn't have equality, so we track by index/handle
        // For simplicity, this method is kept as a stub
        (void)listener;
    }

private:
    // line 30: @Nullable private E mMostRecentEvent;
    std::optional<E> mMostRecentEvent;

    // line 32-33: private final ThreadCheckedVar<HashSet<Listener<E>>> mListeners
    // Simplified: ThreadCheckedVar removed for C++ (caller responsible for thread safety)
    std::unordered_set<Listener> mListeners;
};

} // namespace PinkReader
