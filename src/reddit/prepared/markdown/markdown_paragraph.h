/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_paragraph.h
 * Port of: MarkdownParagraph.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/markdown/MarkdownParagraph.java
 *
 * Every field, method, inner class, and constant ported exactly.
 * Android-specific rendering (Spanned, SpannableStringBuilder) is stubbed
 * with platform-independent equivalents.
 */

#pragma once

#include "char_arr_substring.h"
#include "markdown_parser.h"

#include <string>
#include <vector>
#include <memory>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.markdown.MarkdownParagraph
 *
 * Represents a parsed markdown paragraph. Contains the raw text, token stream,
 * and rendered output. Inner class Link handles hyperlinks.
 */
class MarkdownParagraph {
public:
	// Port of: public static class Link
	class Link {
	public:
		// Port of: final String title;
		std::string title;
		// Port of: final String subtitle;
		std::string subtitle;
		// Port of: private final UriString url;
		std::string url;

		// Port of: public Link(final String title, final String subtitle, final UriString url)
		Link(const std::string& title, const std::string& subtitle, const std::string& url);

		// Port of: public void onClicked(final BaseActivity activity)
		// Stub: Android-specific
		void onClicked() const;

		// Port of: public void onLongClicked(final BaseActivity activity)
		// Stub: Android-specific
		void onLongClicked() const;
	};

	// Port of: final CharArrSubstring raw;
	CharArrSubstring raw;
	// Port of: final MarkdownParagraph parent;
	const MarkdownParagraph* parent;
	// Port of: final MarkdownParser.MarkdownParagraphType type;
	MarkdownParser::MarkdownParagraphType type;
	// Port of: final int[] tokens;
	std::vector<int> tokens;
	// Port of: final int level;
	int level;
	// Port of: final int number;
	int number;

	// Port of: final Spanned spanned;
	// Stub: rendered text string (platform-independent equivalent)
	std::string spanned;
	// Port of: final List<Link> links;
	std::vector<Link> links;

	// Port of: public MarkdownParagraph(...)
	MarkdownParagraph(
			CharArrSubstring raw,
			const MarkdownParagraph* parent,
			MarkdownParser::MarkdownParagraphType type,
			const std::vector<int>& tokens,
			int level,
			int number);

	// Port of: public boolean isEmpty()
	bool isEmpty() const;

private:
	// Port of: private Spanned internalGenerateSpanned()
	std::string internalGenerateSpanned();

	// Port of: private static int indexOf(final int[] haystack, final int needle, final int startPos)
	static int indexOf(const std::vector<int>& haystack, int needle, int startPos);
};

} // namespace PinkReader
