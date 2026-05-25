/*
 * PinkReader - GPLv3
 * File: updated_version_listener_notifier.h
 * Exact port of UpdatedVersionListenerNotifier.java (29 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/UpdatedVersionListenerNotifier.java
 *
 * Original: public class UpdatedVersionListenerNotifier<K, V extends WritableObject<K>>
 *     implements WeakReferenceListManager.ArgOperator<UpdatedVersionListener<K, V>, V>
 *   @Override public void operate(final UpdatedVersionListener<K, V> listener, final V data)
 *     listener.onUpdatedVersion(data);
 *
 * In C++, this is a simple functor struct.
 */

#pragma once

#include "updated_version_listener.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.UpdatedVersionListenerNotifier
 *
 * A functor that calls onUpdatedVersion on a listener with the given data.
 * Used as an ArgOperator for WeakReferenceListManager::map().
 */
template<typename K, typename V>
struct UpdatedVersionListenerNotifier {
    /**
     * Port of: @Override public void operate(
     *     final UpdatedVersionListener<K, V> listener, final V data)
     *   listener.onUpdatedVersion(data);
     */
    void operator()(UpdatedVersionListener<K, V> *listener, const V &data) const {
        if (listener) {
            listener->onUpdatedVersion(data);
        }
    }
};

} // namespace PinkReader
