/*
 * PinkReader - GPLv3
 * File: writable_object.h
 * Exact port of WritableObject.java (62 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/WritableObject.java
 *
 * Original: public interface WritableObject<K>
 *   Inner classes: CreationData, @WritableObjectVersion, @WritableObjectKey,
 *                  @WritableObjectTimestamp, @WritableField
 *   Methods: K getKey(), TimestampUTC getTimestamp()
 *
 * The Java annotations are represented as empty marker structs in C++.
 */

#pragma once

#include <string>
#include <cstdint>
#include "../utils/reddit_time.h"  // Port of common.time.TimestampUTC

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.WritableObject
 *
 * Interface for objects that can be written to/read from a database.
 * Template parameter K is the key type.
 */
template<typename K>
class WritableObject {
public:
    /**
     * Port of: class CreationData
     * Java lines 29-37:
     *   public final String key;
     *   public final long timestamp;
     *   public CreationData(final String key, final long timestamp)
     */
    struct CreationData {
        std::string key;
        int64_t timestamp;

        CreationData(const std::string &key_, int64_t timestamp_)
            : key(key_), timestamp(timestamp_) {}
    };

    // ========================================================================
    // Marker structs — ports of Java annotations (not used at runtime in C++)
    // But preserved for documentation and potential compile-time reflection
    // ========================================================================

    /**
     * Port of: @Retention(RetentionPolicy.RUNTIME) @Target(ElementType.FIELD)
     *          @interface WritableObjectVersion {}
     */
    struct WritableObjectVersion {};

    /**
     * Port of: @Retention(RetentionPolicy.RUNTIME) @Target(ElementType.FIELD)
     *          @interface WritableObjectKey {}
     */
    struct WritableObjectKey {};

    /**
     * Port of: @Retention(RetentionPolicy.RUNTIME) @Target(ElementType.FIELD)
     *          @interface WritableObjectTimestamp {}
     */
    struct WritableObjectTimestamp {};

    /**
     * Port of: @Retention(RetentionPolicy.RUNTIME) @Target(ElementType.FIELD)
     *          @interface WritableField {}
     */
    struct WritableField {};

    // ========================================================================
    // Interface methods
    // ========================================================================

    virtual ~WritableObject() = default;

    /**
     * Port of: K getKey();
     */
    virtual K getKey() const = 0;

    /**
     * Port of: TimestampUTC getTimestamp();
     */
    virtual TimestampUTC getTimestamp() const = 0;
};

} // namespace PinkReader
