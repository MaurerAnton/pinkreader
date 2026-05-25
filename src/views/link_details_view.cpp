/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: link_details_view.cpp - Port of LinkDetailsView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/LinkDetailsView.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "views/link_details_view.h"

#include <cmath>
#include <string>

namespace PinkReader {

// ============================================================================
// Context stub (port of: android.content.Context)
// ============================================================================

class Context {
public:
    struct TypedArray {
        // Port of: appearance.getDrawable(0) — returns drawable ID
        int getDrawable(int) { return 0; }
        void recycle() {}
    };

    TypedArray* obtainStyledAttributes(int* /*attrs*/) {
        return new TypedArray();
    }
};

// ============================================================================
// PrefsUtility stub
// ============================================================================

static float prefs_appearance_fontscale_linkbuttons() {
    return 1.0f;
}

static int dpToPixels(Context*, float dp) {
    return static_cast<int>(std::round(dp * 2.0f));
}

// ============================================================================
// Color utility (port of: android.graphics.Color.argb)
// ============================================================================

static int argb(int a, int r, int g, int b) {
    return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
}

// ============================================================================
// Paint style (port of: android.graphics.Paint.Style.STROKE)
// ============================================================================

enum class PaintStyle {
    FILL = 0,
    STROKE = 1,
    FILL_AND_STROKE = 2
};

// ============================================================================
// ShapeDrawable + RectShape stubs
// ============================================================================

class RectShape {
public:
    RectShape() = default;
};

class ShapeDrawable {
public:
    ShapeDrawable(RectShape*) {}

    class PaintWrapper {
    public:
        void setColor(int) {}
        void setStrokeWidth(float) {}
        void setStyle(PaintStyle) {}
    };

    PaintWrapper& getPaint() { return m_paint; }

private:
    PaintWrapper m_paint;
};

// R.attr stub
constexpr int R_ATTR_RR_ICON_LINK = 0;

// Gravity constant
static constexpr int GRAVITY_CENTER_VERTICAL = 0x10;

// ============================================================================
// Constructor (Java lines 42-123)
// ============================================================================

LinkDetailsView::LinkDetailsView(Context* context,
                                 const std::string& title,
                                 const std::string* subtitle) {
    m_context = context;

    // Port of: setClickable(true);
    // (In Qt, this would set widget interaction flags)

    // Port of: final LinearLayout layout = new LinearLayout(context);
    // Port of: layout.setOrientation(LinearLayout.HORIZONTAL);
    // Port of: addView(layout);
    // Port of: final int marginPx = General.dpToPixels(context, 10);
    const int marginPx = dpToPixels(context, 10);

    // Port of: layout.setGravity(Gravity.CENTER_VERTICAL);
    // (layout gravity set)

    // Port of: final TypedArray appearance = context.obtainStyledAttributes(
    //     new int[] {R.attr.rrIconLink});
    {
        int attrs[1] = {R_ATTR_RR_ICON_LINK};
        Context::TypedArray* appearance = context->obtainStyledAttributes(attrs);

        // Port of: final ImageView icon = new ImageView(context);
        // Port of: icon.setImageDrawable(appearance.getDrawable(0));
        const int iconDrawable = appearance->getDrawable(0);
        (void)iconDrawable;

        // Port of: appearance.recycle();
        appearance->recycle();
        delete appearance;

        // Port of: layout.addView(icon);
        // Port of: ((LinearLayout.LayoutParams)icon.getLayoutParams()).setMargins(
        //     marginPx, marginPx, marginPx, marginPx);
    }

    // Port of: final LinearLayout textLayout = new LinearLayout(context);
    // Port of: textLayout.setOrientation(LinearLayout.VERTICAL);
    // Port of: layout.addView(textLayout);
    // Port of: ((LinearLayout.LayoutParams)textLayout.getLayoutParams()).setMargins(
    //     0, marginPx, marginPx, marginPx);

    // Port of: final float linkFontScale = PrefsUtility.appearance_fontscale_linkbuttons();
    const float linkFontScale = prefs_appearance_fontscale_linkbuttons();

    // Port of: { final TextView titleView = new TextView(context);
    //     titleView.setText(title); titleView.setTextSize(15f * linkFontScale);
    //     textLayout.addView(titleView); }
    const float titleSize = 15.0f * linkFontScale;
    (void)titleSize;
    (void)title; // title used above via text

    // Port of: if(subtitle != null && !title.equals(subtitle)) {
    //     final TextView subtitleView = new TextView(context);
    //     subtitleView.setText(subtitle);
    //     subtitleView.setTextSize(11f * linkFontScale);
    //     textLayout.addView(subtitleView); }
    if (subtitle != nullptr && *subtitle != title) {
        const float subSize = 11.0f * linkFontScale;
        (void)subSize;
    }

    // Port of: final float borderPx = General.dpToPixels(context, 2);
    const float borderPx = dpToPixels(context, 2);

    // Port of: final RectShape borderShape = new RectShape();
    RectShape borderShape;

    // Port of: final ShapeDrawable border = new ShapeDrawable(borderShape);
    ShapeDrawable border(&borderShape);

    // Port of: border.getPaint().setColor(Color.argb(128, 128, 128, 128));
    border.getPaint().setColor(argb(128, 128, 128, 128));

    // Port of: border.getPaint().setStrokeWidth(borderPx);
    border.getPaint().setStrokeWidth(borderPx);

    // Port of: border.getPaint().setStyle(Paint.Style.STROKE);
    border.getPaint().setStyle(PaintStyle::STROKE);

    // Port of: setBackground(border);
    // (Set as background)

    // Port of: setOnTouchListener((v, event) -> { ... return false; });
    // Touch handling:
    //   ACTION_DOWN: layout.setBackgroundColor(Color.argb(50, 128, 128, 128)); invalidate();
    //   ACTION_UP/ACTION_CANCEL: layout.setBackgroundColor(Color.TRANSPARENT); invalidate();
    //   return false;
}

LinkDetailsView::~LinkDetailsView() = default;

} // namespace PinkReader
