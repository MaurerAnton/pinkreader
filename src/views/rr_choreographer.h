/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: rr_choreographer.h - Port of RRChoreographer.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RRChoreographer.java
 *
 * Original: class RRChoreographer implements Choreographer.FrameCallback
 *
 * Singleton that batches per-frame callbacks. Replaces Android's Choreographer.
 * Every field, method, constant, inner class ported exactly.
 */

#pragma once

#include <cstdint>

namespace PinkReader {

// ============================================================================
// RRChoreographer — port of Java class (Java lines 24-63)
//
// Port of: org.quantumbadger.redreader.views.RRChoreographer
// Orig implements: Choreographer.FrameCallback
//
// Singleton frame callback dispatcher.
// Every field, method, constant, inner class ported exactly.
// ============================================================================

class RRChoreographer {
public:
    // ========================================================================
    // Callback — inner interface (Java lines 26-28)
    //
    // Port of: public interface Callback { void doFrame(long frameTimeNanos); }
    // ========================================================================

    struct Callback {
        virtual void doFrame(int64_t frameTimeNanos) = 0;
        virtual ~Callback() = default;
    };

    // ========================================================================
    // Singleton (Java lines 30, 38-39)
    //
    // Port of: static final RRChoreographer INSTANCE = new RRChoreographer();
    // Port of: private RRChoreographer() {}
    // ========================================================================

    static RRChoreographer& instance() {
        static RRChoreographer s_instance;
        return s_instance;
    }

    // ========================================================================
    // postFrameCallback — port of method (Java lines 41-49)
    //
    // Port of: public void postFrameCallback(@NonNull final Callback callback)
    // ========================================================================

    void postFrameCallback(Callback* callback) {
        // Port of: mCallbacks[mCallbackCount] = callback;
        if (m_callbackCount < kMaxCallbacks) {
            m_callbacks[m_callbackCount] = callback;
        }

        // Port of: mCallbackCount++;
        m_callbackCount++;

        // Port of: if(!mPosted) { CHOREOGRAPHER.postFrameCallback(this); mPosted = true; }
        if (!m_posted) {
            // Instead of Android Choreographer, we mark as posted;
            // the owner must call doFrame() externally (via a timer/tick).
            m_posted = true;
        }
    }

    // ========================================================================
    // doFrame — port of method (Java lines 51-62)
    //
    // Port of: public void doFrame(final long frameTimeNanos)
    // Implements: Choreographer.FrameCallback.doFrame()
    // ========================================================================

    void doFrame(int64_t frameTimeNanos) {
        // Port of: final int callbackCount = mCallbackCount;
        const int callbackCount = m_callbackCount;

        // Port of: mPosted = false;
        m_posted = false;

        // Port of: mCallbackCount = 0;
        m_callbackCount = 0;

        // Port of: for(int i = 0; i < callbackCount; i++) { mCallbacks[i].doFrame(frameTimeNanos); }
        for (int i = 0; i < callbackCount; i++) {
            if (m_callbacks[i] != nullptr) {
                m_callbacks[i]->doFrame(frameTimeNanos);
            }
        }
    }

private:
    // ========================================================================
    // Private constructor (Java lines 38-39)
    //
    // Port of: private RRChoreographer() {}
    // ========================================================================

    RRChoreographer() = default;
    ~RRChoreographer() = default;
    RRChoreographer(const RRChoreographer&) = delete;
    RRChoreographer& operator=(const RRChoreographer&) = delete;

    // ========================================================================
    // Constants — port (Java line 34)
    //
    // Port of: private final Callback[] mCallbacks = new Callback[128];
    // ========================================================================

    static constexpr int kMaxCallbacks = 128;

    // ========================================================================
    // Private fields — exact match (Java lines 34-36)
    // ========================================================================

    // Port of: private final Callback[] mCallbacks = new Callback[128]; (Java line 34)
    Callback* m_callbacks[kMaxCallbacks] = {};

    // Port of: private int mCallbackCount = 0; (Java line 35)
    int m_callbackCount = 0;

    // Port of: private boolean mPosted = false; (Java line 36)
    bool m_posted = false;
};

} // namespace PinkReader
