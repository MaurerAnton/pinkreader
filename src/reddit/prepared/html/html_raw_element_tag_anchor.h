/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_anchor.h
 * Port of: HtmlRawElementTagAnchor.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTagAnchor.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include "html_raw_element_tag_attr_change.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class BodyElement;
class HtmlTextAttributes;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagAnchor
 *
 * Represents an anchor <a> tag with href attribute.
 * Every field, method, and logic branch ported exactly.
 *
 * Origin: HtmlRawElementTagAnchor.java lines 26-56
 */
class HtmlRawElementTagAnchor : public HtmlRawElementTagAttributeChange {
public:
    // Port of: @NonNull private final UriString mHref; (line 28)
    // Origin: HtmlRawElementTagAnchor.java line 28
    QString mHref;

    // Port of: public HtmlRawElementTagAnchor(
    //     final ArrayList<HtmlRawElement> children,
    //     @NonNull final UriString href) (lines 30-35)
    // Origin: HtmlRawElementTagAnchor.java lines 30-35
    HtmlRawElementTagAnchor(
            const std::vector<HtmlRawElement*> &children,
            const QString &href)
        : HtmlRawElementTagAttributeChange(children)
        , mHref(href) {}

    // Port of: @Override protected void onLinkButtons(...) (lines 38-45)
    // Origin: HtmlRawElementTagAnchor.java lines 38-45
    void onLinkButtons(std::vector<LinkButtonDetails> &linkButtons) override;

    // Port of: @Override protected void onStart(...) (lines 48-50)
    // activeAttributes.href = mHref;
    // Origin: HtmlRawElementTagAnchor.java lines 48-50
    void onStart(HtmlTextAttributes &activeAttributes) override;

    // Port of: @Override protected void onEnd(...) (lines 53-55)
    // activeAttributes.href = null;
    // Origin: HtmlRawElementTagAnchor.java lines 53-55
    void onEnd(HtmlTextAttributes &activeAttributes) override;
};

} // namespace PinkReader
