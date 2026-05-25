// Origin: org/quantumbadger/redreader/cache/CacheManager.java
#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <functional>
#include <queue>
#include <thread>
#include <condition_variable>
#include <ostream>

namespace PinkReader {

class UriString;
class RedditAccount;
class TimestampUTC;
class TimeDuration;
class UUID;
class CacheEntry;
class CacheRequest;
class CacheDownload;
class CacheDbManager;
class PrioritisedDownloadQueue;
class PrioritisedCachedThreadPool;
class SeekableInputStream;
class SeekableFileInputStream;
enum class CacheCompressionType;

template<typename T, typename E>
class GenericFactory;

struct PrioritisedCacheRequest {
	CacheRequest* request;
	int priority;

	bool operator<(const PrioritisedCacheRequest& other) const {
		return priority < other.priority;
	}
};

// Forward declare ReadableCacheFile before WritableCacheFile
class CacheManager {
public:
	class ReadableCacheFile {
	private:
		int64_t mId;
		CacheCompressionType mCacheCompressionType;
		void* m_cachedUri;

	public:
		ReadableCacheFile(int64_t id, CacheCompressionType cacheCompressionType);

		int64_t getId() const;

		SeekableInputStream* getInputStream();

		void* getUri();

		bool getFile(std::string& outPath) const;

		bool lookupMimetype(std::string& outMimetype) const;

		std::string toString() const;
	};

	// Inner class WritableCacheFile
	class WritableCacheFile {
	private:
		std::ostream* m_outStream;
		CacheManager::ReadableCacheFile* m_readableCacheFile;

	public:
		const UriString& mUrl;
		const RedditAccount& mUser;
		int mFileType;

	private:
		std::string m_location;
		bool m_writeExternally;
		const UUID& mSession;
		const std::string* mMimetype;
		CacheCompressionType mCacheCompressionType;
		std::string m_tmpFile;
		int64_t m_uncompressedLength;
		int64_t m_compressedLength;

	public:
		WritableCacheFile(
				const UriString& url,
				const RedditAccount& user,
				int fileType,
				const UUID& session,
				const std::string* mimetype,
				CacheCompressionType cacheCompressionType);

		CacheManager::ReadableCacheFile& getReadableCacheFile();

		void writeWholeFile(const uint8_t* buf, int offset, int length);

		void onWriteFinished();

		std::string writeExternally();

		void onWriteCancelled();
	};

private:
	static const char* TAG;
	static const char* EXT;
	static const char* TEMP_EXT;

	static std::atomic<bool> isAlreadyInitialized;
	CacheDbManager& m_dbManager;

	std::priority_queue<PrioritisedCacheRequest> m_requests;
	std::mutex m_requestsMutex;
	std::condition_variable m_requestsCV;

	PrioritisedDownloadQueue& m_downloadQueue;

	void* m_context;

	static CacheManager* singleton;
	static std::mutex s_singletonMutex;

public:
	static CacheManager& getInstance(void* context);

private:
	explicit CacheManager(void* context);

public:
	// isCacheFile: returns cache ID if valid, -1 otherwise
	int64_t isCacheFile(const std::string& filePath);

	void getCacheFileList(const std::string& dir, std::unordered_set<int64_t>& currentFiles);

	static void pruneTemp(const std::string& dir);

	static std::vector<std::string> getCacheDirs(void* context);

	void pruneTemp();

	void pruneCache();
	void pruneCache(bool clearListings, bool clearThumbnails, bool clearImages);
	void pruneCache(const std::unordered_map<int, TimeDuration>& maxAge);

	void emptyTheWholeCache();

	std::unordered_map<int, int64_t> getCacheDataUsages();

	void makeRequest(CacheRequest& request);

	std::vector<CacheEntry> getSessions(const UriString& url, const RedditAccount& user);

	std::string getPreferredCacheLocation();

	ReadableCacheFile getExistingCacheFileById(
			int64_t cacheId,
			CacheCompressionType cacheCompressionType);

	WritableCacheFile openNewCacheFile(
			const UriString& url,
			const RedditAccount& user,
			int fileType,
			const UUID& session,
			const std::string& mimetype,
			CacheCompressionType cacheCompressionType);

private:
	std::string getExistingCacheFile(int64_t id);

	SeekableInputStream* getCacheFileInputStream(
			int64_t id,
			CacheCompressionType cacheCompressionType);

	void* getCacheFileUri(int64_t id);

	static std::string getSubdirForCacheFile(
			const std::string& cacheRoot,
			int64_t cacheFileId);

	void handleRequest(CacheRequest& request);
	void handleCacheEntryFound(const CacheEntry& entry, CacheRequest& request);
	void queueDownload(CacheRequest& request);
	CacheEntry* mostRecentFromList(const std::vector<CacheEntry>& list);
};

} // namespace PinkReader
