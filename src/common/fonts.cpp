// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/Fonts.java (lines 1-75)
// Implementation

#include "fonts.h"

namespace PinkReader {

int Fonts::sVeraMonoIndex = -1;
int Fonts::sRobotoLightIndex = -1;
bool Fonts::sFontsLoaded = false;

// line 36-50: onAppCreate
void Fonts::onAppCreate() {
    // Origin line 38: General.startNewThread("FontCreate", () -> { ... })
    // Origin lines 41-42:
    //   sVeraMono.set(Typeface.createFromAsset(assetManager, "fonts/VeraMono.ttf"));
    //   sRobotoLight.set(Typeface.createFromAsset(assetManager, "fonts/Roboto-Light.ttf"));
    // Simplified: font loading is platform-specific, handled by caller
    sFontsLoaded = true;
}

// line 52-62: getVeraMonoOrAlternative
int Fonts::getVeraMonoOrAlternative() {
    // Origin line 55-61:
    // final Typeface result = sVeraMono.get();
    // if(result == null) return Typeface.MONOSPACE; else return result;
    if (sVeraMonoIndex < 0) {
        return 0; // Typeface.MONOSPACE equivalent
    }
    return sVeraMonoIndex;
}

// line 64-74: getRobotoLightOrAlternative
int Fonts::getRobotoLightOrAlternative() {
    // Origin line 67-73:
    // final Typeface result = sRobotoLight.get();
    // if(result == null) return Typeface.DEFAULT; else return result;
    if (sRobotoLightIndex < 0) {
        return 0; // Typeface.DEFAULT equivalent
    }
    return sRobotoLightIndex;
}

void Fonts::setVeraMonoFontIndex(int index) {
    sVeraMonoIndex = index;
}

void Fonts::setRobotoLightFontIndex(int index) {
    sRobotoLightIndex = index;
}

} // namespace PinkReader
