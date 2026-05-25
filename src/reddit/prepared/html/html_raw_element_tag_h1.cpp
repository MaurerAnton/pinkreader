/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h1.cpp
 * Port of: HtmlRawElementTagH1.java
 */

#include "html_raw_element_tag_h1.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagH1::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold++;
    activeAttributes.extraLarge++;
}

void HtmlRawElementTagH1::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold--;
    activeAttributes.extraLarge--;
}

} // namespace PinkReader
