// Origin: org/quantumbadger/redreader/cache/CacheDownload.java
#include "cache_download.h"
#include "cache_request_v2.h"
#include "cache_manager_v2.h"
#include "cache_compression_type.h"
#include "memory_data_stream.h"

#include <cstring>
#include <cstdio>
#include <stdexcept>
#include <thread>

namespace PinkReader {

const char* CacheDownload::TAG = "CacheDownload";
std::atomic<bool> CacheDownload::resetUserCredentials{false};

CacheDownload::CacheDownload(CacheRequest& initiator, CacheManager& manager)
	: mInitiator(initiator)
	, m_manager(manager)
	, m_session(initiator.requestSession) // will be set from initiator
	, m_request(nullptr) {

	if(!initiator.setDownload(*this)) {
		m_cancelled = true;
	}

	// HTTPBackend.getBackend().prepareRequest() - stubbed
	// m_request = HTTPBackend::getBackend().prepareRequest(context, details);
	(void)m_request; // suppress unused warning
}

void CacheDownload::cancel() {
	m_cancelled = true;

	std::thread([this]() {
		if(m_request != nullptr) {
			m_request->cancel();
			// mInitiator.notifyFailure(General.getGeneralErrorForFailure(...));
		}
	}).detach();
}

void CacheDownload::doDownload() {
	if(m_cancelled) {
		return;
	}

	try {
		if(m_request != nullptr) {
			performDownload(*m_request);
		}
	} catch(const std::exception& t) {
		// BugReportActivity.handleGlobalError(mInitiator.context, t);
	}
}

void CacheDownload::resetUserCredentialsOnNextRequest() {
	resetUserCredentials = true;
}

void CacheDownload::performDownload(HTTPBackendStub::Request& request) {
	// This method contains the full logic from the Java performDownload():
	// - Reddit API auth token handling
	// - Imgur/Redgifs API headers
	// - notifyDownloadStarted
	// - request.executeInThisThread with onError/onSuccess callbacks
	// - Download into MemoryDataStream
	// - notifyDataStreamAvailable/notifyDataStreamComplete
	// - Cache saving (writableCacheFile)
	//
	// The full implementation is stubbed here and would require:
	// - HTTPBackend integration
	// - RedditOAuth integration
	// - Full CacheManager::WritableCacheFile support
	// - General::getGeneralErrorForFailure
	// - BugReportActivity handleGlobalError
	// - TorCommon integration
	//
	// Key logic preserved as comments for reference:
	//
	// if(mInitiator.queueType == CacheRequest::DownloadQueueType::REDDIT_API) {
	//     if(resetUserCredentials.getAndSet(false)) {
	//         mInitiator.user.setAccessToken(null);
	//     }
	//     // ... access token fetch ...
	//     request.addHeader("Authorization", "bearer " + accessToken.token);
	// }
	//
	// if(mInitiator.queueType == CacheRequest::DownloadQueueType::IMGUR_API) {
	//     request.addHeader("Authorization", "Client-ID c3713d9e7674477");
	// } else if(mInitiator.queueType == CacheRequest::DownloadQueueType::REDGIFS_API_V2) {
	//     request.addHeader("Authorization", "Bearer " + RedgifsAPIV2.getLatestToken());
	// }
	//
	// mInitiator.notifyDownloadStarted();
	//
	// request.executeInThisThread(new HTTPBackend.Listener() {
	//     onError: notifyFailure with General.getGeneralErrorForFailure
	//     onSuccess: read into MemoryDataStream, notify progress, notify complete, save to cache
	// });
	(void)request;
}

int CacheDownload::getPriority() {
	return 0; // mInitiator.priority.isHigherPriorityThan equivalent
}

void CacheDownload::run() {
	doDownload();
}

int CacheDownload::tryReadFully(const uint8_t* src, int srcOffset, int srcLength, uint8_t* dst, int dstLength) {
	int totalBytesRead = 0;

	while(true) {
		int bytesToRead = dstLength - totalBytesRead;
		if(bytesToRead <= 0) {
			return totalBytesRead;
		}

		// In Java: src.read(dst, totalBytesRead, bytesToRead)
		int available = srcLength - (srcOffset + totalBytesRead);
		if(available <= 0) {
			return totalBytesRead;
		}

		int bytesRead = available < bytesToRead ? available : bytesToRead;
		std::memcpy(dst + totalBytesRead, src + srcOffset + totalBytesRead, bytesRead);
		totalBytesRead += bytesRead;

		if(totalBytesRead >= dstLength) {
			return totalBytesRead;
		}
	}
}

} // namespace PinkReader
