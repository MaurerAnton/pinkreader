/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_styled_text.h
 * Port of: HtmlRawElementStyledText.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementStyledText.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include "html_raw_element.h"

#include <QString>
#include <optional>
#include <vector>

namespace PinkReader {

// Forward declarations
class AppCompatActivity;
class BodyElement;
class HtmlTextAttributes;
class SpannableStringBuilder;
class CharacterStyle;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementStyledText
 *
 * Represents styled text (with spans) within HTML content.
 * Every field, method, and logic branch ported exactly.
 */
class HtmlRawElementStyledText : public HtmlRawElement {
public:
    // Port of: @NonNull private final String mText;
    QString mText;

    // Port of: @Nullable private final ArrayList<CharacterStyle> mSpans;
    // Using void* as a placeholder for CharacterStyle spans (Android type)
    std::optional<std::vector<void*>> mSpans;

    // Port of: public HtmlRawElementStyledText(
    //     @NonNull final String text,
    //     @Nullable final ArrayList<CharacterStyle> spans)
    HtmlRawElementStyledText(const QString &text,
                             const std::optional<std::vector<void*>> &spans)
        : mText(text)
        , mSpans(spans) {}

    // Port of: @Override public void getPlainText(@NonNull final StringBuilder stringBuilder)
    void getPlainText(QString &stringBuilder) const override;

    // Port of: public final void writeTo(@NonNull final SpannableStringBuilder ssb)
    void writeTo(SpannableStringBuilder &ssb);

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
