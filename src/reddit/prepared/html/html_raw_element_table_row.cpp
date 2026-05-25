/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_table_row.cpp
 * Port of: HtmlRawElementTableRow.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_table_row.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementTableRow : public BodyElement {
public:
    std::vector<BodyElement*> cols;
    explicit BodyElementTableRow(const std::vector<BodyElement*> &c) : cols(c) {}
};
class AppCompatActivity {};

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       for(final HtmlRawElement element : mChildren) {
//           element.getPlainText(stringBuilder);
//       }
//   }
void HtmlRawElementTableRow::getPlainText(QString &stringBuilder) const {
    for(const HtmlRawElement* element : mChildren) {
        element->getPlainText(stringBuilder);
    }
}

// ===== reduce (non-virtual, returns new table row) =====
// Port of:
//   public HtmlRawElementTableRow reduce(
//       @NonNull final HtmlTextAttributes activeAttributes,
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<LinkButtonDetails> linkButtons) {
//
//       final ArrayList<HtmlRawElement> reduced = new ArrayList<>();
//
//       for(final HtmlRawElement child : mChildren) {
//           child.reduce(activeAttributes, activity, reduced, linkButtons);
//       }
//
//       return new HtmlRawElementTableRow(reduced);
//   }
HtmlRawElementTableRow HtmlRawElementTableRow::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<LinkButtonDetails> &linkButtons) const {

    std::vector<HtmlRawElement*> reduced;

    // for(final HtmlRawElement child : mChildren) {
    //     child.reduce(activeAttributes, activity, reduced, linkButtons);
    // }
    for(HtmlRawElement* child : mChildren) {
        child->reduce(activeAttributes, activity, reduced, linkButtons);
    }

    // return new HtmlRawElementTableRow(reduced);
    return HtmlRawElementTableRow(reduced);
}

// ===== reduce (virtual, adds to destination) =====
// Port of:
//   @Override
//   public void reduce(
//       @NonNull final HtmlTextAttributes activeAttributes,
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<HtmlRawElement> destination,
//       @NonNull final ArrayList<LinkButtonDetails> linkButtons) {
//
//       destination.add(reduce(activeAttributes, activity, linkButtons));
//   }
void HtmlRawElementTableRow::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(reduce(activeAttributes, activity, linkButtons));
    destination.push_back(
        new HtmlRawElementTableRow(reduce(activeAttributes, activity, linkButtons)));
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       final ArrayList<BodyElement> cols = new ArrayList<>(mChildren.size());
//
//       for(final HtmlRawElement child : mChildren) {
//           child.generate(activity, cols);
//       }
//
//       destination.add(new BodyElementTableRow(cols));
//   }
void HtmlRawElementTableRow::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // final ArrayList<BodyElement> cols = new ArrayList<>(mChildren.size());
    std::vector<BodyElement*> cols;
    cols.reserve(mChildren.size());

    // for(final HtmlRawElement child : mChildren) {
    //     child.generate(activity, cols);
    // }
    for(HtmlRawElement* child : mChildren) {
        child->generate(activity, cols);
    }

    // destination.add(new BodyElementTableRow(cols));
    destination.push_back(new BodyElementTableRow(cols));
}

} // namespace PinkReader
