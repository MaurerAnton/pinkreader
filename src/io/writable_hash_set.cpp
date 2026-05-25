/*
 * PinkReader - GPLv3
 * File: writable_hash_set.cpp
 * Exact port of WritableHashSet.java method bodies (176 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/WritableHashSet.java
 *
 * Every method body ported exactly.
 */

#include "writable_hash_set.h"
#include "../common/unexpected_internal_state_exception.h"
#include <sstream>
#include <algorithm>
#include <QString>

namespace PinkReader {

// Port of: public static int DB_VERSION = 1;
int WritableHashSet::DB_VERSION = 1;

// ============================================================================
// Constructor — Java lines 43-52
// ============================================================================
WritableHashSet::WritableHashSet(
        const std::unordered_set<std::string> &data,
        const TimestampUTC &timestamp,
        const std::string &key)
    : m_hashSet(data)
    , m_hashSetPopulated(true)
    , m_key(key)
    , m_timestamp(timestamp.toUtcMs())
{
    // Port of: serialised = listToEscapedString(hashSet);
    std::vector<std::string> vec(data.begin(), data.end());
    m_serialised = listToEscapedString(vec);
}

// ============================================================================
// Private constructor — Java lines 54-58
// ============================================================================
WritableHashSet::WritableHashSet(
        const std::string &serializedData,
        int64_t timestamp,
        const std::string &key)
    : m_hashSetPopulated(false)
    , m_serialised(serializedData)
    , m_key(key)
    , m_timestamp(timestamp)
{}

// ============================================================================
// CreationData constructor — Java lines 60-63
// ============================================================================
WritableHashSet::WritableHashSet(const CreationData &creationData)
    : m_hashSetPopulated(false)
    , m_timestamp(creationData.timestamp)
    , m_key(creationData.key)
{}

// ============================================================================
// toString — Java lines 66-69
// Port of: throw new UnexpectedInternalStateException(
//     "Using toString() is the wrong way to serialise a WritableHashSet");
// ============================================================================
std::string WritableHashSet::toString() const {
    // Mirror the Java behavior — throw an exception
    throw UnexpectedInternalStateException(
        QString::fromStdString(
            "Using toString() is the wrong way to serialise a WritableHashSet"));
}

// ============================================================================
// serializeWithMetadata — Java lines 71-77
// ============================================================================
std::string WritableHashSet::serializeWithMetadata() const {
    // Port of: final ArrayList<String> result = new ArrayList<>(3);
    std::vector<std::string> result;
    result.reserve(3);

    // Port of: result.add(serialised);
    result.push_back(m_serialised);

    // Port of: result.add(String.valueOf(timestamp));
    result.push_back(std::to_string(m_timestamp));

    // Port of: result.add(key);
    result.push_back(m_key);

    // Port of: return listToEscapedString(result);
    return listToEscapedString(result);
}

// ============================================================================
// unserializeWithMetadata — Java lines 79-82
// ============================================================================
WritableHashSet WritableHashSet::unserializeWithMetadata(const std::string &raw) {
    // Port of: final ArrayList<String> data = escapedStringToList(raw);
    std::vector<std::string> data = escapedStringToList(raw);

    // Port of: return new WritableHashSet(data.get(0), Long.valueOf(data.get(1)), data.get(2));
    return WritableHashSet(data[0], std::stoll(data[1]), data[2]);
}

// ============================================================================
// toHashset — Java lines 84-89
// ============================================================================
std::unordered_set<std::string> WritableHashSet::toHashset() {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Port of: if(hashSet != null) { return hashSet; }
    if (m_hashSetPopulated) {
        return m_hashSet;
    }

    // Port of: return (hashSet = new HashSet<>(escapedStringToList(serialised)));
    std::vector<std::string> list = escapedStringToList(m_serialised);
    m_hashSet = std::unordered_set<std::string>(list.begin(), list.end());
    m_hashSetPopulated = true;
    return m_hashSet;
}

// ============================================================================
// getKey — Java line 92-93
// ============================================================================
std::string WritableHashSet::getKey() const {
    // Port of: return key;
    return m_key;
}

// ============================================================================
// getTimestamp — Java lines 97-99
// ============================================================================
TimestampUTC WritableHashSet::getTimestamp() const {
    // Port of: return TimestampUTC.fromUtcMs(timestamp);
    return TimestampUTC::fromUtcMs(m_timestamp);
}

// ============================================================================
// toVector — C++ adaptation of iterator()
// ============================================================================
std::vector<std::string> WritableHashSet::toVector() const {
    // Port of: toHashset().iterator() — return a snapshot
    auto copy = const_cast<WritableHashSet *>(this)->toHashset();
    return std::vector<std::string>(copy.begin(), copy.end());
}

// ============================================================================
// listToEscapedString — Java lines 101-131
// ============================================================================
std::string WritableHashSet::listToEscapedString(const std::vector<std::string> &list) {
    // Port of: if(list.isEmpty()) { return ""; }
    if (list.empty()) {
        return "";
    }

    // Port of: final StringBuilder sb = new StringBuilder();
    std::ostringstream sb;

    // Port of: for(final String str : list)
    for (const std::string &str : list) {
        // Port of: for(int i = 0; i < str.length(); i++)
        for (size_t i = 0; i < str.length(); i++) {
            // Port of: final char c = str.charAt(i);
            char c = str[i];

            // Port of: switch(c)
            switch (c) {
                // Port of: case '\\': sb.append("\\\\"); break;
                case '\\':
                    sb << "\\\\";
                    break;
                // Port of: case ';': sb.append("\\;"); break;
                case ';':
                    sb << "\\;";
                    break;
                // Port of: default: sb.append(c); break;
                default:
                    sb << c;
                    break;
            }
        }

        // Port of: sb.append(';');
        sb << ';';
    }

    // Port of: return sb.toString();
    return sb.str();
}

// ============================================================================
// escapedStringToList — Java lines 133-169
// ============================================================================
std::vector<std::string> WritableHashSet::escapedStringToList(const std::string &str) {
    // Port of: final ArrayList<String> result = new ArrayList<>();
    std::vector<std::string> result;

    // Port of: if(str != null)
    if (!str.empty()) {

        std::string workStr = str;

        // Port of: // Workaround to improve parsing of lists saved by older versions of the app
        //         if(!str.isEmpty() && !str.endsWith(";")) { str += ";"; }
        if (!workStr.empty() && workStr.back() != ';') {
            workStr += ';';
        }

        // Port of: boolean isEscaped = false;
        bool isEscaped = false;

        // Port of: final StringBuilder sb = new StringBuilder();
        std::ostringstream sb;

        // Port of: for(int i = 0; i < str.length(); i++)
        for (size_t i = 0; i < workStr.length(); i++) {
            // Port of: final char c = str.charAt(i);
            char c = workStr[i];

            // Port of: if(c == ';' && !isEscaped)
            if (c == ';' && !isEscaped) {
                // Port of: result.add(sb.toString()); sb.setLength(0);
                result.push_back(sb.str());
                sb.str("");
                sb.clear();

            // Port of: } else if(c == '\\')
            } else if (c == '\\') {
                // Port of: if(isEscaped) { sb.append('\\'); }
                if (isEscaped) {
                    sb << '\\';
                }
                // (isEscaped toggle handled below)

            // Port of: } else { sb.append(c); }
            } else {
                sb << c;
            }

            // Port of: isEscaped = c == '\\' && !isEscaped;
            isEscaped = (c == '\\' && !isEscaped);
        }
    }

    // Port of: return result;
    return result;
}

} // namespace PinkReader
