// Origin: org/quantumbadger/redreader/cache/CacheEntry.java
#pragma once

#include <string>
#include <cstdint>
#include <array>

namespace PinkReader {

class UriString;
class TimestampUTC;
class UUID;
enum class CacheCompressionType;

class CacheEntry {
public:
	static const std::array<const char*, 8> DB_FIELDS;

	const int64_t id;
	const UriString& url;
	const UUID& session;
	const TimestampUTC& timestamp;
	const std::string mimetype;
	const CacheCompressionType cacheCompressionType;
	const int64_t lengthCompressed;
	const int64_t lengthUncompressed;

	// Constructed from a "cursor" (row data)
	// In C++ we accept individual values rather than a cursor object
	CacheEntry(
			int64_t id,
			const UriString& url,
			const UUID& session,
			const TimestampUTC& timestamp,
			const std::string& mimetype,
			CacheCompressionType cacheCompressionType,
			int64_t lengthCompressed,
			int64_t lengthUncompressed)
		: id(id)
		, url(url)
		, session(session)
		, timestamp(timestamp)
		, mimetype(mimetype)
		, cacheCompressionType(cacheCompressionType)
		, lengthCompressed(lengthCompressed)
		, lengthUncompressed(lengthUncompressed) {}
};

} // namespace PinkReader
