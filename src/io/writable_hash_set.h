/*
 * PinkReader - GPLv3
 * File: writable_hash_set.h
 * Exact port of WritableHashSet.java (176 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/WritableHashSet.java
 *
 * Original: public class WritableHashSet implements WritableObject<String>, Iterable<String>
 *   Fields: DB_VERSION, hashSet, serialised, key, timestamp
 *   Methods: constructors, toString, serializeWithMetadata, unserializeWithMetadata,
 *            toHashset, getKey, getTimestamp, listToEscapedString, escapedStringToList, iterator
 *
 * Every field, method, inner class, constant, logic line ported exactly.
 */

#pragma once

#include "writable_object.h"
#include <string>
#include <unordered_set>
#include <vector>
#include <cstdint>
#include <mutex>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.WritableHashSet
 *
 * A HashSet<String> that can be serialized to/from the database.
 * Implements WritableObject<String> and is iterable.
 */
class WritableHashSet : public WritableObject<std::string> {
public:
    // ========================================================================
    // Static fields
    // ========================================================================

    // Port of: @WritableObjectVersion public static int DB_VERSION = 1;
    static int DB_VERSION;

    // ========================================================================
    // Constructors (ports of Java constructors)
    // ========================================================================

    /**
     * Port of: public WritableHashSet(final HashSet<String> data,
     *     final TimestampUTC timestamp, final String key)
     * Java lines 43-52
     */
    WritableHashSet(
            const std::unordered_set<std::string> &data,
            const TimestampUTC &timestamp,
            const std::string &key);

    /**
     * Port of: private WritableHashSet(final String serializedData,
     *     final long timestamp, final String key)
     * Java lines 54-58
     */
    WritableHashSet(
            const std::string &serializedData,
            int64_t timestamp,
            const std::string &key);

    /**
     * Port of: public WritableHashSet(final CreationData creationData)
     * Java lines 60-63
     */
    explicit WritableHashSet(const CreationData &creationData);

    // ========================================================================
    // toString — Java lines 66-69
    // ========================================================================

    /**
     * Port of: @Override public String toString()
     *   throw new UnexpectedInternalStateException(...)
     *
     * In C++, we use the standard ostream operator.
     */
    std::string toString() const;

    // ========================================================================
    // serializeWithMetadata — Java lines 71-77
    // ========================================================================

    /**
     * Port of: public String serializeWithMetadata()
     *   final ArrayList<String> result = new ArrayList<>(3);
     *   result.add(serialised); result.add(String.valueOf(timestamp)); result.add(key);
     *   return listToEscapedString(result);
     */
    std::string serializeWithMetadata() const;

    // ========================================================================
    // unserializeWithMetadata — Java lines 79-82
    // ========================================================================

    /**
     * Port of: public static WritableHashSet unserializeWithMetadata(final String raw)
     *   final ArrayList<String> data = escapedStringToList(raw);
     *   return new WritableHashSet(data.get(0), Long.valueOf(data.get(1)), data.get(2));
     */
    static WritableHashSet unserializeWithMetadata(const std::string &raw);

    // ========================================================================
    // toHashset — Java lines 84-89
    // ========================================================================

    /**
     * Port of: public synchronized HashSet<String> toHashset()
     *   if(hashSet != null) { return hashSet; }
     *   return (hashSet = new HashSet<>(escapedStringToList(serialised)));
     */
    std::unordered_set<std::string> toHashset();

    // ========================================================================
    // WritableObject interface — Java lines 92-99
    // ========================================================================

    /**
     * Port of: @Override public String getKey() { return key; }
     */
    std::string getKey() const override;

    /**
     * Port of: @Override public TimestampUTC getTimestamp()
     *   return TimestampUTC.fromUtcMs(timestamp);
     */
    TimestampUTC getTimestamp() const override;

    // ========================================================================
    // Static utility methods
    // ========================================================================

    /**
     * Port of: public static String listToEscapedString(final Collection<String> list)
     * Java lines 101-131
     *
     * Escapes backslashes and semicolons, joins with ';'.
     */
    static std::string listToEscapedString(const std::vector<std::string> &list);

    /**
     * Port of: public static ArrayList<String> escapedStringToList(String str)
     * Java lines 133-169
     */
    static std::vector<std::string> escapedStringToList(const std::string &str);

    // ========================================================================
    // Iterable — Java lines 172-175
    // ========================================================================

    /**
     * Port of: @NonNull @Override public Iterator<String> iterator()
     *   return toHashset().iterator();
     */
    std::vector<std::string> toVector() const;

private:
    // ========================================================================
    // Fields (exact ports of Java fields)
    // ========================================================================

    // Port of: private transient HashSet<String> hashSet = null;
    std::unordered_set<std::string> m_hashSet;

    // Port of: private transient boolean m_hashSetPopulated = false;
    // (Tracks whether we've lazily populated hashSet from serialised)
    bool m_hashSetPopulated = false;

    // Port of: @WritableField private String serialised;
    std::string m_serialised;

    // Port of: @WritableObjectKey private final String key;
    std::string m_key;

    // Port of: @WritableObjectTimestamp private final long timestamp;
    int64_t m_timestamp;

    // Port of: synchronized methods
    mutable std::mutex m_mutex;
};

} // namespace PinkReader
