/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h4.cpp
 * Port of: HtmlRawElementTagH4.java
 */

#include "html_raw_element_tag_h4.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagH4::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.large++;
}

void HtmlRawElementTagH4::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.large--;
}

} // namespace PinkReader
