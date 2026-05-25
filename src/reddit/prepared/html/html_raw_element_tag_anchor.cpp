/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_anchor.cpp
 * Port of: HtmlRawElementTagAnchor.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_tag_anchor.h"
#include "html_text_attributes.h"

#include <QString>
#include <vector>

namespace PinkReader {

// ===== onLinkButtons =====
// Port of:
//   @Override
//   protected void onLinkButtons(@NonNull final ArrayList<LinkButtonDetails> linkButtons) {
//       final String text = getPlainText().trim();
//       linkButtons.add(new LinkButtonDetails(
//           text.isEmpty() ? null : text,
//           mHref));
//   }
// Origin: HtmlRawElementTagAnchor.java lines 38-45
void HtmlRawElementTagAnchor::onLinkButtons(std::vector<LinkButtonDetails> &linkButtons) {
    // final String text = getPlainText().trim();
    QString text = getPlainText().trimmed();

    // linkButtons.add(new LinkButtonDetails(text.isEmpty() ? null : text, mHref));
    std::optional<QString> name;
    if(!text.isEmpty()) {
        name = text;
    }
    linkButtons.emplace_back(name, mHref);
}

// ===== onStart =====
// Port of:
//   @Override
//   protected void onStart(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.href = mHref;
//   }
// Origin: HtmlRawElementTagAnchor.java lines 48-50
void HtmlRawElementTagAnchor::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.href = mHref;
}

// ===== onEnd =====
// Port of:
//   @Override
//   protected void onEnd(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.href = null;
//   }
// Origin: HtmlRawElementTagAnchor.java lines 53-55
void HtmlRawElementTagAnchor::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.href = std::nullopt;
}

} // namespace PinkReader
