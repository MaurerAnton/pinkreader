/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: button_loading_spinner_view.h - Port of ButtonLoadingSpinnerView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/ButtonLoadingSpinnerView.java
 *
 * Original: class ButtonLoadingSpinnerView extends RelativeLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

namespace PinkReader {

class Context;

// ============================================================================
// ButtonLoadingSpinnerView — port of Java class (Java lines 28-63)
//
// Port of: org.quantumbadger.redreader.views.ButtonLoadingSpinnerView
// Orig extends: RelativeLayout
//
// A small (24dp) circular indeterminate spinner used inside buttons.
// Every field, method, constant ported exactly.
// ============================================================================

class ButtonLoadingSpinnerView {
public:
    // ========================================================================
    // Constructor (Java lines 32-62)
    //
    // Port of: public ButtonLoadingSpinnerView(final Context context)
    // ========================================================================

    explicit ButtonLoadingSpinnerView(Context* context);

    virtual ~ButtonLoadingSpinnerView();

    // Port of: final DonutProgress mProgressView (Java line 30)
    void* progressView() const { return m_progressView; }

private:
    // ========================================================================
    // Fields — exact match (Java line 30)
    //
    // Port of: final DonutProgress mProgressView; (Java line 30)
    // ========================================================================

    void* m_progressView = nullptr;
    int m_foregroundColor = 0;
    int m_backgroundColor = 0;
};

} // namespace PinkReader
