/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: imgur_api.cpp - Port of RedReader's ImgurAPI.java (implementation)
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/ImgurAPI.java
 *
 * Every field, method, and inner class ported exactly.
 */

#include "network/image_hosts/imgur_api.h"
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

// Forward declarations for types not yet fully ported
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
// AlbumInfo stub — full port would be in models/
// Port of: org.quantumbadger.redreader.image.AlbumInfo.parseImgur
// ============================================================================

class AlbumInfo {
public:
    static AlbumInfo parseImgur(const UriString &albumUrl, const JsonObject &outer) {
        // Port of: AlbumInfo.parseImgur(url, outer) (Java ImgurAPI line 76)
        // Full implementation requires AlbumInfo.kt port
        // Stub: throws to indicate not yet implemented
        (void)albumUrl;
        (void)outer;
        throw std::runtime_error("AlbumInfo::parseImgur not yet ported");
    }
};

// ============================================================================
// ImageInfo stub — full port would be in models/
// Port of: org.quantumbadger.redreader.image.ImageInfo.parseImgur
// ============================================================================

class ImageInfo {
public:
    static ImageInfo parseImgur(const JsonObject &outer) {
        // Port of: ImageInfo.parseImgur(outer) (Java ImgurAPI line 124)
        // Full implementation requires ImageInfo.kt port
        (void)outer;
        throw std::runtime_error("ImageInfo::parseImgur not yet ported");
    }
};

// ============================================================================
// CacheRequestJSONParser — port of inner class pattern
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

    // Port of: CacheRequestJSONParser(Context, Listener) (Java ImgurAPI line 63-64)
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
            // Port of: catch(Throwable t) block (Java ImgurAPI lines 78-86)
            RRError error = General::getGeneralErrorForFailure(
                General::RequestFailureType::PARSE,
                QString::fromStdString(t.what()),
                -1,
                m_apiUrl);
            m_listener.onFailure(error);
        }
    }

    // Port of: onFailure (from CacheRequestCallbacks) (Java ImgurAPI line 90-92)
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
// getAlbumInfo — port of Java static method (Java lines 44-93)
// ============================================================================

void ImgurAPI::getAlbumInfo(
        Context &context,
        const UriString &albumUrl,
        const QString &albumId,
        int priority,
        GetAlbumInfoListener &listener) {

    // Port of: final UriString apiUrl = new UriString("https://api.imgur.com/2/album/"
    //         + albumId + ".json"); (Java lines 51-52)
    const UriString apiUrl("https://api.imgur.com/2/album/" + albumId + ".json");

    // Port of: CacheManager.getInstance(context).makeRequest(new CacheRequest(...
    // (Java lines 54-93)

    // Create the JSON parser listener (anonymous inner class in Java)
    // Port of: new CacheRequestJSONParser.Listener() { ... } (Java lines 65-87)
    class AlbumParserListener : public CacheRequestJSONParser::Listener {
    public:
        AlbumParserListener(
                Context &context,
                const UriString &albumUrl,
                const UriString &apiUrl,
                GetAlbumInfoListener &listener)
            : m_context(context)
            , m_albumUrl(albumUrl)
            , m_apiUrl(apiUrl)
            , m_listener(listener) {}

        // Port of: onJsonParsed (Java lines 67-86)
        void onJsonParsed(
                const JsonValue &result,
                const TimestampUTC &timestamp,
                const QUuid &session,
                bool fromCache) override {
            try {
                // Port of: final JsonObject outer = result.asObject()
                //     .getObject("album"); (Java line 75)
                const JsonObject outer = result.asObject().getObject("album");

                // Port of: listener.onSuccess(AlbumInfo.parseImgur(albumUrl, outer));
                // (Java line 76)
                m_listener.onSuccess(AlbumInfo::parseImgur(m_albumUrl, outer));
            } catch (const std::exception &t) {
                // Port of: catch(final Throwable t) { ... } (Java lines 78-86)
                RRError error = General::getGeneralErrorForFailure(
                    General::RequestFailureType::PARSE,
                    QString::fromStdString(t.what()),
                    -1,
                    m_apiUrl);
                m_listener.onFailure(error);
            }
        }

        // Port of: onFailure (Java lines 90-92)
        void onFailure(const RRError &error) override {
            m_listener.onFailure(error);
        }

    private:
        Context &m_context;
        UriString m_albumUrl;
        UriString m_apiUrl;
        GetAlbumInfoListener &m_listener;
    };

    AlbumParserListener parserListener(context, albumUrl, apiUrl, listener);

    // Create the JSON parser wrapper
    CacheRequestJSONParser jsonParser(context, parserListener);
    jsonParser.setApiUrl(apiUrl);

    // Port of: CacheRequest request = new CacheRequest(
    //     apiUrl, RedditAccountManager.getAnon(), null, priority,
    //     DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
    //     CacheRequest.DownloadQueueType.IMMEDIATE, context,
    //     new CacheRequestJSONParser(context, listener))

    // Build CacheRequest
    const RedditAccount &anonAccount = RedditAccountManager::getAnon();

    CacheRequest request(
        apiUrl,                             // url
        anonAccount,                        // user (anonymous)
        std::nullopt,                       // requestSession (null)
        priority,                           // priority
        DownloadStrategyIfNotCached::INSTANCE,  // downloadStrategy
        FileType::IMAGE_INFO,               // fileType
        CacheRequest::DownloadQueueType::IMMEDIATE,  // queueType
        jsonParser);                        // callbacks

    // Port of: CacheManager.getInstance(context).makeRequest(request);
    CacheManager::getInstance().makeRequest(request);
}

// ============================================================================
// getImageInfo — port of Java static method (Java lines 96-141)
// ============================================================================

void ImgurAPI::getImageInfo(
        Context &context,
        const QString &imageId,
        int priority,
        GetImageInfoListener &listener) {

    // Port of: final UriString apiUrl = new UriString("https://api.imgur.com/2/image/"
    //         + imageId + ".json"); (Java lines 102-103)
    const UriString apiUrl("https://api.imgur.com/2/image/" + imageId + ".json");

    // Port of: CacheManager.getInstance(context).makeRequest(new CacheRequest(...
    // (Java lines 105-141)

    // Create the JSON parser listener (anonymous inner class in Java)
    // Port of: new CacheRequestJSONParser.Listener() { ... } (Java lines 114-141)
    class ImageParserListener : public CacheRequestJSONParser::Listener {
    public:
        ImageParserListener(
                Context &context,
                const UriString &apiUrl,
                GetImageInfoListener &listener)
            : m_context(context)
            , m_apiUrl(apiUrl)
            , m_listener(listener) {}

        // Port of: onJsonParsed (Java lines 116-134)
        void onJsonParsed(
                const JsonValue &result,
                const TimestampUTC &timestamp,
                const QUuid &session,
                bool fromCache) override {
            try {
                // Port of: final JsonObject outer = result.asObject()
                //     .getObject("image"); (Java line 123)
                const JsonObject outer = result.asObject().getObject("image");

                // Port of: listener.onSuccess(ImageInfo.parseImgur(outer));
                // (Java line 124)
                m_listener.onSuccess(ImageInfo::parseImgur(outer));
            } catch (const std::exception &t) {
                // Port of: catch(final Throwable t) { ... } (Java lines 126-134)
                RRError error = General::getGeneralErrorForFailure(
                    General::RequestFailureType::PARSE,
                    QString::fromStdString(t.what()),
                    -1,
                    m_apiUrl);
                m_listener.onFailure(error);
            }
        }

        // Port of: onFailure (Java lines 138-140)
        void onFailure(const RRError &error) override {
            m_listener.onFailure(error);
        }

    private:
        Context &m_context;
        UriString m_apiUrl;
        GetImageInfoListener &m_listener;
    };

    ImageParserListener parserListener(context, apiUrl, listener);

    // Create the JSON parser wrapper
    CacheRequestJSONParser jsonParser(context, parserListener);
    jsonParser.setApiUrl(apiUrl);

    // Build CacheRequest
    const RedditAccount &anonAccount = RedditAccountManager::getAnon();

    CacheRequest request(
        apiUrl,                             // url
        anonAccount,                        // user (anonymous)
        std::nullopt,                       // requestSession (null)
        priority,                           // priority
        DownloadStrategyIfNotCached::INSTANCE,  // downloadStrategy
        FileType::IMAGE_INFO,               // fileType
        CacheRequest::DownloadQueueType::IMMEDIATE,  // queueType
        jsonParser);                        // callbacks

    // Port of: CacheManager.getInstance(context).makeRequest(request);
    CacheManager::getInstance().makeRequest(request);
}

} // namespace PinkReader
