/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_block.h
 * Port of: HtmlRawElementBlock.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementBlock.java
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
class BlockType;
class HtmlTextAttributes;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementBlock
 *
 * Block-level HTML element with children.
 * Every field, method, and logic branch ported exactly.
 */
class HtmlRawElementBlock : public HtmlRawElement {
public:
    // Port of: @NonNull private final BlockType mBlockType;
    // BlockType is an enum; we'll use int as proxy
    int mBlockType;

    // Port of: @NonNull private final ArrayList<HtmlRawElement> mChildren;
    std::vector<HtmlRawElement*> mChildren;

    // Port of: public HtmlRawElementBlock(
    //     @NonNull final BlockType blockType,
    //     @NonNull final ArrayList<HtmlRawElement> children)
    HtmlRawElementBlock(int blockType,
                        const std::vector<HtmlRawElement*> &children)
        : mBlockType(blockType)
        , mChildren(children) {}

    // Port of: public HtmlRawElementBlock(
    //     @NonNull final BlockType blockType,
    //     final HtmlRawElement... children)
    HtmlRawElementBlock(int blockType,
                        const std::initializer_list<HtmlRawElement*> &children)
        : mBlockType(blockType)
        , mChildren(children) {}

    // Port of: @Override public void getPlainText(@NonNull final StringBuilder stringBuilder)
    void getPlainText(QString &stringBuilder) const override;

    // Port of: public HtmlRawElementBlock reduce(
    //     @NonNull final HtmlTextAttributes activeAttributes,
    //     @NonNull final AppCompatActivity activity)
    HtmlRawElementBlock reduce(
            const HtmlTextAttributes &activeAttributes,
            AppCompatActivity &activity) const;

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
