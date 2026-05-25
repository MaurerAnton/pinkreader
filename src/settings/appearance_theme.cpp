// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/types/AppearanceTheme.kt
#include "appearance_theme.h"

namespace PinkReader {

// Port of: RED("red", ThemeLightness.Light, Colors.Red.mid, Colors.Red.s6)
const AppearanceTheme AppearanceTheme::RED(
	"red", ThemeLightness::Light,
	ThemeColor(0xEE, 0x11, 0x11),    // Colors.Red.mid
	ThemeColor(0xBB, 0x0A, 0x1E));   // Colors.Red.s6

// Port of: GREEN("green", ThemeLightness.Light, Color(0x4c, 0xaf, 0x50), Color(0x38, 0x8e, 0x3c))
const AppearanceTheme AppearanceTheme::GREEN(
	"green", ThemeLightness::Light,
	ThemeColor(0x4c, 0xaf, 0x50),
	ThemeColor(0x38, 0x8e, 0x3c));

// Port of: BLUE("blue", ThemeLightness.Light, Color(0x12, 0x62, 0x91), Color(0x10, 0x49, 0x6b))
const AppearanceTheme AppearanceTheme::BLUE(
	"blue", ThemeLightness::Light,
	ThemeColor(0x12, 0x62, 0x91),
	ThemeColor(0x10, 0x49, 0x6b));

// Port of: LTBLUE("ltblue", ThemeLightness.Light, Color(0x03, 0xa9, 0xf4), Color(0x02, 0x88, 0xd1))
const AppearanceTheme AppearanceTheme::LTBLUE(
	"ltblue", ThemeLightness::Light,
	ThemeColor(0x03, 0xa9, 0xf4),
	ThemeColor(0x02, 0x88, 0xd1));

// Port of: ORANGE("orange", ThemeLightness.Light, Color(0xff, 0x98, 0x00), Color(0xf5, 0x7c, 0x00))
const AppearanceTheme AppearanceTheme::ORANGE(
	"orange", ThemeLightness::Light,
	ThemeColor(0xff, 0x98, 0x00),
	ThemeColor(0xf5, 0x7c, 0x00));

// Port of: GRAY("gray", ThemeLightness.Light, Color(0x22, 0x22, 0x22), Color(0x60, 0x7d, 0x8b))
const AppearanceTheme AppearanceTheme::GRAY(
	"gray", ThemeLightness::Light,
	ThemeColor(0x22, 0x22, 0x22),
	ThemeColor(0x60, 0x7d, 0x8b));

// Port of: NIGHT("night", ThemeLightness.Dark, Color(0xCC, 0xCC, 0xCC), Color.Black)
const AppearanceTheme AppearanceTheme::NIGHT(
	"night", ThemeLightness::Dark,
	ThemeColor(0xCC, 0xCC, 0xCC),
	ThemeColor(0x00, 0x00, 0x00));  // Color.Black

// Port of: NIGHT_LOWCONTRAST("night_lowcontrast", ThemeLightness.Dark, Color(0xCC, 0xCC, 0xCC), Color.Black)
const AppearanceTheme AppearanceTheme::NIGHT_LOWCONTRAST(
	"night_lowcontrast", ThemeLightness::Dark,
	ThemeColor(0xCC, 0xCC, 0xCC),
	ThemeColor(0x00, 0x00, 0x00));

// Port of: ULTRABLACK("ultrablack", ThemeLightness.Dark, Color(0xCC, 0xCC, 0xCC), Color.Black)
const AppearanceTheme AppearanceTheme::ULTRABLACK(
	"ultrablack", ThemeLightness::Dark,
	ThemeColor(0xCC, 0xCC, 0xCC),
	ThemeColor(0x00, 0x00, 0x00));

// Port of: entries list
static const std::vector<AppearanceTheme> s_appearance_entries = {
	AppearanceTheme::RED,
	AppearanceTheme::GREEN,
	AppearanceTheme::BLUE,
	AppearanceTheme::LTBLUE,
	AppearanceTheme::ORANGE,
	AppearanceTheme::GRAY,
	AppearanceTheme::NIGHT,
	AppearanceTheme::NIGHT_LOWCONTRAST,
	AppearanceTheme::ULTRABLACK
};

// Port of: entries
const std::vector<AppearanceTheme>& AppearanceTheme::entries() {
	return s_appearance_entries;
}

// Port of: companion object { val settingSerializer = EnumSettingSerializer(AppearanceTheme.entries) }
const EnumSettingSerializer<AppearanceTheme> AppearanceTheme::settingSerializer(s_appearance_entries);

} // namespace PinkReader
