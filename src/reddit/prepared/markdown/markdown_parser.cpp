/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_parser.cpp
 * Port of: MarkdownParser.java
 *
 * Every method ported exactly, line-by-line.
 */

#include "markdown_parser.h"
#include "char_arr_substring.h"
#include "markdown_line.h"
#include "markdown_paragraph.h"
#include "markdown_paragraph_group.h"

#include <vector>
#include <cstring>

namespace PinkReader {

// ===== parse =====
// Port of:
//   public static MarkdownParagraphGroup parse(final char[] raw) {
//       final CharArrSubstring[] rawLines = CharArrSubstring.generateFromLines(raw);
//       final MarkdownLine[] lines = new MarkdownLine[rawLines.length];
//       for(int i = 0; i < rawLines.length; i++) {
//           lines[i] = MarkdownLine.generate(rawLines[i]);
//       }
//       final ArrayList<MarkdownLine> mergedLines = new ArrayList<>(rawLines.length);
//       MarkdownLine currentLine = null;
//       for(int i = 0; i < lines.length; i++) {
//           if(currentLine != null) {
//               switch(lines[i].type) { ... }
//           } else if(lines[i].type != MarkdownParagraphType.EMPTY) {
//               currentLine = lines[i];
//           }
//       }
//       if(currentLine != null) { mergedLines.add(currentLine); }
//       final ArrayList<MarkdownParagraph> outputParagraphs = new ArrayList<>(mergedLines.size());
//       for(final MarkdownLine line : mergedLines) {
//           final MarkdownParagraph lastParagraph = outputParagraphs.isEmpty()
//               ? null : outputParagraphs.get(outputParagraphs.size() - 1);
//           final MarkdownParagraph paragraph = line.tokenize(lastParagraph);
//           if(!paragraph.isEmpty()) { outputParagraphs.add(paragraph); }
//       }
//       return new MarkdownParagraphGroup(outputParagraphs.toArray(new MarkdownParagraph[0]));
//   }
MarkdownParagraphGroup MarkdownParser::parse(const char* raw, int length) {

	// Copy raw to mutable buffer (CharArrSubstring needs mutable char*)
	std::vector<char> mutableRaw(raw, raw + length);

	// final CharArrSubstring[] rawLines = CharArrSubstring.generateFromLines(raw);
	std::vector<CharArrSubstring> rawLines = CharArrSubstring::generateFromLines(
			mutableRaw.data(), length);

	// final MarkdownLine[] lines = new MarkdownLine[rawLines.length];
	std::vector<MarkdownLine> lines;
	lines.reserve(rawLines.size());

	// for(int i = 0; i < rawLines.length; i++) {
	//     lines[i] = MarkdownLine.generate(rawLines[i]);
	// }
	for(size_t i = 0; i < rawLines.size(); i++) {
		lines.push_back(MarkdownLine::generate(rawLines[i]));
	}

	// final ArrayList<MarkdownLine> mergedLines = new ArrayList<>(rawLines.length);
	std::vector<MarkdownLine> mergedLines;
	mergedLines.reserve(rawLines.size());

	// MarkdownLine currentLine = null;
	const MarkdownLine* currentLine = nullptr;
	int currentLineIdx = -1;

	// for(int i = 0; i < lines.length; i++) {
	for(size_t i = 0; i < lines.size(); i++) {

		if(currentLine != nullptr) {

			switch(lines[i].type) {
				case MarkdownParagraphType::BULLET:
				case MarkdownParagraphType::NUMBERED:
				case MarkdownParagraphType::HEADER:
				case MarkdownParagraphType::CODE:
				case MarkdownParagraphType::HLINE:
				case MarkdownParagraphType::QUOTE:

					// mergedLines.add(currentLine);
					mergedLines.push_back(lines[currentLineIdx]);
					// currentLine = lines[i];
					currentLine = &lines[i];
					currentLineIdx = static_cast<int>(i);
					break;

				case MarkdownParagraphType::EMPTY:
					// mergedLines.add(currentLine);
					mergedLines.push_back(lines[currentLineIdx]);
					// currentLine = null;
					currentLine = nullptr;
					currentLineIdx = -1;
					break;

				case MarkdownParagraphType::TEXT:

					switch(lines[i - 1].type) {
						case MarkdownParagraphType::QUOTE:
						case MarkdownParagraphType::BULLET:
						case MarkdownParagraphType::NUMBERED:
						case MarkdownParagraphType::TEXT:

							// if(lines[i - 1].spacesAtEnd >= 2) {
							if(lines[i - 1].spacesAtEnd >= 2) {
								// mergedLines.add(currentLine);
								mergedLines.push_back(lines[currentLineIdx]);
								// currentLine = lines[i];
								currentLine = &lines[i];
								currentLineIdx = static_cast<int>(i);

							} else {
								// currentLine = currentLine.rejoin(lines[i]);
								lines[currentLineIdx] = lines[currentLineIdx].rejoin(lines[i]);
							}
							break;

						case MarkdownParagraphType::CODE:
						case MarkdownParagraphType::HEADER:
						case MarkdownParagraphType::HLINE:
							mergedLines.push_back(lines[currentLineIdx]);
							currentLine = &lines[i];
							currentLineIdx = static_cast<int>(i);
							break;

						default:
							break;
					}

					break;
			}
		} else if(lines[i].type != MarkdownParagraphType::EMPTY) {
			// currentLine = lines[i];
			currentLine = &lines[i];
			currentLineIdx = static_cast<int>(i);
		}
	}

	// if(currentLine != null) { mergedLines.add(currentLine); }
	if(currentLine != nullptr) {
		mergedLines.push_back(lines[currentLineIdx]);
	}

	// final ArrayList<MarkdownParagraph> outputParagraphs = new ArrayList<>(mergedLines.size());
	std::vector<MarkdownParagraph> outputParagraphs;
	outputParagraphs.reserve(mergedLines.size());

	// for(final MarkdownLine line : mergedLines) {
	for(const MarkdownLine& line : mergedLines) {

		// final MarkdownParagraph lastParagraph = outputParagraphs.isEmpty()
		//     ? null : outputParagraphs.get(outputParagraphs.size() - 1);
		const MarkdownParagraph* lastParagraph = outputParagraphs.empty()
				? nullptr : &outputParagraphs.back();

		// final MarkdownParagraph paragraph = line.tokenize(lastParagraph);
		MarkdownParagraph paragraph = line.tokenize(lastParagraph);

		// if(!paragraph.isEmpty()) { outputParagraphs.add(paragraph); }
		if(!paragraph.isEmpty()) {
			outputParagraphs.push_back(std::move(paragraph));
		}
	}

	// return new MarkdownParagraphGroup(outputParagraphs.toArray(new MarkdownParagraph[0]));
	return MarkdownParagraphGroup(outputParagraphs);
}

} // namespace PinkReader
