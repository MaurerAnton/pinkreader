/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: gfycat_api.h - Port of RedReader's GfycatAPI.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/GfycatAPI.java
 *
 * Original: public final class GfycatAPI
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
// Forward declarations for types used in GfycatAPI
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
// GfycatAPI — port of public final class GfycatAPI (Java line 42)
//
// Port of: org.quantumbadger.redreader.image.GfycatAPI
// Public final class with one static method.
// Every field, method ported exactly.
// ============================================================================

class GfycatAPI {
public:
    // Prevent instantiation (Java: final class)
    GfycatAPI() = delete;
    ~GfycatAPI() = delete;

    // ========================================================================
    // getImageInfo — port of Java static method (Java lines 44-87)
    //
    // Port of:
    //   public static void getImageInfo(
    //       final Context context,
    //       final String imageId,
    //       @NonNull final Priority priority,
    //       final GetImageInfoListener listener)
    //
    // Builds API URL "https://api.gfycat.com/v1/gfycats/{imageId}"
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
