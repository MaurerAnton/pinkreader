/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: rr_animation_shrink_height.cpp - Port of RRAnimationShrinkHeight.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RRAnimationShrinkHeight.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "views/rr_animation_shrink_height.h"

#include <cmath>

namespace PinkReader {

// ============================================================================
// View stub (minimal for RRAnimationShrinkHeight needs)
// ============================================================================

class View {
public:
    virtual ~View() = default;

    // Port of: getLayoutParams()
    virtual ViewGroupLayoutParams* getLayoutParams() const { return nullptr; }

    // Port of: setLayoutParams(LayoutParams)
    virtual void setLayoutParams(ViewGroupLayoutParams* params) {}

    // Port of: getMeasuredHeight()
    virtual int getMeasuredHeight() const { return 0; }

    // Port of: setVisibility(int)
    virtual void setVisibility(int visibility) {}

    // Visibility constants (port of: View.GONE)
    static constexpr int GONE = 8;  // View.GONE = 0x00000008
};

// ============================================================================
// ViewGroupLayoutParams stub
// ============================================================================

class ViewGroupLayoutParams {
public:
    int width = 0;
    int height = 0;
};

// ============================================================================
// Constructor (Java lines 31-35)
//
// Port of: public RRAnimationShrinkHeight(final View target)
// ============================================================================

RRAnimationShrinkHeight::RRAnimationShrinkHeight(View* target)
    : m_target(target)
    , m_layoutParams(target ? target->getLayoutParams() : nullptr)
    , m_startHeight(target ? target->getMeasuredHeight() : 0)
{
}

// ============================================================================
// handleFrame — port of method (Java lines 38-52)
//
// Port of: @Override protected boolean handleFrame(final long nanosSinceAnimationStart)
// ============================================================================

bool RRAnimationShrinkHeight::handleFrame(int64_t nanosSinceAnimationStart) {
    if (m_layoutParams == nullptr || m_target == nullptr) {
        return false;
    }

    // Port of: mLayoutParams.height = (int)(mStartHeight * interpolateSine(
    //     1.0 - (double)nanosSinceAnimationStart / (double)DURATION_NANOS));
    const double fraction = 1.0 - static_cast<double>(nanosSinceAnimationStart) /
            static_cast<double>(DURATION_NANOS);
    m_layoutParams->height = static_cast<int>(
            m_startHeight * interpolateSine(fraction));

    // Port of: mTarget.setLayoutParams(mLayoutParams);
    m_target->setLayoutParams(m_layoutParams);

    // Port of: final boolean finished = nanosSinceAnimationStart > DURATION_NANOS;
    const bool finished = nanosSinceAnimationStart > DURATION_NANOS;

    // Port of: if(finished) { mTarget.setVisibility(View.GONE); }
    if (finished) {
        m_target->setVisibility(View::GONE);
    }

    // Port of: return !finished;
    return !finished;
}

} // namespace PinkReader
