/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_del.h
 * Port of: HtmlRawElementTagDel.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTagDel.java
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
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagDel
 *
 * Represents a <del> tag: increments/decrements strikethrough.
 * Every field, method, and logic branch ported exactly.
 *
 * Origin: HtmlRawElementTagDel.java lines 24-39
 */
class HtmlRawElementTagDel : public HtmlRawElementTagAttributeChange {
public:
    // Port of: public HtmlRawElementTagDel(final ArrayList<HtmlRawElement> children) (lines 26-28)
    // Origin: HtmlRawElementTagDel.java lines 26-28
    explicit HtmlRawElementTagDel(const std::vector<HtmlRawElement*> &children)
        : HtmlRawElementTagAttributeChange(children) {}

    // Port of: @Override protected void onStart(...) (lines 31-33)
    // activeAttributes.strikethrough++;
    // Origin: HtmlRawElementTagDel.java lines 31-33
    void onStart(HtmlTextAttributes &activeAttributes) override;

    // Port of: @Override protected void onEnd(...) (lines 36-38)
    // activeAttributes.strikethrough--;
    // Origin: HtmlRawElementTagDel.java lines 36-38
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
