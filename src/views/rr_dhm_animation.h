/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: rr_dhm_animation.h - Port of RRDHMAnimation.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RRDHMAnimation.java
 *
 * Original: public abstract class RRDHMAnimation extends RRAnimation
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include "views/rr_animation.h"
#include "views/live_dhm.h"
#include <cstdint>

namespace PinkReader {

// ============================================================================
// RRDHMAnimation — port of Java class (Java lines 20-61)
//
// Port of: org.quantumbadger.redreader.views.RRDHMAnimation
// Orig extends: RRAnimation
//
// Abstract animation driven by damped harmonic motion (LiveDHM).
// Every field, method, constant ported exactly.
// ============================================================================

class RRDHMAnimation : public RRAnimation {
public:
    // ========================================================================
    // Constructor (Java lines 24-26)
    //
    // Port of: public RRDHMAnimation(final LiveDHM.Params params)
    // ========================================================================

    explicit RRDHMAnimation(const LiveDHM::Params& params)
        : m_dhm(params)
    {
    }

    // ========================================================================
    // handleFrame — port of method (Java lines 29-52)
    //
    // Port of: @Override protected boolean handleFrame(final long nanosSinceAnimationStart)
    // ========================================================================

    bool handleFrame(int64_t nanosSinceAnimationStart) override {
        // Port of: final long microsSinceAnimationStart = nanosSinceAnimationStart / 1000;
        const int64_t microsSinceAnimationStart = nanosSinceAnimationStart / 1000;

        // Port of: final long stepLengthMicros = (long)(mDHM.getParams().stepLengthSeconds
        //     * 1000.0 * 1000.0);
        const int64_t stepLengthMicros = static_cast<int64_t>(
                m_dhm.getParams().stepLengthSeconds * 1000.0 * 1000.0);

        // Port of: final int desiredStepNumber = (int)((microsSinceAnimationStart
        //     + (stepLengthMicros / 2)) / stepLengthMicros);
        const int desiredStepNumber = static_cast<int>(
                (microsSinceAnimationStart + (stepLengthMicros / 2)) / stepLengthMicros);

        // Port of: while(mDHM.getCurrentStep() < desiredStepNumber) { ... }
        while (m_dhm.getCurrentStep() < desiredStepNumber) {
            // Port of: mDHM.calculateStep();
            m_dhm.calculateStep();

            // Port of: if(mDHM.isEndThresholdReached()) { onEndPosition(...); return false; }
            if (m_dhm.isEndThresholdReached()) {
                onEndPosition(m_dhm.getParams().endPosition);
                return false;
            }
        }

        // Port of: onUpdatedPosition(mDHM.getCurrentPosition());
        onUpdatedPosition(m_dhm.getCurrentPosition());

        // Port of: return true;
        return true;
    }

    // ========================================================================
    // getCurrentVelocity — port of method (Java lines 54-56)
    //
    // Port of: public final float getCurrentVelocity()
    // ========================================================================

    float getCurrentVelocity() const {
        // Port of: return mDHM.getCurrentVelocity();
        return m_dhm.getCurrentVelocity();
    }

protected:
    // ========================================================================
    // Abstract methods (Java lines 58-60)
    //
    // Port of: protected abstract void onUpdatedPosition(float position);
    // Port of: protected abstract void onEndPosition(float endPosition);
    // ========================================================================

    virtual void onUpdatedPosition(float position) = 0;
    virtual void onEndPosition(float endPosition) = 0;

private:
    // ========================================================================
    // Private fields — exact match (Java line 22)
    //
    // Port of: private final LiveDHM mDHM; (Java line 22)
    // ========================================================================

    mutable LiveDHM m_dhm;
};

} // namespace PinkReader
