/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: streamable_api.h - Port of RedReader's StreamableAPI.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/StreamableAPI.java
 *
 * Original: public final class StreamableAPI
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
// Forward declarations for types used in StreamableAPI
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
// StreamableAPI — port of public final class StreamableAPI (Java line 42)
//
// Port of: org.quantumbadger.redreader.image.StreamableAPI
// Public final class with one static method.
// Every field, method ported exactly.
// ============================================================================

class StreamableAPI {
public:
    // Prevent instantiation (Java: final class)
    StreamableAPI() = delete;
    ~StreamableAPI() = delete;

    // ========================================================================
    // getImageInfo — port of Java static method (Java lines 44-88)
    //
    // Port of:
    //   public static void getImageInfo(
    //       final Context context,
    //       final String imageId,
    //       @NonNull final Priority priority,
    //       final GetImageInfoListener listener)
    //
    // Builds API URL "https://api.streamable.com/videos/{imageId}"
    // Creates CacheRequest via CacheManager with CacheRequestJSONParser callback.
    // On success: calls ImageInfo.parseStreamable(outer) then listener.onSuccess
    // On failure: calls General.getGeneralErrorForFailure then listener.onFailure
    // ========================================================================
    static void getImageInfo(
            Context &context,
            const QString &imageId,
            int priority,                       // Priority as int (see constants.h:Priority)
            GetImageInfoListener &listener);
};

} // namespace PinkReader
