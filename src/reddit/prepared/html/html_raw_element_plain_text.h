/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_plain_text.h
 * Port of: HtmlRawElementPlainText.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementPlainText.java
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
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementPlainText
 *
 * Represents plain text within HTML content.
 * Every field, method, and logic branch ported exactly.
 */
class HtmlRawElementPlainText : public HtmlRawElement {
public:
    // Port of: @NonNull private final String mText;
    QString mText;

    // Port of: public HtmlRawElementPlainText(@NonNull final String text)
    explicit HtmlRawElementPlainText(const QString &text)
        : mText(text) {}

    // Port of: @Override public void getPlainText(@NonNull final StringBuilder stringBuilder)
    void getPlainText(QString &stringBuilder) const override;

    // Port of: @Override public void reduce(
    //     @NonNull final HtmlTextAttributes attributes,
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
