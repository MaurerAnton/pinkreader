// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRButton.kt
#pragma once

#include <functional>
#include <string>

namespace PinkReader {

struct ComposeThemeButton;

// Ported from: @Composable fun RRButton(modifier, onClick, text, theme)
// A themed button component with click handling.
void RRButton(
	int modifier,                          // Modifier placeholder
	std::function<void()> onClick,
	const std::string& text,
	const ComposeThemeButton& theme
);

// Ported from: @Composable @Preview private fun PreviewRRButton()
void PreviewRRButton();

} // namespace PinkReader
