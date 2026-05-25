// Origin: RedReader http/HTTPBackend.kt
#pragma once

#include <memory>
#include <string>
#include <optional>
#include <functional>
#include "failed_request_body.h"
#include "http_request_body.h"

namespace PinkReader {

// Forward declarations
class Context;     // Android Context equivalent
class UriString;
template<typename T>
class Result;

// CacheRequest::RequestFailureType enum
enum class RequestFailureType {
	CONNECTION,
	REQUEST,
	PARSE,
	STORAGE,
	CANCELLED,
	OTHER
};

// Abstract HTTPBackend base class
class HTTPBackend {
public:
	struct RequestDetails {
		UriString url;
		HTTPRequestBody* requestBody; // nullable

		RequestDetails(const UriString& url, HTTPRequestBody* requestBody = nullptr)
			: url(url), requestBody(requestBody) {}
	};

	// Listener interface for HTTP request results
	struct Listener {
		virtual ~Listener() = default;

		virtual void OnError(
			RequestFailureType failureType,
			const std::exception* exception,
			std::optional<int> httpStatus,
			FailedRequestBody* body) = 0;

		virtual void OnSuccess(
			const std::string* mimetype,
			std::optional<int64_t> bodyBytes,
			std::istream* body) = 0;
	};

	// Request interface
	struct Request {
		virtual ~Request() = default;

		virtual void ExecuteInThisThread(Listener& listener) = 0;
		virtual void Cancel() = 0;
		virtual void AddHeader(const std::string& name, const std::string& value) = 0;
	};

	virtual ~HTTPBackend() = default;

	virtual Result<UriString> ResolveRedirectUri(
		Context& context,
		const UriString& url) = 0;

	virtual std::unique_ptr<Request> PrepareRequest(
		Context& context,
		const RequestDetails& details) = 0;

	virtual void RecreateHttpBackend() = 0;

	// Static singleton accessor (companion object equivalent)
	static HTTPBackend& GetBackend();
};

} // namespace PinkReader
