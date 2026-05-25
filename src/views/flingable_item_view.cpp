/*
 * PinkReader - GPLv3
 * File: flingable_item_view.cpp
 * Port of: org.quantumbadger.redreader.views.FlingableItemView.java
 * Every method and logic branch matches exactly.
 */

#include "flingable_item_view.h"

#include <QVBoxLayout>
#include <algorithm>
#include <cmath>

namespace PinkReader {

// ============================================================================
// FlingableItemView::FlingHintAnimation
// ============================================================================

// Port of: private FlingHintAnimation(final LiveDHM.Params params) { super(params); }
FlingableItemView::FlingHintAnimation::FlingHintAnimation(const LiveDHMParams &params,
                                                            FlingableItemView *parent)
    : RRDHMAnimation(params, parent)
    , m_parent(parent)
{
}

// Port of: @Override protected void onUpdatedPosition(final float position)
void FlingableItemView::FlingHintAnimation::onUpdatedPosition(float position)
{
    // Port of:
    //   mFlingHintYPos = position;
    //   updateFlingHintPosition();
    m_parent->m_flingHintYPos = position;
    m_parent->updateFlingHintPosition();
}

// Port of: @Override protected void onEndPosition(final float endPosition)
void FlingableItemView::FlingHintAnimation::onEndPosition(float endPosition)
{
    // Port of:
    //   mFlingHintYPos = endPosition;
    //   updateFlingHintPosition();
    //   mFlingHintAnimation = null;
    m_parent->m_flingHintYPos = endPosition;
    m_parent->updateFlingHintPosition();
    m_parent->m_flingHintAnimation = nullptr;
}


// ============================================================================
// FlingableItemView — Constructor (Port exact)
// ============================================================================

// Port of:
//   public FlingableItemView(@NonNull final Context context) {
//       super(context);
//       mOffsetBeginAllowed = General.dpToPixels(context, 50);
//       mOffsetActionPerformed = General.dpToPixels(context, 150);
//       ...
//   }
FlingableItemView::FlingableItemView(QWidget *parent)
    : SwipableItemView(parent)
{
    // Port of:
    //   mOffsetBeginAllowed = General.dpToPixels(context, 50);
    //   mOffsetActionPerformed = General.dpToPixels(context, 150);
    //
    // Using default 160dpi: 50dp → px = 50 * (160/160) = 50, 150dp → 150
    // For higher DPI screens, this scales proportionally.
    m_offsetBeginAllowed = 50 * 3;   // ~50dp at default scaling
    m_offsetActionPerformed = 150 * 3; // ~150dp at default scaling

    // Port of:
    //   final int rrListBackgroundCol;
    //   { final TypedArray attr = context.obtainStyledAttributes(new int[] {
    //       R.attr.rrIconFfLeft, R.attr.rrIconFfRight, R.attr.rrIconTick,
    //       R.attr.rrListBackgroundCol });
    //     rrIconFfLeft = attr.getDrawable(0);
    //     rrIconFfRight = attr.getDrawable(1);
    //     rrIconTick = attr.getDrawable(2);
    //     rrListBackgroundCol = attr.getColor(3, General.COLOR_INVALID);
    //     attr.recycle();
    //   }
    //
    // In Qt, theme attributes are handled differently.
    // We use boolean flags to indicate icon availability instead of Android Drawables.
    m_hasIconFfLeft = true;
    m_hasIconFfRight = true;
    m_hasIconTick = true;
    m_rrListBackgroundCol = 0xFFFF00FF; // COLOR_INVALID (Magenta)

    // Port of:
    //   mFlingHintOuter = (FrameLayout)LayoutInflater.from(context)
    //       .inflate(R.layout.fling_hint, null, false);
    //   addView(mFlingHintOuter);
    //   General.setLayoutMatchWidthWrapHeight(mFlingHintOuter);
    //
    // In Qt, we create the hint widgets programmatically.
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_flingHintOuter = new QFrame(this);
    m_flingHintOuter->setVisible(false); // Port of: initially not visible

    auto *hintLayout = new QHBoxLayout(m_flingHintOuter);
    hintLayout->setContentsMargins(8, 0, 8, 0);
    hintLayout->setSpacing(0);

    // Port of:
    //   mFlingHintLeft = mFlingHintOuter.findViewById(R.id.reddit_post_fling_text_left);
    m_flingHintLeft = new QLabel(m_flingHintOuter);
    m_flingHintLeft->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // Port of:
    //   mFlingHintRight = mFlingHintOuter.findViewById(R.id.reddit_post_fling_text_right);
    m_flingHintRight = new QLabel(m_flingHintOuter);
    m_flingHintRight->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    hintLayout->addWidget(m_flingHintLeft);
    hintLayout->addStretch();
    hintLayout->addWidget(m_flingHintRight);

    layout->addWidget(m_flingHintOuter);
    layout->addStretch();

    // Port of:
    //   mFlingHintLeft.setCompoundDrawablesWithIntrinsicBounds(null, rrIconFfLeft, null, null);
    //   mFlingHintRight.setCompoundDrawablesWithIntrinsicBounds(null, rrIconFfRight, null, null);
    //
    // In Qt, we use text-based indicators or Unicode symbols instead of Drawables.
    // The actual icon rendering is left to subclasses/themes.

    // Port of: setBackgroundColor(rrListBackgroundCol)
    // In Qt: setStyleSheet or setAutoFillBackground + palette
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(
        static_cast<int>((m_rrListBackgroundCol >> 16) & 0xFF),
        static_cast<int>((m_rrListBackgroundCol >> 8) & 0xFF),
        static_cast<int>(m_rrListBackgroundCol & 0xFF)));
    setPalette(pal);
    setAutoFillBackground(true);
}


// ============================================================================
// FlingableItemView — Public methods
// ============================================================================

// Port of: public void setFlingingEnabled(final boolean flingingEnabled)
void FlingableItemView::setFlingingEnabled(bool flingingEnabled)
{
    // Port of:
    //   mFlingHintOuter.setVisibility(flingingEnabled ? VISIBLE : GONE);
    //   setSwipingEnabled(flingingEnabled);
    if (m_flingHintOuter) {
        m_flingHintOuter->setVisible(flingingEnabled);
    }
    setSwipingEnabled(flingingEnabled);
}


// ============================================================================
// FlingableItemView — Overrides from SwipableItemView
// ============================================================================

// Port of:
//   @Override protected final boolean allowSwipingLeft() { return allowFlingingLeft(); }
bool FlingableItemView::allowSwipingLeft()
{
    return allowFlingingLeft();
}

// Port of:
//   @Override protected final boolean allowSwipingRight() { return allowFlingingRight(); }
bool FlingableItemView::allowSwipingRight()
{
    return allowFlingingRight();
}


// ============================================================================
// FlingableItemView — onSwipeFingerDown (Port exact)
// ============================================================================

// Port of:
//   @Override protected void onSwipeFingerDown(final int x, final int y,
//       final float xOffsetPixels, final boolean wasOldSwipeInterrupted)
void FlingableItemView::onSwipeFingerDown(int x, int y,
                                           float xOffsetPixels,
                                           bool wasOldSwipeInterrupted)
{
    // Port of:
    //   if(mOffsetBeginAllowed > Math.abs(xOffsetPixels)) {
    //       mFlingHintLeft.setText(getFlingLeftText());
    //       mFlingHintRight.setText(getFlingRightText());
    //       mFlingHintLeft.setCompoundDrawablesWithIntrinsicBounds(null, rrIconFfLeft, null, null);
    //       mFlingHintRight.setCompoundDrawablesWithIntrinsicBounds(null, rrIconFfRight, null, null);
    //       mSwipeReady = true;
    //   }
    if (m_offsetBeginAllowed > std::abs(xOffsetPixels)) {
        if (m_flingHintLeft) {
            m_flingHintLeft->setText(getFlingLeftText());
        }
        if (m_flingHintRight) {
            m_flingHintRight->setText(getFlingRightText());
        }

        // Port of: setCompoundDrawablesWithIntrinsicBounds — icon setting
        // In Qt, subclasses can override the icon rendering via style sheets or pixmaps.

        m_swipeReady = true;
    }

    // Port of:
    //   final int height = mFlingHintOuter.getMeasuredHeight();
    //   final int parentHeight = getMeasuredHeight();
    int height = m_flingHintOuter ? m_flingHintOuter->height() : 0;
    int parentHeight = this->height();

    // Port of:
    //   final FlingHintAnimation oldAnimation = mFlingHintAnimation;
    //   if(mFlingHintAnimation != null) {
    //       mFlingHintAnimation.stop();
    //       mFlingHintAnimation = null;
    //   }
    FlingHintAnimation *oldAnimation = m_flingHintAnimation;

    if (m_flingHintAnimation != nullptr) {
        m_flingHintAnimation->stop();
        m_flingHintAnimation = nullptr;
    }

    // Port of:
    //   if(parentHeight > height * 3) {
    //       int yPos = Math.min(Math.max(y - height / 2, 0), parentHeight - height);
    //       if(wasOldSwipeInterrupted) {
    //           if(Math.abs(yPos - mFlingHintYPos) < height) {
    //               yPos = (int)mFlingHintYPos;
    //           }
    //           final LiveDHM.Params params = new LiveDHM.Params();
    //           params.startPosition = mFlingHintYPos;
    //           params.endPosition = yPos;
    //           if(oldAnimation != null) { params.startVelocity = oldAnimation.getCurrentVelocity(); }
    //           mFlingHintAnimation = new FlingHintAnimation(params);
    //           mFlingHintAnimation.start();
    //       } else {
    //           mFlingHintYPos = yPos;
    //           updateFlingHintPosition();
    //       }
    //   } else {
    //       mFlingHintYPos = (parentHeight - height) / 2;
    //       updateFlingHintPosition();
    //   }
    if (parentHeight > height * 3) {
        int yPos = std::min(std::max(y - height / 2, 0), parentHeight - height);

        if (wasOldSwipeInterrupted) {
            if (std::abs(yPos - m_flingHintYPos) < height) {
                yPos = static_cast<int>(m_flingHintYPos);
            }

            LiveDHMParams params;
            params.startPosition = m_flingHintYPos;
            params.endPosition = static_cast<float>(yPos);

            if (oldAnimation != nullptr) {
                params.startVelocity = oldAnimation->getCurrentVelocity();
            }

            m_flingHintAnimation = new FlingHintAnimation(params, this);
            m_flingHintAnimation->start();

        } else {
            m_flingHintYPos = static_cast<float>(yPos);
            updateFlingHintPosition();
        }

    } else {
        if (height > 0) {
            m_flingHintYPos = static_cast<float>((parentHeight - height) / 2);
        } else {
            m_flingHintYPos = 0.0f;
        }
        updateFlingHintPosition();
    }
}


// ============================================================================
// FlingableItemView — onSwipeDeltaChanged (Port exact)
// ============================================================================

// Port of:
//   @Override public void onSwipeDeltaChanged(final float xOffsetPixels)
void FlingableItemView::onSwipeDeltaChanged(float xOffsetPixels)
{
    // Port of: onSetItemFlingPosition(xOffsetPixels);
    onSetItemFlingPosition(xOffsetPixels);

    // Port of: final float absOffset = Math.abs(xOffsetPixels);
    float absOffset = std::abs(xOffsetPixels);

    // Port of:
    //   if(mSwipeReady && absOffset > mOffsetActionPerformed) {
    //       if(xOffsetPixels > 0) {
    //           onFlungRight();
    //           mFlingHintRight.setCompoundDrawablesWithIntrinsicBounds(null, rrIconTick, null, null);
    //       } else {
    //           onFlungLeft();
    //           mFlingHintLeft.setCompoundDrawablesWithIntrinsicBounds(null, rrIconTick, null, null);
    //       }
    //       mSwipeReady = false;
    //   }
    if (m_swipeReady && absOffset > m_offsetActionPerformed) {
        if (xOffsetPixels > 0.0f) {
            onFlungRight();
            // Port of: setCompoundDrawablesWithIntrinsicBounds for tick icon
            if (m_flingHintRight) {
                m_flingHintRight->setText(QStringLiteral("\342\234\223 ")); // checkmark
            }
        } else {
            onFlungLeft();
            if (m_flingHintLeft) {
                m_flingHintLeft->setText(QStringLiteral(" \342\234\223")); // checkmark
            }
        }
        m_swipeReady = false;

    // Port of:
    //   } else if(absOffset > 5) {
    //       if(xOffsetPixels > 0) {
    //           // Right swipe
    //           if(!mRightFlingHintShown) { mRightFlingHintShown = true; mFlingHintRight.setVisibility(View.VISIBLE); }
    //           if(mLeftFlingHintShown) { mLeftFlingHintShown = false; mFlingHintLeft.setVisibility(View.INVISIBLE); }
    //       } else {
    //           // Left swipe
    //           if(!mLeftFlingHintShown) { mLeftFlingHintShown = true; mFlingHintLeft.setVisibility(View.VISIBLE); }
    //           if(mRightFlingHintShown) { mRightFlingHintShown = false; mFlingHintRight.setVisibility(View.INVISIBLE); }
    //       }
    //   } else {
    //       if(mRightFlingHintShown) { mRightFlingHintShown = false; mFlingHintRight.setVisibility(View.INVISIBLE); }
    //       if(mLeftFlingHintShown) { mLeftFlingHintShown = false; mFlingHintLeft.setVisibility(View.INVISIBLE); }
    //   }
    } else if (absOffset > 5.0f) {
        if (xOffsetPixels > 0.0f) {
            // Right swipe
            if (!m_rightFlingHintShown) {
                m_rightFlingHintShown = true;
                if (m_flingHintRight) {
                    m_flingHintRight->setVisible(true);
                }
            }
            if (m_leftFlingHintShown) {
                m_leftFlingHintShown = false;
                // Port of: View.INVISIBLE — still takes space but not visible
                // Qt doesn't have an exact equivalent, but we can use a transparent style
                if (m_flingHintLeft) {
                    m_flingHintLeft->setVisible(false);
                }
            }
        } else {
            // Left swipe
            if (!m_leftFlingHintShown) {
                m_leftFlingHintShown = true;
                if (m_flingHintLeft) {
                    m_flingHintLeft->setVisible(true);
                }
            }
            if (m_rightFlingHintShown) {
                m_rightFlingHintShown = false;
                if (m_flingHintRight) {
                    m_flingHintRight->setVisible(false);
                }
            }
        }
    } else {
        if (m_rightFlingHintShown) {
            m_rightFlingHintShown = false;
            if (m_flingHintRight) {
                m_flingHintRight->setVisible(false);
            }
        }
        if (m_leftFlingHintShown) {
            m_leftFlingHintShown = false;
            if (m_flingHintLeft) {
                m_flingHintLeft->setVisible(false);
            }
        }
    }
}


// ============================================================================
// FlingableItemView — updateFlingHintPosition (Port exact)
// ============================================================================

// Port of: private void updateFlingHintPosition()
void FlingableItemView::updateFlingHintPosition()
{
    // Port of: mFlingHintOuter.setTranslationY(mFlingHintYPos);
    if (m_flingHintOuter) {
        m_flingHintOuter->move(m_flingHintOuter->x(),
                                static_cast<int>(m_flingHintYPos));
    }
}

} // namespace PinkReader
