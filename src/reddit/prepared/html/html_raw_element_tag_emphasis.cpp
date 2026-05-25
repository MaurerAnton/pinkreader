/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_emphasis.cpp
 * Port of: HtmlRawElementTagEmphasis.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_tag_emphasis.h"
#include "html_text_attributes.h"

namespace PinkReader {

// ===== onStart =====
// Port of:
//   @Override
//   protected void onStart(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.italic++;
//   }
// Origin: HtmlRawElementTagEmphasis.java lines 31-33
void HtmlRawElementTagEmphasis::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.italic++;
}

// ===== onEnd =====
// Port of:
//   @Override
//   protected void onEnd(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.italic--;
//   }
// Origin: HtmlRawElementTagEmphasis.java lines 36-38
void HtmlRawElementTagEmphasis::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.italic--;
}

} // namespace PinkReader
