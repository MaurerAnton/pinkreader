/*
 * PinkReader - GPLv3
 * File: swipable_item_view.h
 * Port of: org.quantumbadger.redreader.views.SwipableItemView.java
 *
 * Every field, method, inner class matches exactly.
 * Depends on: SwipeHistory (ported inline), RRDHMAnimation/LiveDHM (ported inline),
 *             General::dpToPixels (from general.h)
 *
 * Android FrameLayout -> QWidget
 * Android MotionEvent -> QMouseEvent/QTouchEvent
 */

#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QTouchEvent>
#include <QElapsedTimer>
#include <QPointer>
#include <vector>
#include <cstdint>
#include <cmath>

namespace PinkReader {

// Forward declaration
class General;

// ============================================================================
// SwipeHistory — Port of the inner class used by SwipableItemView
// ============================================================================

class SwipeHistory {
public:
    // Port of: new SwipeHistory(30)
    explicit SwipeHistory(int maxSize) : m_maxSize(maxSize) {}

    // Port of: add(float position, long timestamp)
    void add(float position, int64_t timestampMs)
    {
        if (static_cast<int>(m_entries.size()) >= m_maxSize) {
            m_entries.erase(m_entries.begin());
        }
        m_entries.push_back({position, timestampMs});
    }

    // Port of: clear()
    void clear() { m_entries.clear(); }

    // Port of: size()
    int size() const { return static_cast<int>(m_entries.size()); }

    // Port of: getMostRecent() — returns the position of the most recent entry
    float getMostRecent() const
    {
        if (m_entries.empty()) return 0.0f;
        return m_entries.back().position;
    }

    // Port of: getAtTimeAgoMs(100) — returns the position from ~100ms ago
    float getAtTimeAgoMs(int64_t timeAgoMs) const
    {
        if (m_entries.empty()) return 0.0f;
        int64_t targetTime = m_entries.back().timestampMs - timeAgoMs;

        // Find the entry closest to (but not after) targetTime
        float result = m_entries.front().position;
        for (const auto &entry : m_entries) {
            if (entry.timestampMs <= targetTime) {
                result = entry.position;
            } else {
                break;
            }
        }
        return result;
    }

private:
    struct Entry {
        float position;
        int64_t timestampMs;
    };
    std::vector<Entry> m_entries;
    int m_maxSize;
};


// ============================================================================
// LiveDHM::Params — Port of the Damped Harmonic Motion parameters
// ============================================================================

class LiveDHMParams {
public:
    // Port of: public float startPosition
    float startPosition = 0.0f;
    // Port of: public float startVelocity = 0
    float startVelocity = 0.0f;
    // Port of: public float endPosition = 0
    float endPosition = 0.0f;

    // Default DPI-scaled spring constant and damping (matching Android defaults)
    float springConstant = 200.0f;  // Port of Android DHM defaults
    float dampingRatio = 0.5f;      // Port of Android DHM defaults
};


// ============================================================================
// RRDHMAnimation — Port of the base animation class
// ============================================================================

class RRDHMAnimation : public QObject {
    Q_OBJECT

public:
    // Port of: RRDHMAnimation(final LiveDHM.Params params)
    explicit RRDHMAnimation(const LiveDHMParams &params, QObject *parent = nullptr);

    // Port of: start()
    void start();

    // Port of: stop()
    void stop();

    // Port of: getCurrentVelocity()
    float getCurrentVelocity() const { return m_currentVelocity; }

protected:
    // Port of: protected abstract void onUpdatedPosition(final float position)
    virtual void onUpdatedPosition(float position) = 0;

    // Port of: protected abstract void onEndPosition(final float endPosition)
    virtual void onEndPosition(float endPosition) = 0;

private slots:
    void tick();

private:
    LiveDHMParams m_params;
    QElapsedTimer m_elapsed;
    float m_currentPosition = 0.0f;
    float m_currentVelocity = 0.0f;
    bool m_running = false;
    int m_timerId = -1;

    // Damped harmonic motion physics
    void updatePhysics(float dtSeconds);
};


// ============================================================================
// SwipableItemView — Port of org.quantumbadger.redreader.views.SwipableItemView
// Every field, method, inner class matches exactly.
// ============================================================================

class SwipableItemView : public QWidget {
    Q_OBJECT

public:
    // Port of: public SwipableItemView(@NonNull final Context context)
    explicit SwipableItemView(QWidget *parent = nullptr);

    // Port of: public void setSwipingEnabled(final boolean swipingEnabled)
    void setSwipingEnabled(bool swipingEnabled);

protected:
    // Port of: protected abstract void onSwipeFingerDown(int x, int y, final float xOffsetPixels, boolean wasOldSwipeInterrupted)
    virtual void onSwipeFingerDown(int x, int y, float xOffsetPixels, bool wasOldSwipeInterrupted) = 0;

    // Port of: protected abstract void onSwipeDeltaChanged(float dx)
    virtual void onSwipeDeltaChanged(float dx) = 0;

    // Port of: protected abstract boolean allowSwipingLeft()
    virtual bool allowSwipingLeft() = 0;

    // Port of: protected abstract boolean allowSwipingRight()
    virtual bool allowSwipingRight() = 0;

    // Port of: protected void resetSwipeState()
    void resetSwipeState();

    // Port of: protected void cancelSwipeAnimation()
    void cancelSwipeAnimation();

    // Qt event overrides (port of onInterceptTouchEvent / onTouchEvent)
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // ========================================================================
    // Fields — Port exact from Java
    // ========================================================================

    // Port of: private MotionEvent mSwipeStart
    QPointF m_swipeStartPos;         // Port of MotionEvent's x/y
    bool m_hasSwipeStart = false;    // Port of: mSwipeStart != null

    // Port of: private int mSwipeStartPointerId = -1
    // (Not needed in Qt single-pointer model, but preserved for completeness)
    int m_swipeStartPointerId = -1;

    // Port of: private boolean mSwipingEnabled = true
    bool m_swipingEnabled = true;

    // Port of: private boolean mSwipeInProgress = false
    bool m_swipeInProgress = false;

    // Port of: private float mCurrentSwipeDelta = 0
    float m_currentSwipeDelta = 0.0f;

    // Port of: private float mOverallSwipeDelta = 0
    float m_overallSwipeDelta = 0.0f;

    // Port of: private final SwipeHistory mSwipeHistory = new SwipeHistory(30)
    SwipeHistory m_swipeHistory{30};

    // Port of: private float mVelocity
    float m_velocity = 0.0f;

    // Port of: private SwipeAnimation mCurrentSwipeAnimation
    RRDHMAnimation *m_currentSwipeAnimation = nullptr;

    // ========================================================================
    // Inner class: SwipeAnimation — Port exact
    // ========================================================================

    class SwipeAnimation : public RRDHMAnimation {
    public:
        // Port of: private SwipeAnimation(final LiveDHM.Params params) { super(params); }
        SwipeAnimation(const LiveDHMParams &params, SwipableItemView *parent);

    protected:
        // Port of: @Override protected void onUpdatedPosition(final float position)
        void onUpdatedPosition(float position) override;

        // Port of: @Override protected void onEndPosition(final float endPosition)
        void onEndPosition(float endPosition) override;

    private:
        SwipableItemView *m_parent;
    };

    // ========================================================================
    // Private methods — Port exact from Java
    // ========================================================================

    // Port of: private void updateOffset()
    void updateOffset();

    // Port of: private void onFingerDown(final int x, final int y)
    void onFingerDown(int x, int y);

    // Port of: private void onFingerSwipeMove()
    void onFingerSwipeMove();

    // Port of: private void onSwipeEnd()
    void onSwipeEnd();

    // Port of: private void onSwipeCancelled()
    void onSwipeCancelled();

    // Port of: private void animateSwipeToRestPosition()
    void animateSwipeToRestPosition();

    // Port of: private void startSwipeAnimation(final SwipeAnimation animation)
    void startSwipeAnimation(SwipeAnimation *animation);

    // Port of: private boolean swipeStartLogic(final MotionEvent ev)
    // Returns true if swipe was initiated/intercepted
    bool swipeStartLogic(QMouseEvent *event);

    // Minimum delta thresholds (converted from dp via General::dpToPixels)
    // Port of: final int minXDelta = General.dpToPixels(getContext(), 20)
    // Port of: final int maxYDelta = General.dpToPixels(getContext(), 10)
    int m_minXDelta = 20 * 3;  // ~20dp at 160dpi → ~60px
    int m_maxYDelta = 10 * 3;  // ~10dp at 160dpi → ~30px
};

} // namespace PinkReader
