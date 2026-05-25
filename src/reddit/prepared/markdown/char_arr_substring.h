/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: char_arr_substring.h
 * Port of: CharArrSubstring.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/markdown/CharArrSubstring.java
 *
 * A view into a char array with start offset and length. Every method
 * ported exactly.
 */

#pragma once

#include <string>
#include <vector>

namespace PinkReader {

class MarkdownTokenizer; // forward decl for replaceUnicodeSpaces

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.markdown.CharArrSubstring
 *
 * Represents a substring view over a mutable char buffer.
 */
class CharArrSubstring {
public:
	// Port of: final char[] arr;
	char* arr;
	// Port of: final int start;
	int start;
	// Port of: public final int length;
	int length;

	// Port of: CharArrSubstring(final char[] arr, final int start, final int length)
	CharArrSubstring(char* arr, int start, int length);

	// Port of: public static CharArrSubstring generate(final char[] src)
	static CharArrSubstring generate(char* src, int srcLen);

	// Port of: public static CharArrSubstring[] generateFromLines(final char[] src)
	// Returns vector; callers must manage memory
	static std::vector<CharArrSubstring> generateFromLines(char* src, int srcLen);

	// Port of: public CharArrSubstring rejoin(final CharArrSubstring toAppend)
	CharArrSubstring rejoin(const CharArrSubstring& toAppend) const;

	// Port of: public int countSpacesAtStart()
	int countSpacesAtStart() const;

	// Port of: public int countSpacesAtEnd()
	int countSpacesAtEnd() const;

	// Port of: public char charAt(final int index)
	char charAt(int index) const;

	// Port of: public int countPrefixLengthIgnoringSpaces(final char c)
	int countPrefixLengthIgnoringSpaces(char c) const;

	// Port of: public int countPrefixLevelIgnoringSpaces(final char c)
	int countPrefixLevelIgnoringSpaces(char c) const;

	// Port of: public CharArrSubstring left(final int chars)
	CharArrSubstring left(int chars) const;

	// Port of: public CharArrSubstring substring(final int start)
	CharArrSubstring substring(int start) const;

	// Port of: public CharArrSubstring substring(final int start, final int len)
	CharArrSubstring substring(int start, int len) const;

	// Port of: public CharArrSubstring readInteger(final int start)
	CharArrSubstring readInteger(int start) const;

	// Port of: @Override public String toString()
	std::string toString() const;

	// Port of: public boolean isRepeatingChar(final char c, final int start, final int len)
	bool isRepeatingChar(char c, int start, int len) const;

	// Port of: public boolean equalAt(final int position, final String needle)
	bool equalAt(int position, const std::string& needle) const;

	// Port of: public void replaceUnicodeSpaces()
	void replaceUnicodeSpaces();

private:
	// Port of: private static int indexOfLinebreak(final char[] raw, final int startPos)
	static int indexOfLinebreak(const char* raw, int rawLen, int startPos);
};

} // namespace PinkReader
