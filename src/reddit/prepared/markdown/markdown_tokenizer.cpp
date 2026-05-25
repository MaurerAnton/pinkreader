/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_tokenizer.cpp
 * Port of: MarkdownTokenizer.java
 *
 * Every method, static field, initializer block, constant, and logic branch
 * ported exactly, line-by-line.
 */

#include "markdown_tokenizer.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <cctype>

namespace PinkReader {

// ===== Static field definitions =====

// Port of: private static final char[][] reverseLookup = new char[20][];
std::vector<std::vector<char>> MarkdownTokenizer::reverseLookup(20);

// Port of: private static final char[][] linkPrefixes
std::vector<std::string> MarkdownTokenizer::linkPrefixes;

// Port of: private static final char[][] linkPrefixes_reddit
std::vector<std::string> MarkdownTokenizer::linkPrefixes_reddit;

// Port of: private static final HashSet<Integer> unicodeWhitespace
std::unordered_set<int> MarkdownTokenizer::unicodeWhitespace;

bool MarkdownTokenizer::staticsInitialized = false;

// ===== initStatics =====
// Port of: static { ... }
// Initializes reverseLookup, linkPrefixes, unicodeWhitespace
void MarkdownTokenizer::initStatics() {
	if(staticsInitialized) return;
	staticsInitialized = true;

	// Port of:
	//   reverseLookup[20 + TOKEN_UNDERSCORE] = new char[] {'_'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_UNDERSCORE)] = {'_'};
	//   reverseLookup[20 + TOKEN_UNDERSCORE_DOUBLE] = new char[] {'_', '_'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_UNDERSCORE_DOUBLE)] = {'_', '_'};
	//   reverseLookup[20 + TOKEN_ASTERISK] = new char[] {'*'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_ASTERISK)] = {'*'};
	//   reverseLookup[20 + TOKEN_ASTERISK_DOUBLE] = new char[] {'*', '*'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_ASTERISK_DOUBLE)] = {'*', '*'};
	//   reverseLookup[20 + TOKEN_TILDE_DOUBLE] = new char[] {'~', '~'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_TILDE_DOUBLE)] = {'~', '~'};
	//   reverseLookup[20 + TOKEN_CARET] = new char[] {'^'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_CARET)] = {'^'};
	//   reverseLookup[20 + TOKEN_GRAVE] = new char[] {'`'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_GRAVE)] = {'`'};
	//   reverseLookup[20 + TOKEN_BRACKET_SQUARE_OPEN] = new char[] {'['};
	reverseLookup[static_cast<size_t>(20 + TOKEN_BRACKET_SQUARE_OPEN)] = {'['};
	//   reverseLookup[20 + TOKEN_BRACKET_SQUARE_CLOSE] = new char[] {']'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_BRACKET_SQUARE_CLOSE)] = {']'};
	//   reverseLookup[20 + TOKEN_PAREN_OPEN] = new char[] {'('};
	reverseLookup[static_cast<size_t>(20 + TOKEN_PAREN_OPEN)] = {'('};
	//   reverseLookup[20 + TOKEN_PAREN_CLOSE] = new char[] {')'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_PAREN_CLOSE)] = {')'};
	//   reverseLookup[20 + TOKEN_UNICODE_OPEN] = new char[] {'&'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_UNICODE_OPEN)] = {'&'};
	//   reverseLookup[20 + TOKEN_UNICODE_CLOSE] = new char[] {';'};
	reverseLookup[static_cast<size_t>(20 + TOKEN_UNICODE_CLOSE)] = {';'};

	// linkPrefixes = { "http://", "https://", "www." }
	linkPrefixes.push_back("http://");
	linkPrefixes.push_back("https://");
	linkPrefixes.push_back("www.");

	// linkPrefixes_reddit = { "/r/", "r/", "/u/", "u/", "/user/" }
	linkPrefixes_reddit.push_back("/r/");
	linkPrefixes_reddit.push_back("r/");
	linkPrefixes_reddit.push_back("/u/");
	linkPrefixes_reddit.push_back("u/");
	linkPrefixes_reddit.push_back("/user/");

	// unicodeWhitespace.add(0x0009);
	unicodeWhitespace.insert(0x0009);
	unicodeWhitespace.insert(0x000B);
	unicodeWhitespace.insert(0x00A0);
	unicodeWhitespace.insert(0x1680);
	unicodeWhitespace.insert(0x2000);
	unicodeWhitespace.insert(0x2001);
	unicodeWhitespace.insert(0x2002);
	unicodeWhitespace.insert(0x2003);
	unicodeWhitespace.insert(0x2004);
	unicodeWhitespace.insert(0x2005);
	unicodeWhitespace.insert(0x2006);
	unicodeWhitespace.insert(0x2007);
	unicodeWhitespace.insert(0x2008);
	unicodeWhitespace.insert(0x2009);
	unicodeWhitespace.insert(0x200A);
	unicodeWhitespace.insert(0x202F);
	unicodeWhitespace.insert(0x205F);
	unicodeWhitespace.insert(0x3000);
}

// ===== ensureInit =====
void MarkdownTokenizer::ensureInit() {
	initStatics();
}

// ===== isUnicodeWhitespace =====
// Port of:
//   public static boolean isUnicodeWhitespace(final int codepoint) {
//       return unicodeWhitespace.contains(codepoint);
//   }
bool MarkdownTokenizer::isUnicodeWhitespace(int codepoint) {
	ensureInit();
	return unicodeWhitespace.find(codepoint) != unicodeWhitespace.end();
}

// ===== tokenize =====
// Port of:
//   public static IntArrayLengthPair tokenize(final CharArrSubstring input) {
//       final IntArrayLengthPair tmp1 = new IntArrayLengthPair(input.length * 3);
//       final IntArrayLengthPair tmp2 = new IntArrayLengthPair(input.length * 3);
//       tmp1.pos = input.length;
//       for(int i = 0; i < input.length; i++) { tmp1.data[i] = input.charAt(i); }
//       naiveTokenize(tmp1, tmp2);
//       clean(tmp2, tmp1);
//       linkify(tmp1, tmp2);
//       clean(tmp2, tmp1);
//       return tmp1;
//   }
IntArrayLengthPair MarkdownTokenizer::tokenize(const CharArrSubstring& input) {
	ensureInit();

	IntArrayLengthPair tmp1(input.length * 3);
	IntArrayLengthPair tmp2(input.length * 3);

	tmp1.pos = input.length;
	for(int i = 0; i < input.length; i++) {
		tmp1.data[i] = static_cast<int>(static_cast<unsigned char>(input.charAt(i)));
	}

	// Markdown is evil.
	naiveTokenize(tmp1, tmp2);
	clean(tmp2, tmp1);
	linkify(tmp1, tmp2);
	clean(tmp2, tmp1);

	return tmp1;
}

// ===== naiveTokenize =====
// Port of:
//   public static void naiveTokenize(final IntArrayLengthPair input, final IntArrayLengthPair output) { ... }
void MarkdownTokenizer::naiveTokenize(const IntArrayLengthPair& input, IntArrayLengthPair& output) {
	output.clear();

	for(int i = 0; i < input.pos; i++) {

		const int c = input.data[i];

		switch(c) {

			case '*':
				// if(i < input.pos - 1 && input.data[i + 1] == '*') {
				if(i < input.pos - 1 && input.data[i + 1] == '*') {
					i++;
					output.data[output.pos++] = TOKEN_ASTERISK_DOUBLE;
				} else {
					output.data[output.pos++] = TOKEN_ASTERISK;
				}
				break;

			case '_':
				// if(i < input.pos - 1 && input.data[i + 1] == '_') {
				if(i < input.pos - 1 && input.data[i + 1] == '_') {
					i++;
					output.data[output.pos++] = TOKEN_UNDERSCORE_DOUBLE;
				} else {
					// if((i < input.pos - 1 && input.data[i + 1] == ' ')
					//     || (i > 0 && input.data[i - 1] == ' ')
					//     || (i == 0) || (i == input.pos - 1)) {
					if((i < input.pos - 1 && input.data[i + 1] == ' ')
							|| (i > 0 && input.data[i - 1] == ' ')
							|| (i == 0) || (i == input.pos - 1)) {
						output.data[output.pos++] = TOKEN_UNDERSCORE;
					} else {
						output.data[output.pos++] = c;
					}
				}
				break;

			case '~':
				// if(i < input.pos - 1 && input.data[i + 1] == '~') {
				if(i < input.pos - 1 && input.data[i + 1] == '~') {
					i++;
					output.data[output.pos++] = TOKEN_TILDE_DOUBLE;
				} else {
					output.data[output.pos++] = '~';
				}
				break;

			case '^':
				output.data[output.pos++] = TOKEN_CARET;
				break;

			case '`':
				output.data[output.pos++] = TOKEN_GRAVE;
				break;

			case '[':
				output.data[output.pos++] = TOKEN_BRACKET_SQUARE_OPEN;
				break;

			case ']':
				output.data[output.pos++] = TOKEN_BRACKET_SQUARE_CLOSE;
				break;

			case '(':
				output.data[output.pos++] = TOKEN_PAREN_OPEN;
				break;

			case ')':
				output.data[output.pos++] = TOKEN_PAREN_CLOSE;
				break;

			case '&':
				output.data[output.pos++] = TOKEN_UNICODE_OPEN;
				break;

			case ';':
				output.data[output.pos++] = TOKEN_UNICODE_CLOSE;
				break;

			case '\\':
				// if(i < input.pos - 1) {
				if(i < input.pos - 1) {
					output.data[output.pos++] = input.data[++i];
				} else {
					output.data[output.pos++] = '\\';
				}
				break;

			case '\t':
			case '\r':
			case '\f':
			case '\n':
				output.data[output.pos++] = ' ';
				break;

			default:
				output.data[output.pos++] = c;
				break;
		}
	}
}

// ===== indexOf (private static) =====
// Port of:
//   private static int indexOf(final int[] haystack, final int needle,
//       final int startInclusive, final int endExclusive) {
//       for(int i = startInclusive; i < endExclusive; i++) {
//           if(haystack[i] == needle) { return i; }
//       }
//       return -1;
//   }
int MarkdownTokenizer::indexOf(const int* haystack, int needle, int startInclusive, int endExclusive) {
	for(int i = startInclusive; i < endExclusive; i++) {
		if(haystack[i] == needle) {
			return i;
		}
	}
	return -1;
}

// ===== findCloseWellBracketed =====
// Port of:
//   public static int findCloseWellBracketed(final int[] haystack,
//       final int openBracket, final int closeBracket,
//       final int startInclusive, final int endExclusive) { ... }
int MarkdownTokenizer::findCloseWellBracketed(
		const int* haystack,
		int openBracket,
		int closeBracket,
		int startInclusive,
		int endExclusive) {

	if(haystack[startInclusive] != openBracket) {
		throw std::runtime_error("Internal markdown parser error");
	}

	int b = 1;

	for(int i = startInclusive + 1; i < endExclusive; i++) {
		if(haystack[i] == openBracket) {
			b++;
		} else if(haystack[i] == closeBracket) {
			b--;
		}

		if(b == 0) {
			return i;
		}
	}

	return -1;
}

// ===== isSpaces =====
// Port of:
//   private static boolean isSpaces(final int[] haystack,
//       final int startInclusive, final int endExclusive) {
//       for(int i = startInclusive; i < endExclusive; i++) {
//           if(haystack[i] != ' ') { return false; }
//       }
//       return true;
//   }
bool MarkdownTokenizer::isSpaces(const int* haystack, int startInclusive, int endExclusive) {
	for(int i = startInclusive; i < endExclusive; i++) {
		if(haystack[i] != ' ') {
			return false;
		}
	}
	return true;
}

// ===== isDigits =====
// Port of:
//   private static boolean isDigits(final int[] haystack,
//       final int startInclusive, final int endExclusive) {
//       for(int i = startInclusive; i < endExclusive; i++) {
//           if(haystack[i] < '0' || haystack[i] > '9') { return false; }
//       }
//       return true;
//   }
bool MarkdownTokenizer::isDigits(const int* haystack, int startInclusive, int endExclusive) {
	for(int i = startInclusive; i < endExclusive; i++) {
		if(haystack[i] < '0' || haystack[i] > '9') {
			return false;
		}
	}
	return true;
}

// ===== isHexDigits =====
// Port of:
//   private static boolean isHexDigits(final int[] haystack,
//       final int startInclusive, final int endExclusive) {
//       for(int i = startInclusive; i < endExclusive; i++) {
//           final int c = haystack[i];
//           if((c < '0' || c > '9') && (c < 'a' || c > 'f') && (c < 'A' || c > 'F')) {
//               return false;
//           }
//       }
//       return true;
//   }
bool MarkdownTokenizer::isHexDigits(const int* haystack, int startInclusive, int endExclusive) {
	for(int i = startInclusive; i < endExclusive; i++) {
		const int c = haystack[i];
		if((c < '0' || c > '9') && (c < 'a' || c > 'f') && (c < 'A' || c > 'F')) {
			return false;
		}
	}
	return true;
}

// ===== getDecimal =====
// Port of:
//   private static int getDecimal(final int[] chars,
//       final int startInclusive, final int endExclusive) {
//       int result = 0;
//       for(int i = startInclusive; i < endExclusive; i++) {
//           result *= 10;
//           result += chars[i] - '0';
//       }
//       return result;
//   }
int MarkdownTokenizer::getDecimal(const int* chars, int startInclusive, int endExclusive) {
	int result = 0;
	for(int i = startInclusive; i < endExclusive; i++) {
		result *= 10;
		result += chars[i] - '0';
	}
	return result;
}

// ===== fromHex =====
// Port of:
//   private static int fromHex(final int ch) {
//       if(ch >= '0' && ch <= '9') { return ch - '0'; }
//       if(ch >= 'a' && ch <= 'f') { return 10 + ch - 'a'; }
//       return 10 + ch - 'A';
//   }
int MarkdownTokenizer::fromHex(int ch) {
	if(ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	if(ch >= 'a' && ch <= 'f') {
		return 10 + ch - 'a';
	}
	return 10 + ch - 'A';
}

// ===== getHex =====
// Port of:
//   private static int getHex(final int[] chars,
//       final int startInclusive, final int endExclusive) {
//       int result = 0;
//       for(int i = startInclusive; i < endExclusive; i++) {
//           result *= 16;
//           result += fromHex(chars[i]);
//       }
//       return result;
//   }
int MarkdownTokenizer::getHex(const int* chars, int startInclusive, int endExclusive) {
	int result = 0;
	for(int i = startInclusive; i < endExclusive; i++) {
		result *= 16;
		result += fromHex(chars[i]);
	}
	return result;
}

// ===== equals =====
// Port of:
//   private static boolean equals(final int[] haystack, final char[] needle,
//       final int startInclusive) {
//       for(int i = 0; i < needle.length; i++) {
//           if(haystack[startInclusive + i] != needle[i]) { return false; }
//       }
//       return true;
//   }
bool MarkdownTokenizer::equals(const int* haystack, const char* needle, int needleLen, int startInclusive) {
	for(int i = 0; i < needleLen; i++) {
		if(haystack[startInclusive + i] != static_cast<int>(static_cast<unsigned char>(needle[i]))) {
			return false;
		}
	}
	return true;
}

// ===== getLinkStartType =====
// Port of:
//   private static int getLinkStartType(final int[] haystack,
//       final int startInclusive, final int endExclusive) { ... }
int MarkdownTokenizer::getLinkStartType(const int* haystack, int startInclusive, int endExclusive) {
	const int maxLen = endExclusive - startInclusive;
	for(int type = 0; type < static_cast<int>(linkPrefixes.size()); type++) {
		if(static_cast<int>(linkPrefixes[type].length()) <= maxLen
				&& equals(haystack, linkPrefixes[type].c_str(),
						static_cast<int>(linkPrefixes[type].length()), startInclusive)) {
			return type;
		}
	}
	return -1;
}

// ===== getRedditLinkStartType =====
// Port of:
//   private static int getRedditLinkStartType(final int[] haystack,
//       final int startInclusive, final int endExclusive) { ... }
int MarkdownTokenizer::getRedditLinkStartType(const int* haystack, int startInclusive, int endExclusive) {
	const int maxLen = endExclusive - startInclusive;
	for(int type = 0; type < static_cast<int>(linkPrefixes_reddit.size()); type++) {
		if(static_cast<int>(linkPrefixes_reddit[type].length()) <= maxLen
				&& equals(haystack, linkPrefixes_reddit[type].c_str(),
						static_cast<int>(linkPrefixes_reddit[type].length()), startInclusive)) {
			return type;
		}
	}
	return -1;
}

// ===== revert =====
// Port of:
//   private static int[] revert(final int[] tokens,
//       final int startInclusive, final int endExclusive) { ... }
std::vector<int> MarkdownTokenizer::revert(const int* tokens, int startInclusive, int endExclusive) {

	int outputLen = 0;

	// for(int i = startInclusive; i < endExclusive; i++) {
	//     final int token = tokens[i];
	//     if(token < 0) { outputLen += reverseLookup[20 + token].length; }
	//     else { outputLen++; }
	// }
	for(int i = startInclusive; i < endExclusive; i++) {
		const int token = tokens[i];
		if(token < 0) {
			outputLen += static_cast<int>(reverseLookup[static_cast<size_t>(20 + token)].size());
		} else {
			outputLen++;
		}
	}

	// final int[] result = new int[outputLen];
	std::vector<int> result(static_cast<size_t>(outputLen));
	int resultPos = 0;

	// for(int i = startInclusive; i < endExclusive; i++) {
	//     final int token = tokens[i];
	//     if(token < 0) {
	//         for(final char c : reverseLookup[20 + token]) {
	//             result[resultPos++] = c;
	//         }
	//     } else {
	//         result[resultPos++] = token;
	//     }
	// }
	for(int i = startInclusive; i < endExclusive; i++) {
		const int token = tokens[i];
		if(token < 0) {
			const auto& chars = reverseLookup[static_cast<size_t>(20 + token)];
			for(char c : chars) {
				result[resultPos++] = static_cast<int>(static_cast<unsigned char>(c));
			}
		} else {
			result[resultPos++] = token;
		}
	}

	return result;
}

// ===== findParenClosePos =====
// Port of:
//   private static int findParenClosePos(final IntArrayLengthPair tokens, final int startPos) {
//       for(int i = startPos; i < tokens.pos; i++) {
//           switch(tokens.data[i]) {
//               case TOKEN_PAREN_CLOSE: return i;
//               case '"': i = indexOfIgnoreEscaped(tokens, '"', i + 1);
//                   if(i < 0) { return -1; } break;
//           }
//       }
//       return -1;
//   }
int MarkdownTokenizer::findParenClosePos(const IntArrayLengthPair& tokens, int startPos) {
	for(int i = startPos; i < tokens.pos; i++) {
		switch(tokens.data[i]) {
			case TOKEN_PAREN_CLOSE:
				return i;

			case '"':
				i = indexOfIgnoreEscaped(tokens, '"', i + 1);
				if(i < 0) {
					return -1;
				}
				break;
		}
	}
	return -1;
}

// ===== indexOfIgnoreEscaped =====
// Port of:
//   private static int indexOfIgnoreEscaped(final IntArrayLengthPair haystack,
//       final int needle, final int startPos) {
//       for(int i = startPos; i < haystack.pos; i++) {
//           if(haystack.data[i] == '\\') { i++; }
//           else if(haystack.data[i] == needle) { return i; }
//       }
//       return -1;
//   }
int MarkdownTokenizer::indexOfIgnoreEscaped(const IntArrayLengthPair& haystack, int needle, int startPos) {
	for(int i = startPos; i < haystack.pos; i++) {
		if(haystack.data[i] == '\\') {
			i++;
		} else if(haystack.data[i] == needle) {
			return i;
		}
	}
	return -1;
}

// ===== linkify =====
// Port of:
//   private static void linkify(final IntArrayLengthPair input,
//       final IntArrayLengthPair output) { ... }
void MarkdownTokenizer::linkify(IntArrayLengthPair& input, IntArrayLengthPair& output) {

	if(input.data != nullptr && output.data != nullptr
			&& input.pos > output.pos * 3) {
		throw std::runtime_error("linkify buffer overflow");
	}
	output.clear();

	int inBrackets = 0;
	bool lastCharOk = true;

	for(int i = 0; i < input.pos; i++) {

		const int token = input.data[i];

		switch(token) {

			case TOKEN_BRACKET_SQUARE_OPEN:
			case TOKEN_PAREN_OPEN:
				output.data[output.pos++] = token;
				inBrackets++;
				lastCharOk = true;
				break;

			case TOKEN_BRACKET_SQUARE_CLOSE:
			case TOKEN_PAREN_CLOSE:
				output.data[output.pos++] = token;
				inBrackets--;
				lastCharOk = true;
				break;

			case ' ':
				output.data[output.pos++] = ' ';
				lastCharOk = true;
				break;

			case 'h':
			case 'w': {

				if(inBrackets == 0 && lastCharOk) {

					const int linkStartType =
							getLinkStartType(input.data, i, input.pos);
					if(linkStartType >= 0) {

						// Greedily read to space, or <>, or etc
						const int linkStartPos = i;
						const int linkPrefixEndPos =
								static_cast<int>(linkPrefixes[linkStartType].length()) + linkStartPos;
						int linkEndPos = linkPrefixEndPos;

						bool hasOpeningParen = false;

						while(linkEndPos < input.pos) {

							const int lToken = input.data[linkEndPos];

							const bool isValidChar =
									lToken != ' '
										&& lToken != '<'
										&& lToken != '>'
										&& lToken != TOKEN_GRAVE
										&& lToken != TOKEN_BRACKET_SQUARE_OPEN
										&& lToken != TOKEN_BRACKET_SQUARE_CLOSE;

							if(lToken == '(') {
								hasOpeningParen = true;
							}

							if(isValidChar) {
								linkEndPos++;
							} else {
								break;
							}
						}

						// discard many final chars if they are '.', ',', '?', ';' etc
						while(input.data[linkEndPos - 1] == '.'
								|| input.data[linkEndPos - 1] == ','
								|| input.data[linkEndPos - 1] == '?'
								|| input.data[linkEndPos - 1] == ';') {
							linkEndPos--;
						}

						// THEN, discard single final char if it is '\'', '"', etc
						if(input.data[linkEndPos - 1] == '"') {
							linkEndPos--;
						}

						if(input.data[linkEndPos - 1] == '\'') {
							linkEndPos--;
						}

						if(!hasOpeningParen && input.data[linkEndPos - 1] == ')') {
							linkEndPos--;
						}

						if(linkEndPos - linkPrefixEndPos >= 2) {

							const std::vector<int> reverted =
									revert(input.data, linkStartPos, linkEndPos);

							output.data[output.pos++] = TOKEN_BRACKET_SQUARE_OPEN;
							output.append(reverted.data(), static_cast<int>(reverted.size()));
							output.data[output.pos++] = TOKEN_BRACKET_SQUARE_CLOSE;
							output.data[output.pos++] = TOKEN_PAREN_OPEN;
							output.append(reverted.data(), static_cast<int>(reverted.size()));
							output.data[output.pos++] = TOKEN_PAREN_CLOSE;

							i = linkEndPos - 1;

						} else {
							output.data[output.pos++] = token;
						}

					} else {
						output.data[output.pos++] = token;
					}

				} else {
					output.data[output.pos++] = token;
				}

				lastCharOk = false;
				break;
			}

			case 'r':
			case 'u':
			case '/': {

				if(inBrackets == 0 && lastCharOk) {

					const int linkStartType =
							getRedditLinkStartType(input.data, i, input.pos);
					if(linkStartType >= 0) {

						const int linkStartPos = i;
						const int linkPrefixEndPos =
								static_cast<int>(linkPrefixes_reddit[linkStartType].length())
										+ linkStartPos;
						int linkEndPos = linkPrefixEndPos;

						while(linkEndPos < input.pos) {

							const int lToken = input.data[linkEndPos];

							const bool isValidChar =
									(lToken >= 'a' && lToken <= 'z')
											|| (lToken >= 'A' && lToken <= 'Z')
											|| (lToken >= '0' && lToken <= '9')
											|| lToken == '_'
											|| lToken == TOKEN_UNDERSCORE
											|| lToken == TOKEN_UNDERSCORE_DOUBLE
											|| lToken == '+'
											|| lToken == '-';

							if(isValidChar) {
								linkEndPos++;
							} else {
								break;
							}
						}

						if(linkEndPos - linkPrefixEndPos > 2) {

							const std::vector<int> reverted =
									revert(input.data, linkStartPos, linkEndPos);

							output.data[output.pos++] = TOKEN_BRACKET_SQUARE_OPEN;
							output.append(reverted.data(), static_cast<int>(reverted.size()));
							output.data[output.pos++] = TOKEN_BRACKET_SQUARE_CLOSE;
							output.data[output.pos++] = TOKEN_PAREN_OPEN;
							output.append(reverted.data(), static_cast<int>(reverted.size()));
							output.data[output.pos++] = TOKEN_PAREN_CLOSE;

							i = linkEndPos - 1;

						} else {
							output.data[output.pos++] = token;
						}

					} else {
						output.data[output.pos++] = token;
					}

				} else {
					output.data[output.pos++] = token;
				}

				lastCharOk = false;
				break;
			}

			default:
				// TODO test this against reddits impl
				// lastCharOk = token < 0 || (!Character.isLetterOrDigit(token));
				lastCharOk = (token < 0) || !std::isalnum(static_cast<unsigned char>(token));
				output.data[output.pos++] = token;
				break;
		}
	}
}

// ===== clean =====
// Port of:
//   public static void clean(final IntArrayLengthPair input,
//       final IntArrayLengthPair output) { ... }
void MarkdownTokenizer::clean(IntArrayLengthPair& input, IntArrayLengthPair& output) {
	ensureInit();

	// TODO use single byte array, flags
	// final boolean[] toRevert = new boolean[input.pos];
	std::vector<bool> toRevert(static_cast<size_t>(input.pos), false);
	// final boolean[] toDelete = new boolean[input.pos];
	std::vector<bool> toDelete(static_cast<size_t>(input.pos), false);

	int openingUnderscore = -1;
	int openingUnderscoreDouble = -1;
	int openingAsterisk = -1;
	int openingAsteriskDouble = -1;
	int openingTildeDouble = -1;

	int lastBracketSquareOpen = -1;

	for(int i = 0; i < input.pos; i++) {

		const int c = input.data[i];

		const bool beforeASpace = i + 1 < input.pos && input.data[i + 1] == ' ';
		const bool afterASpace = i > 0 && input.data[i - 1] == ' ';

		switch(c) {

			case TOKEN_UNDERSCORE: {

				if(openingUnderscore < 0) {
					// Opening underscore
					if(beforeASpace) {
						toRevert[i] = true;
					} else {
						openingUnderscore = i;
					}

				} else {
					// Closing underscore
					if(afterASpace) {
						toRevert[i] = true;
					} else {
						openingUnderscore = -1;
					}
				}

				break;
			}

			case TOKEN_UNDERSCORE_DOUBLE: {

				if(i != 0 && openingUnderscoreDouble == i - 1) {
					toRevert[openingUnderscoreDouble] = true;
					toRevert[i] = true;
					openingUnderscoreDouble = -1;

				} else {

					if(openingUnderscoreDouble < 0) {
						// Opening double underscore
						if(beforeASpace) {
							toRevert[i] = true;
						} else {
							openingUnderscoreDouble = i;
						}

					} else {
						// Closing double underscore
						if(afterASpace) {
							toRevert[i] = true;
						} else {
							openingUnderscoreDouble = -1;
						}
					}
				}

				break;
			}

			case TOKEN_ASTERISK: {

				if(openingAsterisk < 0) {
					// Opening asterisk
					if(beforeASpace) {
						toRevert[i] = true;
					} else {
						openingAsterisk = i;
					}

				} else {
					// Closing asterisk
					if(afterASpace) {
						toRevert[i] = true;
					} else {
						openingAsterisk = -1;
					}
				}

				break;
			}

			case TOKEN_ASTERISK_DOUBLE: {

				if(i != 0 && openingAsteriskDouble == i - 1) {
					toRevert[openingAsteriskDouble] = true;
					toRevert[i] = true;
					openingAsteriskDouble = -1;

				} else {

					if(openingAsteriskDouble < 0) {
						// Opening double asterisk
						if(beforeASpace) {
							toRevert[i] = true;
						} else {
							openingAsteriskDouble = i;
						}

					} else {
						// Closing double asterisk
						if(afterASpace) {
							toRevert[i] = true;
						} else {
							openingAsteriskDouble = -1;
						}
					}
				}

				break;
			}

			case TOKEN_TILDE_DOUBLE: {

				if(i != 0 && openingTildeDouble == i - 1) {
					toRevert[openingTildeDouble] = true;
					toRevert[i] = true;
					openingTildeDouble = -1;

				} else {

					if(openingTildeDouble < 0) {
						// Opening double tilde
						if(beforeASpace) {
							toRevert[i] = true;
						} else {
							openingTildeDouble = i;
						}

					} else {
						// Closing double tilde
						if(afterASpace) {
							toRevert[i] = true;
						} else {
							openingTildeDouble = -1;
						}
					}
				}

				break;
			}

			case TOKEN_GRAVE: {

				const int openingGrave = i;
				const int closingGrave =
						indexOf(input.data, TOKEN_GRAVE, i + 1, input.pos);

				if(closingGrave < 0) {
					toRevert[i] = true;
				} else {

					for(int j = openingGrave + 1; j < closingGrave; j++) {
						if(input.data[j] < 0) {
							toRevert[j] = true;
						}
					}

					i = closingGrave;
				}

				break;
			}

			case TOKEN_BRACKET_SQUARE_OPEN: {

				if(lastBracketSquareOpen < 0) {

					// Attempt to parse link text with well-bracketed square brackets
					const int closingSquareBracket = findCloseWellBracketed(
							input.data,
							TOKEN_BRACKET_SQUARE_OPEN,
							TOKEN_BRACKET_SQUARE_CLOSE,
							i,
							input.pos);

					if(closingSquareBracket > i) {

						const int parenOpenPos = indexOf(
								input.data,
								TOKEN_PAREN_OPEN,
								closingSquareBracket + 1,
								input.pos);

						if(parenOpenPos > closingSquareBracket
								&& isSpaces(
										input.data,
										closingSquareBracket + 1,
										parenOpenPos)) {

							lastBracketSquareOpen = i;

							for(int j = i + 1; j < closingSquareBracket; j++) {
								if(input.data[j] == TOKEN_BRACKET_SQUARE_OPEN) {
									input.data[j] = '[';
								} else if(input.data[j] == TOKEN_BRACKET_SQUARE_CLOSE) {
									input.data[j] = ']';
								}
							}

						} else {
							toRevert[i] = true;
						}

					} else {
						toRevert[i] = true;
					}

				} else {
					toRevert[lastBracketSquareOpen] = true;
					lastBracketSquareOpen = i;
				}
				break;
			}

			case TOKEN_BRACKET_SQUARE_CLOSE: {

				if(lastBracketSquareOpen < 0) {
					toRevert[i] = true;

				} else {

					const int lastBracketSquareClose = i;

					const int parenOpenPos = indexOf(input.data, TOKEN_PAREN_OPEN,
							lastBracketSquareClose + 1, input.pos);

					bool linkParseSuccess = false;

					if(parenOpenPos >= 0) {

						if(isSpaces(
								input.data,
								lastBracketSquareClose + 1,
								parenOpenPos)) {

							const int parenClosePos =
									findParenClosePos(input, parenOpenPos + 1);

							if(parenClosePos >= 0) {

								linkParseSuccess = true;

								for(int j = lastBracketSquareOpen + 1;
										j < lastBracketSquareClose;
										j++) {
									if(input.data[j] == TOKEN_BRACKET_SQUARE_OPEN
											|| input.data[j] == TOKEN_BRACKET_SQUARE_CLOSE) {
										toRevert[j] = true;
									}
								}

								for(int j = lastBracketSquareClose + 1;
										j < parenOpenPos;
										j++) {
									toDelete[j] = true;
								}

								for(int j = parenOpenPos + 1;
										j < parenClosePos;
										j++) {
									if(input.data[j] < 0) {
										toRevert[j] = true;
									} else if(input.data[j] == ' '
											&& input.data[j - 1] == ' ') {
										toDelete[j] = true;
									}
								}

								for(int j = parenOpenPos + 1;
										input.data[j] == ' ';
										j++) {
									toDelete[j] = true;
								}

								for(int j = parenClosePos - 1;
										input.data[j] == ' ';
										j--) {
									toDelete[j] = true;
								}

								i = parenClosePos;
							}
						}
					}

					if(!linkParseSuccess) {
						toRevert[lastBracketSquareOpen] = true;
						toRevert[lastBracketSquareClose] = true;
					}
				}

				lastBracketSquareOpen = -1;
				break;
			}

			case TOKEN_PAREN_OPEN:
			case TOKEN_PAREN_CLOSE:
			case TOKEN_UNICODE_CLOSE:
				toRevert[i] = true;
				break;

			case TOKEN_UNICODE_OPEN: {

				const int openingUnicode = i;
				const int closingUnicode =
						indexOf(input.data, TOKEN_UNICODE_CLOSE, i + 1,
								std::min(input.pos, i + 20));

				if(closingUnicode < 0) {
					toRevert[i] = true;

				} else if(input.data[i + 1] == '#') {

					if(input.data[i + 2] == 'x' && isHexDigits(
							input.data,
							openingUnicode + 3,
							closingUnicode)) {

						const int codePoint = getHex(
								input.data,
								openingUnicode + 3,
								closingUnicode);

						if(unicodeWhitespace.find(codePoint) != unicodeWhitespace.end()) {
							input.data[openingUnicode] = ' ';
						} else {
							input.data[openingUnicode] = codePoint;
						}

						for(int j = openingUnicode + 1; j <= closingUnicode; j++) {
							toDelete[j] = true;
						}

						i = closingUnicode;

					} else if(isDigits(
							input.data,
							openingUnicode + 2,
							closingUnicode)) {

						const int codePoint = getDecimal(
								input.data,
								openingUnicode + 2,
								closingUnicode);

						if(unicodeWhitespace.find(codePoint) != unicodeWhitespace.end()) {
							input.data[openingUnicode] = ' ';
						} else {
							input.data[openingUnicode] = codePoint;
						}

						for(int j = openingUnicode + 1; j <= closingUnicode; j++) {
							toDelete[j] = true;
						}

						i = closingUnicode;

					} else {
						toRevert[i] = true;
					}

				} else {

					// Named entity - try to resolve
					// The original uses StringEscapeUtils.unescapeHtml4()
					// We implement a minimal version for common cases

					std::string name;
					name.reserve(static_cast<size_t>(closingUnicode - openingUnicode - 1));
					for(int j = openingUnicode + 1; j < closingUnicode; j++) {
						name += static_cast<char>(input.data[j]);
					}

					int codePoint = -1;

					// Common HTML entities
					if(name == "amp") codePoint = '&';
					else if(name == "lt") codePoint = '<';
					else if(name == "gt") codePoint = '>';
					else if(name == "quot") codePoint = '"';
					else if(name == "apos") codePoint = '\'';
					else if(name == "nbsp") codePoint = ' ';
					else if(name == "nsub") codePoint = 0x2284; // ⊄

					if(codePoint >= 0) {

						if(unicodeWhitespace.find(codePoint) != unicodeWhitespace.end()) {
							input.data[openingUnicode] = ' ';
						} else {
							input.data[openingUnicode] = codePoint;
						}

						for(int j = openingUnicode + 1; j <= closingUnicode; j++) {
							toDelete[j] = true;
						}

						i = closingUnicode;

					} else {
						toRevert[i] = true;
					}
				}

				break;
			}

			case TOKEN_CARET: {

				if(input.pos <= i + 1 || input.data[i + 1] == ' ') {
					toRevert[i] = true;
				}

				break;
			}

			case ' ': {

				if(i < 1 || input.data[i - 1] == ' ') {
					toDelete[i] = true;
				}

				break;
			}
		}
	}

	// Close unmatched opening tokens
	if(openingUnderscore >= 0) {
		toRevert[openingUnderscore] = true;
	}
	if(openingUnderscoreDouble >= 0) {
		toRevert[openingUnderscoreDouble] = true;
	}
	if(openingAsterisk >= 0) {
		toRevert[openingAsterisk] = true;
	}
	if(openingAsteriskDouble >= 0) {
		toRevert[openingAsteriskDouble] = true;
	}
	if(openingTildeDouble >= 0) {
		toRevert[openingTildeDouble] = true;
	}
	if(lastBracketSquareOpen >= 0) {
		toRevert[lastBracketSquareOpen] = true;
	}

	// Delete trailing spaces
	for(int j = input.pos - 1; j >= 0 && input.data[j] == ' '; j--) {
		toDelete[j] = true;
	}

	output.clear();

	for(int i = 0; i < input.pos; i++) {

		if(toDelete[i]) {
			continue;
		}

		if(toRevert[i]) {

			const auto& revertTo = reverseLookup[static_cast<size_t>(20 + input.data[i])];
			output.append(revertTo.data(), static_cast<int>(revertTo.size()));

		} else {
			output.data[output.pos++] = input.data[i];
		}
	}
}

} // namespace PinkReader
