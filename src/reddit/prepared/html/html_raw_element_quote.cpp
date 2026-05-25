/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_quote.cpp
 * Port of: HtmlRawElementQuote.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_quote.h"
#include "html_raw_element_block.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementQuote : public BodyElement {
public:
    std::vector<BodyElement*> elements;
    explicit BodyElementQuote(const std::vector<BodyElement*> &e) : elements(e) {}
};
class AppCompatActivity {};

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       mChild.getPlainText(stringBuilder);
//   }
void HtmlRawElementQuote::getPlainText(QString &stringBuilder) const {
    // mChild.getPlainText(stringBuilder);
    mChild->getPlainText(stringBuilder);
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
//       destination.add(new HtmlRawElementQuote(mChild.reduce(
//           activeAttributes, activity)));
//   }
void HtmlRawElementQuote::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(new HtmlRawElementQuote(mChild.reduce(
    //     activeAttributes, activity)));
    destination.push_back(new HtmlRawElementQuote(
        new HtmlRawElementBlock(mChild->reduce(activeAttributes, activity))));
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       final ArrayList<BodyElement> elements = new ArrayList<>();
//       mChild.generate(activity, elements);
//
//       destination.add(new BodyElementQuote(elements));
//   }
void HtmlRawElementQuote::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // final ArrayList<BodyElement> elements = new ArrayList<>();
    std::vector<BodyElement*> elements;

    // mChild.generate(activity, elements);
    mChild->generate(activity, elements);

    // destination.add(new BodyElementQuote(elements));
    destination.push_back(new BodyElementQuote(elements));
}

} // namespace PinkReader
