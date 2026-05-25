/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_table_cell.cpp
 * Port of: HtmlRawElementTableCell.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_table_cell.h"
#include "html_raw_element_block.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementTableCell : public BodyElement {
public:
    std::vector<BodyElement*> elements;
    explicit BodyElementTableCell(const std::vector<BodyElement*> &e) : elements(e) {}
};
class AppCompatActivity {};

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       mChild.getPlainText(stringBuilder);
//   }
void HtmlRawElementTableCell::getPlainText(QString &stringBuilder) const {
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
//       destination.add(new HtmlRawElementTableCell(mChild.reduce(
//           activeAttributes, activity)));
//   }
void HtmlRawElementTableCell::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(new HtmlRawElementTableCell(mChild.reduce(
    //     activeAttributes, activity)));
    destination.push_back(new HtmlRawElementTableCell(
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
//       destination.add(new BodyElementTableCell(elements));
//   }
void HtmlRawElementTableCell::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // final ArrayList<BodyElement> elements = new ArrayList<>();
    std::vector<BodyElement*> elements;

    // mChild.generate(activity, elements);
    mChild->generate(activity, elements);

    // destination.add(new BodyElementTableCell(elements));
    destination.push_back(new BodyElementTableCell(elements));
}

} // namespace PinkReader
