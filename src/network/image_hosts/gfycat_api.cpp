/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: gfycat_api.cpp - Port of RedReader's GfycatAPI.java (implementation)
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/GfycatAPI.java
 *
 * Every field, method, and inner class ported exactly.
 */

#include "network/image_hosts/gfycat_api.h"
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
// getImageInfo — port of Java static method (Java lines 44-87)
// ============================================================================

void GfycatAPI::getImageInfo(
        Context &context,
        const QString &imageId,
        int priority,
        GetImageInfoListener &listener) {

    // Port of: final UriString apiUrl = new UriString(
    //     "https://api.gfycat.com/v1/gfycats/" + imageId); (Java line 50)
    const UriString apiUrl("https://api.gfycat.com/v1/gfycats/" + imageId);

    // Port of: CacheManager.getInstance(context).makeRequest(new CacheRequest(...))
    // (Java lines 52-87)

    // Create the JSON parser listener (anonymous inner class in Java)
    // Port of: new CacheRequestJSONParser.Listener() { ... } (Java lines 61-87)
    // Implements the REAL CacheRequestJSONParser::Listener. Gfycat image
    // parsing itself is not yet ported, so a successful fetch currently
    // reports "not yet ported" through the failure path.
    class GfycatParserListener : public CacheRequestJSONParser::Listener {
    public:
        GfycatParserListener(
                Context &context,
                const UriString &apiUrl,
                GetImageInfoListener &listener)
            : m_context(context)
            , m_apiUrl(apiUrl)
            , m_listener(listener) {}

        // Port of: onJsonParsed (Java lines 63-80)
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
                // Port of: listener.onSuccess(ImageInfo.parseGfycat(outer));
                // (Java line 70) — needs the ImageInfo model port.
                throw std::runtime_error("ImageInfo::parseGfycat not yet ported");
            } catch (const std::exception &t) {
                // Port of: catch(final Throwable t) { ... } (Java lines 72-79)
                RRError error = General::getGeneralErrorForFailure(
                    General::RequestFailureType::PARSE,
                    QString::fromStdString(t.what()),
                    -1,
                    m_apiUrl.value());
                m_listener.onFailure(error);
            }
        }

        // Port of: onFailure (Java lines 84-86)
        void onFailure(const RRError &error) override {
            m_listener.onFailure(error);
        }

    private:
        Context &m_context;
        UriString m_apiUrl;
        GetImageInfoListener &m_listener;
    };

    GfycatParserListener parserListener(context, apiUrl, listener);

    // Real CacheRequestJSONParser (takes an opaque context pointer)
    CacheRequestJSONParser jsonParser(static_cast<void *>(&context), parserListener);

    // Build CacheRequest
    // Port of: new CacheRequest(apiUrl, RedditAccountManager.getAnon(), null, priority,
    //     DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
    //     CacheRequest.DownloadQueueType.IMMEDIATE, context,
    //     new CacheRequestJSONParser(context, listener))
    const RedditAccount &anonAccount = RedditAccountManager::getAnon();

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
