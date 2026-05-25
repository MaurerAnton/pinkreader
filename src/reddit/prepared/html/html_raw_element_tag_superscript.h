/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_superscript.h
 * Port of: HtmlRawElementTagSuperscript.java
 *
 * Origin: HtmlRawElementTagSuperscript.java lines 24-39
 *   activeAttributes.superscript++ / superscript--
 */
#pragma once

#include "html_raw_element_tag_attr_change.h"
#include <vector>

namespace PinkReader {
class HtmlTextAttributes;

class HtmlRawElementTagSuperscript : public HtmlRawElementTagAttributeChange {
public:
    explicit HtmlRawElementTagSuperscript(const std::vector<HtmlRawElement*> &children)
        : HtmlRawElementTagAttributeChange(children) {}
    void onStart(HtmlTextAttributes &activeAttributes) override;
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
