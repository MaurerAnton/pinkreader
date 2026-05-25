/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_hr.h
 * Port of: HtmlRawElementTagHorizontalRule.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTagHorizontalRule.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include "html_raw_element.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class BodyElement;
class HtmlTextAttributes;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagHorizontalRule
 *
 * Represents a <hr> horizontal rule element.
 * Extends HtmlRawElement directly (not HtmlRawElementTag).
 * Every field, method, and logic branch ported exactly.
 *
 * Origin: HtmlRawElementTagHorizontalRule.java lines 27-51
 */
class HtmlRawElementTagHorizontalRule : public HtmlRawElement {
public:
    // Port of: @Override public void getPlainText(@NonNull final StringBuilder stringBuilder)
    // Nothing to do
    // Origin: HtmlRawElementTagHorizontalRule.java lines 30-32
    void getPlainText(QString &stringBuilder) const override;

    // Port of: @Override public void reduce(...) - destination.add(this);
    // Origin: HtmlRawElementTagHorizontalRule.java lines 35-42
    void reduce(
            const HtmlTextAttributes &activeAttributes,
            AppCompatActivity &activity,
            std::vector<HtmlRawElement*> &destination,
            std::vector<LinkButtonDetails> &linkButtons) override;

    // Port of: @Override public void generate(...) - destination.add(new BodyElementHorizontalRule());
    // Origin: HtmlRawElementTagHorizontalRule.java lines 45-50
    void generate(
            AppCompatActivity &activity,
            std::vector<BodyElement*> &destination) override;
};

} // namespace PinkReader
