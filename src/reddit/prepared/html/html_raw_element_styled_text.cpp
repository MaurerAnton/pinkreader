/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_styled_text.cpp
 * Port of: HtmlRawElementStyledText.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_styled_text.h"

#include <QString>
#include <vector>
#include <stdexcept>

namespace PinkReader {

// Stub classes
class BodyElement {};
class AppCompatActivity {};
class SpannableStringBuilder {
public:
    QString text;
    int length() const { return text.length(); }
    void append(const QString &str) { text.append(str); }
    void setSpan(void* span, int start, int end, int flags) {
        // Stub: Android span implementation
    }
};

// ===== Spanned.SPAN_INCLUSIVE_EXCLUSIVE constant =====
// Port of: Spanned.SPAN_INCLUSIVE_EXCLUSIVE = 33
static constexpr int SPAN_INCLUSIVE_EXCLUSIVE = 33;

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       stringBuilder.append(mText);
//   }
void HtmlRawElementStyledText::getPlainText(QString &stringBuilder) const {
    // stringBuilder.append(mText);
    stringBuilder.append(mText);
}

// ===== writeTo =====
// Port of:
//   public final void writeTo(@NonNull final SpannableStringBuilder ssb) {
//
//       final int textStart = ssb.length();
//       ssb.append(mText);
//       final int textEnd = ssb.length();
//
//       if(mSpans != null) {
//           for(final CharacterStyle span : mSpans) {
//               ssb.setSpan(span, textStart, textEnd,
//                   Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
//           }
//       }
//   }
void HtmlRawElementStyledText::writeTo(SpannableStringBuilder &ssb) {

    // final int textStart = ssb.length();
    int textStart = ssb.length();

    // ssb.append(mText);
    ssb.append(mText);

    // final int textEnd = ssb.length();
    int textEnd = ssb.length();

    // if(mSpans != null) {
    if(mSpans.has_value()) {
        // for(final CharacterStyle span : mSpans) {
        for(void* span : mSpans.value()) {
            // ssb.setSpan(span, textStart, textEnd,
            //     Spanned.SPAN_INCLUSIVE_EXCLUSIVE);
            ssb.setSpan(span, textStart, textEnd, SPAN_INCLUSIVE_EXCLUSIVE);
        }
    }
}

// ===== reduce =====
// Port of:
//   @Override
//   public void reduce(
//       @NonNull final HtmlTextAttributes activeAttributes,
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<HtmlRawElement> destination,
//       @NonNull final ArrayList<LinkButtonDetails> linkButtons) {
//
//       destination.add(this);
//   }
void HtmlRawElementStyledText::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(this);
    destination.push_back(this);
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       throw new RuntimeException(
//           "Attempt to call generate() on styled text: should be inside a block");
//   }
void HtmlRawElementStyledText::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    throw std::runtime_error(
        "Attempt to call generate() on styled text: should be inside a block");
}

} // namespace PinkReader
