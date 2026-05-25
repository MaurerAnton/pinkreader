// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/theme/ComposeTheme.kt
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>

namespace PinkReader {

// Forward declarations
class ComposePrefs;

// Ported from: Compose UI types
using Color = uint32_t;   // ARGB
using Dp = float;
using Shape = int;        // Placeholder for shape types (RoundedCornerShape, RectangleShape, etc.)
using Modifier = int;     // Placeholder for Modifier chain

// Ported from: data class ComposeThemeButton
struct ComposeThemeButton {
	int textStyle = 0;         // TextStyle placeholder
	Color background = 0;
	Shape shape = 0;
	std::optional<Color> border;
	std::optional<Dp> borderThickness;
};

// Ported from: data class ComposeThemeLinkButton
struct ComposeThemeLinkButton {
	int titleStyle = 0;        // TextStyle placeholder
	int subtitleStyle = 0;     // TextStyle placeholder
	Shape shape = 0;
	Color borderColor = 0;
	Dp borderThickness = 0.0f;
	Color iconColor = 0;
};

// Ported from: interface ComposeThemePostCard
struct ComposeThemePostCard {
	int titleStyle = 0;
	int subtitleStyle = 0;
	int captionStyle = 0;
	Color iconColor = 0;
	Color backgroundColor = 0;
	Color listBackgroundColor = 0;
	Color previewImageBackgroundColor = 0;
};

// Ported from: interface ComposeThemeAlbum
struct ComposeThemeAlbum {
	Color toolbarIconColor = 0;
	int titleStyle = 0;
	int subtitleStyle = 0;
	int titleCompactStyle = 0;
};

// Ported from: interface ComposeThemeDropdownMenu
struct ComposeThemeDropdownMenu {
	int textStyle = 0;
	Color background = 0;
};

// Ported from: interface ComposeThemeError
struct ComposeThemeError {
	int titleStyle = 0;
	int messageStyle = 0;
	Color border = 0;
	Color background = 0;
	ComposeThemeButton primaryButton;
	ComposeThemeButton secondaryButton;
};

// Ported from: interface ComposeTheme
struct ComposeTheme {
	virtual ~ComposeTheme() = default;

	virtual const ComposeThemeDropdownMenu& dropdownMenu() const = 0;
	virtual const ComposeThemePostCard& postCard() const = 0;
	virtual const ComposeThemeAlbum& album() const = 0;
	virtual const ComposeThemeError& error() const = 0;
	virtual const ComposeThemeLinkButton& linkButton() const = 0;
};

// Ported from: class ComposeThemeImpl(prefs: ComposePrefs) : ComposeTheme
class ComposeThemeImpl : public ComposeTheme {
public:
	explicit ComposeThemeImpl(const ComposePrefs& prefs);

	const ComposeThemeDropdownMenu& dropdownMenu() const override;
	const ComposeThemePostCard& postCard() const override;
	const ComposeThemeAlbum& album() const override;
	const ComposeThemeError& error() const override;
	const ComposeThemeLinkButton& linkButton() const override;

private:
	// Ported from: val theme = prefs.appearanceTheme.value; val light = theme.lightness == ThemeLightness.Light
	bool light_ = false;
	bool isNightLowContrast_ = false;
	bool isUltraBlack_ = false;

	// Computed colors matching the Kotlin version
	Color colorText_ = 0;
	Color colorSubtext_ = 0;
	Color colorIcon_ = 0;
	Color colorCardBackground_ = 0;
	Color colorListBackground_ = 0;
	Color colorPopupBackground_ = 0;
	Color colorImageBackground_ = 0;

	ComposeThemeDropdownMenu dropdownMenu_;
	ComposeThemePostCard postCard_;
	ComposeThemeAlbum album_;
	ComposeThemeError error_;
	ComposeThemeLinkButton linkButton_;
};

// Ported from: val LocalComposeTheme = staticCompositionLocalOf<ComposeTheme> { throw ... }
const ComposeTheme& LocalComposeTheme();

// Ported from: object Colors { object Grey { ... }; object Red { ... } }
namespace Colors {
	namespace Grey {
		inline constexpr Color s1  = 0xFFF8F9FA;
		inline constexpr Color s2  = 0xFFE9ECEF;
		inline constexpr Color s3  = 0xFFDEE2E6;
		inline constexpr Color s4  = 0xFFCED4DA;
		inline constexpr Color s5  = 0xFFADB5BD;
		inline constexpr Color s6  = 0xFF6C757D;
		inline constexpr Color s7  = 0xFF495057;
		inline constexpr Color s8  = 0xFF343A40;
		inline constexpr Color s9  = 0xFF212529;
		inline constexpr Color s10 = 0xFF111213;
	}
	namespace Red {
		inline constexpr Color s0  = 0xFFfdf6f6;
		inline constexpr Color s1  = 0xFFf6d8d8;
		inline constexpr Color s2  = 0xFFeeb2b2;
		inline constexpr Color s3  = 0xFFe68c8c;
		inline constexpr Color s4  = 0xFFde6565;
		inline constexpr Color s5  = 0xFFd63f3f;
		inline constexpr Color mid = 0xFFd32f2f;
		inline constexpr Color s6  = 0xFFbf2828;
		inline constexpr Color s7  = 0xFF992020;
		inline constexpr Color s8  = 0xFF721818;
		inline constexpr Color s9  = 0xFF4c1010;
		inline constexpr Color s10 = 0xFF260808;
	}
}

// Ported from: @Composable fun RRComposeContextTheme(content)
void RRComposeContextTheme(std::function<void()> content);

// Ported from: @Composable fun TextStyle.StyledText(text, modifier, overflow, maxLines)
// StyledText renders styled text using the given TextStyle (represented as int index here)
void StyledText(int styleIndex, const std::string& text,
	Modifier modifier = 0, int overflow = 0, int maxLines = 0x7FFFFFFF);

// Ported from: @Composable fun Modifier.combinedClickableWithHaptics(...)
Modifier combinedClickableWithHaptics(
	Modifier base,
	bool enabled,
	const char* onClickLabel,
	int role,
	const char* onLongClickLabel,
	std::function<void()> onLongClick,
	std::function<void()> onDoubleClick,
	std::function<void()> onClick
);

} // namespace PinkReader
