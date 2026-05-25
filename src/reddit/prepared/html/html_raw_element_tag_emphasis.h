/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_emphasis.h
 * Port of: HtmlRawElementTagEmphasis.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTagEmphasis.java
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
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagEmphasis
 *
 * Represents <em> tag: increments/decrements italic.
 * Every field, method, and logic branch ported exactly.
 *
 * Origin: HtmlRawElementTagEmphasis.java lines 24-39
 */
class HtmlRawElementTagEmphasis : public HtmlRawElementTagAttributeChange {
public:
    // Port of: public HtmlRawElementTagEmphasis(final ArrayList<HtmlRawElement> children) (lines 26-28)
    // Origin: HtmlRawElementTagEmphasis.java lines 26-28
    explicit HtmlRawElementTagEmphasis(const std::vector<HtmlRawElement*> &children)
        : HtmlRawElementTagAttributeChange(children) {}

    // Port of: @Override protected void onStart(...) (lines 31-33)
    // activeAttributes.italic++;
    // Origin: HtmlRawElementTagEmphasis.java lines 31-33
    void onStart(HtmlTextAttributes &activeAttributes) override;

    // Port of: @Override protected void onEnd(...) (lines 36-38)
    // activeAttributes.italic--;
    // Origin: HtmlRawElementTagEmphasis.java lines 36-38
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
