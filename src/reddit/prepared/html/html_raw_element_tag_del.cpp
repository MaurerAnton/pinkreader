/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_del.cpp
 * Port of: HtmlRawElementTagDel.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_tag_del.h"
#include "html_text_attributes.h"

namespace PinkReader {

// ===== onStart =====
// Port of:
//   @Override
//   protected void onStart(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.strikethrough++;
//   }
// Origin: HtmlRawElementTagDel.java lines 31-33
void HtmlRawElementTagDel::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.strikethrough++;
}

// ===== onEnd =====
// Port of:
//   @Override
//   protected void onEnd(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.strikethrough--;
//   }
// Origin: HtmlRawElementTagDel.java lines 36-38
void HtmlRawElementTagDel::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.strikethrough--;
}

} // namespace PinkReader
