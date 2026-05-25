/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h5.cpp
 * Port of: HtmlRawElementTagH5.java
 */

#include "html_raw_element_tag_h5.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagH5::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold++;
}

void HtmlRawElementTagH5::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.bold--;
}

} // namespace PinkReader
