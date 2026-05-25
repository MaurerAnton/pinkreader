// Origin: RedReader/src/main/java/org/quantumbadger/redreader/settings/types/SettingSerializer.kt
#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.settings.types.SettingSerializer<T>
 *
 * Original: interface SettingSerializer<T> {
 *   fun serialize(value: T): String
 *   fun deserialize(value: String): T?
 * }
 */
template<typename T>
class SettingSerializer {
public:
	virtual ~SettingSerializer() = default;

	// Port of: fun serialize(value: T): String
	virtual std::string serialize(const T& value) const = 0;

	// Port of: fun deserialize(value: String): T?
	virtual std::optional<T> deserialize(const std::string& value) const = 0;
};

/**
 * @brief Port of org.quantumbadger.redreader.settings.types.SerializableEnum<T>
 *
 * Original: interface SerializableEnum<T : Enum<T>> {
 *   val stringValue: String
 * }
 */
template<typename T>
class SerializableEnum {
public:
	virtual ~SerializableEnum() = default;

	// Port of: val stringValue: String
	virtual std::string stringValue() const = 0;
};

/**
 * @brief Port of org.quantumbadger.redreader.settings.types.EnumSettingSerializer<T>
 *
 * Original: class EnumSettingSerializer<T>(
 *   values: Iterable<T>
 * ) : SettingSerializer<T> where T : SerializableEnum<T>, T : Enum<T> {
 *   private val lookupTable = values.associateBy { it.stringValue }
 *   override fun serialize(value: T) = value.stringValue
 *   override fun deserialize(value: String) = lookupTable[value]
 * }
 *
 * Every field and method from the original Kotlin file.
 */
template<typename T>
class EnumSettingSerializer : public SettingSerializer<T> {
public:
	// Port of: constructor(values: Iterable<T>)
	// Port of: private val lookupTable = values.associateBy { it.stringValue }
	explicit EnumSettingSerializer(const std::vector<T>& values) {
		for (const auto& v : values) {
			m_lookupTable[v.stringValue()] = v;
		}
	}

	// Port of: override fun serialize(value: T) = value.stringValue
	std::string serialize(const T& value) const override {
		return value.stringValue();
	}

	// Port of: override fun deserialize(value: String) = lookupTable[value]
	std::optional<T> deserialize(const std::string& value) const override {
		auto it = m_lookupTable.find(value);
		if (it != m_lookupTable.end()) {
			return it->second;
		}
		return std::nullopt;
	}

private:
	// Port of: private val lookupTable = values.associateBy { it.stringValue }
	std::unordered_map<std::string, T> m_lookupTable;
};

} // namespace PinkReader
