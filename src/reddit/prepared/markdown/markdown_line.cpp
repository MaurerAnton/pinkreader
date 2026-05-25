/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_line.cpp
 * Port of: MarkdownLine.java
 *
 * Every method ported exactly, line-by-line.
 */

#include "markdown_line.h"
#include "markdown_paragraph.h"
#include "markdown_tokenizer.h"
#include "int_array_length_pair.h"

#include <stdexcept>
#include <cstdlib>

namespace PinkReader {

// ===== Constructor =====
// Port of:
//   MarkdownLine(final CharArrSubstring src, final MarkdownParser.MarkdownParagraphType type,
//       final int spacesAtStart, final int spacesAtEnd, final int prefixLength,
//       final int level, final int number) {
//       this.src = src; ...
//   }
MarkdownLine::MarkdownLine(
		CharArrSubstring src,
		MarkdownParser::MarkdownParagraphType type,
		int spacesAtStart,
		int spacesAtEnd,
		int prefixLength,
		int level,
		int number)
	: src(src)
	, type(type)
	, spacesAtStart(spacesAtStart)
	, spacesAtEnd(spacesAtEnd)
	, prefixLength(prefixLength)
	, level(level)
	, number(number) {}

// ===== generate =====
// Port of:
//   public static MarkdownLine generate(final CharArrSubstring src) { ... }
MarkdownLine MarkdownLine::generate(const CharArrSubstring& src) {

	const int spacesAtStart = src.countSpacesAtStart();
	const int spacesAtEnd = src.countSpacesAtEnd();

	// if(spacesAtStart == src.length) {
	if(spacesAtStart == src.length) {
		// New paragraph
		// return new MarkdownLine(null, MarkdownParser.MarkdownParagraphType.EMPTY, 0, 0, 0, 0, 0);
		return MarkdownLine(
				CharArrSubstring(nullptr, 0, 0),
				MarkdownParser::MarkdownParagraphType::EMPTY,
				0, 0, 0, 0, 0);
	}

	// if(spacesAtStart >= 4) {
	if(spacesAtStart >= 4) {
		// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.CODE,
		//     spacesAtStart, spacesAtEnd, 4, 0, 0);
		return MarkdownLine(
				src,
				MarkdownParser::MarkdownParagraphType::CODE,
				spacesAtStart, spacesAtEnd, 4, 0, 0);
	}

	// final char firstNonSpaceChar = src.charAt(spacesAtStart);
	const char firstNonSpaceChar = src.charAt(spacesAtStart);

	switch(firstNonSpaceChar) {
		case '>': {
			// final int level = src.countPrefixLevelIgnoringSpaces('>');
			// final int prefixLen = src.countPrefixLengthIgnoringSpaces('>');
			const int level = src.countPrefixLevelIgnoringSpaces('>');
			const int prefixLen = src.countPrefixLengthIgnoringSpaces('>');

			// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.QUOTE,
			//     spacesAtStart, spacesAtEnd, prefixLen, level, 0);
			return MarkdownLine(
					src,
					MarkdownParser::MarkdownParagraphType::QUOTE,
					spacesAtStart, spacesAtEnd, prefixLen, level, 0);
		}

		case '-':
		case '*': {
			// if(src.length > spacesAtStart + 1 && src.charAt(spacesAtStart + 1) == ' ') {
			if(src.length > spacesAtStart + 1
					&& src.charAt(spacesAtStart + 1) == ' ') {

				// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.BULLET,
				//     spacesAtStart, spacesAtEnd, spacesAtStart + 2, spacesAtStart == 0 ? 0 : 1, 0);
				return MarkdownLine(
						src,
						MarkdownParser::MarkdownParagraphType::BULLET,
						spacesAtStart, spacesAtEnd,
						spacesAtStart + 2,
						spacesAtStart == 0 ? 0 : 1,
						0);

			} else if(src.length >= 3 && src.isRepeatingChar(
					'*',
					spacesAtStart,
					src.length - spacesAtEnd)) {
				// Horizontal rule: *** or --- (repeating char)
				return MarkdownLine(
						src,
						MarkdownParser::MarkdownParagraphType::HLINE,
						0, 0, 0, 0, 0);

			} else {
				// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.TEXT,
				//     spacesAtStart, spacesAtEnd, spacesAtStart, 0, 0);
				return MarkdownLine(
						src,
						MarkdownParser::MarkdownParagraphType::TEXT,
						spacesAtStart, spacesAtEnd, spacesAtStart, 0, 0);
			}
		}

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9': {
			// final CharArrSubstring num = src.readInteger(spacesAtStart);
			const CharArrSubstring num = src.readInteger(spacesAtStart);

			// if(src.length > spacesAtStart + num.length + 2
			//     && src.charAt(spacesAtStart + num.length) == '.'
			//     && src.charAt(spacesAtStart + num.length + 1) == ' ') {
			if(src.length > spacesAtStart + num.length + 2
					&& src.charAt(spacesAtStart + num.length) == '.'
					&& src.charAt(spacesAtStart + num.length + 1) == ' ') {

				// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.NUMBERED,
				//     spacesAtStart, spacesAtEnd, spacesAtStart + num.length + 2,
				//     spacesAtStart == 0 ? 0 : 1, Integer.parseInt(num.toString()));
				return MarkdownLine(
						src,
						MarkdownParser::MarkdownParagraphType::NUMBERED,
						spacesAtStart, spacesAtEnd,
						spacesAtStart + num.length + 2,
						spacesAtStart == 0 ? 0 : 1,
						std::atoi(num.toString().c_str()));

			} else {
				// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.TEXT,
				//     spacesAtStart, spacesAtEnd, spacesAtStart, 0, 0);
				return MarkdownLine(
						src,
						MarkdownParser::MarkdownParagraphType::TEXT,
						spacesAtStart, spacesAtEnd, spacesAtStart, 0, 0);
			}
		}

		case '#': {
			// TODO prefix and suffix length
			// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.HEADER,
			//     spacesAtStart, spacesAtEnd,
			//     src.countPrefixLengthIgnoringSpaces('#'), 0, 0);
			return MarkdownLine(
					src,
					MarkdownParser::MarkdownParagraphType::HEADER,
					spacesAtStart, spacesAtEnd,
					src.countPrefixLengthIgnoringSpaces('#'), 0, 0);
		}

		default: {
			// return new MarkdownLine(src, MarkdownParser.MarkdownParagraphType.TEXT,
			//     spacesAtStart, spacesAtEnd, spacesAtStart, 0, 0);
			return MarkdownLine(
					src,
					MarkdownParser::MarkdownParagraphType::TEXT,
					spacesAtStart, spacesAtEnd, spacesAtStart, 0, 0);
		}
	}
}

// ===== rejoin =====
// Port of:
//   public MarkdownLine rejoin(final MarkdownLine toAppend) {
//       src.arr[src.start + src.length] = ' ';
//       return new MarkdownLine(src.rejoin(toAppend.src), type,
//           spacesAtStart, toAppend.spacesAtEnd, prefixLength, level, number);
//   }
MarkdownLine MarkdownLine::rejoin(const MarkdownLine& toAppend) const {
	src.arr[src.start + src.length] = ' ';
	return MarkdownLine(
			src.rejoin(toAppend.src),
			type,
			spacesAtStart,
			toAppend.spacesAtEnd,
			prefixLength,
			level,
			number);
}

// ===== tokenize =====
// Port of:
//   public MarkdownParagraph tokenize(final MarkdownParagraph parent) {
//       final CharArrSubstring cleanedSrc =
//           prefixLength == 0 ? src : src.substring(prefixLength);
//       if(type != MarkdownParser.MarkdownParagraphType.CODE
//           && type != MarkdownParser.MarkdownParagraphType.HLINE) {
//           if(isPlainText()) {
//               return new MarkdownParagraph(cleanedSrc, parent, type, null, level, number);
//           } else {
//               final IntArrayLengthPair tokens = MarkdownTokenizer.tokenize(cleanedSrc);
//               return new MarkdownParagraph(cleanedSrc, parent, type,
//                   tokens.substringAsArray(0), level, number);
//           }
//       } else {
//           return new MarkdownParagraph(cleanedSrc, parent, type, null, level, number);
//       }
//   }
MarkdownParagraph MarkdownLine::tokenize(const MarkdownParagraph* parent) const {

	const CharArrSubstring cleanedSrc =
			prefixLength == 0 ? src : src.substring(prefixLength);

	if(type != MarkdownParser::MarkdownParagraphType::CODE
			&& type != MarkdownParser::MarkdownParagraphType::HLINE) {

		if(isPlainText()) {
			return MarkdownParagraph(
					cleanedSrc, parent, type,
					std::vector<int>(), // null tokens
					level, number);
		} else {
			IntArrayLengthPair tokens = MarkdownTokenizer::tokenize(cleanedSrc);
			std::vector<int> tokenVec = tokens.substringAsArray(0);
			return MarkdownParagraph(
					cleanedSrc, parent, type,
					tokenVec,
					level, number);
		}

	} else {
		return MarkdownParagraph(cleanedSrc, parent, type,
				std::vector<int>(), // null tokens
				level, number);
	}
}

// ===== isPlainText =====
// Port of:
//   private boolean isPlainText() {
//       for(int i = prefixLength; i < src.length; i++) {
//           switch(src.arr[i + src.start]) {
//               case '*': case '_': case '^': case '`': case '\\':
//               case '[': case '~': case '#': case '&':
//                   return false;
//               case '/':
//                   if(src.equalAt(i + 1, "u/") || src.equalAt(i + 1, "r/")) { return false; }
//                   break;
//               case 'h':
//                   if(src.equalAt(i + 1, "ttp://") || src.equalAt(i + 1, "ttps://")) { return false; }
//                   break;
//               case 'w':
//                   if(src.equalAt(i + 1, "ww.")) { return false; }
//                   break;
//               case 'r': case 'u':
//                   if(src.length > i + 1 && src.arr[src.start + i + 1] == '/') { return false; }
//                   break;
//               default:
//           }
//       }
//       return true;
//   }
bool MarkdownLine::isPlainText() const {

	for(int i = prefixLength; i < src.length; i++) {
		switch(src.arr[i + src.start]) {
			case '*':
			case '_':
			case '^':
			case '`':
			case '\\':
			case '[':
			case '~':
			case '#':
			case '&':
				return false;

			case '/':
				if(src.equalAt(i + 1, "u/") || src.equalAt(i + 1, "r/")) {
					return false;
				}
				break;

			case 'h':
				if(src.equalAt(i + 1, "ttp://") || src.equalAt(i + 1, "ttps://")) {
					return false;
				}
				break;

			case 'w':
				if(src.equalAt(i + 1, "ww.")) {
					return false;
				}
				break;

			case 'r':
			case 'u':
				if(src.length > i + 1 && src.arr[src.start + i + 1] == '/') {
					return false;
				}
				break;

			default:
				break;
		}
	}

	return true;
}

} // namespace PinkReader
