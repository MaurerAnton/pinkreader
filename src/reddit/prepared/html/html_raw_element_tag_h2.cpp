/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h2.cpp
 * Port of: HtmlRawElementTagH2.java
 */

#include "html_raw_element_tag_h2.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagH2::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.extraLarge++;
}

void HtmlRawElementTagH2::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.extraLarge--;
}

} // namespace PinkReader
