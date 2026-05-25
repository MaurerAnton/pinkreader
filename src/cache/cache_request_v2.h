// Origin: org/quantumbadger/redreader/cache/CacheRequest.java
#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <atomic>
#include <stdexcept>

#include "download_strategy.h"
#include "cache_request_callbacks.h"

namespace PinkReader {

class UriString;
class RedditAccount;
class TimestampUTC;
class UUID;
class CacheDownload;
class CacheManager;
class RRError;
class SeekableInputStream;
struct HTTPRequestBody;

class CacheRequest {
public:
	enum class DownloadQueueType {
		REDDIT_API,
		IMGUR_API,
		IMMEDIATE,
		IMAGE_PRECACHE,
		REDGIFS_API_V2
	};

	enum class RequestFailureType {
		CONNECTION,
		REQUEST,
		STORAGE,
		CACHE_MISS,
		CANCELLED,
		MALFORMED_URL,
		PARSE,
		DISK_SPACE,
		REDDIT_REDIRECT,
		PARSE_IMGUR,
		UPLOAD_FAIL_IMGUR,
		CACHE_DIR_DOES_NOT_EXIST
	};

	const UriString& url;
	const RedditAccount& user;
	const UUID* requestSession;

	int priority; // Priority type stubbed as int (higher = more important)

	const DownloadStrategy& downloadStrategy;

	int fileType;
	DownloadQueueType queueType;
	const HTTPRequestBody* requestBody; // optional
	bool cache;

	void* context; // Opaque context pointer (was android.content.Context)

private:
	CacheDownload* m_download;
	bool m_cancelled;
	CacheRequestCallbacks& m_callbacks;

public:
	// Constructor 1 (no body, cache=true defaulted)
	CacheRequest(
			const UriString& url,
			const RedditAccount& user,
			const UUID* requestSession,
			int priority,
			const DownloadStrategy& downloadStrategy,
			int fileType,
			DownloadQueueType queueType,
			bool cache,
			void* context,
			CacheRequestCallbacks& callbacks);

	// Constructor 2 (no body, cache=true)
	CacheRequest(
			const UriString& url,
			const RedditAccount& user,
			const UUID* requestSession,
			int priority,
			const DownloadStrategy& downloadStrategy,
			int fileType,
			DownloadQueueType queueType,
			void* context,
			CacheRequestCallbacks& callbacks);

	// Constructor 3 (no body param, no cache param)
	CacheRequest(
			const UriString& url,
			const RedditAccount& user,
			const UUID* requestSession,
			int priority,
			const DownloadStrategy& downloadStrategy,
			int fileType,
			DownloadQueueType queueType,
			const HTTPRequestBody* requestBody,
			void* context,
			CacheRequestCallbacks& callbacks);

	// Private full constructor
private:
	CacheRequest(
			const UriString& url,
			const RedditAccount& user,
			const UUID* requestSession,
			int priority,
			const DownloadStrategy& downloadStrategy,
			int fileType,
			DownloadQueueType queueType,
			const HTTPRequestBody* requestBody,
			bool cache,
			void* context,
			CacheRequestCallbacks& callbacks);

public:
	// Called by CacheDownload
	bool setDownload(CacheDownload& download);

	// Can be called to cancel the request
	void cancel();

	// Comparable (priority-based)
	int compareTo(const CacheRequest& another) const;

	// Callbacks
	void notifyDataStreamAvailable(
			const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
			const TimestampUTC& timestamp,
			const UUID& session,
			bool fromCache,
			const std::string* mimetype);

	void notifyDataStreamComplete(
			const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
			const TimestampUTC& timestamp,
			const UUID& session,
			bool fromCache,
			const std::string* mimetype);

	void notifyFailure(const RRError& error);

	void notifyProgress(
			bool authorizationInProgress,
			int64_t bytesRead,
			int64_t totalBytes);

	void notifyCacheFileWritten(
			const CacheManager::ReadableCacheFile& cacheFile,
			const TimestampUTC& timestamp,
			const UUID& session,
			bool fromCache,
			const std::string& mimetype);

	void notifyDownloadNecessary();
	void notifyDownloadStarted();

private:
	void onCallbackException(const std::exception& t);
};

} // namespace PinkReader
