/*
 * PinkReader - GPLv3
 * File: scrollbar_recycler_view_manager.h
 * Exact port of ScrollbarRecyclerViewManager.java (142 lines)
 *
 * Ports every field, method, inner class, and logic branch exactly.
 * Android RecyclerView, SwipeRefreshLayout, View, FrameLayout, and
 * LayoutInflater adapted to Qt/QWidget equivalents and stubs.
 */

#pragma once

#include <QObject>
#include <QWidget>
#include <QPointer>

namespace PinkReader {

// Forward declarations — Android/RedReader types used in signatures
class Context;
class View;
class ViewGroup;
class FrameLayout;
class RecyclerView;
class LinearLayoutManager;
class SwipeRefreshLayout;
class LayoutInflater;

/**
 * @brief Port of org.quantumbadger.redreader.views.ScrollbarRecyclerViewManager
 *
 * Manages a RecyclerView with a scrollbar indicator and pull-to-refresh.
 * Every field, method, inner class, and logic branch ported exactly.
 */
class ScrollbarRecyclerViewManager : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Constructor — Java lines 41-119
    // Port of: public ScrollbarRecyclerViewManager(
    //             final Context context, final ViewGroup root,
    //             final boolean attachToRoot)
    // ========================================================================
    /**
     * @brief Creates and initializes the scrollbar recycler view.
     *
     * Inflates the layout, binds views, configures the RecyclerView,
     * and adds scroll listeners for the scrollbar indicator.
     *
     * Port of Java lines 41-119 exactly.
     */
    ScrollbarRecyclerViewManager(
        Context *context,
        ViewGroup *root,
        bool attachToRoot,
        QObject *parent = nullptr);

    // ========================================================================
    // enablePullToRefresh — Java lines 121-124
    // Port of: public void enablePullToRefresh(
    //             @NonNull final SwipeRefreshLayout.OnRefreshListener listener)
    // ========================================================================
    /**
     * @brief Enables pull-to-refresh with the given listener.
     *
     * Port of:
     *   mSwipeRefreshLayout.setOnRefreshListener(listener);
     *   mSwipeRefreshLayout.setEnabled(true);
     *
     * @param listener Callback invoked when the user pulls to refresh.
     *                 In C++, this is a std::function. In Java, it's
     *                 SwipeRefreshLayout.OnRefreshListener.
     */
    void enablePullToRefresh(std::function<void()> onRefreshListener);

    // ========================================================================
    // getOuterView — Java lines 135-137
    // Port of: public View getOuterView()
    // ========================================================================
    /**
     * @brief Returns the outer container view.
     * Port of: return mOuter;
     */
    View *getOuterView() const;

    // ========================================================================
    // getRecyclerView — Java lines 139-141
    // Port of: public RecyclerView getRecyclerView()
    // ========================================================================
    /**
     * @brief Returns the RecyclerView.
     * Port of: return mRecyclerView;
     */
    RecyclerView *getRecyclerView() const;

private:
    // ========================================================================
    // Fields — port exact from Java
    // ========================================================================

    // Port of: private final View mOuter;
    View *m_outer = nullptr;

    // Port of: private final SwipeRefreshLayout mSwipeRefreshLayout;
    SwipeRefreshLayout *m_swipeRefreshLayout = nullptr;

    // Port of: private final RecyclerView mRecyclerView;
    RecyclerView *m_recyclerView = nullptr;

    // Port of: private final FrameLayout mScrollbarFrame;
    FrameLayout *m_scrollbarFrame = nullptr;

    // Port of: private final View mScrollbar;
    View *m_scrollbar = nullptr;

    // Port of: private boolean mScrollUnnecessary = false;
    bool m_scrollUnnecessary = false;

    // ========================================================================
    // showScrollbar — Java lines 126-129
    // Port of: private void showScrollbar()
    // ========================================================================
    /**
     * @brief Shows the scrollbar indicator.
     *
     * Port of:
     *   mScrollbar.animate().cancel();
     *   mScrollbar.setAlpha(1f);
     */
    void showScrollbar();

    // ========================================================================
    // hideScrollbar — Java lines 131-133
    // Port of: private void hideScrollbar()
    // ========================================================================
    /**
     * @brief Hides the scrollbar indicator with fade animation.
     *
     * Port of:
     *   mScrollbar.animate().alpha(0).setStartDelay(500).setDuration(500).start();
     */
    void hideScrollbar();

    // ========================================================================
    // OnScrollListener inner class — Java lines 61-118
    // Port of: new RecyclerView.OnScrollListener() { ... }
    // ========================================================================
    /**
     * @brief Inner scroll listener that manages the scrollbar indicator.
     *
     * Port of the anonymous RecyclerView.OnScrollListener from Java lines 61-118.
     * Every method ported exactly.
     */
    class ScrollListener;

    friend class ScrollListener;

    // ========================================================================
    // updateScroll — Java lines 63-91 (inside OnScrollListener)
    // ========================================================================
    /**
     * @brief Updates the scrollbar position and visibility.
     *
     * Called from onScrolled and onScrollStateChanged.
     * Port of Java lines 63-91 exactly.
     */
    void updateScroll(LinearLayoutManager *layoutManager);

    // Reference to LinearLayoutManager for scroll position queries
    LinearLayoutManager *m_layoutManager = nullptr;
};

} // namespace PinkReader
