/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_tag_attr_change.h
 * Port of: HtmlRawElementTagAttributeChange.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTagAttributeChange.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include "html_raw_element_tag.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class BodyElement;
class HtmlTextAttributes;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTagAttributeChange
 *
 * Abstract base for tags that modify text attributes (bold, italic, etc.)
 * during their scope, then restore them.
 * Every field, method, and logic branch ported exactly.
 */
class HtmlRawElementTagAttributeChange : public HtmlRawElementTag {
public:
    // Port of: private final ArrayList<HtmlRawElement> mChildren; (line 28)
    // Origin: HtmlRawElementTagAttributeChange.java line 28
    std::vector<HtmlRawElement*> mChildren;

    // Port of: public HtmlRawElementTagAttributeChange(final ArrayList<HtmlRawElement> children) (lines 30-32)
    // Origin: HtmlRawElementTagAttributeChange.java lines 30-32
    explicit HtmlRawElementTagAttributeChange(const std::vector<HtmlRawElement*> &children)
        : mChildren(children) {}

    // Port of: protected void onLinkButtons(@NonNull final ArrayList<LinkButtonDetails> linkButtons) (lines 34-36)
    // Add nothing by default
    // Origin: HtmlRawElementTagAttributeChange.java lines 34-36
    virtual void onLinkButtons(std::vector<LinkButtonDetails> &linkButtons) {
        // Add nothing by default
    }

    // Port of: protected abstract void onStart(@NonNull HtmlTextAttributes activeAttributes); (line 38)
    // Origin: HtmlRawElementTagAttributeChange.java line 38
    virtual void onStart(HtmlTextAttributes &activeAttributes) = 0;

    // Port of: protected abstract void onEnd(@NonNull HtmlTextAttributes activeAttributes); (line 40)
    // Origin: HtmlRawElementTagAttributeChange.java line 40
    virtual void onEnd(HtmlTextAttributes &activeAttributes) = 0;

    // Port of: @Override public void getPlainText(@NonNull final StringBuilder stringBuilder) (lines 43-47)
    // Origin: HtmlRawElementTagAttributeChange.java lines 43-47
    void getPlainText(QString &stringBuilder) const override;

    // Port of: @Override public final void reduce(...) (lines 50-68)
    // Origin: HtmlRawElementTagAttributeChange.java lines 50-68
    void reduce(
            const HtmlTextAttributes &activeAttributes,
            AppCompatActivity &activity,
            std::vector<HtmlRawElement*> &destination,
            std::vector<LinkButtonDetails> &linkButtons) override;

    // Port of: @Override public final void generate(...) (lines 71-76)
    // throws RuntimeException - "Attempt to call generate() on reducible element"
    // Origin: HtmlRawElementTagAttributeChange.java lines 71-76
    void generate(
            AppCompatActivity &activity,
            std::vector<BodyElement*> &destination) override;
};

} // namespace PinkReader
