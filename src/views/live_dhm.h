/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: live_dhm.h - Port of LiveDHM.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LiveDHM.java
 *
 * Original: class LiveDHM
 * Simulates damped harmonic motion.
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <cmath>
#include <cstdlib>

namespace PinkReader {

// ============================================================================
// LiveDHM — port of Java class (Java lines 20-93)
//
// Port of: org.quantumbadger.redreader.views.LiveDHM
//
// Damped harmonic motion simulator.
// Every field, method, constant, inner class ported exactly.
// ============================================================================

class LiveDHM {
public:
    // ========================================================================
    // Params — inner static class (Java lines 22-37)
    //
    // Port of: public static class Params
    // ========================================================================

    struct Params {
        // Port of: public float startPosition = 0; (Java line 24)
        float startPosition = 0.0f;

        // Port of: public float endPosition = 0; (Java line 25)
        float endPosition = 0.0f;

        // Port of: public float startVelocity = 0; (Java line 27)
        float startVelocity = 0.0f;

        // Port of: public static final float accelerationCoefficient = 30; (Java line 29)
        static constexpr float accelerationCoefficient = 30.0f;

        // Port of: public static final float velocityDamping = 0.87f; (Java line 30)
        static constexpr float velocityDamping = 0.87f;

        // Port of: public static final float stepLengthSeconds = 1f / 60f; (Java line 32)
        static constexpr float stepLengthSeconds = 1.0f / 60.0f;

        // Port of: public static final float thresholdPositionDifference = 0.49f; (Java line 34)
        static constexpr float thresholdPositionDifference = 0.49f;

        // Port of: public static final float thresholdVelocity = 15; (Java line 35)
        static constexpr float thresholdVelocity = 15.0f;

        // Port of: public static final int thresholdMaxSteps = 1000; (Java line 36)
        static constexpr int thresholdMaxSteps = 1000;
    };

    // ========================================================================
    // Constructor (Java lines 46-50)
    //
    // Port of: public LiveDHM(final Params params)
    // ========================================================================

    explicit LiveDHM(const Params& params)
        : mParams(params)
        , mPosition(params.startPosition)
        , mVelocity(params.startVelocity)
    {
    }

    // ========================================================================
    // calculateStep — port of method (Java lines 52-58)
    //
    // Port of: public void calculateStep()
    // ========================================================================

    void calculateStep() {
        // Port of: mVelocity -= mParams.stepLengthSeconds * ((mPosition - mParams.endPosition)
        //     * mParams.accelerationCoefficient);
        mVelocity -= mParams.stepLengthSeconds *
                ((mPosition - mParams.endPosition) * mParams.accelerationCoefficient);

        // Port of: mVelocity *= mParams.velocityDamping;
        mVelocity *= mParams.velocityDamping;

        // Port of: mPosition += mVelocity * mParams.stepLengthSeconds;
        mPosition += mVelocity * mParams.stepLengthSeconds;

        // Port of: mStep++;
        mStep++;
    }

    // ========================================================================
    // getCurrentStep — port of method (Java lines 60-62)
    //
    // Port of: public int getCurrentStep()
    // ========================================================================

    int getCurrentStep() const {
        return mStep;
    }

    // ========================================================================
    // getCurrentPosition — port of method (Java lines 64-66)
    //
    // Port of: public float getCurrentPosition()
    // ========================================================================

    float getCurrentPosition() const {
        return mPosition;
    }

    // ========================================================================
    // getCurrentVelocity — port of method (Java lines 68-70)
    //
    // Port of: public float getCurrentVelocity()
    // ========================================================================

    float getCurrentVelocity() const {
        return mVelocity;
    }

    // ========================================================================
    // getParams — port of method (Java lines 72-74)
    //
    // Port of: public Params getParams()
    // ========================================================================

    const Params& getParams() const {
        return mParams;
    }

    // ========================================================================
    // isEndThresholdReached — port of method (Java lines 76-92)
    //
    // Port of: public boolean isEndThresholdReached()
    // ========================================================================

    bool isEndThresholdReached() const {
        // Port of: if(mStep >= mParams.thresholdMaxSteps) { return true; }
        if (mStep >= mParams.thresholdMaxSteps) {
            return true;
        }

        // Port of: if(Math.abs(mPosition) > mParams.thresholdPositionDifference) { return false; }
        if (std::abs(mPosition) > mParams.thresholdPositionDifference) {
            return false;
        }

        // Port of: if(Math.abs(mVelocity) > mParams.thresholdVelocity) { return false; }
        if (std::abs(mVelocity) > mParams.thresholdVelocity) {
            return false;
        }

        // Port of: return true;
        return true;
    }

private:
    // ========================================================================
    // Private fields — exact match (Java lines 39-44)
    // ========================================================================

    // Port of: private final Params mParams; (Java line 39)
    Params mParams;

    // Port of: private int mStep = 0; (Java line 41)
    int mStep = 0;

    // Port of: private float mPosition; (Java line 43)
    float mPosition = 0.0f;

    // Port of: private float mVelocity; (Java line 44)
    float mVelocity = 0.0f;
};

} // namespace PinkReader
