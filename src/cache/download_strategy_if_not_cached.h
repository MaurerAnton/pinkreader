// Origin: org/quantumbadger/redreader/cache/downloadstrategy/DownloadStrategyIfNotCached.java
#pragma once

#include "download_strategy.h"

namespace PinkReader {

class DownloadStrategyIfNotCached : public DownloadStrategy {
private:
	DownloadStrategyIfNotCached() {}

public:
	static const DownloadStrategyIfNotCached INSTANCE;

	bool shouldDownloadWithoutCheckingCache() const override {
		return false;
	}

	bool shouldDownloadIfCacheEntryFound(const CacheEntry& entry) const override {
		return false;
	}

	bool shouldDownloadIfNotCached() const override {
		return true;
	}
};

} // namespace PinkReader
