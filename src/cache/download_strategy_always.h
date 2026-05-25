// Origin: org/quantumbadger/redreader/cache/downloadstrategy/DownloadStrategyAlways.java
#pragma once

#include "download_strategy.h"

namespace PinkReader {

class DownloadStrategyAlways : public DownloadStrategy {
private:
	DownloadStrategyAlways() {}

public:
	static const DownloadStrategyAlways INSTANCE;

	bool shouldDownloadWithoutCheckingCache() const override {
		return true;
	}

	bool shouldDownloadIfCacheEntryFound(const CacheEntry& entry) const override {
		// Should never get here
		return true;
	}

	bool shouldDownloadIfNotCached() const override {
		// Should never get here
		return true;
	}
};

} // namespace PinkReader
