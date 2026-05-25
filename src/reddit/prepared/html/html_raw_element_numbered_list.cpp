/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_numbered_list.cpp
 * Port of: HtmlRawElementNumberedList.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_numbered_list.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementNumberedListElement : public BodyElement {
public:
    int number;
    std::vector<BodyElement*> elements;
    BodyElementNumberedListElement(int n, const std::vector<BodyElement*> &e)
        : number(n), elements(e) {}
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
void HtmlRawElementNumberedList::getPlainText(QString &stringBuilder) const {
    for(const HtmlRawElement* element : mChildren) {
        element->getPlainText(stringBuilder);
    }
}

// ===== reduce (non-virtual, returns new numbered list) =====
// Port of:
//   public HtmlRawElementNumberedList reduce(
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
//       return new HtmlRawElementNumberedList(reduced);
//   }
HtmlRawElementNumberedList HtmlRawElementNumberedList::reduce(
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

    // return new HtmlRawElementNumberedList(reduced);
    return HtmlRawElementNumberedList(reduced);
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
void HtmlRawElementNumberedList::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(reduce(activeAttributes, activity, linkButtons));
    destination.push_back(
        new HtmlRawElementNumberedList(reduce(activeAttributes, activity, linkButtons)));
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       int number = 1;
//
//       for(final HtmlRawElement child : mChildren) {
//           final ArrayList<BodyElement> thisElement = new ArrayList<>();
//           child.generate(activity, thisElement);
//
//           destination.add(new BodyElementNumberedListElement(
//               number, thisElement));
//
//           number++;
//       }
//   }
void HtmlRawElementNumberedList::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // int number = 1;
    int number = 1;

    // for(final HtmlRawElement child : mChildren) {
    for(HtmlRawElement* child : mChildren) {

        // final ArrayList<BodyElement> thisElement = new ArrayList<>();
        std::vector<BodyElement*> thisElement;

        // child.generate(activity, thisElement);
        child->generate(activity, thisElement);

        // destination.add(new BodyElementNumberedListElement(
        //     number, thisElement));
        destination.push_back(
            new BodyElementNumberedListElement(number, thisElement));

        // number++;
        number++;
    }
}

} // namespace PinkReader
