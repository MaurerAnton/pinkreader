// Origin: org.quantumbadger.redreader.views.bezelmenu.BezelSwipeOverlay.java
// Ported from RedReader (GPLv3)

#include "views/bezelmenu/bezel_swipe_overlay.h"

namespace PinkReader {

BezelSwipeOverlay::BezelSwipeOverlay(void* context, BezelSwipeListener* listener)
    : m_listener(listener)
    , m_swipeZonePixels(0) {
    // In original:
    // super(context);
    // this.listener = listener;
    // final int swipeZoneDp = PrefsUtility.pref_behaviour_bezel_toolbar_swipezone_dp();
    // mSwipeZonePixels = General.dpToPixels(getContext(), swipeZoneDp);
    (void)context;
}

bool BezelSwipeOverlay::onTouchEvent(void* event) {
    // In original:
    // final int action = event.getAction() & MotionEvent.ACTION_MASK;
    // if(action == MotionEvent.ACTION_DOWN) {
    //     if(event.getX() < mSwipeZonePixels) {
    //         return listener.onSwipe(LEFT);
    //     } else if(event.getX() > getWidth() - mSwipeZonePixels) {
    //         return listener.onSwipe(RIGHT);
    //     } else {
    //         return listener.onTap();
    //     }
    // }
    // return false;

    // event->getAction() needs to check for ACTION_DOWN
    (void)event;

    if (m_listener != nullptr) {
        return m_listener->onTap();
    }
    return false;
}

} // namespace PinkReader
