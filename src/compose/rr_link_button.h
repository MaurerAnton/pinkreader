// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRLinkButton.kt
#pragma once

#include <string>

namespace PinkReader {

class UriString;
struct ComposeThemeLinkButton;

// Ported from: @Composable fun RRLinkButton(title, link, theme)
// A styled link button that shows a title, URL subtitle, and navigates on click.
void RRLinkButton(
	const std::string& title,
	const UriString& link,
	const ComposeThemeLinkButton& theme
);

// Ported from: @Composable @Preview private fun PreviewRRLinkButton()
void PreviewRRLinkButton();

} // namespace PinkReader
