// Origin: org/quantumbadger/redreader/cache/PrioritisedDownloadQueue.java
#pragma once

#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <stdexcept>

namespace PinkReader {

class CacheRequest;
class CacheManager;
class CacheDownload;

class PrioritisedDownloadQueue {
private:
	std::unordered_set<CacheDownload*> m_redditDownloadsQueued;
	std::mutex m_mutex;
	std::condition_variable m_cv;

	// Thread pool for non-reddit downloads (5 threads)
	// PrioritisedCachedThreadPool m_downloadThreadPool(5, "Download");
	void* m_context;

public:
	explicit PrioritisedDownloadQueue(void* context);

	void add(const CacheRequest& request, CacheManager& manager);

private:
	CacheDownload* getNextRedditInQueue();

	// Inner class RedditQueueProcessor
	void redditQueueProcessorLoop();

	std::thread m_redditQueueThread;
};

} // namespace PinkReader
