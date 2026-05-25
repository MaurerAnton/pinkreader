/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h6.h
 * Port of: HtmlRawElementTagH6.java
 *
 * Origin: HtmlRawElementTagH6.java lines 24-39
 *   activeAttributes.underline++ / underline--
 */
#pragma once

#include "html_raw_element_tag_attr_change.h"
#include <vector>

namespace PinkReader {
class HtmlTextAttributes;

class HtmlRawElementTagH6 : public HtmlRawElementTagAttributeChange {
public:
    explicit HtmlRawElementTagH6(const std::vector<HtmlRawElement*> &children)
        : HtmlRawElementTagAttributeChange(children) {}
    void onStart(HtmlTextAttributes &activeAttributes) override;
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
