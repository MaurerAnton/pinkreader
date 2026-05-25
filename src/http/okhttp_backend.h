// Origin: RedReader http/okhttp/OKHTTPBackend.kt
#pragma once

#include "http_backend.h"
#include <string>
#include <memory>
#include <atomic>
#include <mutex>

namespace PinkReader {

// Forward declarations
class OkHttpClient;
class PrefsUtility;
class TorCommon;
class Constants;
class CacheRequest;

// Port of OKHTTPBackend - HTTP backend using OkHttp (or equivalent C++ HTTP library)
// In C++, this would use libcurl or a similar HTTP client library.
class OKHTTPBackend : public HTTPBackend {
private:
	OkHttpClient* mClient; // OkHttpClient equivalent

	OKHTTPBackend();

public:
	// Singleton pattern (companion object equivalent)
	static OKHTTPBackend& GetHttpBackend();

	// HTTPBackend overrides
	void RecreateHttpBackend() override;
	Result<UriString> ResolveRedirectUri(Context& context, const UriString& url) override;
	std::unique_ptr<Request> PrepareRequest(Context& context, const RequestDetails& details) override;

private:
	static constexpr const char* TAG = "OKHTTPBackend";
	static OKHTTPBackend* sHttpBackend;
	static std::mutex sMutex;
};

} // namespace PinkReader
