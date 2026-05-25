// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ParcelHelper.java
#pragma once

#include <string>
#include <cstdint>
#include <memory>

namespace PinkReader {

// Forward-declare a simple DataStream interface to replace Android's Parcel.
// In practice, platform-specific implementations would fill this in.

/**
 * Abstract byte-oriented read stream (replaces android.os.Parcel read side).
 */
class DataReader {
public:
	virtual ~DataReader() = default;
	virtual uint8_t readByte() = 0;
	virtual int32_t readInt() = 0;
	virtual int64_t readLong() = 0;
	virtual std::string readString() = 0;
};

/**
 * Abstract byte-oriented write stream (replaces android.os.Parcel write side).
 */
class DataWriter {
public:
	virtual ~DataWriter() = default;
	virtual void writeByte(uint8_t value) = 0;
	virtual void writeInt(int32_t value) = 0;
	virtual void writeLong(int64_t value) = 0;
	virtual void writeString(const std::string& value) = 0;
};

/**
 * Helper methods for reading/writing nullable types, booleans, and enums
 * using a parcel-like binary stream.
 * Equivalent to: org.quantumbadger.redreader.common.ParcelHelper
 */
class ParcelHelper {
public:
	ParcelHelper() = delete;

	/** Reads a boolean (1 byte: 0 = false, 1 = true). */
	static bool readBoolean(DataReader& in);

	/** Reads a nullable string (preceded by a boolean flag). */
	static std::string* readNullableString(DataReader& in);

	/** Reads a nullable int (preceded by a boolean flag). */
	static int32_t* readNullableInt(DataReader& in);

	/** Reads a nullable long (preceded by a boolean flag). */
	static int64_t* readNullableLong(DataReader& in);

	/** Reads a nullable boolean (preceded by a boolean flag). */
	static bool* readNullableBoolean(DataReader& in);

	/** Writes a boolean as a single byte. */
	static void writeBoolean(DataWriter& writer, bool b);

	/** Writes a nullable string with a boolean presence flag. */
	static void writeNullableString(DataWriter& writer, const std::string* value);

	/** Writes a nullable long with a boolean presence flag. */
	static void writeNullableLong(DataWriter& writer, const int64_t* value);

	/** Writes a nullable boolean with a boolean presence flag. */
	static void writeNullableBoolean(DataWriter& writer, const bool* value);

	/**
	 * Writes a nullable enum by writing its name (or a null flag).
	 * Equivalent to: ParcelHelper.writeNullableEnum(Parcel, Enum)
	 */
	template<typename EnumType>
	static void writeNullableEnum(DataWriter& writer, const EnumType* value) {
		if (value == nullptr) {
			writeBoolean(writer, false);
		} else {
			writeBoolean(writer, true);
			writer.writeString(enumToString(*value));
		}
	}

	/**
	 * Writes a non-null enum by writing its name.
	 * Equivalent to: ParcelHelper.writeNonNullEnum(Parcel, Enum)
	 */
	template<typename EnumType>
	static void writeNonNullEnum(DataWriter& writer, const EnumType& value) {
		writer.writeString(enumToString(value));
	}
};

} // namespace PinkReader
