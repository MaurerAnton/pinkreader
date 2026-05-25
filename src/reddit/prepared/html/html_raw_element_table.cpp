/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_table.cpp
 * Port of: HtmlRawElementTable.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_table.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementTable : public BodyElement {
public:
    std::vector<BodyElement*> rows;
    explicit BodyElementTable(const std::vector<BodyElement*> &r) : rows(r) {}
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
void HtmlRawElementTable::getPlainText(QString &stringBuilder) const {
    for(const HtmlRawElement* element : mChildren) {
        element->getPlainText(stringBuilder);
    }
}

// ===== reduce (non-virtual, returns new table) =====
// Port of:
//   public HtmlRawElementTable reduce(
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
//       return new HtmlRawElementTable(reduced);
//   }
HtmlRawElementTable HtmlRawElementTable::reduce(
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

    // return new HtmlRawElementTable(reduced);
    return HtmlRawElementTable(reduced);
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
void HtmlRawElementTable::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(reduce(activeAttributes, activity, linkButtons));
    destination.push_back(
        new HtmlRawElementTable(reduce(activeAttributes, activity, linkButtons)));
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       final ArrayList<BodyElement> rows = new ArrayList<>(mChildren.size());
//
//       for(final HtmlRawElement child : mChildren) {
//           child.generate(activity, rows);
//       }
//
//       destination.add(new BodyElementTable(rows));
//   }
void HtmlRawElementTable::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // final ArrayList<BodyElement> rows = new ArrayList<>(mChildren.size());
    std::vector<BodyElement*> rows;
    rows.reserve(mChildren.size());

    // for(final HtmlRawElement child : mChildren) {
    //     child.generate(activity, rows);
    // }
    for(HtmlRawElement* child : mChildren) {
        child->generate(activity, rows);
    }

    // destination.add(new BodyElementTable(rows));
    destination.push_back(new BodyElementTable(rows));
}

} // namespace PinkReader
