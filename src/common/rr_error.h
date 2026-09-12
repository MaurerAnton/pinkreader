/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: rr_error.h - Port of RedReader's RRError.java
 *
 * Provides RRError class used throughout the codebase for error handling.
 * Line-by-line port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/common/RRError.java
 */

#pragma once

#include <QString>
#include <optional>

namespace PinkReader {

// Forward declaration of CacheRequest::RequestFailureType
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

// From reddit_api_action.h
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

/**
 * @brief Error class for RedReader operations.
 *
 * Port of RRError.java. Used by CacheRequest, API handlers, and UI.
 */
class RRError {
public:
    RRError() = default;

    QString title;
    QString message;
    bool reportable = true;
    std::optional<QString> exceptionMessage;
    std::optional<int> httpStatus;
    std::optional<QString> url;
    std::optional<QString> responseBody;

    // Static factory matching the existing call sites in the codebase.
    // Called as: RRError::generalErrorForFailure(type, optional<string>, optional<int>,
    //                                             optional<string>, optional<string>)
    static RRError generalErrorForFailure(
            RequestFailureType type,
            const std::optional<QString> &exceptionMessage = std::nullopt,
            const std::optional<int> &httpStatus = std::nullopt,
            const std::optional<QString> &url = std::nullopt,
            const std::optional<QString> &responseBody = std::nullopt) {
        RRError e;
        e.exceptionMessage = exceptionMessage;
        e.httpStatus = httpStatus;
        e.url = url;
        e.responseBody = responseBody;
        switch (type) {
            case RequestFailureType::MALFORMED_URL:
                e.title = QStringLiteral("Invalid URL");
                e.message = QStringLiteral("The URL was malformed.");
                break;
            case RequestFailureType::CACHE_MISS:
                e.title = QStringLiteral("Cache miss");
                e.message = QStringLiteral("No cached copy was available.");
                e.reportable = false;
                break;
            default:
                e.title = QStringLiteral("Error");
                e.message = QStringLiteral("Something went wrong.");
                break;
        }
        return e;
    }

    // Static factory for API failures.
    // Called as: RRError::generalErrorForFailure(type, debuggingContext, responseBody)
    static RRError generalErrorForFailure(
            APIFailureType type,
            const std::optional<QString> &debuggingContext = std::nullopt,
            const std::optional<QString> &responseBody = std::nullopt) {
        (void)type;
        RRError e;
        e.url = debuggingContext;
        e.responseBody = responseBody;
        e.title = QStringLiteral("API Error");
        e.message = QStringLiteral("The request failed.");
        return e;
    }

private:
    // Port of: private fun isTorError(t: Throwable?): Boolean
    static bool isTorError(const std::optional<QString> &message);

    // Port of: private fun isContentBlockerError(t: Throwable?): Boolean
    static bool isContentBlockerError(const std::optional<QString> &message);

    // Port of: private fun toHex(bytes: ByteArray): String (unused but from original)
    static QString toHex(const QByteArray &bytes);
};

} // namespace PinkReader
