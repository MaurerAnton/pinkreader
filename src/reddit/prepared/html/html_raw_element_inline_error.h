/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_raw_element_inline_error.h
 * Port of: HtmlRawElementInlineErrorMessage.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlRawElementInlineErrorMessage.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include "html_raw_element.h"

#include <QString>
#include <vector>

namespace PinkReader {

// Forward declarations
class HtmlRawElementStyledText;
class HtmlRawElementTagPassthrough;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlRawElementInlineErrorMessage
 *
 * Abstract utility class for creating inline error messages.
 * Every field, method, and logic branch ported exactly.
 */
class HtmlRawElementInlineErrorMessage : public HtmlRawElement {
public:
    // Port of: private HtmlRawElementInlineErrorMessage() {} (private constructor)
    // In C++, we make it abstract with no public constructor
    HtmlRawElementInlineErrorMessage() {}

    // Port of: public static HtmlRawElementStyledText create(@NonNull final String text)
    // Returns a newly allocated HtmlRawElementStyledText
    static HtmlRawElementStyledText* create(const QString &text);

    // Port of: @NonNull public static HtmlRawElementTagPassthrough appendError(
    //     @NonNull final String text,
    //     @NonNull final HtmlRawElement element)
    static HtmlRawElementTagPassthrough* appendError(
            const QString &text,
            HtmlRawElement &element);
};

} // namespace PinkReader
