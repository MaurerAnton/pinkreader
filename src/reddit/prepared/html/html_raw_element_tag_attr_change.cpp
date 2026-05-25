/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_attr_change.cpp
 * Port of: HtmlRawElementTagAttributeChange.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_tag_attr_change.h"
#include "html_text_attributes.h"

#include <QString>
#include <stdexcept>
#include <vector>

namespace PinkReader {

// ===== getPlainText =====
// Port of:
//   @Override
//   public void getPlainText(@NonNull final StringBuilder stringBuilder) {
//       for(final HtmlRawElement element : mChildren) {
//           element.getPlainText(stringBuilder);
//       }
//   }
// Origin: HtmlRawElementTagAttributeChange.java lines 43-47
void HtmlRawElementTagAttributeChange::getPlainText(QString &stringBuilder) const {
    for(HtmlRawElement *element : mChildren) {
        element->getPlainText(stringBuilder);
    }
}

// ===== reduce (final) =====
// Port of:
//   @Override
//   public final void reduce(
//       @NonNull final HtmlTextAttributes activeAttributes,
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<HtmlRawElement> destination,
//       @NonNull final ArrayList<LinkButtonDetails> linkButtons) {
//
//       onStart(activeAttributes);
//
//       try {
//           for(final HtmlRawElement child : mChildren) {
//               child.reduce(activeAttributes, activity, destination, linkButtons);
//           }
//
//       } finally {
//           onEnd(activeAttributes);
//       }
//
//       onLinkButtons(linkButtons);
//   }
// Origin: HtmlRawElementTagAttributeChange.java lines 50-68
void HtmlRawElementTagAttributeChange::reduce(
        const HtmlTextAttributes &activeAttributes,
        AppCompatActivity &activity,
        std::vector<HtmlRawElement*> &destination,
        std::vector<LinkButtonDetails> &linkButtons) {

    // onStart(activeAttributes);
    // NOTE: onStart takes mutable reference since it modifies attributes
    // We create a mutable copy to match the Java semantics (try/finally pattern)
    HtmlTextAttributes mutableAttrs = activeAttributes;
    onStart(mutableAttrs);

    try {
        // for(final HtmlRawElement child : mChildren)
        for(HtmlRawElement *child : mChildren) {
            child->reduce(mutableAttrs, activity, destination, linkButtons);
        }

    } catch(...) {
        // finally block: always call onEnd
        onEnd(mutableAttrs);
        throw;
    }

    // finally block (normal path)
    onEnd(mutableAttrs);

    // onLinkButtons(linkButtons);
    onLinkButtons(linkButtons);
}

// ===== generate =====
// Port of:
//   @Override
//   public final void generate(
//       @NonNull final AppCompatActivity activity,
//       @NonNull final ArrayList<BodyElement> destination) {
//
//       throw new RuntimeException("Attempt to call generate() on reducible element");
//   }
// Origin: HtmlRawElementTagAttributeChange.java lines 71-76
void HtmlRawElementTagAttributeChange::generate(
        AppCompatActivity &activity,
        std::vector<BodyElement*> &destination) {

    throw std::runtime_error("Attempt to call generate() on reducible element");
}

} // namespace PinkReader
