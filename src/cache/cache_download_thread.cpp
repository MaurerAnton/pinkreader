// Origin: org/quantumbadger/redreader/cache/CacheDownloadThread.java
#include "cache_download_thread.h"
#include "cache_download.h"

namespace PinkReader {

CacheDownloadThread::CacheDownloadThread(
		CacheDownload& singleDownload,
		bool start,
		const std::string& name)
	: m_singleDownload(singleDownload) {
	if(start) {
		m_thread = std::thread(&CacheDownloadThread::run, this);
	}
}

CacheDownloadThread::~CacheDownloadThread() {
	if(m_thread.joinable()) {
		m_thread.join();
	}
}

void CacheDownloadThread::run() {
	// android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_BACKGROUND);
	m_singleDownload.doDownload();
}

void CacheDownloadThread::join() {
	if(m_thread.joinable()) {
		m_thread.join();
	}
}

bool CacheDownloadThread::joinable() const {
	return m_thread.joinable();
}

} // namespace PinkReader
