/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: swipe_history.h - Port of SwipeHistory.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/SwipeHistory.java
 *
 * Original: public final class SwipeHistory
 *
 * Tracks recent swipe positions and timestamps in a circular buffer.
 * Every field, method, constant ported exactly.
 */

#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>

namespace PinkReader {

// ============================================================================
// SwipeHistory — port of Java class (Java lines 20-84)
//
// Port of: org.quantumbadger.redreader.views.SwipeHistory
//
// Circular buffer tracking swipe positions + timestamps for velocity calc.
// Every field, method, constant ported exactly.
// ============================================================================

class SwipeHistory {
public:
    // ========================================================================
    // Constructor (Java lines 27-30)
    //
    // Port of: public SwipeHistory(final int len)
    // ========================================================================

    explicit SwipeHistory(int len)
        : m_positions(len)
        , m_timestamps(len)
    {
    }

    // ========================================================================
    // add — port of method (Java lines 32-44)
    //
    // Port of: public void add(final float position, final long timestamp)
    // ========================================================================

    void add(float position, int64_t timestamp) {
        // Port of: if(len >= positions.length) { ... } else { ... }
        if (m_len >= static_cast<int>(m_positions.size())) {
            // Port of: positions[start] = position;
            m_positions[m_start] = position;

            // Port of: timestamps[start] = timestamp;
            m_timestamps[m_start] = timestamp;

            // Port of: start = (start + 1) % positions.length;
            m_start = (m_start + 1) % static_cast<int>(m_positions.size());
        } else {
            // Port of: positions[(start + len) % positions.length] = position;
            m_positions[(m_start + m_len) % static_cast<int>(m_positions.size())] = position;

            // Port of: timestamps[(start + len) % timestamps.length] = timestamp;
            m_timestamps[(m_start + m_len) % static_cast<int>(m_timestamps.size())] = timestamp;

            // Port of: len++;
            m_len++;
        }
    }

    // ========================================================================
    // getMostRecent — port of method (Java lines 46-48)
    //
    // Port of: public float getMostRecent()
    // ========================================================================

    float getMostRecent() const {
        // Port of: return positions[getNthMostRecentIndex(0)];
        return m_positions[getNthMostRecentIndex(0)];
    }

    // ========================================================================
    // getAtTimeAgoMs — port of method (Java lines 50-67)
    //
    // Port of: public float getAtTimeAgoMs(final long timeAgo)
    // ========================================================================

    float getAtTimeAgoMs(int64_t timeAgo) const {
        // Port of: final long timestamp = timestamps[getNthMostRecentIndex(0)] - timeAgo;
        const int64_t timestamp = m_timestamps[getNthMostRecentIndex(0)] - timeAgo;

        // Port of: float result = getMostRecent();
        float result = getMostRecent();

        // Port of: for(int i = 0; i < len; i++) { ... }
        for (int i = 0; i < m_len; i++) {
            // Port of: final int index = getNthMostRecentIndex(i);
            const int index = getNthMostRecentIndex(i);

            // Port of: if(timestamp > timestamps[index]) { return result; }
            if (timestamp > m_timestamps[index]) {
                return result;
            } else {
                // Port of: else { result = positions[index]; }
                result = m_positions[index];
            }
        }

        // Port of: return result;
        return result;
    }

    // ========================================================================
    // clear — port of method (Java lines 76-79)
    //
    // Port of: public void clear()
    // ========================================================================

    void clear() {
        // Port of: len = 0; start = 0;
        m_len = 0;
        m_start = 0;
    }

    // ========================================================================
    // size — port of method (Java lines 81-83)
    //
    // Port of: public int size()
    // ========================================================================

    int size() const {
        // Port of: return len;
        return m_len;
    }

private:
    // ========================================================================
    // getNthMostRecentIndex — port of method (Java lines 69-74)
    //
    // Port of: private int getNthMostRecentIndex(final int n)
    // ========================================================================

    int getNthMostRecentIndex(int n) const {
        // Port of: if(n >= len || n < 0) { throw new ArrayIndexOutOfBoundsException(n); }
        if (n >= m_len || n < 0) {
            throw std::out_of_range("SwipeHistory index out of range");
        }

        // Port of: return (start + len - n - 1) % positions.length;
        return (m_start + m_len - n - 1) % static_cast<int>(m_positions.size());
    }

    // ========================================================================
    // Private fields — exact match (Java lines 22-25)
    // ========================================================================

    // Port of: private final float[] positions; (Java line 22)
    std::vector<float> m_positions;

    // Port of: private final long[] timestamps; (Java line 23)
    std::vector<int64_t> m_timestamps;

    // Port of: private int start = 0; (Java line 24)
    int m_start = 0;

    // Port of: private int len = 0; (Java line 25)
    int m_len = 0;
};

} // namespace PinkReader
