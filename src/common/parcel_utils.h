// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ParcelUtils.java
#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace PinkReader {

// Forward-declare DataReader/DataWriter from parcel_helper.h
class DataReader;
class DataWriter;

/**
 * Alternative nullable type serialization utilities for parcel-like streams.
 * Uses int/long markers instead of boolean flags for nullable encoding.
 * Equivalent to: org.quantumbadger.redreader.common.ParcelUtils
 */
class ParcelUtils {
public:
	ParcelUtils() = delete;

	/**
	 * Writes a nullable boolean using an int marker:
	 *   0 = null, 1 = true, -1 = false
	 */
	static void writeNullableBoolean(DataWriter& writer, const bool* value);

	/**
	 * Reads a nullable boolean using an int marker:
	 *   -1 = false, 0 = null, 1 = true
	 */
	static bool* readNullableBoolean(DataReader& reader);

	/**
	 * Writes a nullable int with a presence marker:
	 *   0 = null, 1 = present (followed by value)
	 */
	static void writeNullableInt(DataWriter& writer, const int32_t* value);

	/**
	 * Reads a nullable int with a presence marker.
	 */
	static int32_t* readNullableInt(DataReader& reader);

	/**
	 * Writes a nullable long with a presence marker:
	 *   0 = null, 1 = present (followed by value)
	 */
	static void writeNullableLong(DataWriter& writer, const int64_t* value);

	/**
	 * Reads a nullable long with a presence marker.
	 */
	static int64_t* readNullableLong(DataReader& reader);
};

} // namespace PinkReader
