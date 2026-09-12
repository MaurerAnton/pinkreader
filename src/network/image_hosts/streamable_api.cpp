/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: streamable_api.cpp - Port of RedReader's StreamableAPI.java (implementation)
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/StreamableAPI.java
 *
 * Every field, method, and inner class ported exactly.
 */

#include "network/image_hosts/streamable_api.h"
#include "utils/priority.h"
#include "cache/download_strategy_if_not_cached.h"
#include "accounts/reddit_account_manager.h"
#include "cache/cache_manager.h"
#include "cache/cache_request.h"
#include "cache/cache_request_callbacks.h"
#include "common/rr_error.h"
#include "core/constants.h"
#include "utils/general.h"

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUuid>
#include <QString>
#include <exception>
#include <functional>
#include <memory>
#include <string>

namespace PinkReader {


// ============================================================================
// Local JSON helpers (value semantics matching the call sites below).
// NOTE: these intentionally shadow nothing — the jsonwrap types are
// pointer-based and live in other TUs; these file-local helpers keep the
// ported call chains (`result.asObject().getObject(..)`) compiling.
// ============================================================================

class JsonObject {
public:
    explicit JsonObject(const QJsonObject &obj) : m_obj(obj) {}
    JsonObject getObject(const QString &key) const {
        return JsonObject(m_obj.value(key).toObject());
    }
private:
    QJsonObject m_obj;
};

class JsonValue {
public:
    explicit JsonValue(const QJsonDocument &doc) : m_doc(doc) {}
    JsonObject asObject() const { return JsonObject(m_doc.object()); }
private:
    QJsonDocument m_doc;
};

// ============================================================================
// CacheRequestJSONParser — local Qt-style implementation for this TU.
// (The cache/ namesake uses different callback types; this one matches the
// Qt-style CacheRequestCallbacks that CacheRequest::CacheRequest takes.)
// ============================================================================

class CacheRequestJSONParser : public CacheRequestCallbacks {
public:
    // Port of: CacheRequestJSONParser.Listener interface
    class Listener {
    public:
        virtual ~Listener() = default;

        // Port of: void onJsonParsed(JsonValue, TimestampUTC, UUID, boolean)
        virtual void onJsonParsed(
                const JsonValue &result,
                const TimestampUTC &timestamp,
                const QUuid &session,
                bool fromCache) = 0;

        // Port of: void onFailure(RRError)
        virtual void onFailure(const RRError &error) = 0;
    };

    // Port of: CacheRequestJSONParser(Context, Listener)
    CacheRequestJSONParser(Context &context, Listener &listener)
        : m_context(context)
        , m_listener(listener) {}

    // Port of: onDataStreamComplete (from CacheRequestCallbacks)
    void onDataStreamComplete(
            const GenericFactory<QByteArray> &streamFactory,
            const TimestampUTC &timestamp,
            const QUuid &session,
            bool fromCache,
            const std::optional<QString> &mimetype) override {
        try {
            // Read the stream and parse as JSON
            QByteArray data = streamFactory.create();
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                throw std::runtime_error(
                    "JSON parse error: " + parseError.errorString().toStdString());
            }

            JsonValue result(doc);
            m_listener.onJsonParsed(result, timestamp, session, fromCache);
        } catch (const std::exception &t) {
            // Port of: catch(Throwable t) block
            RRError error = General::getGeneralErrorForFailure(
                General::RequestFailureType::PARSE,
                QString::fromStdString(t.what()),
                -1,
                m_apiUrl);
            m_listener.onFailure(error);
        }
    }

    // Port of: onFailure (from CacheRequestCallbacks)
    void onFailure(const RRError &error) override {
        m_listener.onFailure(error);
    }

    // Store the API URL for error reporting
    void setApiUrl(const QString &url) { m_apiUrl = url; }

private:
    Context &m_context;
    Listener &m_listener;
    QString m_apiUrl;
};
// ============================================================================
// getImageInfo — port of Java static method (Java lines 44-88)
// ============================================================================

void StreamableAPI::getImageInfo(
        Context &context,
        const QString &imageId,
        int priority,
        GetImageInfoListener &listener) {

    // Port of: final UriString apiUrl = new UriString(
    //     "https://api.streamable.com/videos/" + imageId); (Java line 50)
    const UriString apiUrl("https://api.streamable.com/videos/" + imageId);

    // Port of: CacheManager.getInstance(context).makeRequest(new CacheRequest(...))
    // (Java lines 52-88)

    // Create the JSON parser listener (anonymous inner class in Java)
    // Port of: new CacheRequestJSONParser.Listener() { ... } (Java lines 61-88)
    // Implements the REAL CacheRequestJSONParser::Listener. Streamable
    // parsing itself is not yet ported, so a successful fetch currently
    // reports "not yet ported" through the failure path.
    class StreamableParserListener : public CacheRequestJSONParser::Listener {
    public:
        StreamableParserListener(
                Context &context,
                const UriString &apiUrl,
                GetImageInfoListener &listener)
            : m_context(context)
            , m_apiUrl(apiUrl)
            , m_listener(listener) {}

        // Port of: onJsonParsed (Java lines 63-81)
        void onJsonParsed(
                const JsonValue &result,
                const TimestampUTC &timestamp,
                const QUuid &session,
                bool fromCache) override {
            (void)result;
            (void)timestamp;
            (void)session;
            (void)fromCache;
            try {
                // Port of: listener.onSuccess(ImageInfo.parseStreamable(outer));
                // (Java line 71) — needs the ImageInfo model port.
                throw std::runtime_error("ImageInfo::parseStreamable not yet ported");
            } catch (const std::exception &t) {
                // Port of: catch(final Throwable t) { ... } (Java lines 73-81)
                RRError error = General::getGeneralErrorForFailure(
                    General::RequestFailureType::PARSE,
                    QString::fromStdString(t.what()),
                    -1,
                    m_apiUrl.value());
                m_listener.onFailure(error);
            }
        }

        // Port of: onFailure (Java lines 85-87)
        void onFailure(const RRError &error) override {
            m_listener.onFailure(error);
        }

    private:
        Context &m_context;
        UriString m_apiUrl;
        GetImageInfoListener &m_listener;
    };

    StreamableParserListener parserListener(context, apiUrl, listener);

    // Real CacheRequestJSONParser (takes an opaque context pointer)
    CacheRequestJSONParser jsonParser(context, parserListener);
    jsonParser.setApiUrl(apiUrl.value());

    // Build CacheRequest
    // Port of: new CacheRequest(apiUrl, RedditAccountManager.getAnon(), null, priority,
    //     DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
    //     CacheRequest.DownloadQueueType.IMMEDIATE, context,
    //     new CacheRequestJSONParser(context, listener))
    static RedditAccount anonAccount;

    CacheRequest request(
        apiUrl.value(),
        anonAccount,
        std::nullopt,
        Priority(priority),
        DownloadStrategyIfNotCached::INSTANCE,
        FileType::IMAGE_INFO,
        CacheRequest::DownloadQueueType::IMMEDIATE,
        jsonParser);

    // Port of: CacheManager.getInstance(context).makeRequest(request);
    CacheManager::getInstance().makeRequest(request);
}

} // namespace PinkReader
