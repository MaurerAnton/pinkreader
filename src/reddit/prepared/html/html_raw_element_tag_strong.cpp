/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_strong.cpp
 * Port of: HtmlRawElementTagStrong.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_tag_strong.h"
#include "html_text_attributes.h"

namespace PinkReader {

// ===== onStart =====
// Port of:
//   @Override
//   protected void onStart(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.bold++;
//   }
// Origin: HtmlRawElementTagStrong.java lines 31-33
void HtmlRawElementTagStrong::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold++;
}

// ===== onEnd =====
// Port of:
//   @Override
//   protected void onEnd(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.bold--;
//   }
// Origin: HtmlRawElementTagStrong.java lines 36-38
void HtmlRawElementTagStrong::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold--;
}

} // namespace PinkReader
