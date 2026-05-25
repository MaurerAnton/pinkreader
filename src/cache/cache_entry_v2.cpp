// Origin: org/quantumbadger/redreader/cache/CacheEntry.java
#include "cache_entry_v2.h"

namespace PinkReader {

// DB_FIELDS matches: FIELD_ID, FIELD_URL, FIELD_SESSION, FIELD_TIMESTAMP,
// FIELD_MIMETYPE, FIELD_COMPRESSION_TYPE, FIELD_LENGTH_COMPRESSED, FIELD_LENGTH_UNCOMPRESSED
const std::array<const char*, 8> CacheEntry::DB_FIELDS = {
	"id",
	"url",
	"session",
	"timestamp",
	"mimetype",
	"compressionType",
	"lengthCompressed",
	"lengthUncompressed"
};

} // namespace PinkReader
