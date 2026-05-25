// Origin: org/quantumbadger/redreader/cache/CacheCompressionType.java
#pragma once

#include <stdexcept>
#include <string>

namespace PinkReader {

enum class CacheCompressionType {
	NONE = 0,
	ZSTD = 1
};

inline int cacheCompressionTypeToDatabaseId(CacheCompressionType type) {
	return static_cast<int>(type);
}

inline CacheCompressionType cacheCompressionTypeFromDatabaseId(int databaseId) {
	switch(databaseId) {
		case 0: return CacheCompressionType::NONE;
		case 1: return CacheCompressionType::ZSTD;
		default:
			throw std::runtime_error("Unknown compression type " + std::to_string(databaseId));
	}
}

} // namespace PinkReader
