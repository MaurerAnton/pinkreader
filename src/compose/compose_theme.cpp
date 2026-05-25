// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/theme/ComposeTheme.kt
#include "compose_theme.h"

namespace PinkReader {

// --- Static theme instance ---
static ComposeThemeImpl* g_currentTheme = nullptr;

const ComposeTheme& LocalComposeTheme() {
	// Ported from: val LocalComposeTheme = staticCompositionLocalOf<ComposeTheme> { throw ... }
	if (!g_currentTheme) {
		// Would throw: "Theme not initialized"
		static ComposeThemeImpl defaultTheme = []() {
			// Create with a default ComposePrefs - placeholder
			// In real code this would use the actual ComposePrefs
			return ComposeThemeImpl(*(ComposePrefs*)nullptr);  // placeholder
		}();
		return defaultTheme;
	}
	return *g_currentTheme;
}

// --- ComposeThemeImpl ---

ComposeThemeImpl::ComposeThemeImpl(const ComposePrefs& prefs) {
	// Ported from: class ComposeThemeImpl(prefs: ComposePrefs) : ComposeTheme {
	// Determine theme properties from prefs
	// val theme = prefs.appearanceTheme.value (AppearanceTheme enum)
	// val light = theme.lightness == ThemeLightness.Light

	// Default: dark theme
	light_ = false;
	isNightLowContrast_ = false;
	isUltraBlack_ = false;

	// Compute colors per original logic:
	// colorText = if (light) Grey.s10 else if (NIGHT_LOWCONTRAST) Grey.s2 else Grey.s1
	colorText_ = Colors::Grey::s1;
	// colorSubtext = if (light) Grey.s6 else if (NIGHT_LOWCONTRAST) Grey.s5 else Grey.s3
	colorSubtext_ = Colors::Grey::s3;
	// colorIcon = if (light) Grey.s7 else if (NIGHT_LOWCONTRAST) Grey.s6 else Grey.s4
	colorIcon_ = Colors::Grey::s4;
	// colorCardBackground = if (light) White else if (ULTRABLACK) Black else Grey.s9
	colorCardBackground_ = Colors::Grey::s9;
	// colorListBackground = if (light) Grey.s1 else if (NIGHT_LOWCONTRAST) Grey.s10 else Black
	colorListBackground_ = 0xFF000000;  // Black
	// colorPopupBackground = if (light) White else if (NIGHT_LOWCONTRAST) Grey.s7 else Grey.s8
	colorPopupBackground_ = Colors::Grey::s8;
	// colorImageBackground = if (light) Grey.s2 else Grey.s8
	colorImageBackground_ = Colors::Grey::s8;

	// --- postCard ---
	postCard_.titleStyle = 0;
	postCard_.subtitleStyle = 1;
	postCard_.captionStyle = 2;
	postCard_.iconColor = colorIcon_;
	postCard_.backgroundColor = colorCardBackground_;
	postCard_.listBackgroundColor = colorListBackground_;
	postCard_.previewImageBackgroundColor = colorImageBackground_;

	// --- album ---
	album_.titleStyle = 3;
	album_.subtitleStyle = 4;
	album_.titleCompactStyle = 5;
	album_.toolbarIconColor = colorIcon_;

	// --- dropdownMenu ---
	dropdownMenu_.textStyle = 6;
	dropdownMenu_.background = colorPopupBackground_;

	// --- error ---
	error_.titleStyle = 7;
	error_.messageStyle = 8;
	error_.background = Colors::Red::s10;   // dark
	error_.border = Colors::Red::s7;         // dark

	error_.primaryButton.textStyle = 9;
	error_.primaryButton.background = Colors::Red::s7;
	error_.primaryButton.shape = 1;  // RoundedCornerShape(6.dp)

	error_.secondaryButton.textStyle = 9;
	error_.secondaryButton.background = 0x00000000;  // Transparent
	error_.secondaryButton.shape = 1;

	// --- linkButton ---
	linkButton_.titleStyle = 10;
	linkButton_.subtitleStyle = 11;
	linkButton_.shape = 1;  // RoundedCornerShape(6.dp)
	linkButton_.borderColor = Colors::Grey::s7;
	linkButton_.borderThickness = 1.0f;
	linkButton_.iconColor = Colors::Grey::s7;
}

const ComposeThemeDropdownMenu& ComposeThemeImpl::dropdownMenu() const { return dropdownMenu_; }
const ComposeThemePostCard& ComposeThemeImpl::postCard() const { return postCard_; }
const ComposeThemeAlbum& ComposeThemeImpl::album() const { return album_; }
const ComposeThemeError& ComposeThemeImpl::error() const { return error_; }
const ComposeThemeLinkButton& ComposeThemeImpl::linkButton() const { return linkButton_; }

// --- RRComposeContextTheme ---

void RRComposeContextTheme(std::function<void()> content) {
	// Ported from @Composable fun RRComposeContextTheme(content):
	// 1. val prefs = LocalComposePrefs.current
	// 2. val themePref = prefs.appearanceTheme.value
	// 3. val theme = ComposeThemeImpl(prefs)
	// 4. SideEffect: set status bar appearance
	// 5. MaterialTheme(colorScheme = ...) {
	//       CompositionLocalProvider(LocalComposeTheme provides theme) {
	//           content()
	//       }
	//   }
	content();
}

// --- StyledText ---

void StyledText(int styleIndex, const std::string& text,
	Modifier modifier, int overflow, int maxLines) {
	// Ported from @Composable fun TextStyle.StyledText(text, modifier, overflow, maxLines):
	// Text(modifier = modifier, text = text, style = this, fontSize = fontSize,
	//      fontWeight = fontWeight, color = color, overflow = overflow, maxLines = maxLines)
	// Rendering is platform-specific
}

// --- combinedClickableWithHaptics ---

Modifier combinedClickableWithHaptics(
	Modifier base,
	bool enabled,
	const char* onClickLabel,
	int role,
	const char* onLongClickLabel,
	std::function<void()> onLongClick,
	std::function<void()> onDoubleClick,
	std::function<void()> onClick)
{
	// Ported from @Composable fun Modifier.combinedClickableWithHaptics(...):
	// val haptics = LocalHapticFeedback.current
	// return this.combinedClickable(
	//     enabled = enabled,
	//     onClickLabel = onClickLabel,
	//     role = role,
	//     onLongClickLabel = onLongClickLabel,
	//     onLongClick = onLongClick?.let { { haptics.performHapticFeedback(HapticFeedbackType.LongPress); it() } },
	//     onDoubleClick = onDoubleClick,
	//     onClick = onClick
	// )
	return base;  // modifier chain passthrough
}

} // namespace PinkReader
