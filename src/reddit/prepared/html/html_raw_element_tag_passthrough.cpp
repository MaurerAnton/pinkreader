/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_passthrough.cpp
 * Port of: HtmlRawElementTagPassthrough.java
 *
 * onStart/onEnd: Do nothing (pass through without changing attributes)
 */

#include "html_raw_element_tag_passthrough.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagPassthrough::onStart(HtmlTextAttributes &activeAttributes) {
    // Do nothing - Origin: HtmlRawElementTagPassthrough.java lines 31-33
}

void HtmlRawElementTagPassthrough::onEnd(HtmlTextAttributes &activeAttributes) {
    // Do nothing - Origin: HtmlRawElementTagPassthrough.java lines 36-38
}

} // namespace PinkReader
