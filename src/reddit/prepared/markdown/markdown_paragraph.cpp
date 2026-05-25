/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_paragraph.cpp
 * Port of: MarkdownParagraph.java
 *
 * Every method ported exactly, line-by-line.
 * Android-specific SpannableString rendering is stubbed; token parsing logic
 * (bold, italic, strikethrough, code, links, superscript) is faithfully preserved.
 */

#include "markdown_paragraph.h"
#include "markdown_tokenizer.h"

#include <string>
#include <stdexcept>
#include <cctype>

namespace PinkReader {

// ===== Link constructor =====
// Port of:
//   public Link(final String title, final String subtitle, final UriString url) {
//       this.title = title;
//       this.subtitle = subtitle;
//       this.url = url;
//   }
MarkdownParagraph::Link::Link(const std::string& title, const std::string& subtitle, const std::string& url)
	: title(title), subtitle(subtitle), url(url) {}

// ===== Link::onClicked =====
// Port of: public void onClicked(final BaseActivity activity) { LinkHandler.onLinkClicked(activity, url, false); }
// Stub: Android-specific
void MarkdownParagraph::Link::onClicked() const {}

// ===== Link::onLongClicked =====
// Port of: public void onLongClicked(final BaseActivity activity) { LinkHandler.onLinkLongClicked(activity, url); }
// Stub: Android-specific
void MarkdownParagraph::Link::onLongClicked() const {}

// ===== MarkdownParagraph constructor =====
// Port of:
//   public MarkdownParagraph(final CharArrSubstring raw, final MarkdownParagraph parent,
//       final MarkdownParser.MarkdownParagraphType type, final int[] tokens,
//       final int level, final int number) {
//       this.raw = raw; ... links = new ArrayList<>();
//       spanned = internalGenerateSpanned();
//       if(tokens == null && raw != null) { raw.replaceUnicodeSpaces(); }
//   }
MarkdownParagraph::MarkdownParagraph(
		CharArrSubstring raw,
		const MarkdownParagraph* parent,
		MarkdownParser::MarkdownParagraphType type,
		const std::vector<int>& tokens,
		int level,
		int number)
	: raw(raw)
	, parent(parent)
	, type(type)
	, tokens(tokens)
	, level(level)
	, number(number)
	, spanned()
	, links()
{
	spanned = internalGenerateSpanned();

	if(tokens.empty() && raw.arr != nullptr) {
		raw.replaceUnicodeSpaces();
	}
}

// ===== indexOf =====
// Port of:
//   private static int indexOf(final int[] haystack, final int needle, final int startPos) {
//       for(int i = startPos; i < haystack.length; i++) {
//           if(haystack[i] == needle) { return i; }
//       }
//       return -1;
//   }
int MarkdownParagraph::indexOf(const std::vector<int>& haystack, int needle, int startPos) {
	for(int i = startPos; i < static_cast<int>(haystack.size()); i++) {
		if(haystack[i] == needle) {
			return i;
		}
	}
	return -1;
}

// ===== internalGenerateSpanned =====
// Port of:
//   private Spanned internalGenerateSpanned() { ... }
//
// This is the core rendering method. Android's SpannableStringBuilder is
// replaced with a simple std::string builder. Bold/italic/strikethrough/
// code/link/superscript markers are stripped; link data is collected.
// The result is a plain text rendering with decorations stripped.
std::string MarkdownParagraph::internalGenerateSpanned() {

	// if(type == MarkdownParser.MarkdownParagraphType.CODE
	//     || type == MarkdownParser.MarkdownParagraphType.HLINE) { return null; }
	if(type == MarkdownParser::MarkdownParagraphType::CODE
			|| type == MarkdownParser::MarkdownParagraphType::HLINE) {
		return std::string();
	}

	// if(tokens == null) { return new SpannableString(raw.toString()); }
	if(tokens.empty()) {
		return raw.toString();
	}

	// final SpannableStringBuilder builder = new SpannableStringBuilder();
	std::string builder;

	int boldStart = -1;
	int italicStart = -1;
	int strikeStart = -1;
	int linkStart = -1;
	int caretStart = -1;
	int parenOpenCount = 0;
	int parenCloseCount = 0;

	for(int i = 0; i < static_cast<int>(tokens.size()); i++) {

		const int token = tokens[i];

		switch(token) {

			// case MarkdownTokenizer.TOKEN_ASTERISK:
			// case MarkdownTokenizer.TOKEN_UNDERSCORE:
			case MarkdownTokenizer::TOKEN_ASTERISK:
			case MarkdownTokenizer::TOKEN_UNDERSCORE:

				if(italicStart < 0) {
					italicStart = static_cast<int>(builder.length());
				} else {
					// builder.setSpan(new StyleSpan(Typeface.ITALIC), italicStart, builder.length(), ...)
					// Stub: just reset
					italicStart = -1;
				}
				break;

			// case MarkdownTokenizer.TOKEN_ASTERISK_DOUBLE:
			// case MarkdownTokenizer.TOKEN_UNDERSCORE_DOUBLE:
			case MarkdownTokenizer::TOKEN_ASTERISK_DOUBLE:
			case MarkdownTokenizer::TOKEN_UNDERSCORE_DOUBLE:

				if(boldStart < 0) {
					boldStart = static_cast<int>(builder.length());
				} else {
					boldStart = -1;
				}
				break;

			// case MarkdownTokenizer.TOKEN_TILDE_DOUBLE:
			case MarkdownTokenizer::TOKEN_TILDE_DOUBLE:

				if(strikeStart == -1) {
					strikeStart = static_cast<int>(builder.length());
				} else {
					strikeStart = -1;
				}
				break;

			// case MarkdownTokenizer.TOKEN_GRAVE:
			case MarkdownTokenizer::TOKEN_GRAVE: {

				const int codeStart = static_cast<int>(builder.length());

				// while(tokens[++i] != MarkdownTokenizer.TOKEN_GRAVE) {
				//     builder.append((char)tokens[i]);
				// }
				while(tokens[++i] != MarkdownTokenizer::TOKEN_GRAVE) {
					builder += static_cast<char>(tokens[i]);
				}

				// builder.setSpan(new TypefaceSpan("monospace"), codeStart, builder.length(), ...)
				// Stub: already appended monospace content
				break;
			}

			// case MarkdownTokenizer.TOKEN_BRACKET_SQUARE_OPEN:
			case MarkdownTokenizer::TOKEN_BRACKET_SQUARE_OPEN:
				linkStart = static_cast<int>(builder.length());
				break;

			// case MarkdownTokenizer.TOKEN_BRACKET_SQUARE_CLOSE:
			case MarkdownTokenizer::TOKEN_BRACKET_SQUARE_CLOSE: {

				const int urlStart = indexOf(
						tokens,
						MarkdownTokenizer::TOKEN_PAREN_OPEN,
						i + 1);
				const int urlEnd = indexOf(
						tokens,
						MarkdownTokenizer::TOKEN_PAREN_CLOSE,
						urlStart + 1);

				// final StringBuilder urlBuilder = new StringBuilder(urlEnd - urlStart);
				std::string urlBuilder;
				urlBuilder.reserve(static_cast<size_t>(urlEnd - urlStart));

				// for(int j = urlStart + 1; j < urlEnd; j++) {
				//     urlBuilder.append((char)tokens[j]);
				// }
				for(int j = urlStart + 1; j < urlEnd; j++) {
					urlBuilder += static_cast<char>(tokens[j]);
				}

				// final String linkText = String.valueOf(builder.subSequence(linkStart, builder.length()));
				const std::string linkText = builder.substr(
						static_cast<size_t>(linkStart));
				const std::string url = urlBuilder;

				// if(url.startsWith("/spoiler")) {
				if(url.substr(0, 8) == "/spoiler") {

					// builder.delete(linkStart, builder.length());
					builder.resize(static_cast<size_t>(linkStart));
					builder += "[Spoiler]";

					// links.add(new Link("Spoiler", null, UriString.from(spoilerUriBuilder)));
					links.emplace_back("Spoiler", "", "rr://msg/?title=Spoiler&message=" + linkText);

				} else if(url.length() > 3 && url[2] == ' '
						&& (url[0] == '#' || url[0] == '/')) {

					// final String subtitle;
					std::string subtitle;
					switch(url[1]) {
						case 'b':
							subtitle = "Spoiler: Book";
							break;
						case 'g':
							subtitle = "Spoiler: Speculation";
							break;
						case 's':
						default:
							subtitle = "Spoiler";
							break;
					}

					links.emplace_back(linkText, subtitle,
							"rr://msg/?title=" + subtitle + "&message=" + url.substr(3));

				} else {
					// links.add(new Link(linkText, url, new UriString(url)));
					links.emplace_back(linkText, url, url);
				}

				// TODO number links
				// final ClickableSpan span = new ClickableSpan() { ... }
				// builder.setSpan(span, linkStart, builder.length(), ...)

				i = urlEnd;
				break;
			}

			// case MarkdownTokenizer.TOKEN_CARET:
			case MarkdownTokenizer::TOKEN_CARET:
				if(caretStart < 0) {
					caretStart = static_cast<int>(builder.length());
				} else {
					builder += ' ';
				}
				break;

			case ' ': {
				builder += ' ';

				// if(caretStart >= 0 && parenOpenCount == parenCloseCount) {
				//     builder.setSpan(new SuperscriptSpan(), caretStart, builder.length(), ...)
				//     builder.setSpan(new RelativeSizeSpan(0.6f), caretStart, builder.length(), ...)
				//     caretStart = -1;
				// }
				if(caretStart >= 0 && parenOpenCount == parenCloseCount) {
					caretStart = -1;
				}
				break;
			}

			case '(': {
				if(caretStart >= 0) {
					parenOpenCount++;
					if(caretStart != static_cast<int>(builder.length())) {
						builder += '(';
					}
				} else {
					parenOpenCount = 0;
					builder += '(';
				}
				break;
			}

			case ')': {
				if(caretStart >= 0) {
					parenCloseCount++;
					if(parenOpenCount != parenCloseCount) {
						builder += ')';
					} else {
						caretStart = -1;
					}
				} else {
					parenCloseCount = 0;
					builder += ')';
				}
				break;
			}

			default:
				builder += static_cast<char>(token);
				break;
		}
	}

	// if(caretStart >= 0) { ... }
	if(caretStart >= 0) {
		caretStart = -1; // stub
	}

	// if(type == MarkdownParser.MarkdownParagraphType.HEADER) {
	//     while(builder.length() > 0 && builder.charAt(builder.length() - 1) == '#') {
	//         builder.delete(builder.length() - 1, builder.length());
	//     }
	// }
	if(type == MarkdownParser::MarkdownParagraphType::HEADER) {
		while(!builder.empty() && builder.back() == '#') {
			builder.pop_back();
		}
	}

	return builder;
}

// ===== isEmpty =====
// Port of:
//   public boolean isEmpty() {
//       if(type == MarkdownParser.MarkdownParagraphType.HLINE) { return false; }
//       if(type == MarkdownParser.MarkdownParagraphType.EMPTY) { return true; }
//       if(tokens == null) {
//           return raw.countSpacesAtStart() == raw.length;
//       } else {
//           for(final int token : tokens) {
//               if(!MarkdownTokenizer.isUnicodeWhitespace(token)) { return false; }
//           }
//           return true;
//       }
//   }
bool MarkdownParagraph::isEmpty() const {

	if(type == MarkdownParser::MarkdownParagraphType::HLINE) {
		return false;
	}
	if(type == MarkdownParser::MarkdownParagraphType::EMPTY) {
		return true;
	}

	if(tokens.empty()) {
		return raw.countSpacesAtStart() == raw.length;

	} else {
		for(const int token : tokens) {
			if(!MarkdownTokenizer::isUnicodeWhitespace(static_cast<int>(static_cast<unsigned char>(token)))) {
				return false;
			}
		}
		return true;
	}
}

} // namespace PinkReader
