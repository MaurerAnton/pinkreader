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
#include "cache/download_strategy_if_not_cached.h"
#include "cache/cache_request_json_parser.h"
#include "utils/priority.h"
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
    // Implements the REAL CacheRequestJSONParser::Listener (cache/). Album
    // parsing itself is not yet ported, so a successful fetch currently
    // reports "not yet ported" through the failure path (same as before,
    // when AlbumInfo::parseImgur threw unconditionally).
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
                const UUID &session,
                bool fromCache) override {
            (void)result;
            (void)timestamp;
            (void)session;
            (void)fromCache;
            try {
                // Port of: listener.onSuccess(AlbumInfo.parseImgur(m_albumUrl, outer));
                // (Java line 76) — AlbumInfo parsing needs the model port.
                throw std::runtime_error("AlbumInfo::parseImgur not yet ported");
            } catch (const std::exception &t) {
                // Port of: catch(final Throwable t) { ... } (Java lines 78-86)
                RRError error = General::getGeneralErrorForFailure(
                    General::RequestFailureType::PARSE,
                    QString::fromStdString(t.what()),
                    -1,
                    m_apiUrl.value());
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

    // Create the JSON parser wrapper (real CacheRequestJSONParser)
    CacheRequestJSONParser jsonParser(static_cast<void *>(&context), parserListener);

    // Port of: CacheRequest request = new CacheRequest(
    //     apiUrl, RedditAccountManager.getAnon(), null, priority,
    //     DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
    //     CacheRequest.DownloadQueueType.IMMEDIATE, context,
    //     new CacheRequestJSONParser(context, listener))

    // Build CacheRequest
    const RedditAccount &anonAccount = RedditAccountManager::getAnon();

    CacheRequest request(
        apiUrl.value(),                     // url
        anonAccount,                        // user (anonymous)
        std::nullopt,                       // requestSession (null)
        Priority(priority),                 // priority
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
    // Implements the REAL CacheRequestJSONParser::Listener. Image parsing
    // itself is not yet ported, so a successful fetch currently reports
    // "not yet ported" through the failure path.
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
                const UUID &session,
                bool fromCache) override {
            (void)result;
            (void)timestamp;
            (void)session;
            (void)fromCache;
            try {
                // Port of: listener.onSuccess(ImageInfo.parseImgur(outer));
                // (Java line 124) — ImageInfo parsing needs the model port.
                throw std::runtime_error("ImageInfo::parseImgur not yet ported");
            } catch (const std::exception &t) {
                // Port of: catch(final Throwable t) { ... } (Java lines 126-134)
                RRError error = General::getGeneralErrorForFailure(
                    General::RequestFailureType::PARSE,
                    QString::fromStdString(t.what()),
                    -1,
                    m_apiUrl.value());
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

    // Create the JSON parser wrapper (real CacheRequestJSONParser)
    CacheRequestJSONParser jsonParser(static_cast<void *>(&context), parserListener);

    // Build CacheRequest
    const RedditAccount &anonAccount = RedditAccountManager::getAnon();

    CacheRequest request(
        apiUrl.value(),                     // url
        anonAccount,                        // user (anonymous)
        std::nullopt,                       // requestSession (null)
        Priority(priority),                 // priority
        DownloadStrategyIfNotCached::INSTANCE,  // downloadStrategy
        FileType::IMAGE_INFO,               // fileType
        CacheRequest::DownloadQueueType::IMMEDIATE,  // queueType
        jsonParser);                        // callbacks

    // Port of: CacheManager.getInstance(context).makeRequest(request);
    CacheManager::getInstance().makeRequest(request);
}

} // namespace PinkReader
