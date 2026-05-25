/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: button_loading_spinner_view.cpp - Port of ButtonLoadingSpinnerView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/ButtonLoadingSpinnerView.java
 */

#include "views/button_loading_spinner_view.h"

#include <cmath>

namespace PinkReader {

// ============================================================================
// Stubs
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

static constexpr int COLOR_MAGENTA = 0xFFFF00FF;
static constexpr int COLOR_GREEN = 0xFF00FF00;
static constexpr int CENTER_IN_PARENT = 13;

constexpr int R_ATTR_LOADING_RING_FG = 0;
constexpr int R_ATTR_LOADING_RING_BG = 1;

static int dpToPixels(Context*, float dp) {
    return static_cast<int>(std::round(dp * 2.0f));
}

class DonutProgress {
public:
    DonutProgress(Context*) {}

    void setAspectIndicatorDisplay(bool) {}
    void setIndeterminate(bool) {}
    void setFinishedStrokeColor(int) {}
    void setUnfinishedStrokeColor(int) {}
    void setUnfinishedStrokeWidth(int) {}
    void setFinishedStrokeWidth(int) {}
    void setStartingDegree(int) {}
    void initPainters() {}

    struct LayoutParams {
        int width = 0;
        int height = 0;
        void addRule(int) {}
    };

    LayoutParams* getLayoutParams() { return &m_layoutParams; }

private:
    LayoutParams m_layoutParams;
};

// ============================================================================
// Constructor (Java lines 32-62)
// ============================================================================

ButtonLoadingSpinnerView::ButtonLoadingSpinnerView(Context* context) {
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

    // Port of: final int progressStrokeWidthPx = General.dpToPixels(context, 4);
    const int strokeWidthPx = dpToPixels(context, 4);

    // Port of: mProgressView.setUnfinishedStrokeWidth(progressStrokeWidthPx);
    progress->setUnfinishedStrokeWidth(strokeWidthPx);

    // Port of: mProgressView.setFinishedStrokeWidth(progressStrokeWidthPx);
    progress->setFinishedStrokeWidth(strokeWidthPx);

    // Port of: mProgressView.setStartingDegree(-90);
    progress->setStartingDegree(-90);

    // Port of: mProgressView.initPainters();
    progress->initPainters();

    // Port of: addView(mProgressView);
    m_progressView = progress;

    // Port of: final int progressDimensionsPx = General.dpToPixels(context, 24);
    const int dimsPx = dpToPixels(context, 24);

    // Port of: mProgressView.getLayoutParams().width = progressDimensionsPx;
    progress->getLayoutParams()->width = dimsPx;

    // Port of: mProgressView.getLayoutParams().height = progressDimensionsPx;
    progress->getLayoutParams()->height = dimsPx;

    // Port of: ((LayoutParams)mProgressView.getLayoutParams()).addRule(CENTER_IN_PARENT);
    progress->getLayoutParams()->addRule(CENTER_IN_PARENT);
}

ButtonLoadingSpinnerView::~ButtonLoadingSpinnerView() {
    delete static_cast<DonutProgress*>(m_progressView);
}

} // namespace PinkReader
