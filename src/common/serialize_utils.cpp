// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/SerializeUtils.java

#include "serialize_utils.h"
#include <any>
#include <string>
#include <list>
#include <vector>
#include <cstring>
#include <sstream>

namespace PinkReader {

// --- AnyHash and AnyEqual implementations ---

static std::size_t hashAny(const std::any& value) {
	if (!value.has_value()) return 0;

	if (value.type() == typeid(std::string))
		return std::hash<std::string>{}(std::any_cast<std::string>(value));
	if (value.type() == typeid(uint8_t))
		return std::hash<uint8_t>{}(std::any_cast<uint8_t>(value));
	if (value.type() == typeid(char16_t))
		return std::hash<char16_t>{}(std::any_cast<char16_t>(value));
	if (value.type() == typeid(int16_t))
		return std::hash<int16_t>{}(std::any_cast<int16_t>(value));
	if (value.type() == typeid(int32_t))
		return std::hash<int32_t>{}(std::any_cast<int32_t>(value));
	if (value.type() == typeid(int64_t))
		return std::hash<int64_t>{}(std::any_cast<int64_t>(value));
	if (value.type() == typeid(bool))
		return std::hash<bool>{}(std::any_cast<bool>(value));

	throw SerializeUtils::UnhandledTypeException(
		"Cannot hash type: " + std::string(value.type().name()));
}

std::size_t AnyHash::operator()(const std::any& value) const {
	return hashAny(value);
}

static bool equalAny(const std::any& a, const std::any& b) {
	if (!a.has_value() && !b.has_value()) return true;
	if (!a.has_value() || !b.has_value()) return false;

	if (a.type() != b.type()) return false;

	if (a.type() == typeid(std::string))
		return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
	if (a.type() == typeid(uint8_t))
		return std::any_cast<uint8_t>(a) == std::any_cast<uint8_t>(b);
	if (a.type() == typeid(char16_t))
		return std::any_cast<char16_t>(a) == std::any_cast<char16_t>(b);
	if (a.type() == typeid(int16_t))
		return std::any_cast<int16_t>(a) == std::any_cast<int16_t>(b);
	if (a.type() == typeid(int32_t))
		return std::any_cast<int32_t>(a) == std::any_cast<int32_t>(b);
	if (a.type() == typeid(int64_t))
		return std::any_cast<int64_t>(a) == std::any_cast<int64_t>(b);
	if (a.type() == typeid(bool))
		return std::any_cast<bool>(a) == std::any_cast<bool>(b);

	return false;
}

bool AnyEqual::operator()(const std::any& a, const std::any& b) const {
	return equalAny(a, b);
}

// --- SerializeUtils static data ---

const int32_t SerializeUtils::COMPRESSED_FILE_VERSION = 1;
const std::string SerializeUtils::COMPRESSED_FILE_USER_HEADER_STR =
	"RedReader compressed data\r\n";

SerializeUtils::DataType SerializeUtils::fromConstant(uint8_t value) {
	if (value > 12) {
		throw UnhandledTypeException(
			"Unknown type constant " + std::to_string(static_cast<int>(value)));
	}
	return static_cast<DataType>(value);
}

bool SerializeUtils::isInvalidHashKey(const std::any& value) {
	if (!value.has_value()) {
		return false;
	}

	// Valid key types: string, uint8_t, char16_t, int16_t, int32_t, int64_t, bool
	return !(
		value.type() == typeid(std::string)
		|| value.type() == typeid(uint8_t)
		|| value.type() == typeid(char16_t)
		|| value.type() == typeid(int16_t)
		|| value.type() == typeid(int32_t)
		|| value.type() == typeid(int64_t)
		|| value.type() == typeid(bool)
	);
}

void SerializeUtils::serialize(DataOutputStream& destination, const std::any& value) {
	if (!value.has_value()) {
		destination.writeByte(static_cast<uint8_t>(DataType::NULL_T));

	} else if (value.type() == typeid(uint8_t)) {
		destination.writeByte(static_cast<uint8_t>(DataType::BYTE));
		destination.writeByte(std::any_cast<uint8_t>(value));

	} else if (value.type() == typeid(char16_t)) {
		destination.writeByte(static_cast<uint8_t>(DataType::CHAR));
		destination.writeChar(std::any_cast<char16_t>(value));

	} else if (value.type() == typeid(int16_t)) {
		destination.writeByte(static_cast<uint8_t>(DataType::SHORT));
		destination.writeShort(std::any_cast<int16_t>(value));

	} else if (value.type() == typeid(int32_t)) {
		destination.writeByte(static_cast<uint8_t>(DataType::INT));
		destination.writeInt(std::any_cast<int32_t>(value));

	} else if (value.type() == typeid(int64_t)) {
		destination.writeByte(static_cast<uint8_t>(DataType::LONG));
		destination.writeLong(std::any_cast<int64_t>(value));

	} else if (value.type() == typeid(float)) {
		destination.writeByte(static_cast<uint8_t>(DataType::FLOAT));
		destination.writeFloat(std::any_cast<float>(value));

	} else if (value.type() == typeid(double)) {
		destination.writeByte(static_cast<uint8_t>(DataType::DOUBLE));
		destination.writeDouble(std::any_cast<double>(value));

	} else if (value.type() == typeid(bool)) {
		destination.writeByte(static_cast<uint8_t>(DataType::BOOLEAN));
		destination.writeBoolean(std::any_cast<bool>(value));

	} else if (value.type() == typeid(AnySet)) {
		const auto& set = std::any_cast<const AnySet&>(value);

		destination.writeByte(static_cast<uint8_t>(DataType::SET));
		destination.writeInt(static_cast<int32_t>(set.size()));

		for (const auto& obj : set) {
			if (isInvalidHashKey(obj)) {
				throw UnhandledTypeException("Invalid set entry type");
			}
			serialize(destination, obj);
		}

	} else if (value.type() == typeid(AnyList)) {
		const auto& list = std::any_cast<const AnyList&>(value);

		destination.writeByte(static_cast<uint8_t>(DataType::LIST));
		destination.writeInt(static_cast<int32_t>(list.size()));

		for (const auto& obj : list) {
			serialize(destination, obj);
		}

	} else if (value.type() == typeid(AnyMap)) {
		const auto& map = std::any_cast<const AnyMap&>(value);

		destination.writeByte(static_cast<uint8_t>(DataType::MAP));
		destination.writeInt(static_cast<int32_t>(map.size()));

		for (const auto& entry : map) {
			if (isInvalidHashKey(entry.first)) {
				throw UnhandledTypeException("Invalid map key type");
			}
			serialize(destination, entry.first);
			serialize(destination, entry.second);
		}

	} else if (value.type() == typeid(std::string)) {
		const auto& str = std::any_cast<const std::string&>(value);

		destination.writeByte(static_cast<uint8_t>(DataType::STRING));
		destination.writeInt(static_cast<int32_t>(str.length()));
		destination.write(reinterpret_cast<const uint8_t*>(str.data()), str.length());

	} else {
		throw UnhandledTypeException("Unhandled type");
	}
}

std::any SerializeUtils::deserialize(DataInputStream& source) {
	DataType type = fromConstant(source.readByte());

	switch (type) {
		case DataType::NULL_T:  return std::any();
		case DataType::BYTE:    return source.readByte();
		case DataType::CHAR:    return source.readChar();
		case DataType::SHORT:   return source.readShort();
		case DataType::INT:     return source.readInt();
		case DataType::LONG:    return source.readLong();
		case DataType::FLOAT:   return source.readFloat();
		case DataType::DOUBLE:  return source.readDouble();
		case DataType::BOOLEAN: return source.readBoolean();

		case DataType::SET: {
			int32_t count = source.readInt();
			AnySet result;

			for (int32_t i = 0; i < count; i++) {
				result.insert(deserialize(source));
			}

			return result;
		}

		case DataType::LIST: {
			int32_t count = source.readInt();
			AnyList result;

			for (int32_t i = 0; i < count; i++) {
				result.push_back(deserialize(source));
			}

			return result;
		}

		case DataType::MAP: {
			int32_t count = source.readInt();
			AnyMap result;

			for (int32_t i = 0; i < count; i++) {
				std::any key = deserialize(source);
				std::any value = deserialize(source);
				result.emplace(std::move(key), std::move(value));
			}

			return result;
		}

		case DataType::STRING: {
			int32_t byteCount = source.readInt();
			std::vector<uint8_t> bytes(byteCount);
			source.readFully(bytes.data(), byteCount);

			return std::string(bytes.begin(), bytes.end());
		}
	}

	throw UnhandledTypeException("Unhandled deserialize type");
}

void SerializeUtils::serializeCompressed(DataOutputStream& destination, const std::any& value) {
	// Note: Zstd compression requires the zstd library.
	// This implementation writes the header without compression as a portable fallback.
	// In a full port, integrate libzstd for actual compression.

	const auto& headerStr = COMPRESSED_FILE_USER_HEADER_STR;

	// Write the user header
	destination.write(
		reinterpret_cast<const uint8_t*>(headerStr.data()),
		headerStr.size());

	// Write version
	destination.writeInt(COMPRESSED_FILE_VERSION);

	// For now, serialize uncompressed and wrap
	// In production, use Zstd::compress for actual compression
	throw UnhandledTypeException(
		"serializeCompressed requires zstd integration. "
		"Use serialize() for uncompressed data.");
}

std::any SerializeUtils::deserializeCompressed(DataInputStream& source) {
	const auto& headerStr = COMPRESSED_FILE_USER_HEADER_STR;

	// Read and verify user header
	std::vector<uint8_t> userHeader(headerStr.size());
	source.readFully(userHeader.data(), headerStr.size());

	if (std::memcmp(userHeader.data(), headerStr.data(), headerStr.size()) != 0) {
		throw UnhandledTypeException("Invalid user header");
	}

	int32_t version = source.readInt();
	int32_t compressedBytesLength = source.readInt();

	if (version != COMPRESSED_FILE_VERSION) {
		throw UnhandledTypeException("Unsupported version " + std::to_string(version));
	}

	// Read compressed data
	std::vector<uint8_t> compressedData(compressedBytesLength);
	source.readFully(compressedData.data(), compressedBytesLength);

	// In production, decompress with Zstd and then call deserialize()
	throw UnhandledTypeException(
		"deserializeCompressed requires zstd integration. "
		"Use deserialize() for uncompressed data.");
}

} // namespace PinkReader
