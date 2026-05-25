/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_passthrough.h
 * Port of: HtmlRawElementTagPassthrough.java
 *
 * Origin: HtmlRawElementTagPassthrough.java lines 24-39
 *   onStart/onEnd: Do nothing
 */
#pragma once

#include "html_raw_element_tag_attr_change.h"
#include <vector>

namespace PinkReader {
class HtmlTextAttributes;

class HtmlRawElementTagPassthrough : public HtmlRawElementTagAttributeChange {
public:
    explicit HtmlRawElementTagPassthrough(const std::vector<HtmlRawElement*> &children)
        : HtmlRawElementTagAttributeChange(children) {}
    void onStart(HtmlTextAttributes &activeAttributes) override;
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
