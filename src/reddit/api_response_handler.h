// Origin: org/quantumbadger/redreader/reddit/APIResponseHandler.java
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

// Forward declarations for types used but not yet defined
namespace PinkReader {
class RRError;
template<typename T> class Optional;
class FailedRequestBody;
class TimestampUTC;
class RedditUser;
}

namespace PinkReader {

class APIResponseHandler {
public:
	enum class APIFailureType {
		INVALID_USER,
		BAD_CAPTCHA,
		NOTALLOWED,
		SUBREDDIT_REQUIRED,
		URL_REQUIRED,
		UNKNOWN,
		TOO_FAST,
		TOO_LONG,
		ALREADY_SUBMITTED,
		POST_FLAIR_REQUIRED
	};

	// protected final AppCompatActivity context;
	// NOTE: context is stubbed as void* since AppCompatActivity is Android-specific
	void* context;

protected:
	APIResponseHandler(void* context) : context(context) {}

	virtual void onCallbackException(const std::exception& t) = 0;
	virtual void onFailure(const RRError& error) = 0;

public:
	virtual ~APIResponseHandler() = default;

	void notifyFailure(const RRError& error) {
		try {
			onFailure(error);
		} catch (const std::exception& t1) {
			try {
				onCallbackException(t1);
			} catch (const std::exception& t2) {
				// BugReportActivity.addGlobalError(new RRError(null, null, true, t1));
				// BugReportActivity.handleGlobalError(context, t2);
			}
		}
	}

	void notifyFailure(
			APIFailureType type,
			const std::string* debuggingContext,
			const Optional<FailedRequestBody>& response) {
		// notifyFailure(General.getGeneralErrorForFailure(
		//         context, type, debuggingContext, response));
		// NOTE: General::getGeneralErrorForFailure not yet ported
		(void)type;
		(void)debuggingContext;
		(void)response;
	}
};

// Inner class: SubmitResponseHandler
class SubmitResponseHandler : public APIResponseHandler {
protected:
	SubmitResponseHandler(void* context) : APIResponseHandler(context) {}

public:
	virtual void onSubmitErrors(const std::vector<std::string>& errors) = 0;
	virtual void onSuccess(
			const Optional<std::string>& redirectUrl,
			const Optional<std::string>& thingId) = 0;
};

// Inner class: ActionResponseHandler
class ActionResponseHandler : public APIResponseHandler {
protected:
	ActionResponseHandler(void* context) : APIResponseHandler(context) {}

public:
	void notifySuccess() {
		try {
			onSuccess();
		} catch (const std::exception& t1) {
			try {
				onCallbackException(t1);
			} catch (const std::exception& t2) {
				// BugReportActivity.addGlobalError(new RRError(null, null, true, t1));
				// BugReportActivity.handleGlobalError(context, t2);
			}
		}
	}

protected:
	virtual void onSuccess() = 0;
};

// Inner class: ValueResponseHandler<E>
template<typename E>
class ValueResponseHandler : public APIResponseHandler {
protected:
	ValueResponseHandler(void* context) : APIResponseHandler(context) {}

public:
	void notifySuccess(const E& value) {
		try {
			onSuccess(value);
		} catch (const std::exception& t1) {
			try {
				onCallbackException(t1);
			} catch (const std::exception& t2) {
				// BugReportActivity.addGlobalError(new RRError(null, null, true, t1));
				// BugReportActivity.handleGlobalError(context, t2);
			}
		}
	}

protected:
	virtual void onSuccess(const E& value) = 0;
};

// Inner class: UserResponseHandler
class UserResponseHandler : public APIResponseHandler {
protected:
	UserResponseHandler(void* context) : APIResponseHandler(context) {}

public:
	void notifySuccess(const RedditUser& result, const TimestampUTC& timestamp) {
		try {
			onSuccess(result, timestamp);
		} catch (const std::exception& t1) {
			try {
				onCallbackException(t1);
			} catch (const std::exception& t2) {
				// BugReportActivity.addGlobalError(new RRError(null, null, true, t1));
				// BugReportActivity.handleGlobalError(context, t2);
			}
		}
	}

	void notifyDownloadStarted() {
		try {
			onDownloadStarted();
		} catch (const std::exception& t1) {
			try {
				onCallbackException(t1);
			} catch (const std::exception& t2) {
				// BugReportActivity.addGlobalError(new RRError(null, null, true, t1));
				// BugReportActivity.handleGlobalError(context, t2);
			}
		}
	}

protected:
	virtual void onDownloadStarted() = 0;
	virtual void onSuccess(const RedditUser& result, const TimestampUTC& timestamp) = 0;
};

} // namespace PinkReader
