// Origin: org/quantumbadger/redreader/cache/CacheDownloadThread.java
#pragma once

#include <thread>
#include <string>
#include <memory>

namespace PinkReader {

class CacheDownload;

class CacheDownloadThread {
private:
	CacheDownload& m_singleDownload;
	std::thread m_thread;

public:
	CacheDownloadThread(
			CacheDownload& singleDownload,
			bool start,
			const std::string& name);

	~CacheDownloadThread();

	void run();

	void join();
	bool joinable() const;
};

} // namespace PinkReader
