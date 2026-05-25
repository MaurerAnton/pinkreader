// Origin: org.quantumbadger.redreader.views.bezelmenu.SideToolbarOverlay.java
// Ported from RedReader (GPLv3)

#include "views/bezelmenu/side_toolbar_overlay.h"

#include <memory>

namespace PinkReader {

SideToolbarOverlay::SideToolbarOverlay(void* context)
    : m_contents(nullptr)
    , m_shownPosition(nullptr) {
    // super(context);
    (void)context;
}

void SideToolbarOverlay::setContents(void* contents) {
    m_contents = contents;
    if (m_shownPosition != nullptr) {
        show(*m_shownPosition);
    }
}

void SideToolbarOverlay::show(SideToolbarPosition pos) {
    // In original:
    // removeAllViews();
    // addView(contents);
    // final ViewGroup.LayoutParams layoutParams = contents.getLayoutParams();
    // ((LayoutParams)layoutParams).gravity = (pos == SideToolbarPosition.LEFT
    //         ? Gravity.LEFT : Gravity.RIGHT);
    // layoutParams.width = ViewGroup.LayoutParams.WRAP_CONTENT;
    // layoutParams.height = ViewGroup.LayoutParams.MATCH_PARENT;
    // contents.setLayoutParams(layoutParams);
    // shownPosition = pos;

    m_shownPosition = new SideToolbarPosition(pos);
}

void SideToolbarOverlay::hide() {
    // In original:
    // shownPosition = null;
    // removeAllViews();
    delete m_shownPosition;
    m_shownPosition = nullptr;
}

bool SideToolbarOverlay::isShown() const {
    return m_shownPosition != nullptr;
}

} // namespace PinkReader
