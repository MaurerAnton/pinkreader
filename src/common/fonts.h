// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/Fonts.java (lines 1-75)
// Ported from Java to C++

#pragma once

#include <string>

namespace PinkReader {

// Fonts.java line 27: public final class Fonts
class Fonts {
public:
    // line 34: private Fonts() {} - private constructor, all static methods

    // line 36-50: onAppCreate
    // Origin: public static void onAppCreate(@NonNull final AssetManager assetManager)
    // Simplified: stub for non-Android C++ port
    static void onAppCreate();

    // line 52-62: getVeraMonoOrAlternative
    // Origin: @NonNull public static Typeface getVeraMonoOrAlternative()
    // Returns index of VeraMono font (or 0 for MONOSPACE alternative)
    static int getVeraMonoOrAlternative();

    // line 64-74: getRobotoLightOrAlternative
    // Origin: @NonNull public static Typeface getRobotoLightOrAlternative()
    // Returns index of RobotoLight font (or 0 for DEFAULT alternative)
    static int getRobotoLightOrAlternative();

    // Setters for font indices
    static void setVeraMonoFontIndex(int index);
    static void setRobotoLightFontIndex(int index);

private:
    // line 29: private static final String TAG = "Fonts";
    static constexpr const char* TAG = "Fonts";

    // line 31: @NonNull private static final AtomicReference<Typeface> sVeraMono
    // line 32: @NonNull private static final AtomicReference<Typeface> sRobotoLight
    // Simplified: int indices instead of Typeface objects
    static int sVeraMonoIndex;
    static int sRobotoLightIndex;
    static bool sFontsLoaded;
};

} // namespace PinkReader
