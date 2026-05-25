// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/BetterSSB.java (lines 1-223)
// Implementation

#include "better_ssb.h"

namespace PinkReader {

// line 53-55: constructor
BetterSSB::BetterSSB() {
    // line 54: this.sb = new SpannableStringBuilder();
    mStringBuilder.clear();
}

// line 57-59: append(String, int)
void BetterSSB::append(const std::string& str, int flags) {
    // line 58: append(str, flags, 0, 0, 1f);
    append(str, flags, 0, 0, 1.0f);
}

// line 61-63: append(String, int, String)
void BetterSSB::append(const std::string& str, int flags, const std::string& url) {
    // line 62: append(str, flags, 0, 0, 1f, url);
    append(str, flags, 0, 0, 1.0f, url);
}

// line 65-72: append(String, int, int, int, float)
void BetterSSB::append(const std::string& str, int flags,
                       int foregroundCol, int backgroundCol, float scale) {
    // line 71: append(str, flags, foregroundCol, backgroundCol, scale, null);
    append(str, flags, foregroundCol, backgroundCol, scale, "");
}

// line 74-157: append(String, int, int, int, float, String)
void BetterSSB::append(const std::string& str, int flags,
                        int foregroundCol, int backgroundCol, float scale,
                        const std::string& url) {
    // line 82: final int strStart = sb.length();
    int strStart = static_cast<int>(mStringBuilder.size());
    // line 83: sb.append(str);
    mStringBuilder += str;
    // line 84: final int strEnd = sb.length();
    int strEnd = static_cast<int>(mStringBuilder.size());

    // line 86-92: if((flags & BOLD) != 0)
    if ((flags & BOLD) != 0) {
        // line 87-91: sb.setSpan(new StyleSpan(Typeface.BOLD), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({StyleSpan(StyleSpan::BOLD), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 94-100: if((flags & ITALIC) != 0)
    if ((flags & ITALIC) != 0) {
        // line 95-99: sb.setSpan(new StyleSpan(Typeface.ITALIC), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({StyleSpan(StyleSpan::ITALIC), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 102-108: if((flags & UNDERLINE) != 0)
    if ((flags & UNDERLINE) != 0) {
        // line 103-107: sb.setSpan(new UnderlineSpan(), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({UnderlineSpan{}, strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 110-116: if((flags & STRIKETHROUGH) != 0)
    if ((flags & STRIKETHROUGH) != 0) {
        // line 111-115: sb.setSpan(new StrikethroughSpan(), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({StrikethroughSpan{}, strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 118-124: if((flags & FOREGROUND_COLOR) != 0)
    if ((flags & FOREGROUND_COLOR) != 0) {
        // line 119-123: sb.setSpan(new ForegroundColorSpan(foregroundCol), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({ForegroundColorSpan(foregroundCol), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 126-132: if((flags & BACKGROUND_COLOR) != 0)
    if ((flags & BACKGROUND_COLOR) != 0) {
        // line 127-131: sb.setSpan(new BackgroundColorSpan(backgroundCol), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({BackgroundColorSpan(backgroundCol), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 134-140: if((flags & SIZE) != 0)
    if ((flags & SIZE) != 0) {
        // line 135-139: sb.setSpan(new RelativeSizeSpan(scale), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({RelativeSizeSpan(scale), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 142-148: if((flags & SUPERSCRIPT) != 0)
    if ((flags & SUPERSCRIPT) != 0) {
        // line 143-147: sb.setSpan(new SuperscriptSpan(), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({SuperscriptSpan{}, strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }

    // line 150-156: if(url != null)
    if (!url.empty()) {
        // line 151-155: sb.setSpan(new URLSpan(url), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE)
        mSpans.push_back({URLSpan(url), strStart, strEnd, SPAN_INCLUSIVE_EXCLUSIVE});
    }
}

// line 159-181: linkify()
void BetterSSB::linkify() {
    // line 161: final String asText = sb.toString();
    // line 162: final HashSet<UriString> links = LinkHandler.computeAllLinks(asText);
    // Simplified: link scanning is platform-specific
    // Origin loops through links and adds URLSpan for each match
    // Stub for non-Android C++ port
}

// line 183-187: append(CharSequence)
void BetterSSB::appendText(const std::string& text) {
    // line 184: this.sb.append(text);
    mStringBuilder += text;
    // line 185-186: this.setChanged(); this.notifyObservers(this.sb);
    setChanged();
    notifyObservers();
}

// line 189-193: replace(int, int, CharSequence)
void BetterSSB::replace(int start, int end, const std::string& text) {
    // line 190: this.sb.replace(start, end, text);
    if (start >= 0 && end <= static_cast<int>(mStringBuilder.size()) && start <= end) {
        mStringBuilder.replace(start, end - start, text);
    }
    // line 191-192: this.setChanged(); this.notifyObservers(this.sb);
    setChanged();
    notifyObservers();
}

// line 195-207: replace(CharSequence, Object)
void BetterSSB::replaceSpan(const std::string& textToBeReplaced, const SpanVariant& replacement) {
    // line 198: final int textStartIndex = TextUtils.indexOf(this.sb, textToBeReplaced);
    size_t pos = mStringBuilder.find(textToBeReplaced);
    if (pos == std::string::npos) return;
    int textStartIndex = static_cast<int>(pos);

    // line 200-203: this.sb.setSpan(replacement, textStartIndex, textStartIndex + textToBeReplaced.length(), SPAN_INCLUSIVE_EXCLUSIVE);
    mSpans.push_back({replacement, textStartIndex,
                      textStartIndex + static_cast<int>(textToBeReplaced.size()),
                      SPAN_INCLUSIVE_EXCLUSIVE});

    // line 205-206: this.setChanged(); this.notifyObservers(this.sb);
    setChanged();
    notifyObservers();
}

// line 209-211: isEmpty()
bool BetterSSB::isEmpty() const {
    // line 210: return this.sb.length() == 0;
    return mStringBuilder.empty();
}

// line 213-218: setSpan(Object, int, int, int)
void BetterSSB::setSpan(const SpanVariant& what, int start, int end, int flags) {
    // line 214: this.sb.setSpan(what, start, end, flag);
    mSpans.push_back({what, start, end, flags});

    // line 216-217: this.setChanged(); this.notifyObservers(this.sb);
    setChanged();
    notifyObservers();
}

// line 220-222: get()
const std::string& BetterSSB::get() const {
    // line 221: return sb;
    return mStringBuilder;
}

std::string& BetterSSB::get() {
    return mStringBuilder;
}

void BetterSSB::setChanged() {
    mChanged = true;
}

void BetterSSB::notifyObservers() {
    // Origin: extends Observable, calls Observable.notifyObservers(sb)
    // In C++ port, observers are handled externally
    mChanged = false;
}

} // namespace PinkReader
