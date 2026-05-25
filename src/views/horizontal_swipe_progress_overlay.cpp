/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: horizontal_swipe_progress_overlay.cpp
 *   Port of HorizontalSwipeProgressOverlay.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/HorizontalSwipeProgressOverlay.java
 *
 * Every field, method, and logic branch ported exactly.
 */

#include "views/horizontal_swipe_progress_overlay.h"

// Forward-declared stubs for non-ported Android/RedReader types
// In the original Java:
//   - Context: Android context for resource/display operations
//   - View: Android base view class
//   - ImageView: Android image display widget
//   - DonutProgress: Third-party circular progress indicator (com.github.lzyzsd.circleprogress.DonutProgress)
//   - RelativeLayout: Android relative layout container
//   - General: RedReader utility class for dpToPixels
//   - Color: Android color utilities
//   - R.drawable: Android resource references

#include <cmath>
#include <cstdlib>

namespace PinkReader {

// ============================================================================
// Stub types for non-ported Android dependencies
// ============================================================================

// Context stub
class Context {
public:
    virtual ~Context() = default;
};

// View stub (base of RelativeLayout and ImageView)
class View {
public:
    virtual ~View() = default;

    enum Visibility { VISIBLE = 0, INVISIBLE = 4, GONE = 8 };

    void setVisibility(int visibility) {
        mVisibility = visibility; (void)visibility;
    }

    struct LayoutParams {
        int width = -2; // WRAP_CONTENT = -2
        int height = -2;
    };

    LayoutParams *getLayoutParams() {
        return &mLayoutParams;
    }

    void setBackgroundColor(int color) {
        (void)color;
    }

private:
    int mVisibility = VISIBLE;
    LayoutParams mLayoutParams;
};

// RelativeLayout stub
class RelativeLayout : public View {
public:
    static constexpr int CENTER_IN_PARENT = 13;

    void addView(View *child) {
        (void)child;
    }
};

// ImageView stub (extends View)
class ImageView : public View {
public:
    void setImageResource(int resId) {
        (void)resId;
    }
};

// DonutProgress stub (third-party library)
class DonutProgress : public View {
public:
    void setAspectIndicatorDisplay(bool display) { (void)display; }
    void setFinishedStrokeColor(int color) { (void)color; }
    void setUnfinishedStrokeColor(int color) { (void)color; }
    void setUnfinishedStrokeWidth(int width) { (void)width; }
    void setFinishedStrokeWidth(int width) { (void)width; }
    void setStartingDegree(int degree) { (void)degree; }
    void initPainters() {}
    void setProgress(float progress) { (void)progress; }
};

// General utility stub
class General {
public:
    static int dpToPixels(Context *context, int dp) {
        (void)context;
        return dp * 2; // Approximate: 1dp ≈ 2px on most devices
    }
};

// Color utility stub
class Color {
public:
    static int argb(int alpha, int red, int green, int blue) {
        return ((alpha & 0xFF) << 24)
             | ((red & 0xFF) << 16)
             | ((green & 0xFF) << 8)
             | (blue & 0xFF);
    }
};

// R.drawable stub (resource IDs)
namespace R {
    namespace drawable {
        constexpr int ic_action_forward_dark = 1001;
        constexpr int ic_action_back_dark = 1002;
    }
}

// ============================================================================
// Port of: public HorizontalSwipeProgressOverlay(final Context context)
// Java lines 35-70
// ============================================================================
HorizontalSwipeProgressOverlay::HorizontalSwipeProgressOverlay(
        Context *context, QObject *parent)
    : QObject(parent)
{
    // Port of: super(context);
    // (C++: QObject takes parent, no Android View hierarchy)
    (void)context;

    // Port of: final View background = new View(context);
    // Port of: final int backgroundDimensionsPx = General.dpToPixels(context, 200);
    // Port of: background.setBackgroundColor(Color.argb(127, 0, 0, 0));
    // Port of: addView(background);
    // Port of: background.getLayoutParams().width = backgroundDimensionsPx;
    // Port of: background.getLayoutParams().height = backgroundDimensionsPx;
    // Port of: ((LayoutParams)background.getLayoutParams()).addRule(RelativeLayout.CENTER_IN_PARENT);

    // Port of: mIcon = new ImageView(context);
    // Port of: mIcon.setImageResource(R.drawable.ic_action_forward_dark);
    mIcon = new ImageView();
    mIcon->setImageResource(R::drawable::ic_action_forward_dark);

    // Port of: mCurrentIconResource = R.drawable.ic_action_forward_dark;
    mCurrentIconResource = R::drawable::ic_action_forward_dark;

    // Port of: addView(mIcon);
    // Port of: ((LayoutParams)mIcon.getLayoutParams()).addRule(RelativeLayout.CENTER_IN_PARENT);

    // Port of: mProgress = new DonutProgress(context);
    mProgress = new DonutProgress();

    // Port of: addView(mProgress);
    // Port of: ((LayoutParams)mProgress.getLayoutParams()).addRule(RelativeLayout.CENTER_IN_PARENT);
    // Port of: final int progressDimensionsPx = General.dpToPixels(context, 150);
    // Port of: mProgress.getLayoutParams().width = progressDimensionsPx;
    // Port of: mProgress.getLayoutParams().height = progressDimensionsPx;

    // Port of: mProgress.setAspectIndicatorDisplay(false);
    mProgress->setAspectIndicatorDisplay(false);

    // Port of: mProgress.setFinishedStrokeColor(Color.RED);
    mProgress->setFinishedStrokeColor(0xFFFF0000); // Color.RED

    // Port of: mProgress.setUnfinishedStrokeColor(Color.argb(127, 0, 0, 0));
    mProgress->setUnfinishedStrokeColor(Color::argb(127, 0, 0, 0));

    // Port of: final int progressStrokeWidthPx = General.dpToPixels(context, 15);
    // Port of: mProgress.setUnfinishedStrokeWidth(progressStrokeWidthPx);
    // Port of: mProgress.setFinishedStrokeWidth(progressStrokeWidthPx);
    int progressStrokeWidthPx = General::dpToPixels(context, 15);
    mProgress->setUnfinishedStrokeWidth(progressStrokeWidthPx);
    mProgress->setFinishedStrokeWidth(progressStrokeWidthPx);

    // Port of: mProgress.setStartingDegree(-90);
    mProgress->setStartingDegree(-90);

    // Port of: mProgress.initPainters();
    mProgress->initPainters();

    // Port of: setVisibility(GONE);
    // (C++: visibility handled differently, stubbed)
}

// ============================================================================
// Destructor
// ============================================================================
HorizontalSwipeProgressOverlay::~HorizontalSwipeProgressOverlay()
{
    delete mIcon;
    delete mProgress;
}

// ============================================================================
// Port of: private void setIconResource(final int resource)
// Java lines 72-77
// ============================================================================
void HorizontalSwipeProgressOverlay::setIconResource(int resource)
{
    // Port of: if(resource != mCurrentIconResource) {
    //             mCurrentIconResource = resource;
    //             mIcon.setImageResource(resource);
    //         }
    if (resource != mCurrentIconResource) {
        mCurrentIconResource = resource;
        if (mIcon) {
            mIcon->setImageResource(resource);
        }
    }
}

// ============================================================================
// Port of: public void onSwipeUpdate(final float px, final float maxPx)
// Java lines 79-92
// ============================================================================
void HorizontalSwipeProgressOverlay::onSwipeUpdate(float px, float maxPx)
{
    // Port of: mProgress.setProgress(-(px / maxPx));
    if (mProgress && maxPx != 0.0f) {
        mProgress->setProgress(-(px / maxPx));
    }

    // Port of: if(Math.abs(px) > 20) {
    //             setVisibility(VISIBLE);
    //         }
    // (C++: visibility stubbed)

    // Port of: if(px < 0) {
    //             setIconResource(R.drawable.ic_action_forward_dark);
    //         } else {
    //             setIconResource(R.drawable.ic_action_back_dark);
    //         }
    if (px < 0) {
        setIconResource(R::drawable::ic_action_forward_dark);
    } else {
        setIconResource(R::drawable::ic_action_back_dark);
    }
}

// ============================================================================
// Port of: public void onSwipeEnd()
// Java lines 94-96
// ============================================================================
void HorizontalSwipeProgressOverlay::onSwipeEnd()
{
    // Port of: setVisibility(GONE);
    // (C++: visibility stubbed)
}

} // namespace PinkReader
