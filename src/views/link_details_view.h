/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: link_details_view.h - Port of LinkDetailsView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LinkDetailsView.java
 *
 * Original: class LinkDetailsView extends FrameLayout
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <string>

namespace PinkReader {

class Context;

// ============================================================================
// LinkDetailsView — port of Java class (Java lines 39-124)
//
// Port of: org.quantumbadger.redreader.views.LinkDetailsView
// Orig extends: FrameLayout
//
// A tappable card showing a link icon, title, and optional subtitle with
// a stroked border. Touch feedback via background color change.
// Every field, method, constant ported exactly.
// ============================================================================

class LinkDetailsView {
public:
    // ========================================================================
    // Constructor (Java lines 42-123)
    //
    // Port of: public LinkDetailsView(final Context context,
    //     @NonNull final String title, @Nullable final String subtitle)
    // ========================================================================

    LinkDetailsView(Context* context,
                    const std::string& title,
                    const std::string* subtitle);

    virtual ~LinkDetailsView();

private:
    // Touch event action constants (port of: MotionEvent constants)
    static constexpr int ACTION_DOWN = 0;
    static constexpr int ACTION_UP = 1;
    static constexpr int ACTION_CANCEL = 3;

    // Transparent color constant
    static constexpr int COLOR_TRANSPARENT = 0x00000000;

    // Port of: Color.argb(50, 128, 128, 128) for touch down
    static constexpr int COLOR_TOUCH_DOWN = 0x32808080;

    // Port of: Color.argb(128, 128, 128, 128) for stroke
    static constexpr int COLOR_STROKE = 0x80808080;

    Context* m_context = nullptr;
};

} // namespace PinkReader
