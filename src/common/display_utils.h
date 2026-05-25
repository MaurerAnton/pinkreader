// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/DisplayUtils.java (lines 1-40)
// Ported from Java to C++

#pragma once

namespace PinkReader {

// Origin: android.graphics.Rect equivalent
struct Rect {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;
};

// DisplayUtils.java line 25: public final class DisplayUtils
class DisplayUtils {
public:
    // line 29: private DisplayUtils() {} - private constructor, all static methods

    // line 31-39: getWindowVisibleDisplayFrame
    static Rect getWindowVisibleDisplayFrame();

    static void setWindowVisibleDisplayFrame(const Rect& frame);

private:
    // line 27: private static final Rect sWindowVisibleDisplayFrame = new Rect();
    static Rect sWindowVisibleDisplayFrame;
};

} // namespace PinkReader
