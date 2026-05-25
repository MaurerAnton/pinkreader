// Origin: org/quantumbadger/redreader/cache/downloadstrategy/DownloadStrategy.java
#pragma once

namespace PinkReader {

class CacheEntry;

class DownloadStrategy {
public:
	virtual ~DownloadStrategy() = default;

	virtual bool shouldDownloadWithoutCheckingCache() const = 0;

	virtual bool shouldDownloadIfCacheEntryFound(const CacheEntry& entry) const = 0;

	virtual bool shouldDownloadIfNotCached() const = 0;
};

} // namespace PinkReader
