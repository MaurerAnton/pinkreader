// Origin: org/quantumbadger/redreader/cache/CacheDownload.java
#pragma once

#include <string>
#include <cstdint>
#include <atomic>
#include <memory>

namespace PinkReader {

class CacheRequest;
class CacheManager;
class HTTPBackend;
class MemoryDataStream;
struct TimestampUTC;
class UUID;
enum class CacheCompressionType;

// Stub for HTTPBackend::Request
namespace HTTPBackendStub {
	struct RequestDetails {
		std::string url;
		std::string requestBody; // nullable
	};

	class Request {
	public:
		virtual ~Request() = default;
		virtual void cancel() = 0;
		virtual void addHeader(const std::string& key, const std::string& value) = 0;

		class Listener {
		public:
			virtual ~Listener() = default;
			virtual void onError(
					int failureType,
					const std::runtime_error* exception,
					int httpStatus,
					const void* body) = 0;
			virtual void onSuccess(
					const std::string& mimetype,
					int64_t bodyBytes,
					const uint8_t* data,
					int64_t length) = 0;
		};

		virtual void executeInThisThread(Listener& listener) = 0;
	};
}

// Stub for PrioritisedCachedThreadPool::Task
class PrioritisedCachedThreadPoolTask {
public:
	virtual ~PrioritisedCachedThreadPoolTask() = default;
	virtual int getPriority() = 0;
	virtual void run() = 0;
};

class CacheDownload : public PrioritisedCachedThreadPoolTask {
private:
	static const char* TAG;

	CacheRequest& mInitiator;
	CacheManager& m_manager;
	const UUID& m_session;

	std::atomic<bool> m_cancelled{false};
	static std::atomic<bool> resetUserCredentials;
	HTTPBackendStub::Request* m_request;

public:
	CacheDownload(CacheRequest& initiator, CacheManager& manager);

	void cancel();

	void doDownload();

	int getPriority() override;
	void run() override;

	static void resetUserCredentialsOnNextRequest();

private:
	void performDownload(HTTPBackendStub::Request& request);
	static int tryReadFully(const uint8_t* src, int srcOffset, int srcLength, uint8_t* dst, int dstLength);
};

} // namespace PinkReader
