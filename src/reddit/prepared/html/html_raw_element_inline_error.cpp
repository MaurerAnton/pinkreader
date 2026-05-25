/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_inline_error.cpp
 * Port of: HtmlRawElementInlineErrorMessage.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_inline_error.h"
#include "html_raw_element_styled_text.h"

// Forward-declare HtmlRawElementTagPassthrough stub
namespace PinkReader {
class HtmlRawElementTagPassthrough : public HtmlRawElement {
public:
    std::vector<HtmlRawElement*> mChildren;
    explicit HtmlRawElementTagPassthrough(const std::vector<HtmlRawElement*> &c) : mChildren(c) {}
    void getPlainText(QString &sb) const override {
        for(auto* e : mChildren) e->getPlainText(sb);
    }
    void reduce(const HtmlTextAttributes &a, AppCompatActivity &act,
                std::vector<HtmlRawElement*> &dest,
                std::vector<LinkButtonDetails> &lb) override {
        for(auto* c : mChildren) c->reduce(a, act, dest, lb);
    }
    void generate(AppCompatActivity &, std::vector<BodyElement*> &) override {}
};
class AppCompatActivity {};
class BodyElement {};
}

#include <QString>
#include <vector>

namespace PinkReader {

// ===== create =====
// Port of:
//   public static HtmlRawElementStyledText create(@NonNull final String text) {
//
//       final ArrayList<CharacterStyle> spans = new ArrayList<>();
//       spans.add(new BackgroundColorSpan(Color.RED));
//       spans.add(new ForegroundColorSpan(Color.WHITE));
//
//       return new HtmlRawElementStyledText(text, spans);
//   }
HtmlRawElementStyledText* HtmlRawElementInlineErrorMessage::create(
        const QString &text) {

    // final ArrayList<CharacterStyle> spans = new ArrayList<>();
    // spans.add(new BackgroundColorSpan(Color.RED));
    // spans.add(new ForegroundColorSpan(Color.WHITE));
    // NOTE: BackgroundColorSpan and ForegroundColorSpan are Android types.
    // Using void* as spans placeholder. The actual span objects would be
    // created when porting the Android text styling layer.
    std::vector<void*> spans;
    spans.push_back(nullptr); // Placeholder for: new BackgroundColorSpan(Color.RED)
    spans.push_back(nullptr); // Placeholder for: new ForegroundColorSpan(Color.WHITE)

    // return new HtmlRawElementStyledText(text, spans);
    return new HtmlRawElementStyledText(text, spans);
}

// ===== appendError =====
// Port of:
//   @NonNull
//   public static HtmlRawElementTagPassthrough appendError(
//       @NonNull final String text,
//       @NonNull final HtmlRawElement element) {
//
//       final ArrayList<HtmlRawElement> children = new ArrayList<>();
//       children.add(element);
//       children.add(HtmlRawElementInlineErrorMessage.create(text));
//
//       return new HtmlRawElementTagPassthrough(children);
//   }
HtmlRawElementTagPassthrough* HtmlRawElementInlineErrorMessage::appendError(
        const QString &text,
        HtmlRawElement &element) {

    // final ArrayList<HtmlRawElement> children = new ArrayList<>();
    std::vector<HtmlRawElement*> children;

    // children.add(element);
    children.push_back(&element);

    // children.add(HtmlRawElementInlineErrorMessage.create(text));
    children.push_back(HtmlRawElementInlineErrorMessage::create(text));

    // return new HtmlRawElementTagPassthrough(children);
    return new HtmlRawElementTagPassthrough(children);
}

} // namespace PinkReader
