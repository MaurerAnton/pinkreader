/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_break.cpp
 * Port of: HtmlRawElementBreak.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_break.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class AppCompatActivity {};

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       // Nothing to do
//   }
void HtmlRawElementBreak::getPlainText(QString &stringBuilder) const {
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
void HtmlRawElementBreak::reduce(
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
//       // Nothing to do
//   }
void HtmlRawElementBreak::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {
    // Nothing to do
}

} // namespace PinkReader
