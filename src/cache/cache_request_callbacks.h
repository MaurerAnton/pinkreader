// Origin: org/quantumbadger/redreader/cache/CacheRequestCallbacks.java
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <stdexcept>
#include <cstdint>

namespace PinkReader {

class RRError;
class ReadableCacheFile;
class SeekableInputStream;
class TimestampUTC;
class UUID;

template<typename T, typename E>
class GenericFactory {
public:
	virtual ~GenericFactory() = default;
	virtual T create() = 0;
};

class CacheRequestCallbacks {
public:
	virtual ~CacheRequestCallbacks() = default;

	virtual void onDownloadNecessary() {}

	virtual void onDownloadStarted() {}

	virtual void onDataStreamAvailable(
			const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
			const TimestampUTC& timestamp,
			const UUID& session,
			bool fromCache,
			const std::string* mimetype) {}

	virtual void onDataStreamComplete(
			const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
			const TimestampUTC& timestamp,
			const UUID& session,
			bool fromCache,
			const std::string* mimetype) {}

	virtual void onProgress(
			bool authorizationInProgress,
			int64_t bytesRead,
			int64_t totalBytes) {}

	virtual void onFailure(const RRError& error) = 0;

	virtual void onCacheFileWritten(
			const ReadableCacheFile& cacheFile,
			const TimestampUTC& timestamp,
			const UUID& session,
			bool fromCache,
			const std::string* mimetype) {}
};

} // namespace PinkReader
