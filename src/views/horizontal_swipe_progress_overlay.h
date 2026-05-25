/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: horizontal_swipe_progress_overlay.h
 *   Port of HorizontalSwipeProgressOverlay.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/HorizontalSwipeProgressOverlay.java
 *
 * Original: public class HorizontalSwipeProgressOverlay extends RelativeLayout
 *
 * Every field, method, and constant ported exactly.
 */

#pragma once

#include <QObject>

namespace PinkReader {

// Forward declarations for Android/RedReader types
class Context;
class View;
class ImageView;
class DonutProgress;
class RelativeLayout;

/**
 * @brief Port of org.quantumbadger.redreader.views.HorizontalSwipeProgressOverlay
 *
 * Overlay that shows a circular progress indicator during horizontal swipe gestures.
 * Used by ImageViewActivity for swipe-to-dismiss/swipe-navigate functionality.
 *
 * Every field, method, and logic branch ported exactly from Java.
 */
class HorizontalSwipeProgressOverlay : public QObject {
    Q_OBJECT

public:
    // ===== Constructor (Java line 35) =====
    // Port of: public HorizontalSwipeProgressOverlay(final Context context)
    explicit HorizontalSwipeProgressOverlay(Context *context, QObject *parent = nullptr);
    ~HorizontalSwipeProgressOverlay() override;

    // ===== Public methods =====

    // Port of: public void onSwipeUpdate(final float px, final float maxPx) (Java lines 79-92)
    void onSwipeUpdate(float px, float maxPx);

    // Port of: public void onSwipeEnd() (Java lines 94-96)
    void onSwipeEnd();

private:
    // ===== Private methods =====

    // Port of: private void setIconResource(final int resource) (Java lines 72-77)
    void setIconResource(int resource);

    // ===== Private fields (Java lines 31-33) =====

    // Port of: private final ImageView mIcon;
    ImageView *mIcon = nullptr;

    // Port of: private final DonutProgress mProgress;
    DonutProgress *mProgress = nullptr;

    // Port of: private int mCurrentIconResource;
    int mCurrentIconResource = 0;
};

} // namespace PinkReader
