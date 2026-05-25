/*
 * PinkReader - GPLv3
 * File: updated_version_listener.h
 * Exact port of UpdatedVersionListener.java (22 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/UpdatedVersionListener.java
 *
 * Original: public interface UpdatedVersionListener<K, V extends WritableObject<K>>
 *   void onUpdatedVersion(V data);
 */

#pragma once

#include "writable_object.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.UpdatedVersionListener
 *
 * Listener interface for when a cached object's version is updated.
 * Template parameters:
 *   K - key type
 *   V - value type (must extend WritableObject<K>)
 */
template<typename K, typename V>
class UpdatedVersionListener {
public:
    virtual ~UpdatedVersionListener() = default;

    /**
     * Port of: void onUpdatedVersion(V data);
     */
    virtual void onUpdatedVersion(const V &data) = 0;
};

} // namespace PinkReader
