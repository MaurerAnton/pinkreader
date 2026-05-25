// Origin: org/quantumbadger/redreader/cache/downloadstrategy/DownloadStrategyIfTimestampOutsideBounds.kt
// (Kotlin data class ported to C++)
#pragma once

#include "download_strategy.h"
#include <memory>

namespace PinkReader {

class TimestampBound;

class DownloadStrategyIfTimestampOutsideBounds : public DownloadStrategy {
private:
	const TimestampBound& m_timestampBound;

public:
	explicit DownloadStrategyIfTimestampOutsideBounds(const TimestampBound& timestampBound)
		: m_timestampBound(timestampBound) {}

	bool shouldDownloadWithoutCheckingCache() const override {
		return false;
	}

	bool shouldDownloadIfCacheEntryFound(const CacheEntry& entry) const override;

	bool shouldDownloadIfNotCached() const override {
		return true;
	}
};

} // namespace PinkReader
