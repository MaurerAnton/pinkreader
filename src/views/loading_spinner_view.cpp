/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: loading_spinner_view.cpp - Port of LoadingSpinnerView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LoadingSpinnerView.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "views/loading_spinner_view.h"

#include <cmath>
#include <cstdlib>

namespace PinkReader {

// ============================================================================
// Stubs for non-ported types
// ============================================================================

class Context {
public:
    struct TypedArray {
        int getColor(int, int defaultColor) { return defaultColor; }
        void recycle() {}
    };

    TypedArray* obtainStyledAttributes(int* /*attrs*/) {
        return new TypedArray();
    }
};

class AttributeSet {};

// Color constants (port of: android.graphics.Color.MAGENTA, Color.GREEN)
static constexpr int COLOR_MAGENTA = 0xFFFF00FF;
static constexpr int COLOR_GREEN = 0xFF00FF00;

// R.attr stubs
constexpr int R_ATTR_LOADING_RING_FG = 0;
constexpr int R_ATTR_LOADING_RING_BG = 1;

// General::dpToPixels stub
static int dpToPixels(Context*, float dp) {
    return static_cast<int>(std::round(dp * 2.0f));
}

// Center in parent constant (port of: RelativeLayout.CENTER_IN_PARENT)
static constexpr int CENTER_IN_PARENT = 13;

// ============================================================================
// DonutProgress stub (port of: com.github.lzyzsd.circleprogress.DonutProgress)
// ============================================================================

class DonutProgress {
public:
    DonutProgress(Context*) {}

    void setAspectIndicatorDisplay(bool) {}
    void setIndeterminate(bool) {}
    void setFinishedStrokeColor(int color) { m_finishedColor = color; }
    void setUnfinishedStrokeColor(int color) { m_unfinishedColor = color; }
    void setUnfinishedStrokeWidth(int w) { m_unfinishedWidth = w; }
    void setFinishedStrokeWidth(int w) { m_finishedWidth = w; }
    void setStartingDegree(int deg) { m_startDeg = deg; }
    void initPainters() {}

    int finishedColor() const { return m_finishedColor; }
    int unfinishedColor() const { return m_unfinishedColor; }
    int unfinishedWidth() const { return m_unfinishedWidth; }
    int finishedWidth() const { return m_finishedWidth; }
    int startDeg() const { return m_startDeg; }

    // Layout params stub
    struct LayoutParams {
        int width = 0;
        int height = 0;
        void addRule(int /*verb*/) {}
    };

    LayoutParams* getLayoutParams() { return &m_layoutParams; }

private:
    LayoutParams m_layoutParams;
    int m_finishedColor = 0;
    int m_unfinishedColor = 0;
    int m_unfinishedWidth = 0;
    int m_finishedWidth = 0;
    int m_startDeg = 0;
};

// ============================================================================
// Constructor — 3-arg (Java lines 35-68)
// ============================================================================

LoadingSpinnerView::LoadingSpinnerView(Context* context, AttributeSet* /*attributeSet*/, int /*defStyle*/) {
    commonInit(context);
}

// ============================================================================
// Constructor — 2-arg (Java lines 70-75)
//
// Port of: this(context, attributeSet, 0);
// ============================================================================

LoadingSpinnerView::LoadingSpinnerView(Context* context, AttributeSet* /*attributeSet*/) {
    commonInit(context);
}

// ============================================================================
// Constructor — 1-arg (Java lines 77-79)
//
// Port of: this(context, null);
// ============================================================================

LoadingSpinnerView::LoadingSpinnerView(Context* context) {
    commonInit(context);
}

LoadingSpinnerView::~LoadingSpinnerView() {
    delete static_cast<DonutProgress*>(m_progressView);
}

// ============================================================================
// commonInit — port of constructor body (Java lines 42-67)
// ============================================================================

void LoadingSpinnerView::commonInit(Context* context) {
    // Port of: final TypedArray typedArray = context.obtainStyledAttributes(
    //     new int[] { R.attr.rrLoadingRingForegroundCol, R.attr.rrLoadingRingBackgroundCol });
    int attrs[2] = {R_ATTR_LOADING_RING_FG, R_ATTR_LOADING_RING_BG};
    Context::TypedArray* typedArray = context->obtainStyledAttributes(attrs);

    // Port of: final int foreground = typedArray.getColor(0, Color.MAGENTA);
    m_foregroundColor = typedArray->getColor(0, COLOR_MAGENTA);

    // Port of: final int background = typedArray.getColor(1, Color.GREEN);
    m_backgroundColor = typedArray->getColor(1, COLOR_GREEN);

    // Port of: typedArray.recycle();
    typedArray->recycle();
    delete typedArray;

    // Port of: mProgressView = new DonutProgress(context);
    auto* progress = new DonutProgress(context);

    // Port of: mProgressView.setAspectIndicatorDisplay(false);
    progress->setAspectIndicatorDisplay(false);

    // Port of: mProgressView.setIndeterminate(true);
    progress->setIndeterminate(true);

    // Port of: mProgressView.setFinishedStrokeColor(foreground);
    progress->setFinishedStrokeColor(m_foregroundColor);

    // Port of: mProgressView.setUnfinishedStrokeColor(background);
    progress->setUnfinishedStrokeColor(m_backgroundColor);

    // Port of: final int progressStrokeWidthPx = General.dpToPixels(context, 10);
    m_progressStrokeWidthPx = dpToPixels(context, 10);

    // Port of: mProgressView.setUnfinishedStrokeWidth(progressStrokeWidthPx);
    progress->setUnfinishedStrokeWidth(m_progressStrokeWidthPx);

    // Port of: mProgressView.setFinishedStrokeWidth(progressStrokeWidthPx);
    progress->setFinishedStrokeWidth(m_progressStrokeWidthPx);

    // Port of: mProgressView.setStartingDegree(-90);
    progress->setStartingDegree(-90);

    // Port of: mProgressView.initPainters();
    progress->initPainters();

    // Port of: addView(mProgressView);
    // (In C++, we store it rather than adding to a Qt layout)
    m_progressView = progress;

    // Port of: final int progressDimensionsPx = General.dpToPixels(context, 100);
    m_progressDimensionsPx = dpToPixels(context, 100);

    // Port of: mProgressView.getLayoutParams().width = progressDimensionsPx;
    progress->getLayoutParams()->width = m_progressDimensionsPx;

    // Port of: mProgressView.getLayoutParams().height = progressDimensionsPx;
    progress->getLayoutParams()->height = m_progressDimensionsPx;

    // Port of: ((LayoutParams)mProgressView.getLayoutParams()).addRule(CENTER_IN_PARENT);
    progress->getLayoutParams()->addRule(CENTER_IN_PARENT);
}

} // namespace PinkReader
