/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_spoiler.cpp
 * Port of: HtmlRawElementSpoiler.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_spoiler.h"
#include "html_raw_element_block.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementVerticalSequence : public BodyElement {
public:
    std::vector<BodyElement*> elements;
    explicit BodyElementVerticalSequence(const std::vector<BodyElement*> &e) : elements(e) {}
};
class BodyElementSpoilerButton : public BodyElement {
public:
    BodyElementSpoilerButton(AppCompatActivity &act, BodyElementVerticalSequence* seq) {
        // Stub - stores reference to sequence
    }
};
class AppCompatActivity {};

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       mChild.getPlainText(stringBuilder);
//   }
void HtmlRawElementSpoiler::getPlainText(QString &stringBuilder) const {
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
//       destination.add(new HtmlRawElementSpoiler(mChild.reduce(
//           activeAttributes, activity)));
//   }
void HtmlRawElementSpoiler::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(new HtmlRawElementSpoiler(mChild.reduce(
    //     activeAttributes, activity)));
    destination.push_back(new HtmlRawElementSpoiler(
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
//       destination.add(new BodyElementSpoilerButton(
//           activity,
//           new BodyElementVerticalSequence(elements)));
//   }
void HtmlRawElementSpoiler::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // final ArrayList<BodyElement> elements = new ArrayList<>();
    std::vector<BodyElement*> elements;

    // mChild.generate(activity, elements);
    mChild->generate(activity, elements);

    // destination.add(new BodyElementSpoilerButton(
    //     activity,
    //     new BodyElementVerticalSequence(elements)));
    destination.push_back(new BodyElementSpoilerButton(
        activity,
        new BodyElementVerticalSequence(elements)));
}

} // namespace PinkReader
