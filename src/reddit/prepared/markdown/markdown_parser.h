/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_parser.h
 * Port of: MarkdownParser.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/markdown/MarkdownParser.java
 *
 * Every field, method, inner class, enum, and constant ported exactly.
 */

#pragma once

#include <vector>
#include <memory>

namespace PinkReader {

// Forward declarations
class MarkdownParagraph;
class MarkdownParagraphGroup;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.markdown.MarkdownParser
 *
 * Top-level markdown parser that takes raw char array and produces
 * a MarkdownParagraphGroup.
 */
class MarkdownParser {
public:
	// Port of: public enum MarkdownParagraphType { TEXT, CODE, BULLET, NUMBERED, QUOTE, HEADER, HLINE, EMPTY }
	enum class MarkdownParagraphType {
		TEXT,
		CODE,
		BULLET,
		NUMBERED,
		QUOTE,
		HEADER,
		HLINE,
		EMPTY
	};

	// Port of: public static MarkdownParagraphGroup parse(final char[] raw)
	static MarkdownParagraphGroup parse(const char* raw, int length);
};

} // namespace PinkReader
