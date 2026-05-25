// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/types/AlbumViewMode.kt
#include "album_view_mode.h"

namespace PinkReader {

// Port of: Cards("cards")
const AlbumViewMode AlbumViewMode::Cards("cards");
// Port of: List("list")
const AlbumViewMode AlbumViewMode::List("list");
// Port of: Grid("grid")
const AlbumViewMode AlbumViewMode::Grid("grid");

// Port of: companion object { val settingSerializer = EnumSettingSerializer(entries) }
// Port of: entries = listOf(Cards, List, Grid)
static const std::vector<AlbumViewMode> s_entries = {
	AlbumViewMode::Cards,
	AlbumViewMode::List,
	AlbumViewMode::Grid
};

// Port of: entries (all values)
const std::vector<AlbumViewMode>& AlbumViewMode::entries() {
	return s_entries;
}

// Port of: companion object { val settingSerializer = EnumSettingSerializer(entries) }
const EnumSettingSerializer<AlbumViewMode> AlbumViewMode::settingSerializer(s_entries);

} // namespace PinkReader
