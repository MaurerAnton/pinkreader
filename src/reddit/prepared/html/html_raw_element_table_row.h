/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_table_row.h
 * Port of: HtmlRawElementTableRow.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementTableRow.java
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
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementTableRow
 *
 * Represents a <tr> table row element.
 * Every field, method, and logic branch ported exactly.
 */
class HtmlRawElementTableRow : public HtmlRawElement {
public:
    // Port of: @NonNull private final ArrayList<HtmlRawElement> mChildren;
    std::vector<HtmlRawElement*> mChildren;

    // Port of: public HtmlRawElementTableRow(@NonNull final ArrayList<HtmlRawElement> children)
    explicit HtmlRawElementTableRow(const std::vector<HtmlRawElement*> &children)
        : mChildren(children) {}

    // Port of: @Override public void getPlainText(@NonNull final StringBuilder stringBuilder)
    void getPlainText(QString &stringBuilder) const override;

    // Port of: public HtmlRawElementTableRow reduce(
    //     @NonNull final HtmlTextAttributes activeAttributes,
    //     @NonNull final AppCompatActivity activity,
    //     @NonNull final ArrayList<LinkButtonDetails> linkButtons)
    HtmlRawElementTableRow reduce(
            const HtmlTextAttributes &activeAttributes,
            AppCompatActivity &activity,
            std::vector<LinkButtonDetails> &linkButtons) const;

    // Port of: @Override public void reduce(
    //     @NonNull final HtmlTextAttributes activeAttributes,
    //     @NonNull final AppCompatActivity activity,
    //     @NonNull final ArrayList<HtmlRawElement> destination,
    //     @NonNull final ArrayList<LinkButtonDetails> linkButtons)
    void reduce(
            const HtmlTextAttributes &activeAttributes,
            AppCompatActivity &activity,
            std::vector<HtmlRawElement*> &destination,
            std::vector<LinkButtonDetails> &linkButtons) override;

    // Port of: @Override public void generate(
    //     @NonNull final AppCompatActivity activity,
    //     @NonNull final ArrayList<BodyElement> destination)
    void generate(
            AppCompatActivity &activity,
            std::vector<BodyElement*> &destination) override;
};

} // namespace PinkReader
