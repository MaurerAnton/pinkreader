// Origin: org/quantumbadger/redreader/cache/PrioritisedDownloadQueue.java
#include "prioritised_download_queue.h"
#include "cache_request_v2.h"
#include "cache_manager_v2.h"
#include "cache_download.h"
#include "cache_download_thread.h"

namespace PinkReader {

PrioritisedDownloadQueue::PrioritisedDownloadQueue(void* context)
	: m_context(context) {
	// new RedditQueueProcessor().start();
	m_redditQueueThread = std::thread(&PrioritisedDownloadQueue::redditQueueProcessorLoop, this);
}

void PrioritisedDownloadQueue::add(const CacheRequest& request, CacheManager& manager) {
	std::unique_lock<std::mutex> lock(m_mutex);

	CacheDownload* download = new CacheDownload(
		const_cast<CacheRequest&>(request), manager);

	if(request.queueType == CacheRequest::DownloadQueueType::REDDIT_API) {
		m_redditDownloadsQueued.insert(download);
		m_cv.notify_all();

	} else if(request.queueType == CacheRequest::DownloadQueueType::IMMEDIATE
			|| request.queueType == CacheRequest::DownloadQueueType::IMGUR_API) {
		// new CacheDownloadThread(download, true, "Cache Download Thread: Immediate");
		CacheDownloadThread* thread = new CacheDownloadThread(
			*download, true, "Cache Download Thread: Immediate");
		(void)thread; // Thread runs autonomously

	} else {
		// m_downloadThreadPool.add(download);
		// Stub: run directly
		download->run();
	}
}

CacheDownload* PrioritisedDownloadQueue::getNextRedditInQueue() {
	while(m_redditDownloadsQueued.empty()) {
		m_cv.wait(m_mutex);
	}

	CacheDownload* next = nullptr;

	for(CacheDownload* entry : m_redditDownloadsQueued) {
		if(next == nullptr || entry->getPriority() > next->getPriority()) {
			next = entry;
		}
	}

	m_redditDownloadsQueued.erase(next);

	return next;
}

void PrioritisedDownloadQueue::redditQueueProcessorLoop() {
	while(true) {
		CacheDownload* download;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			download = getNextRedditInQueue();
		}

		// new CacheDownloadThread(download, true, "Cache Download Thread: Reddit");
		CacheDownloadThread* thread = new CacheDownloadThread(
			*download, true, "Cache Download Thread: Reddit");
		(void)thread;

		// sleep(1200); // Delay imposed by reddit API restrictions.
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
	}
}

} // namespace PinkReader
