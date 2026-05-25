/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: rr_animation.h - Port of RRAnimation.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RRAnimation.java
 *
 * Original: public abstract class RRAnimation implements RRChoreographer.Callback
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include "views/rr_choreographer.h"
#include <cstdint>
#include <stdexcept>

namespace PinkReader {

// ============================================================================
// RRAnimation — port of Java class (Java lines 20-69)
//
// Port of: org.quantumbadger.redreader.views.RRAnimation
// Orig implements: RRChoreographer.Callback
//
// Abstract base for frame-based animations. Extends RRChoreographer::Callback.
// Every field, method, constant ported exactly.
// ============================================================================

class RRAnimation : public RRChoreographer::Callback {
public:
    virtual ~RRAnimation() = default;

    // ========================================================================
    // start — port of method (Java lines 27-36)
    //
    // Port of: public final void start()
    // ========================================================================

    void start() {
        // Port of: if(mStarted) { throw new RuntimeException("Attempted to start animation twice!"); }
        if (m_started) {
            throw std::runtime_error("Attempted to start animation twice!");
        }

        // Port of: mStarted = true;
        m_started = true;

        // Port of: RRChoreographer.INSTANCE.postFrameCallback(this);
        RRChoreographer::instance().postFrameCallback(this);
    }

    // ========================================================================
    // stop — port of method (Java lines 38-49)
    //
    // Port of: public final void stop()
    // ========================================================================

    void stop() {
        // Port of: if(!mStarted) { throw new RuntimeException("Attempted to stop animation before it's started!"); }
        if (!m_started) {
            throw std::runtime_error("Attempted to stop animation before it's started!");
        }

        // Port of: if(mStopped) { throw new RuntimeException("Attempted to stop animation twice!"); }
        if (m_stopped) {
            throw std::runtime_error("Attempted to stop animation twice!");
        }

        // Port of: mStopped = true;
        m_stopped = true;
    }

    // ========================================================================
    // doFrame — port of method (Java lines 54-68)
    //
    // Port of: @Override public final void doFrame(final long frameTimeNanos)
    // Implements: RRChoreographer.Callback.doFrame()
    // ========================================================================

    void doFrame(int64_t frameTimeNanos) override final {
        // Port of: if(mStopped) { return; }
        if (m_stopped) {
            return;
        }

        // Port of: if(mFirstFrameNanos == -1) { mFirstFrameNanos = frameTimeNanos; }
        if (m_firstFrameNanos == -1) {
            m_firstFrameNanos = frameTimeNanos;
        }

        // Port of: if(handleFrame(frameTimeNanos - mFirstFrameNanos)) {
        //     RRChoreographer.INSTANCE.postFrameCallback(this); }
        if (handleFrame(frameTimeNanos - m_firstFrameNanos)) {
            RRChoreographer::instance().postFrameCallback(this);
        }
    }

protected:
    // ========================================================================
    // handleFrame — abstract (Java lines 51-52)
    //
    // Port of: // Return true to continue animating
    // Port of: protected abstract boolean handleFrame(final long nanosSinceAnimationStart);
    // ========================================================================

    virtual bool handleFrame(int64_t nanosSinceAnimationStart) = 0;

private:
    // ========================================================================
    // Private fields — exact match (Java lines 22-24)
    // ========================================================================

    // Port of: private long mFirstFrameNanos = -1; (Java line 22)
    int64_t m_firstFrameNanos = -1;

    // Port of: private boolean mStarted = false; (Java line 24)
    bool m_started = false;

    // Port of: private boolean mStopped = false; (Java line 24)
    bool m_stopped = false;
};

} // namespace PinkReader
