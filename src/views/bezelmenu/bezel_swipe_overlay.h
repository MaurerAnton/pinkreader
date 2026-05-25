// Origin: org.quantumbadger.redreader.views.bezelmenu.BezelSwipeOverlay.java
// Ported from RedReader (GPLv3)
#pragma once

#include <cstdint>

namespace PinkReader {

class BezelSwipeOverlay {
public:
    // --- SwipeEdge constants ---
    static constexpr int LEFT = 0;
    static constexpr int RIGHT = 1;

    // --- BezelSwipeListener interface ---
    struct BezelSwipeListener {
        virtual ~BezelSwipeListener() = default;
        virtual bool onSwipe(int edge) = 0; // @SwipeEdge
        virtual bool onTap() = 0;
    };

    // --- Constructor ---
    BezelSwipeOverlay(void* context, BezelSwipeListener* listener);

    // --- Touch event handling ---
    bool onTouchEvent(void* event); // MotionEvent*

private:
    BezelSwipeListener* m_listener;
    int m_swipeZonePixels;
};

} // namespace PinkReader
