// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/types/AlbumViewMode.kt
#pragma once

#include <string>
#include <vector>
#include "setting_serializer.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.settings.types.AlbumViewMode
 *
 * Original: enum class AlbumViewMode(override val stringValue: String) : SerializableEnum<AlbumViewMode> {
 *   Cards("cards"), List("list"), Grid("grid");
 *   companion object { val settingSerializer = EnumSettingSerializer(entries) }
 * }
 *
 * Every enum value, field, and companion object from the original Kotlin file.
 */
class AlbumViewMode : public SerializableEnum<AlbumViewMode> {
public:
	// Port of: Cards("cards")
	static const AlbumViewMode Cards;
	// Port of: List("list")
	static const AlbumViewMode List;
	// Port of: Grid("grid")
	static const AlbumViewMode Grid;

	// Port of: companion object { val settingSerializer = EnumSettingSerializer(entries) }
	static const EnumSettingSerializer<AlbumViewMode> settingSerializer;

	// Port of: entries (all values)
	static const std::vector<AlbumViewMode>& entries();

	// Port of: override val stringValue: String
	std::string stringValue() const override { return m_stringValue; }

	// Equality operators
	bool operator==(const AlbumViewMode& other) const { return m_stringValue == other.m_stringValue; }
	bool operator!=(const AlbumViewMode& other) const { return m_stringValue != other.m_stringValue; }

private:
	// Port of: constructor(val stringValue: String)
	explicit AlbumViewMode(const std::string& strValue) : m_stringValue(strValue) {}

	std::string m_stringValue;
};

} // namespace PinkReader
