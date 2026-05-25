/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: char_arr_substring.cpp
 * Port of: CharArrSubstring.java
 *
 * Every method ported exactly, line-by-line.
 */

#include "char_arr_substring.h"
#include "markdown_tokenizer.h"

#include <vector>
#include <stdexcept>

namespace PinkReader {

// ===== Constructor =====
// Port of:
//   CharArrSubstring(final char[] arr, final int start, final int length) {
//       this.arr = arr;
//       this.start = start;
//       this.length = length;
//   }
CharArrSubstring::CharArrSubstring(char* arr, int start, int length)
	: arr(arr), start(start), length(length) {}

// ===== generate =====
// Port of:
//   public static CharArrSubstring generate(final char[] src) {
//       return new CharArrSubstring(src, 0, src.length);
//   }
CharArrSubstring CharArrSubstring::generate(char* src, int srcLen) {
	return CharArrSubstring(src, 0, srcLen);
}

// ===== indexOfLinebreak =====
// Port of:
//   private static int indexOfLinebreak(final char[] raw, final int startPos) {
//       for(int i = startPos; i < raw.length; i++) {
//           if(raw[i] == '\n') { return i; }
//       }
//       return -1;
//   }
int CharArrSubstring::indexOfLinebreak(const char* raw, int rawLen, int startPos) {
	for(int i = startPos; i < rawLen; i++) {
		if(raw[i] == '\n') {
			return i;
		}
	}
	return -1;
}

// ===== generateFromLines =====
// Port of:
//   public static CharArrSubstring[] generateFromLines(final char[] src) {
//       int curPos = 0;
//       final LinkedList<CharArrSubstring> result = new LinkedList<>();
//       int nextLinebreak;
//       while((nextLinebreak = indexOfLinebreak(src, curPos)) != -1) {
//           result.add(new CharArrSubstring(src, curPos, nextLinebreak - curPos));
//           curPos = nextLinebreak + 1;
//       }
//       result.add(new CharArrSubstring(src, curPos, src.length - curPos));
//       return result.toArray(new CharArrSubstring[0]);
//   }
std::vector<CharArrSubstring> CharArrSubstring::generateFromLines(char* src, int srcLen) {
	int curPos = 0;
	std::vector<CharArrSubstring> result;

	int nextLinebreak;
	while((nextLinebreak = indexOfLinebreak(src, srcLen, curPos)) != -1) {
		result.emplace_back(src, curPos, nextLinebreak - curPos);
		curPos = nextLinebreak + 1;
	}

	result.emplace_back(src, curPos, srcLen - curPos);

	return result;
}

// ===== rejoin =====
// Port of:
//   public CharArrSubstring rejoin(final CharArrSubstring toAppend) {
//       if(toAppend.start - 1 != start + length) {
//           throw new RuntimeException(...)
//       }
//       return new CharArrSubstring(arr, start, length + 1 + toAppend.length);
//   }
CharArrSubstring CharArrSubstring::rejoin(const CharArrSubstring& toAppend) const {
	if(toAppend.start - 1 != start + length) {
		throw std::runtime_error(
				"Internal error: attempt to join non-consecutive substrings");
	}
	return CharArrSubstring(arr, start, length + 1 + toAppend.length);
}

// ===== countSpacesAtStart =====
// Port of:
//   public int countSpacesAtStart() {
//       for(int i = 0; i < length; i++) {
//           if(arr[start + i] != ' ') { return i; }
//       }
//       return length;
//   }
int CharArrSubstring::countSpacesAtStart() const {
	for(int i = 0; i < length; i++) {
		if(arr[start + i] != ' ') {
			return i;
		}
	}
	return length;
}

// ===== countSpacesAtEnd =====
// Port of:
//   public int countSpacesAtEnd() {
//       for(int i = 0; i < length; i++) {
//           if(arr[start + length - 1 - i] != ' ') { return i; }
//       }
//       return length;
//   }
int CharArrSubstring::countSpacesAtEnd() const {
	for(int i = 0; i < length; i++) {
		if(arr[start + length - 1 - i] != ' ') {
			return i;
		}
	}
	return length;
}

// ===== charAt =====
// Port of:
//   public char charAt(final int index) {
//       return arr[start + index];
//   }
char CharArrSubstring::charAt(int index) const {
	return arr[start + index];
}

// ===== countPrefixLengthIgnoringSpaces =====
// Port of:
//   public int countPrefixLengthIgnoringSpaces(final char c) {
//       for(int i = 0; i < length; i++) {
//           if(arr[start + i] != ' ' && arr[start + i] != c) { return i; }
//       }
//       return length;
//   }
int CharArrSubstring::countPrefixLengthIgnoringSpaces(char c) const {
	for(int i = 0; i < length; i++) {
		if(arr[start + i] != ' ' && arr[start + i] != c) {
			return i;
		}
	}
	return length;
}

// ===== countPrefixLevelIgnoringSpaces =====
// Port of:
//   public int countPrefixLevelIgnoringSpaces(final char c) {
//       int level = 0;
//       for(int i = 0; i < length; i++) {
//           if(arr[start + i] != ' ' && arr[start + i] != c) {
//               return level;
//           } else if(arr[start + i] == c) {
//               level++;
//           }
//       }
//       return length;
//   }
int CharArrSubstring::countPrefixLevelIgnoringSpaces(char c) const {
	int level = 0;
	for(int i = 0; i < length; i++) {
		if(arr[start + i] != ' ' && arr[start + i] != c) {
			return level;
		} else if(arr[start + i] == c) {
			level++;
		}
	}
	return length;
}

// ===== left =====
// Port of:
//   public CharArrSubstring left(final int chars) {
//       return new CharArrSubstring(arr, start, chars);
//   }
CharArrSubstring CharArrSubstring::left(int chars) const {
	return CharArrSubstring(arr, start, chars);
}

// ===== substring (single arg) =====
// Port of:
//   public CharArrSubstring substring(final int start) {
//       return new CharArrSubstring(arr, this.start + start, length - start);
//   }
CharArrSubstring CharArrSubstring::substring(int startPos) const {
	return CharArrSubstring(arr, this->start + startPos, length - startPos);
}

// ===== substring (two args) =====
// Port of:
//   public CharArrSubstring substring(final int start, final int len) {
//       return new CharArrSubstring(arr, this.start + start, len);
//   }
CharArrSubstring CharArrSubstring::substring(int startPos, int len) const {
	return CharArrSubstring(arr, this->start + startPos, len);
}

// ===== readInteger =====
// Port of:
//   public CharArrSubstring readInteger(final int start) {
//       for(int i = start; i < length; i++) {
//           final char c = arr[this.start + i];
//           if(c < '0' || c > '9') {
//               return new CharArrSubstring(arr, this.start + start, i - start);
//           }
//       }
//       return new CharArrSubstring(arr, this.start + start, length - start);
//   }
CharArrSubstring CharArrSubstring::readInteger(int startPos) const {
	for(int i = startPos; i < length; i++) {
		const char c = arr[this->start + i];
		if(c < '0' || c > '9') {
			return CharArrSubstring(arr, this->start + startPos, i - startPos);
		}
	}
	return CharArrSubstring(arr, this->start + startPos, length - startPos);
}

// ===== toString =====
// Port of:
//   @Override public String toString() {
//       return new String(arr, start, length);
//   }
std::string CharArrSubstring::toString() const {
	return std::string(arr + start, static_cast<size_t>(length));
}

// ===== isRepeatingChar =====
// Port of:
//   public boolean isRepeatingChar(final char c, final int start, final int len) {
//       for(int i = 0; i < len; i++) {
//           if(arr[i + start + this.start] != c) { return false; }
//       }
//       return true;
//   }
bool CharArrSubstring::isRepeatingChar(char c, int startPos, int len) const {
	for(int i = 0; i < len; i++) {
		if(arr[i + startPos + this->start] != c) {
			return false;
		}
	}
	return true;
}

// ===== equalAt =====
// Port of:
//   public boolean equalAt(final int position, final String needle) {
//       if(length < position + needle.length()) { return false; }
//       for(int i = 0; i < needle.length(); i++) {
//           if(needle.charAt(i) != arr[start + position + i]) { return false; }
//       }
//       return true;
//   }
bool CharArrSubstring::equalAt(int position, const std::string& needle) const {
	if(length < position + static_cast<int>(needle.length())) {
		return false;
	}
	for(int i = 0; i < static_cast<int>(needle.length()); i++) {
		if(needle[i] != arr[start + position + i]) {
			return false;
		}
	}
	return true;
}

// ===== replaceUnicodeSpaces =====
// Port of:
//   public void replaceUnicodeSpaces() {
//       for(int i = 0; i < length; i++) {
//           if(MarkdownTokenizer.isUnicodeWhitespace(arr[start + i])) {
//               arr[start + i] = ' ';
//           }
//       }
//   }
void CharArrSubstring::replaceUnicodeSpaces() {
	for(int i = 0; i < length; i++) {
		if(MarkdownTokenizer::isUnicodeWhitespace(arr[start + i])) {
			arr[start + i] = ' ';
		}
	}
}

} // namespace PinkReader
