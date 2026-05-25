/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: request_response_handler.h
 * Port of RedReader's RequestResponseHandler.java
 *
 * Exact line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/io/RequestResponseHandler.java
 *
 * Interface for handling cache request responses.
 * Java: public interface RequestResponseHandler<E, F>
 *   void onRequestFailed(F failureReason);
 *   void onRequestSuccess(E result, TimestampUTC timeCached);
 */

#pragma once

#include <functional>
#include "cache/cache_db_manager.h"  // for TimestampUTC

namespace PinkReader {

/**
 * RequestResponseHandler — callback interface for request results.
 * Port of RequestResponseHandler.java (26 lines).
 *
 * Template parameters:
 *   E - result type on success
 *   F - failure reason type on error
 *
 * Methods:
 *   onRequestFailed(F failureReason)
 *   onRequestSuccess(E result, TimestampUTC timeCached)
 *
 * In C++, users can either implement this as a subclass or use
 * std::function-based convenience wrappers.
 */
template<typename E, typename F>
class RequestResponseHandler {
public:
    virtual ~RequestResponseHandler() = default;

    // onRequestFailed (Java line 23)
    virtual void onRequestFailed(const F &failureReason) = 0;

    // onRequestSuccess (Java line 25)
    virtual void onRequestSuccess(const E &result,
                                   const TimestampUTC &timeCached) = 0;
};

/**
 * Convenience class: FunctionRequestResponseHandler
 * Wraps std::function callbacks, matching the anonymous inner class
 * pattern used throughout RedReader's Java code.
 */
template<typename E, typename F>
class FunctionRequestResponseHandler : public RequestResponseHandler<E, F> {
public:
    using SuccessFn = std::function<void(const E &, const TimestampUTC &)>;
    using FailureFn = std::function<void(const F &)>;

    FunctionRequestResponseHandler(SuccessFn onSuccess, FailureFn onFailure)
        : m_onSuccess(std::move(onSuccess))
        , m_onFailure(std::move(onFailure)) {}

    void onRequestFailed(const F &failureReason) override {
        if (m_onFailure) {
            m_onFailure(failureReason);
        }
    }

    void onRequestSuccess(const E &result,
                           const TimestampUTC &timeCached) override {
        if (m_onSuccess) {
            m_onSuccess(result, timeCached);
        }
    }

private:
    SuccessFn m_onSuccess;
    FailureFn m_onFailure;
};

} // namespace PinkReader
