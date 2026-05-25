/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_code.cpp
 * Port of: HtmlRawElementTagCode.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_raw_element_tag_code.h"
#include "html_text_attributes.h"

namespace PinkReader {

// ===== onStart =====
// Port of:
//   @Override
//   protected void onStart(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.monospace++;
//   }
// Origin: HtmlRawElementTagCode.java lines 31-33
void HtmlRawElementTagCode::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.monospace++;
}

// ===== onEnd =====
// Port of:
//   @Override
//   protected void onEnd(@NonNull final HtmlTextAttributes activeAttributes) {
//       activeAttributes.monospace--;
//   }
// Origin: HtmlRawElementTagCode.java lines 36-38
void HtmlRawElementTagCode::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.monospace--;
}

} // namespace PinkReader
