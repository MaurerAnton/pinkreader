/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: http_backend.h
 * Port of RedReader's HTTPBackend.kt (Kotlin abstract class)
 *
 * Exact line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/http/HTTPBackend.kt
 *   (65 lines)
 *
 * Every field, method, inner class/interface, and companion object ported 1:1.
 */

#pragma once

#include <QString>
#include <QByteArray>
#include <QObject>
#include <functional>
#include <memory>
#include <optional>

namespace PinkReader {

// UriString — simple URL string wrapper (also defined in cache_db_manager.h)
// Defined here to avoid circular include dependencies
class UriString {
public:
    QString value;
    UriString() = default;
    explicit UriString(const QString &v) : value(v) {}
    QString toString() const { return value; }
    bool isEmpty() const { return value.isEmpty(); }
    bool operator==(const UriString &other) const {
        return value == other.value;
    }
    bool operator!=(const UriString &other) const {
        return value != other.value;
    }
};

inline uint qHash(const UriString &key, uint seed = 0) {
    return qHash(key.value, seed);
}

// ========================================================================
// RequestFailureType — port of CacheRequest.RequestFailureType (Java lines 52-65)
// Used by HTTPBackend.Listener.onError
// ========================================================================
enum class RequestFailureType {
    CONNECTION,            // ordinal 0
    REQUEST,               // ordinal 1
    STORAGE,               // ordinal 2
    CACHE_MISS,            // ordinal 3
    CANCELLED,             // ordinal 4
    MALFORMED_URL,         // ordinal 5
    PARSE,                 // ordinal 6
    DISK_SPACE,            // ordinal 7
    REDDIT_REDIRECT,       // ordinal 8
    PARSE_IMGUR,           // ordinal 9
    UPLOAD_FAIL_IMGUR,     // ordinal 10
    CACHE_DIR_DOES_NOT_EXIST // ordinal 11
};

// ========================================================================
// HTTPRequestBody — port of HTTPRequestBody.kt
// Base class for HTTP request bodies
// ========================================================================
class HTTPRequestBody {
public:
    virtual ~HTTPRequestBody() = default;
    virtual QString contentType() const = 0;
    virtual qint64 contentLength() const = 0;
};

// ========================================================================
// FailedRequestBody — port of FailedRequestBody.java
// Contains the failed response body for error handling
// ========================================================================
class FailedRequestBody {
public:
    QString body;
    QString mimetype;

    FailedRequestBody() = default;
    FailedRequestBody(const QString &b, const QString &m)
        : body(b), mimetype(m) {}
};

// ========================================================================
// HTTPBackend — abstract HTTP backend
// Port of HTTPBackend.kt (65 lines)
//
// Inner types:
//   - RequestDetails (data class, Kotlin lines 29-32)
//   - Request (interface, Kotlin lines 34-38)
//   - Listener (interface, Kotlin lines 40-49)
//
// Abstract methods:
//   - resolveRedirectUri (Kotlin lines 51-54)
//   - prepareRequest (Kotlin line 56)
//   - recreateHttpBackend (Kotlin line 58)
//
// Companion object:
//   - backend property (Kotlin lines 60-63)
// ========================================================================
class HTTPBackend {
public:
    // ====================================================================
    // RequestDetails — data class (Kotlin lines 29-32)
    // ====================================================================
    struct RequestDetails {
        UriString url;
        std::shared_ptr<HTTPRequestBody> requestBody; // nullable

        RequestDetails() = default;
        RequestDetails(const UriString &u,
                       std::shared_ptr<HTTPRequestBody> body = nullptr)
            : url(u), requestBody(std::move(body)) {}
    };

    // ====================================================================
    // Request — interface (Kotlin lines 34-38)
    // ====================================================================
    class Request {
    public:
        virtual ~Request() = default;

        // executeInThisThread (Kotlin line 35)
        virtual void executeInThisThread(class Listener &listener) = 0;

        // cancel (Kotlin line 36)
        virtual void cancel() = 0;

        // addHeader (Kotlin line 37)
        virtual void addHeader(const QString &name,
                                const QString &value) = 0;
    };

    // ====================================================================
    // Listener — interface (Kotlin lines 40-49)
    // ====================================================================
    class Listener {
    public:
        virtual ~Listener() = default;

        // onError (Kotlin lines 41-46)
        virtual void onError(
                RequestFailureType failureType,
                const std::exception *exception,       // Throwable?
                std::optional<int> httpStatus,          // Int?
                const FailedRequestBody *body           // FailedRequestBody?
                ) = 0;

        // onSuccess (Kotlin line 48)
        virtual void onSuccess(
                const QString *mimetype,                // String?
                std::optional<qint64> bodyBytes,         // Long?
                const QByteArray *body                   // InputStream?
                ) = 0;
    };

    // ====================================================================
    // Construction / Destruction
    // ====================================================================
    HTTPBackend() = default;
    virtual ~HTTPBackend() = default;

    // ====================================================================
    // Abstract methods
    // ====================================================================

    // resolveRedirectUri (Kotlin lines 51-54)
    // Returns Result<UriString> — we use std::optional for simplicity
    virtual std::optional<UriString> resolveRedirectUri(
            const UriString &url) = 0;

    // prepareRequest (Kotlin line 56)
    virtual std::unique_ptr<Request> prepareRequest(
            const RequestDetails &details) = 0;

    // recreateHttpBackend (Kotlin line 58)
    virtual void recreateHttpBackend() = 0;

    // ====================================================================
    // Static backend accessor (companion object, Kotlin lines 60-63)
    //
    // In Java: @JvmStatic val backend: HTTPBackend
    //          get() = OKHTTPBackend.getHttpBackend()
    // ====================================================================
    static HTTPBackend &getBackend();
    static void setBackend(HTTPBackend *backend);

private:
    static HTTPBackend *s_backend;
};

} // namespace PinkReader
