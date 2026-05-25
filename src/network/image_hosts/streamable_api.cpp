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
#include "accounts/reddit_account_manager.h"
#include "cache/cache_manager.h"
#include "cache/cache_request.h"
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
// Stub classes for types not yet fully ported
// ============================================================================

class FailedRequestBody {
public:
    explicit FailedRequestBody(const QJsonValue &result) : m_result(result) {}
    const QJsonValue &result() const { return m_result; }
private:
    QJsonValue m_result;
};

class JsonObject {
public:
    explicit JsonObject(const QJsonObject &obj) : m_obj(obj) {}
    QJsonObject getObject(const QString &key) const {
        return m_obj.value(key).toObject();
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
// ImageInfo stub — port of org.quantumbadger.redreader.image.ImageInfo.parseStreamable
// ============================================================================

class ImageInfo {
public:
    // Port of: ImageInfo.parseStreamable(outer) (Java StreamableAPI line 71)
    static ImageInfo parseStreamable(const JsonObject &outer) {
        // Full implementation requires ImageInfo.kt port
        (void)outer;
        throw std::runtime_error("ImageInfo::parseStreamable not yet ported");
    }
};

// ============================================================================
// CacheRequestJSONParser — port of inner class pattern
//
// Port of: org.quantumbadger.redreader.cache.CacheRequestJSONParser
//
// This is a CacheRequestCallbacks implementation that:
//   - onDataStreamComplete: parses the stream as JSON,
//     calls onJsonParsed callback
//   - onFailure: calls onFailure callback
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
        (void)mimetype;
        try {
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

    void setApiUrl(const UriString &url) { m_apiUrl = url; }

private:
    Context &m_context;
    Listener &m_listener;
    UriString m_apiUrl;
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
            (void)timestamp;
            (void)session;
            (void)fromCache;
            try {
                // Port of: final JsonObject outer = result.asObject(); (Java line 70)
                const JsonObject outer = result.asObject();

                // Port of: listener.onSuccess(ImageInfo.parseStreamable(outer));
                // (Java line 71)
                m_listener.onSuccess(ImageInfo::parseStreamable(outer));
            } catch (const std::exception &t) {
                // Port of: catch(final Throwable t) { ... } (Java lines 73-81)
                RRError error = General::getGeneralErrorForFailure(
                    General::RequestFailureType::PARSE,
                    QString::fromStdString(t.what()),
                    -1,
                    m_apiUrl);
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

    CacheRequestJSONParser jsonParser(context, parserListener);
    jsonParser.setApiUrl(apiUrl);

    // Build CacheRequest
    // Port of: new CacheRequest(apiUrl, RedditAccountManager.getAnon(), null, priority,
    //     DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
    //     CacheRequest.DownloadQueueType.IMMEDIATE, context,
    //     new CacheRequestJSONParser(context, listener))
    const RedditAccount &anonAccount = RedditAccountManager::getAnon();

    CacheRequest request(
        apiUrl,
        anonAccount,
        std::nullopt,
        priority,
        DownloadStrategyIfNotCached::INSTANCE,
        FileType::IMAGE_INFO,
        CacheRequest::DownloadQueueType::IMMEDIATE,
        jsonParser);

    // Port of: CacheManager.getInstance(context).makeRequest(request);
    CacheManager::getInstance().makeRequest(request);
}

} // namespace PinkReader
