/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_reader_peekable.h
 * Port of: HtmlReaderPeekable.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/HtmlReaderPeekable.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include "html_reader.h"

#include <QString>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.HtmlReaderPeekable
 *
 * Wraps HtmlReader with peek/advance semantics.
 * Every field, method, and logic branch ported exactly.
 *
 * Origin: HtmlReaderPeekable.java lines 22-51
 */
class HtmlReaderPeekable {
public:
    // Port of: public HtmlReaderPeekable(@NonNull final HtmlReader htmlReader)
    //         throws MalformedHtmlException
    // Origin: HtmlReaderPeekable.java lines 28-32
    explicit HtmlReaderPeekable(HtmlReader &htmlReader);

    // Port of: public HtmlReader.Token peek()
    // Origin: HtmlReaderPeekable.java lines 34-36
    const HtmlReader::Token &peek() const;

    // Port of: public HtmlReader.Token advance() throws MalformedHtmlException
    // Origin: HtmlReaderPeekable.java lines 38-41
    HtmlReader::Token advance();

    // Port of: @NonNull public String getHtml()
    // Origin: HtmlReaderPeekable.java lines 44-46
    QString getHtml() const;

    // Port of: public int getPos()
    // Origin: HtmlReaderPeekable.java lines 48-50
    int getPos() const;

private:
    // Port of: @NonNull private final HtmlReader mHtmlReader;
    // Origin: HtmlReaderPeekable.java line 24
    HtmlReader &mHtmlReader;

    // Port of: @NonNull private HtmlReader.Token mNext;
    // Origin: HtmlReaderPeekable.java line 26
    HtmlReader::Token mNext;
};

} // namespace PinkReader
