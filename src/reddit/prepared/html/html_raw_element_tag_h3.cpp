/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h3.cpp
 * Port of: HtmlRawElementTagH3.java
 */

#include "html_raw_element_tag_h3.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagH3::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold++;
    activeAttributes.large++;
}

void HtmlRawElementTagH3::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold--;
    activeAttributes.large--;
}

} // namespace PinkReader
