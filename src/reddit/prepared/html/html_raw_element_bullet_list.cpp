/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_bullet_list.cpp
 * Port of: HtmlRawElementBulletList.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_bullet_list.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Stub classes for BodyElement types
class BodyElement {};
class BodyElementBullet : public BodyElement {
public:
    std::vector<BodyElement*> elements;
    explicit BodyElementBullet(const std::vector<BodyElement*> &e) : elements(e) {}
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
void HtmlRawElementBulletList::getPlainText(QString &stringBuilder) const {
    for(const HtmlRawElement* element : mChildren) {
        element->getPlainText(stringBuilder);
    }
}

// ===== reduce (non-virtual, returns new bullet list) =====
// Port of:
//   public HtmlRawElementBulletList reduce(
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
//       return new HtmlRawElementBulletList(reduced);
//   }
HtmlRawElementBulletList HtmlRawElementBulletList::reduce(
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

    // return new HtmlRawElementBulletList(reduced);
    return HtmlRawElementBulletList(reduced);
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
void HtmlRawElementBulletList::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // destination.add(reduce(activeAttributes, activity, linkButtons));
    destination.push_back(
        new HtmlRawElementBulletList(reduce(activeAttributes, activity, linkButtons)));
}

// ===== generate =====
// Port of:
//   @Override
//   public void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       for(final HtmlRawElement child : mChildren) {
//           final ArrayList<BodyElement> thisBullet = new ArrayList<>();
//           child.generate(activity, thisBullet);
//           destination.add(new BodyElementBullet(thisBullet));
//       }
//   }
void HtmlRawElementBulletList::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    // for(final HtmlRawElement child : mChildren) {
    for(HtmlRawElement* child : mChildren) {

        // final ArrayList<BodyElement> thisBullet = new ArrayList<>();
        std::vector<BodyElement*> thisBullet;

        // child.generate(activity, thisBullet);
        child->generate(activity, thisBullet);

        // destination.add(new BodyElementBullet(thisBullet));
        destination.push_back(new BodyElementBullet(thisBullet));
    }
}

} // namespace PinkReader
