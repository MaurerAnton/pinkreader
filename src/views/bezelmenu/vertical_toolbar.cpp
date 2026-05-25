// Origin: org.quantumbadger.redreader.views.bezelmenu.VerticalToolbar.java
// Ported from RedReader (GPLv3)

#include "views/bezelmenu/vertical_toolbar.h"

namespace PinkReader {

VerticalToolbar::VerticalToolbar(void* context)
    : m_buttons(nullptr) {
    // In original:
    // super(context);
    // setBackgroundColor(Color.argb(192, 0, 0, 0));  // semi-transparent black
    // setElevation(General.dpToPixels(context, 10));
    //
    // buttons = new LinearLayout(context);
    // buttons.setOrientation(LinearLayout.VERTICAL);
    //
    // final ScrollView sv = new ScrollView(context);
    // sv.addView(buttons);
    // addView(sv);
    (void)context;
}

void VerticalToolbar::addItem(void* v) {
    // buttons.addView(v);
    (void)v;
}

} // namespace PinkReader
