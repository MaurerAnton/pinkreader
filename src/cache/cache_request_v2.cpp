// Origin: org/quantumbadger/redreader/cache/CacheRequest.java
#include "cache_request_v2.h"

namespace PinkReader {

// Constructor 1 (with cache param)
CacheRequest::CacheRequest(
		const UriString& url,
		const RedditAccount& user,
		const UUID* requestSession,
		int priority,
		const DownloadStrategy& downloadStrategy,
		int fileType,
		DownloadQueueType queueType,
		bool cache,
		void* context,
		CacheRequestCallbacks& callbacks)
	: CacheRequest(url, user, requestSession, priority, downloadStrategy,
		fileType, queueType, nullptr, cache, context, callbacks) {}

// Constructor 2 (cache=true default)
CacheRequest::CacheRequest(
		const UriString& url,
		const RedditAccount& user,
		const UUID* requestSession,
		int priority,
		const DownloadStrategy& downloadStrategy,
		int fileType,
		DownloadQueueType queueType,
		void* context,
		CacheRequestCallbacks& callbacks)
	: CacheRequest(url, user, requestSession, priority, downloadStrategy,
		fileType, queueType, nullptr, true, context, callbacks) {}

// Constructor 3 (with body, no cache)
CacheRequest::CacheRequest(
		const UriString& url,
		const RedditAccount& user,
		const UUID* requestSession,
		int priority,
		const DownloadStrategy& downloadStrategy,
		int fileType,
		DownloadQueueType queueType,
		const HTTPRequestBody* requestBody,
		void* context,
		CacheRequestCallbacks& callbacks)
	: CacheRequest(url, user, requestSession, priority, downloadStrategy,
		fileType, queueType, requestBody, false, context, callbacks) {}

// Private full constructor
CacheRequest::CacheRequest(
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
		CacheRequestCallbacks& callbacks)
	: url(url)
	, user(user)
	, requestSession(requestSession)
	, priority(priority)
	, downloadStrategy(downloadStrategy)
	, fileType(fileType)
	, queueType(queueType)
	, requestBody(requestBody)
	, cache(requestBody == nullptr && cache)
	, context(context)
	, m_download(nullptr)
	, m_cancelled(false)
	, m_callbacks(callbacks) {

	if(!downloadStrategy.shouldDownloadWithoutCheckingCache() && requestBody != nullptr) {
		throw std::invalid_argument(
			"Should not perform cache lookup for POST requests");
	}

	// context = context.getApplicationContext() - stubbed
	// Null checks for user, url handled upstream
}

bool CacheRequest::setDownload(CacheDownload& download) {
	if(m_cancelled) {
		return false;
	}
	m_download = &download;
	return true;
}

void CacheRequest::cancel() {
	m_cancelled = true;

	if(m_download != nullptr) {
		m_download->cancel();
		m_download = nullptr;
	}
}

int CacheRequest::compareTo(const CacheRequest& another) const {
	// Higher priority = lower value (Java convention)
	if(priority > another.priority) {
		return -1;
	} else if(another.priority > priority) {
		return 1;
	}
	return 0;
}

void CacheRequest::onCallbackException(const std::exception& t) {
	// Log.e("CacheRequest", "Exception thrown from callback", t);
	// BugReportActivity.handleGlobalError(context, t);
}

void CacheRequest::notifyDataStreamAvailable(
		const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
		const TimestampUTC& timestamp,
		const UUID& session,
		bool fromCache,
		const std::string* mimetype) {
	m_callbacks.onDataStreamAvailable(streamFactory, timestamp, session, fromCache, mimetype);
}

void CacheRequest::notifyDataStreamComplete(
		const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
		const TimestampUTC& timestamp,
		const UUID& session,
		bool fromCache,
		const std::string* mimetype) {
	m_callbacks.onDataStreamComplete(streamFactory, timestamp, session, fromCache, mimetype);
}

void CacheRequest::notifyFailure(const RRError& error) {
	try {
		m_callbacks.onFailure(error);
	} catch(const std::exception& t1) {
		onCallbackException(t1);
	}
}

void CacheRequest::notifyProgress(
		bool authorizationInProgress,
		int64_t bytesRead,
		int64_t totalBytes) {
	try {
		m_callbacks.onProgress(authorizationInProgress, bytesRead, totalBytes);
	} catch(const std::exception& t) {
		onCallbackException(t);
	}
}

void CacheRequest::notifyCacheFileWritten(
		const CacheManager::ReadableCacheFile& cacheFile,
		const TimestampUTC& timestamp,
		const UUID& session,
		bool fromCache,
		const std::string& mimetype) {
	try {
		m_callbacks.onCacheFileWritten(cacheFile, timestamp, session, fromCache, &mimetype);
	} catch(const std::exception& t) {
		onCallbackException(t);
	}
}

void CacheRequest::notifyDownloadNecessary() {
	try {
		m_callbacks.onDownloadNecessary();
	} catch(const std::exception& t1) {
		// Log.e("CacheRequest", "Exception thrown by onDownloadNecessary", t1);
		try {
			onCallbackException(t1);
		} catch(const std::exception& t2) {
			// Log.e("CacheRequest", "Exception thrown by onCallbackException", t2);
			// BugReportActivity.addGlobalError(...);
			// BugReportActivity.handleGlobalError(context, t2);
		}
	}
}

void CacheRequest::notifyDownloadStarted() {
	try {
		m_callbacks.onDownloadStarted();
	} catch(const std::exception& t1) {
		// Log.e("CacheRequest", "Exception thrown by onDownloadStarted", t1);
		try {
			onCallbackException(t1);
		} catch(const std::exception& t2) {
			// Log.e("CacheRequest", "Exception thrown by onCallbackException", t2);
			// BugReportActivity.addGlobalError(...);
			// BugReportActivity.handleGlobalError(context, t2);
		}
	}
}

} // namespace PinkReader
