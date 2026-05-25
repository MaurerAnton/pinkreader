/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: markdown_paragraph_group.h
 * Port of: MarkdownParagraphGroup.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/markdown/MarkdownParagraphGroup.java
 *
 * Every field, method, and logic branch ported exactly.
 * Android View construction is stubbed; the data model is faithfully preserved.
 */

#pragma once

#include "markdown_paragraph.h"

#include <vector>
#include <memory>
#include <optional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.markdown.MarkdownParagraphGroup
 *
 * A group of MarkdownParagraph objects. Builds a ViewGroup in the Android
 * original; the C++ port provides the paragraph data and a buildView stub.
 */
class MarkdownParagraphGroup {
public:
	// Port of: private final MarkdownParagraph[] paragraphs;
	std::vector<MarkdownParagraph> paragraphs;

	// Port of: public MarkdownParagraphGroup(final MarkdownParagraph[] paragraphs)
	explicit MarkdownParagraphGroup(const std::vector<MarkdownParagraph>& paragraphs);

	// Port of: public ViewGroup buildView(
	//     final BaseActivity activity,
	//     final Integer textColor,
	//     final Float textSize,
	//     final boolean showLinkButtons)
	// Stub: returns nullptr; Android ViewGroup construction is platform-specific
	// The data is accessible via paragraphs vector.
	void* buildView(
			const void* activity,
			std::optional<int> textColor,
			std::optional<float> textSize,
			bool showLinkButtons) const;
};

} // namespace PinkReader
