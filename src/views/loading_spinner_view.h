/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: loading_spinner_view.h - Port of LoadingSpinnerView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LoadingSpinnerView.java
 *
 * Original: class LoadingSpinnerView extends RelativeLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

namespace PinkReader {

// Forward declarations
class Context;
class AttributeSet;

// ============================================================================
// LoadingSpinnerView — port of Java class (Java lines 31-80)
//
// Port of: org.quantumbadger.redreader.views.LoadingSpinnerView
// Orig extends: RelativeLayout
//
// A RelativeLayout containing a circular indeterminate progress spinner.
// Every field, method, constant ported exactly.
// ============================================================================

class LoadingSpinnerView {
public:
    // ========================================================================
    // Constructor — 3-arg (Java lines 35-68)
    //
    // Port of: public LoadingSpinnerView(@NonNull final Context context,
    //     @Nullable final AttributeSet attributeSet, final int defStyle)
    // ========================================================================

    LoadingSpinnerView(Context* context, AttributeSet* attributeSet, int defStyle);

    // ========================================================================
    // Constructor — 2-arg (Java lines 70-75)
    //
    // Port of: public LoadingSpinnerView(@NonNull final Context context,
    //     @Nullable final AttributeSet attributeSet)
    //     { this(context, attributeSet, 0); }
    // ========================================================================

    LoadingSpinnerView(Context* context, AttributeSet* attributeSet);

    // ========================================================================
    // Constructor — 1-arg (Java lines 77-79)
    //
    // Port of: public LoadingSpinnerView(@NonNull final Context context)
    //     { this(context, null); }
    // ========================================================================

    explicit LoadingSpinnerView(Context* context);

    virtual ~LoadingSpinnerView();

    // ========================================================================
    // Accessors
    // ========================================================================

    // Port of: final DonutProgress mProgressView (Java line 33)
    // In C++, we store a handle/ID for the progress indicator
    void* progressView() const { return m_progressView; }

private:
    // ========================================================================
    // Common init — shared by all constructors (Java lines 42-67)
    // ========================================================================

    void commonInit(Context* context);

    // ========================================================================
    // Fields — exact match (Java line 33)
    //
    // Port of: final DonutProgress mProgressView; (Java line 33)
    // ========================================================================

    void* m_progressView = nullptr;
    int m_foregroundColor = 0;
    int m_backgroundColor = 0;
    int m_progressDimensionsPx = 0;
    int m_progressStrokeWidthPx = 0;
};

} // namespace PinkReader
