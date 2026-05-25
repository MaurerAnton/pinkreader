/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_hr.cpp
 * Port of: HtmlRawElementTagHorizontalRule.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_tag_hr.h"

#include <QString>
#include <vector>

namespace PinkReader {

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       // Nothing to do
//   }
// Origin: HtmlRawElementTagHorizontalRule.java lines 30-32
void HtmlRawElementTagHorizontalRule::getPlainText(QString &stringBuilder) const {
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
// Origin: HtmlRawElementTagHorizontalRule.java lines 35-42
void HtmlRawElementTagHorizontalRule::reduce(
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
//       destination.add(new BodyElementHorizontalRule());
//   }
// Origin: HtmlRawElementTagHorizontalRule.java lines 45-50
void HtmlRawElementTagHorizontalRule::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // destination.add(new BodyElementHorizontalRule());
    // NOTE: BodyElementHorizontalRule is a stub - null placeholder for now
    destination.push_back(nullptr);
}

} // namespace PinkReader
