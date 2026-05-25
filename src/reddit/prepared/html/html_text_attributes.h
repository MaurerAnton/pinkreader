/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_text_attributes.h
 * Port of: HtmlTextAttributes.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlTextAttributes.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include <QString>
#include <optional>

namespace PinkReader {

// Forward declarations
class UriString;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlTextAttributes
 *
 * Mutable text attributes that are pushed/popped as HTML tags are entered/exited.
 * Every field ported exactly.
 *
 * Origin: HtmlTextAttributes.java lines 24-37
 */
class HtmlTextAttributes {
public:
    // Port of: public int bold; (line 26)
    int bold = 0;

    // Port of: public int italic; (line 27)
    int italic = 0;

    // Port of: public int underline; (line 28)
    int underline = 0;

    // Port of: public int strikethrough; (line 29)
    int strikethrough = 0;

    // Port of: public int monospace; (line 30)
    int monospace = 0;

    // Port of: public int superscript; (line 31)
    int superscript = 0;

    // Port of: public int extraLarge; (line 33)
    int extraLarge = 0;

    // Port of: public int large; (line 34)
    int large = 0;

    // Port of: @Nullable public UriString href; (line 36)
    std::optional<QString> href;
};

} // namespace PinkReader
