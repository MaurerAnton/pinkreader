/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_img.h
 * Port of: HtmlRawElementImg.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementImg.java
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
class UriString;
class SpannableStringBuilder;
class DynamicSpanned;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementImg
 *
 * Represents an inline image/emote element.
 * Every field, method, and logic branch ported exactly.
 */
class HtmlRawElementImg : public HtmlRawElement {
public:
    // Port of: @NonNull private final ArrayList<HtmlRawElement> mChildren;
    std::vector<HtmlRawElement*> mChildren;

    // Port of: @NonNull private final String mTitle;
    QString mTitle;

    // Port of: @NonNull private final UriString mSrc;
    // Stored as QString proxy for UriString
    QString mSrcValue;

    // Port of: public HtmlRawElementImg(
    //     @NonNull final ArrayList<HtmlRawElement> children,
    //     @NonNull final String title,
    //     @NonNull final UriString src)
    HtmlRawElementImg(const std::vector<HtmlRawElement*> &children,
                      const QString &title,
                      const QString &srcValue)
        : mChildren(children)
        , mTitle(title)
        , mSrcValue(srcValue) {}

    // Port of: @Override public void getPlainText(@NonNull final StringBuilder stringBuilder)
    void getPlainText(QString &stringBuilder) const override;

    // Port of: public final synchronized void writeTo(
    //     @NonNull final SpannableStringBuilder ssb,
    //     @NonNull final AppCompatActivity activity,
    //     @NonNull final DynamicSpanned dynamicSpanned)
    void writeTo(SpannableStringBuilder &ssb,
                 AppCompatActivity &activity,
                 DynamicSpanned &dynamicSpanned);

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
