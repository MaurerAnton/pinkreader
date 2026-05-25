// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/DisplayUtils.java (lines 1-40)
// Implementation

#include "display_utils.h"

namespace PinkReader {

Rect DisplayUtils::sWindowVisibleDisplayFrame;

// line 31-39: getWindowVisibleDisplayFrame
Rect DisplayUtils::getWindowVisibleDisplayFrame() {
    return sWindowVisibleDisplayFrame;
}

void DisplayUtils::setWindowVisibleDisplayFrame(const Rect& frame) {
    sWindowVisibleDisplayFrame = frame;
}

} // namespace PinkReader
