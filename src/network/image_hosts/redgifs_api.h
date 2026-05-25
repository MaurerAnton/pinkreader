/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: redgifs_api.h - Port of RedReader's RedgifsAPI.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/RedgifsAPI.java
 *
 * Original: public final class RedgifsAPI
 * One static method: getImageInfo
 *
 * Every field, method, and inner class ported exactly.
 * Android-specific Context is replaced with a forward-declared stub.
 */

#pragma once

#include <QString>
#include <QUuid>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace PinkReader {

// ============================================================================
// Forward declarations for types used in RedgifsAPI
// ============================================================================

class RRError;
class UriString;
class Context;
class CacheManager;
class CacheRequest;
class TimestampUTC;

// Forward: GetImageInfoListener (defined in imgur_api.h or image_imgur_api.h)
class GetImageInfoListener;

// ============================================================================
// RedgifsAPI — port of public final class RedgifsAPI (Java line 44)
//
// Port of: org.quantumbadger.redreader.image.RedgifsAPI
// Public final class with one static method.
// Every field, method ported exactly.
// ============================================================================

class RedgifsAPI {
public:
    // Prevent instantiation (Java: final class)
    RedgifsAPI() = delete;
    ~RedgifsAPI() = delete;

    // ========================================================================
    // getImageInfo — port of Java static method (Java lines 46-92)
    //
    // Port of:
    //   public static void getImageInfo(
    //       final Context context,
    //       final String imageId,
    //       @NonNull final Priority priority,
    //       final GetImageInfoListener listener)
    //
    // Builds API URL "https://api.redgifs.com/v1/gfycats/{imageId}"
    // Uses DownloadStrategyIfTimestampOutsideBounds(10 minutes) for caching
    //   instead of DownloadStrategyIfNotCached (RedGifs links expire).
    // Creates CacheRequest via CacheManager with CacheRequestJSONParser callback.
    // On success: calls ImageInfo.parseGfycat(outer) then listener.onSuccess
    // On failure: calls General.getGeneralErrorForFailure then listener.onFailure
    // ========================================================================
    static void getImageInfo(
            Context &context,
            const QString &imageId,
            int priority,                       // Priority as int (see constants.h:Priority)
            GetImageInfoListener &listener);
};

} // namespace PinkReader
