/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_h1.h
 * Port of: HtmlRawElementTagH1.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTagH1.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include "html_raw_element_tag_attr_change.h"

#include <vector>

namespace PinkReader {

// Forward declarations
class HtmlTextAttributes;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagH1
 *
 * Represents <h1> tag: bold + extraLarge.
 * Origin: HtmlRawElementTagH1.java lines 24-41
 */
class HtmlRawElementTagH1 : public HtmlRawElementTagAttributeChange {
public:
    explicit HtmlRawElementTagH1(const std::vector<HtmlRawElement*> &children)
        : HtmlRawElementTagAttributeChange(children) {}

    void onStart(HtmlTextAttributes &activeAttributes) override;
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
