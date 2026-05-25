/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_link_button.cpp
 * Port of: HtmlRawElementLinkButton.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_link_button.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementLinkButton : public BodyElement {
public:
    HtmlRawElement::LinkButtonDetails details;
    explicit BodyElementLinkButton(const HtmlRawElement::LinkButtonDetails &d) : details(d) {}
};
class AppCompatActivity {};

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       // Nothing to do
//   }
void HtmlRawElementLinkButton::getPlainText(QString &stringBuilder) const {
    // Nothing to do
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
void HtmlRawElementLinkButton::reduce(
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
//       destination.add(new BodyElementLinkButton(mDetails));
//   }
void HtmlRawElementLinkButton::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // destination.add(new BodyElementLinkButton(mDetails));
    destination.push_back(new BodyElementLinkButton(mDetails));
}

} // namespace PinkReader
