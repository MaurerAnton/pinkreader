/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_line.h
 * Port of: MarkdownLine.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/markdown/MarkdownLine.java
 *
 * Every field, method, and logic branch ported exactly.
 */

#pragma once

#include "char_arr_substring.h"
#include "markdown_parser.h"

namespace PinkReader {

// Forward declarations
class MarkdownParagraph;
class IntArrayLengthPair;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.markdown.MarkdownLine
 *
 * Classifies a line of text into a MarkdownParagraphType and stores
 * metadata. Also handles line re-joining and tokenization to paragraph.
 */
class MarkdownLine {
public:
	// Port of: public final CharArrSubstring src;
	CharArrSubstring src;
	// Port of: public final MarkdownParser.MarkdownParagraphType type;
	MarkdownParser::MarkdownParagraphType type;
	// Port of: public final int spacesAtStart;
	int spacesAtStart;
	// Port of: public final int spacesAtEnd;
	int spacesAtEnd;
	// Port of: public final int prefixLength;
	int prefixLength;
	// Port of: public final int level;
	int level;
	// Port of: public final int number;
	int number;

	// Port of: MarkdownLine(...)
	MarkdownLine(
			CharArrSubstring src,
			MarkdownParser::MarkdownParagraphType type,
			int spacesAtStart,
			int spacesAtEnd,
			int prefixLength,
			int level,
			int number);

	// Port of: public static MarkdownLine generate(final CharArrSubstring src)
	static MarkdownLine generate(const CharArrSubstring& src);

	// Port of: public MarkdownLine rejoin(final MarkdownLine toAppend)
	MarkdownLine rejoin(const MarkdownLine& toAppend) const;

	// Port of: public MarkdownParagraph tokenize(final MarkdownParagraph parent)
	MarkdownParagraph tokenize(const MarkdownParagraph* parent) const;

private:
	// Port of: private boolean isPlainText()
	bool isPlainText() const;
};

} // namespace PinkReader
