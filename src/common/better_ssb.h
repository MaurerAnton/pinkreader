// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/BetterSSB.java (lines 1-223)
// Ported from Java to C++

#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>

namespace PinkReader {

// Forward declarations for span types (Android Spannable equivalents)
struct StyleSpan {
    enum Type { BOLD = 1, ITALIC };
    int mStyle;
    explicit StyleSpan(int style) : mStyle(style) {}
};

struct UnderlineSpan {};
struct StrikethroughSpan {};
struct ForegroundColorSpan {
    int mColor;
    explicit ForegroundColorSpan(int c) : mColor(c) {}
};
struct BackgroundColorSpan {
    int mColor;
    explicit BackgroundColorSpan(int c) : mColor(c) {}
};
struct RelativeSizeSpan {
    float mScale;
    explicit RelativeSizeSpan(float s) : mScale(s) {}
};
struct SuperscriptSpan {};
struct URLSpan {
    std::string mURL;
    explicit URLSpan(std::string url) : mURL(std::move(url)) {}
};

// Span variant covering all span types
using SpanVariant = std::variant<
    StyleSpan,
    UnderlineSpan,
    StrikethroughSpan,
    ForegroundColorSpan,
    BackgroundColorSpan,
    RelativeSizeSpan,
    SuperscriptSpan,
    URLSpan
>;

// Span entry with position tracking
struct SpanEntry {
    SpanVariant span;
    int start;
    int end;
    int flags; // Spanned.SPAN_INCLUSIVE_EXCLUSIVE = 33
};

// BetterSSB.java line 38: public class BetterSSB extends Observable
class BetterSSB {
public:
    // line 42-49: Flag constants
    static constexpr int BOLD = 1;
    static constexpr int ITALIC = 1 << 1;
    static constexpr int UNDERLINE = 1 << 2;
    static constexpr int STRIKETHROUGH = 1 << 3;
    static constexpr int FOREGROUND_COLOR = 1 << 4;
    static constexpr int BACKGROUND_COLOR = 1 << 5;
    static constexpr int SIZE = 1 << 6;
    static constexpr int SUPERSCRIPT = 1 << 7;

    // line 51: public static final char NBSP = '\u00A0';
    static constexpr char16_t NBSP = u'\u00A0';

    // line 53-55: constructor
    BetterSSB();

    // line 57-59: append(String, int)
    void append(const std::string& str, int flags);

    // line 61-63: append(String, int, String)
    void append(const std::string& str, int flags, const std::string& url);

    // line 65-72: append(String, int, int, int, float)
    void append(const std::string& str, int flags,
                int foregroundCol, int backgroundCol, float scale);

    // line 74-157: append(String, int, int, int, float, String)
    void append(const std::string& str, int flags,
                int foregroundCol, int backgroundCol, float scale,
                const std::string& url);

    // line 159-181: linkify()
    void linkify();

    // line 183-187: append(CharSequence)
    void appendText(const std::string& text);

    // line 189-193: replace(int, int, CharSequence)
    void replace(int start, int end, const std::string& text);

    // line 195-207: replace(CharSequence, Object)
    void replaceSpan(const std::string& textToBeReplaced, const SpanVariant& replacement);

    // line 209-211: isEmpty()
    bool isEmpty() const;

    // line 213-218: setSpan(Object, int, int, int)
    void setSpan(const SpanVariant& what, int start, int end, int flags);

    // line 220-222: get()
    const std::string& get() const;
    std::string& get();

    // Observable pattern (setChanged + notifyObservers)
    void setChanged();
    void notifyObservers();

private:
    static constexpr int SPAN_INCLUSIVE_EXCLUSIVE = 33;

    // line 40: private final SpannableStringBuilder sb;
    std::string mStringBuilder;
    std::vector<SpanEntry> mSpans;
    bool mChanged = false;
};

} // namespace PinkReader
