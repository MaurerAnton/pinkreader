/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_code.h
 * Port of: HtmlRawElementTagCode.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTagCode.java
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
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagCode
 *
 * Represents a <code> tag: increments/decrements monospace.
 * Every field, method, and logic branch ported exactly.
 *
 * Origin: HtmlRawElementTagCode.java lines 24-39
 */
class HtmlRawElementTagCode : public HtmlRawElementTagAttributeChange {
public:
    // Port of: public HtmlRawElementTagCode(final ArrayList<HtmlRawElement> children) (lines 26-28)
    // Origin: HtmlRawElementTagCode.java lines 26-28
    explicit HtmlRawElementTagCode(const std::vector<HtmlRawElement*> &children)
        : HtmlRawElementTagAttributeChange(children) {}

    // Port of: @Override protected void onStart(...) (lines 31-33)
    // activeAttributes.monospace++;
    // Origin: HtmlRawElementTagCode.java lines 31-33
    void onStart(HtmlTextAttributes &activeAttributes) override;

    // Port of: @Override protected void onEnd(...) (lines 36-38)
    // activeAttributes.monospace--;
    // Origin: HtmlRawElementTagCode.java lines 36-38
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
