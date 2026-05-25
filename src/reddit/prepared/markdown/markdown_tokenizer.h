/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_tokenizer.h
 * Port of: MarkdownTokenizer.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/markdown/MarkdownTokenizer.java
 *
 * Every field, method, constant, static initializer, and logic branch ported exactly.
 */

#pragma once

#include "char_arr_substring.h"
#include "int_array_length_pair.h"

#include <vector>
#include <unordered_set>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.markdown.MarkdownTokenizer
 *
 * Tokenizes markdown text into a stream of special tokens and raw character
 * codes. Manages markdown semantics like bold, italic, strikethrough, links,
 * and unicode entity decoding.
 */
class MarkdownTokenizer {
public:
	// Port of: public static final int TOKEN_UNDERSCORE = -1;
	static constexpr int TOKEN_UNDERSCORE = -1;
	// Port of: public static final int TOKEN_UNDERSCORE_DOUBLE = -2;
	static constexpr int TOKEN_UNDERSCORE_DOUBLE = -2;
	// Port of: public static final int TOKEN_ASTERISK = -3;
	static constexpr int TOKEN_ASTERISK = -3;
	// Port of: public static final int TOKEN_ASTERISK_DOUBLE = -4;
	static constexpr int TOKEN_ASTERISK_DOUBLE = -4;
	// Port of: public static final int TOKEN_TILDE_DOUBLE = -5;
	static constexpr int TOKEN_TILDE_DOUBLE = -5;
	// Port of: public static final int TOKEN_CARET = -6;
	static constexpr int TOKEN_CARET = -6;
	// Port of: public static final int TOKEN_GRAVE = -7;
	static constexpr int TOKEN_GRAVE = -7;
	// Port of: public static final int TOKEN_BRACKET_SQUARE_OPEN = -8;
	static constexpr int TOKEN_BRACKET_SQUARE_OPEN = -8;
	// Port of: public static final int TOKEN_BRACKET_SQUARE_CLOSE = -9;
	static constexpr int TOKEN_BRACKET_SQUARE_CLOSE = -9;
	// Port of: public static final int TOKEN_PAREN_OPEN = -10;
	static constexpr int TOKEN_PAREN_OPEN = -10;
	// Port of: public static final int TOKEN_PAREN_CLOSE = -11;
	static constexpr int TOKEN_PAREN_CLOSE = -11;
	// Port of: public static final int TOKEN_UNICODE_OPEN = -12;
	static constexpr int TOKEN_UNICODE_OPEN = -12;
	// Port of: public static final int TOKEN_UNICODE_CLOSE = -13;
	static constexpr int TOKEN_UNICODE_CLOSE = -13;

	// Port of: public static boolean isUnicodeWhitespace(final int codepoint)
	static bool isUnicodeWhitespace(int codepoint);

	// Port of: public static IntArrayLengthPair tokenize(final CharArrSubstring input)
	static IntArrayLengthPair tokenize(const CharArrSubstring& input);

	// Port of: public static void clean(
	//     final IntArrayLengthPair input, final IntArrayLengthPair output)
	static void clean(IntArrayLengthPair& input, IntArrayLengthPair& output);

private:
	// Port of: private static final char[][] reverseLookup = new char[20][];
	static std::vector<std::vector<char>> reverseLookup;

	// Port of: private static final char[][] linkPrefixes
	static std::vector<std::string> linkPrefixes;

	// Port of: private static final char[][] linkPrefixes_reddit
	static std::vector<std::string> linkPrefixes_reddit;

	// Port of: private static final HashSet<Integer> unicodeWhitespace
	static std::unordered_set<int> unicodeWhitespace;

	// Port of: static { ... } initializer
	static void initStatics();
	static bool staticsInitialized;

	// Port of: private static void linkify(...)
	static void linkify(IntArrayLengthPair& input, IntArrayLengthPair& output);

	// Port of: public static void naiveTokenize(...)
	static void naiveTokenize(const IntArrayLengthPair& input, IntArrayLengthPair& output);

	// Port of: private static int indexOf(...)
	static int indexOf(const int* haystack, int needle, int startInclusive, int endExclusive);

	// Port of: public static int findCloseWellBracketed(...)
	static int findCloseWellBracketed(
			const int* haystack,
			int openBracket,
			int closeBracket,
			int startInclusive,
			int endExclusive);

	// Port of: private static boolean isSpaces(...)
	static bool isSpaces(const int* haystack, int startInclusive, int endExclusive);

	// Port of: private static boolean isDigits(...)
	static bool isDigits(const int* haystack, int startInclusive, int endExclusive);

	// Port of: private static boolean isHexDigits(...)
	static bool isHexDigits(const int* haystack, int startInclusive, int endExclusive);

	// Port of: private static int getDecimal(...)
	static int getDecimal(const int* chars, int startInclusive, int endExclusive);

	// Port of: private static int fromHex(final int ch)
	static int fromHex(int ch);

	// Port of: private static int getHex(...)
	static int getHex(const int* chars, int startInclusive, int endExclusive);

	// Port of: private static boolean equals(...)
	static bool equals(const int* haystack, const char* needle, int needleLen, int startInclusive);

	// Port of: private static int getLinkStartType(...)
	static int getLinkStartType(const int* haystack, int startInclusive, int endExclusive);

	// Port of: private static int getRedditLinkStartType(...)
	static int getRedditLinkStartType(const int* haystack, int startInclusive, int endExclusive);

	// Port of: private static int[] revert(...)
	static std::vector<int> revert(const int* tokens, int startInclusive, int endExclusive);

	// Port of: private static int findParenClosePos(...)
	static int findParenClosePos(const IntArrayLengthPair& tokens, int startPos);

	// Port of: private static int indexOfIgnoreEscaped(...)
	static int indexOfIgnoreEscaped(const IntArrayLengthPair& haystack, int needle, int startPos);

	// Ensure statics are initialized on first use
	static void ensureInit();
};

} // namespace PinkReader
