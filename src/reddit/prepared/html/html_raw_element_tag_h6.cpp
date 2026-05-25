/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h6.cpp
 * Port of: HtmlRawElementTagH6.java
 */

#include "html_raw_element_tag_h6.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagH6::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.underline++;
}

void HtmlRawElementTagH6::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.underline--;
}

} // namespace PinkReader
