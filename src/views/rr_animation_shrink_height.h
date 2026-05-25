/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: rr_animation_shrink_height.h - Port of RRAnimationShrinkHeight.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RRAnimationShrinkHeight.java
 *
 * Original: class RRAnimationShrinkHeight extends RRAnimation
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include "views/rr_animation.h"
#include <cmath>
#include <cstdint>

namespace PinkReader {

// Forward declaration for View (stub: android.view.View)
class View;
class ViewGroupLayoutParams;

// ============================================================================
// RRAnimationShrinkHeight — port of Java class (Java lines 23-57)
//
// Port of: org.quantumbadger.redreader.views.RRAnimationShrinkHeight
// Orig extends: RRAnimation
//
// Animates a View's height down to zero using a sine interpolation.
// Every field, method, constant ported exactly.
// ============================================================================

class RRAnimationShrinkHeight : public RRAnimation {
public:
    // ========================================================================
    // Constants — exact match (Java line 25)
    //
    // Port of: private static final long DURATION_NANOS = 500L * 1000 * 1000;
    // ========================================================================

    static constexpr int64_t DURATION_NANOS = 500LL * 1000LL * 1000LL;

    // ========================================================================
    // Constructor (Java lines 31-35)
    //
    // Port of: public RRAnimationShrinkHeight(final View target)
    // ========================================================================

    // We accept a View* and a function to get layout params / set visibility.
    // In original: mTarget = target; mLayoutParams = target.getLayoutParams();
    //              mStartHeight = target.getMeasuredHeight();
    RRAnimationShrinkHeight(View* target);

    // ========================================================================
    // handleFrame — port of method (Java lines 38-52)
    //
    // Port of: @Override protected boolean handleFrame(final long nanosSinceAnimationStart)
    // ========================================================================

    bool handleFrame(int64_t nanosSinceAnimationStart) override;

private:
    // ========================================================================
    // interpolateSine — port of static method (Java lines 54-56)
    //
    // Port of: private static double interpolateSine(final double fraction)
    // ========================================================================

    static double interpolateSine(double fraction) {
        // Port of: return 0.5 + Math.sin((fraction - 0.5) * Math.PI) / 2;
        return 0.5 + std::sin((fraction - 0.5) * M_PI) / 2.0;
    }

    // ========================================================================
    // Private fields — exact match (Java lines 27-29)
    // ========================================================================

    // Port of: private final View mTarget; (Java line 27)
    View* m_target;

    // Port of: private final ViewGroup.LayoutParams mLayoutParams; (Java line 28)
    ViewGroupLayoutParams* m_layoutParams;

    // Port of: private final int mStartHeight; (Java line 29)
    int m_startHeight = 0;
};

} // namespace PinkReader
