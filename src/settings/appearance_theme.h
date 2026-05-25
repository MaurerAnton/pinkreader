// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/types/AppearanceTheme.kt
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "setting_serializer.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.settings.types.ThemeLightness
 *
 * Original: enum class ThemeLightness { Light, Dark }
 */
enum class ThemeLightness {
	// Port of: Light
	Light,
	// Port of: Dark
	Dark
};

/**
 * @brief Simple 32-bit ARGB color struct
 *
 * Equivalent to: androidx.compose.ui.graphics.Color
 */
struct ThemeColor {
	uint8_t r = 0, g = 0, b = 0, a = 0xFF;

	ThemeColor() = default;
	ThemeColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF)
		: r(red), g(green), b(blue), a(alpha) {}

	bool operator==(const ThemeColor& other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
	bool operator!=(const ThemeColor& other) const { return !(*this == other); }
};

/**
 * @brief Port of org.quantumbadger.redreader.settings.types.AppearanceTheme
 *
 * Original: enum class AppearanceTheme(
 *   override val stringValue: String,
 *   val lightness: ThemeLightness,
 *   val colorPrimary: Color,
 *   val colorPrimaryDark: Color,
 * ) : SerializableEnum<AppearanceTheme>
 *
 * Every enum value with all fields from the original Kotlin file.
 */
class AppearanceTheme : public SerializableEnum<AppearanceTheme> {
public:
	// Port of: RED("red", ThemeLightness.Light, Colors.Red.mid, Colors.Red.s6)
	static const AppearanceTheme RED;
	// Port of: GREEN("green", ThemeLightness.Light, Color(0x4c, 0xaf, 0x50), Color(0x38, 0x8e, 0x3c))
	static const AppearanceTheme GREEN;
	// Port of: BLUE("blue", ThemeLightness.Light, Color(0x12, 0x62, 0x91), Color(0x10, 0x49, 0x6b))
	static const AppearanceTheme BLUE;
	// Port of: LTBLUE("ltblue", ThemeLightness.Light, Color(0x03, 0xa9, 0xf4), Color(0x02, 0x88, 0xd1))
	static const AppearanceTheme LTBLUE;
	// Port of: ORANGE("orange", ThemeLightness.Light, Color(0xff, 0x98, 0x00), Color(0xf5, 0x7c, 0x00))
	static const AppearanceTheme ORANGE;
	// Port of: GRAY("gray", ThemeLightness.Light, Color(0x22, 0x22, 0x22), Color(0x60, 0x7d, 0x8b))
	static const AppearanceTheme GRAY;
	// Port of: NIGHT("night", ThemeLightness.Dark, Color(0xCC, 0xCC, 0xCC), Color.Black)
	static const AppearanceTheme NIGHT;
	// Port of: NIGHT_LOWCONTRAST("night_lowcontrast", ThemeLightness.Dark, Color(0xCC, 0xCC, 0xCC), Color.Black)
	static const AppearanceTheme NIGHT_LOWCONTRAST;
	// Port of: ULTRABLACK("ultrablack", ThemeLightness.Dark, Color(0xCC, 0xCC, 0xCC), Color.Black)
	static const AppearanceTheme ULTRABLACK;

	// Port of: companion object { val settingSerializer = EnumSettingSerializer(AppearanceTheme.entries) }
	static const EnumSettingSerializer<AppearanceTheme> settingSerializer;

	// Port of: entries (all values)
	static const std::vector<AppearanceTheme>& entries();

	// Port of: override val stringValue: String
	std::string stringValue() const override { return m_stringValue; }

	// Port of: val lightness: ThemeLightness
	ThemeLightness lightness() const { return m_lightness; }

	// Port of: val colorPrimary: Color
	ThemeColor colorPrimary() const { return m_colorPrimary; }

	// Port of: val colorPrimaryDark: Color
	ThemeColor colorPrimaryDark() const { return m_colorPrimaryDark; }

	// Equality
	bool operator==(const AppearanceTheme& other) const { return m_stringValue == other.m_stringValue; }
	bool operator!=(const AppearanceTheme& other) const { return m_stringValue != other.m_stringValue; }

private:
	// Port of: constructor(stringValue, lightness, colorPrimary, colorPrimaryDark)
	AppearanceTheme(const std::string& strValue, ThemeLightness lightness,
	                ThemeColor colorPrimary, ThemeColor colorPrimaryDark)
		: m_stringValue(strValue)
		, m_lightness(lightness)
		, m_colorPrimary(colorPrimary)
		, m_colorPrimaryDark(colorPrimaryDark) {}

	std::string m_stringValue;
	ThemeLightness m_lightness;
	ThemeColor m_colorPrimary;
	ThemeColor m_colorPrimaryDark;
};

} // namespace PinkReader
