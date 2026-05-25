// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/SerializeUtils.java
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <stdexcept>
#include <memory>
#include <any>
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace PinkReader {

// Forward-declare stream types
class DataOutputStream;
class DataInputStream;

/**
 * Custom hash functor for std::any used as map/set keys.
 * Only supports valid hash key types: std::string, uint8_t, char16_t,
 * int16_t, int32_t, int64_t, bool.
 */
struct AnyHash {
	std::size_t operator()(const std::any& value) const;
};

/**
 * Custom equality functor for std::any used as map/set keys.
 */
struct AnyEqual {
	bool operator()(const std::any& a, const std::any& b) const;
};

/** A map with std::any keys and std::any values (serialized map type). */
using AnyMap = std::unordered_map<std::any, std::any, AnyHash, AnyEqual>;

/** A set with std::any elements (serialized set type). */
using AnySet = std::unordered_set<std::any, AnyHash, AnyEqual>;

/** A list with std::any elements (serialized list type). */
using AnyList = std::list<std::any>;

/**
 * Binary serialization/deserialization utility for primitive types,
 * Strings, Sets, Lists, and Maps.
 * Equivalent to: org.quantumbadger.redreader.common.SerializeUtils
 */
class SerializeUtils {
public:
	/**
	 * Exception thrown when an unhandled type is encountered during serialization.
	 * Equivalent to: SerializeUtils.UnhandledTypeException
	 */
	class UnhandledTypeException : public std::runtime_error {
	public:
		explicit UnhandledTypeException(const std::string& message)
			: std::runtime_error(message) {}
	};

private:
	SerializeUtils() = delete;

	static const int32_t COMPRESSED_FILE_VERSION;
	static const std::string COMPRESSED_FILE_USER_HEADER_STR;

	/** Data type constants for binary serialization. Equivalent to SerializeUtils.DataType enum. */
	enum class DataType : uint8_t {
		NULL_T   = 0,
		BYTE     = 1,
		CHAR     = 2,
		SHORT    = 3,
		INT      = 4,
		LONG     = 5,
		FLOAT    = 6,
		DOUBLE   = 7,
		SET      = 8,
		LIST     = 9,
		MAP      = 10,
		STRING   = 11,
		BOOLEAN  = 12,
	};

	static DataType fromConstant(uint8_t value);

	/**
	 * Checks whether a value can be used as a hash key in serialized Sets/Maps.
	 * Equivalent to: SerializeUtils.isInvalidHashKey(Object)
	 */
	static bool isInvalidHashKey(const std::any& value);

public:
	/**
	 * Serializes a value to a DataOutputStream.
	 * Equivalent to: SerializeUtils.serialize(DataOutputStream, Object)
	 */
	static void serialize(DataOutputStream& destination, const std::any& value);

	/**
	 * Deserializes a value from a DataInputStream.
	 * Equivalent to: SerializeUtils.deserialize(DataInputStream)
	 */
	static std::any deserialize(DataInputStream& source);

	/**
	 * Serializes a value with Zstd compression.
	 * Equivalent to: SerializeUtils.serializeCompressed(DataOutputStream, Object)
	 */
	static void serializeCompressed(DataOutputStream& destination, const std::any& value);

	/**
	 * Deserializes a Zstd-compressed value from a DataInputStream.
	 * Equivalent to: SerializeUtils.deserializeCompressed(DataInputStream)
	 */
	static std::any deserializeCompressed(DataInputStream& source);
};

// -- Portable DataOutputStream/DataInputStream interfaces --

/** Portable binary output stream (replaces java.io.DataOutputStream). */
class DataOutputStream {
public:
	virtual ~DataOutputStream() = default;
	virtual void writeByte(uint8_t value) = 0;
	virtual void writeChar(char16_t value) = 0;
	virtual void writeShort(int16_t value) = 0;
	virtual void writeInt(int32_t value) = 0;
	virtual void writeLong(int64_t value) = 0;
	virtual void writeFloat(float value) = 0;
	virtual void writeDouble(double value) = 0;
	virtual void writeBoolean(bool value) = 0;
	virtual void write(const uint8_t* data, size_t length) = 0;
	virtual void flush() = 0;
};

/** Portable binary input stream (replaces java.io.DataInputStream). */
class DataInputStream {
public:
	virtual ~DataInputStream() = default;
	virtual uint8_t readByte() = 0;
	virtual char16_t readChar() = 0;
	virtual int16_t readShort() = 0;
	virtual int32_t readInt() = 0;
	virtual int64_t readLong() = 0;
	virtual float readFloat() = 0;
	virtual double readDouble() = 0;
	virtual bool readBoolean() = 0;
	virtual void readFully(uint8_t* data, size_t length) = 0;
};

} // namespace PinkReader
