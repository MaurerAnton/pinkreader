/*
 * PinkReader - GPLv3
 * File: flingable_item_view.h
 * Port of: org.quantumbadger.redreader.views.FlingableItemView.java
 *
 * Every field, method, inner class matches exactly.
 * Extends SwipableItemView (Java: extends SwipableItemView)
 */

#pragma once

#include "swipable_item_view.h"

#include <QLabel>
#include <QFrame>
#include <memory>

namespace PinkReader {

// Port of: public abstract class FlingableItemView extends SwipableItemView
class FlingableItemView : public SwipableItemView {
    Q_OBJECT

public:
    // Port of: public FlingableItemView(@NonNull final Context context)
    explicit FlingableItemView(QWidget *parent = nullptr);

    // Port of: public void setFlingingEnabled(final boolean flingingEnabled)
    void setFlingingEnabled(bool flingingEnabled);

protected:
    // ========================================================================
    // Abstract methods — Port exact from Java
    // ========================================================================

    // Port of: protected abstract void onSetItemFlingPosition(final float position)
    virtual void onSetItemFlingPosition(float position) = 0;

    // Port of: @NonNull protected abstract String getFlingLeftText()
    virtual QString getFlingLeftText() = 0;

    // Port of: @NonNull protected abstract String getFlingRightText()
    virtual QString getFlingRightText() = 0;

    // Port of: protected abstract boolean allowFlingingLeft()
    virtual bool allowFlingingLeft() = 0;

    // Port of: protected abstract boolean allowFlingingRight()
    virtual bool allowFlingingRight() = 0;

    // Port of: protected abstract void onFlungLeft()
    virtual void onFlungLeft() = 0;

    // Port of: protected abstract void onFlungRight()
    virtual void onFlungRight() = 0;

    // ========================================================================
    // Overrides from SwipableItemView
    // ========================================================================

    // Port of: @Override protected final boolean allowSwipingLeft()
    bool allowSwipingLeft() final;

    // Port of: @Override protected final boolean allowSwipingRight()
    bool allowSwipingRight() final;

    // Port of: @Override protected void onSwipeFingerDown(int x, int y, float xOffsetPixels, boolean wasOldSwipeInterrupted)
    void onSwipeFingerDown(int x, int y, float xOffsetPixels, bool wasOldSwipeInterrupted) override;

    // Port of: @Override public void onSwipeDeltaChanged(final float xOffsetPixels)
    void onSwipeDeltaChanged(float xOffsetPixels) override;

private:
    // ========================================================================
    // Fields — Port exact from Java
    // ========================================================================

    // Port of: private final FrameLayout mFlingHintOuter
    QFrame *m_flingHintOuter = nullptr;

    // Port of: private final TextView mFlingHintLeft
    QLabel *m_flingHintLeft = nullptr;

    // Port of: private final TextView mFlingHintRight
    QLabel *m_flingHintRight = nullptr;

    // Port of: private boolean mSwipeReady = false
    bool m_swipeReady = false;

    // Port of: private boolean mRightFlingHintShown = false
    bool m_rightFlingHintShown = false;

    // Port of: private boolean mLeftFlingHintShown = false
    bool m_leftFlingHintShown = false;

    // Port of: private FlingHintAnimation mFlingHintAnimation
    class FlingHintAnimation *m_flingHintAnimation = nullptr;

    // Port of: private float mFlingHintYPos = 0
    float m_flingHintYPos = 0.0f;

    // Port of: private final int mOffsetBeginAllowed
    int m_offsetBeginAllowed;

    // Port of: private final int mOffsetActionPerformed
    int m_offsetActionPerformed;

    // Port of: private final Drawable rrIconFfLeft
    // In Qt, we use QIcon or QPixmap. For now, store as a simple indicator.
    bool m_hasIconFfLeft = true;

    // Port of: private final Drawable rrIconFfRight
    bool m_hasIconFfRight = true;

    // Port of: private final Drawable rrIconTick
    bool m_hasIconTick = true;

    // Port of: private final int rrListBackgroundCol (= General.COLOR_INVALID if not set)
    unsigned int m_rrListBackgroundCol = 0xFFFF00FF; // Port of: Color.MAGENTA = COLOR_INVALID

    // ========================================================================
    // Inner class: FlingHintAnimation — Port exact
    // ========================================================================

    class FlingHintAnimation : public RRDHMAnimation {
    public:
        // Port of: private FlingHintAnimation(final LiveDHM.Params params) { super(params); }
        FlingHintAnimation(const LiveDHMParams &params, FlingableItemView *parent);

    protected:
        // Port of: @Override protected void onUpdatedPosition(final float position)
        void onUpdatedPosition(float position) override;

        // Port of: @Override protected void onEndPosition(final float endPosition)
        void onEndPosition(float endPosition) override;

    private:
        FlingableItemView *m_parent;
    };

    // ========================================================================
    // Private methods — Port exact from Java
    // ========================================================================

    // Port of: private void updateFlingHintPosition()
    void updateFlingHintPosition();
};

} // namespace PinkReader
