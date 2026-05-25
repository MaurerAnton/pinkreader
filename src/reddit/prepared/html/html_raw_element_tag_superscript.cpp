/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_superscript.cpp
 * Port of: HtmlRawElementTagSuperscript.java
 */

#include "html_raw_element_tag_superscript.h"
#include "html_text_attributes.h"

namespace PinkReader {

void HtmlRawElementTagSuperscript::onStart(HtmlTextAttributes &activeAttributes) {
    activeAttributes.superscript++;
}

void HtmlRawElementTagSuperscript::onEnd(HtmlTextAttributes &activeAttributes) {
    activeAttributes.superscript--;
}

} // namespace PinkReader
