/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: html_reader_peekable.cpp
 * Port of: HtmlReaderPeekable.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#include "html_reader_peekable.h"
#include "html_reader.h"

#include <QString>

namespace PinkReader {

// ===== Constructor =====
// Port of:
//   public HtmlReaderPeekable(@NonNull final HtmlReader htmlReader)
//       throws MalformedHtmlException {
//       mHtmlReader = htmlReader;
//       mNext = mHtmlReader.readNext();
//   }
// Origin: HtmlReaderPeekable.java lines 28-32
HtmlReaderPeekable::HtmlReaderPeekable(HtmlReader &htmlReader)
    : mHtmlReader(htmlReader)
    , mNext(htmlReader.readNext()) {}

// ===== peek =====
// Port of:
//   public HtmlReader.Token peek() {
//       return mNext;
//   }
// Origin: HtmlReaderPeekable.java lines 34-36
const HtmlReader::Token &HtmlReaderPeekable::peek() const {
    return mNext;
}

// ===== advance =====
// Port of:
//   public HtmlReader.Token advance() throws MalformedHtmlException {
//       mNext = mHtmlReader.readNext();
//       return mNext;
//   }
// Origin: HtmlReaderPeekable.java lines 38-41
HtmlReader::Token HtmlReaderPeekable::advance() {
    mNext = mHtmlReader.readNext();
    return mNext;
}

// ===== getHtml =====
// Port of:
//   @NonNull public String getHtml() {
//       return mHtmlReader.getHtml();
//   }
// Origin: HtmlReaderPeekable.java lines 44-46
QString HtmlReaderPeekable::getHtml() const {
    return mHtmlReader.getHtml();
}

// ===== getPos =====
// Port of:
//   public int getPos() {
//       return mHtmlReader.getPos();
//   }
// Origin: HtmlReaderPeekable.java lines 48-50
int HtmlReaderPeekable::getPos() const {
    return mHtmlReader.getPos();
}

} // namespace PinkReader
