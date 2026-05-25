// Origin: org/quantumbadger/redreader/cache/downloadstrategy/DownloadStrategyNever.java
#pragma once

#include "download_strategy.h"

namespace PinkReader {

class DownloadStrategyNever : public DownloadStrategy {
private:
	DownloadStrategyNever() {}

public:
	static const DownloadStrategyNever INSTANCE;

	bool shouldDownloadWithoutCheckingCache() const override {
		return false;
	}

	bool shouldDownloadIfCacheEntryFound(const CacheEntry& entry) const override {
		return false;
	}

	bool shouldDownloadIfNotCached() const override {
		return false;
	}
};

} // namespace PinkReader
