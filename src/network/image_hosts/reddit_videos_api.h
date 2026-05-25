/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_videos_api.h - Port of RedReader's RedditVideosAPI.kt
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/RedditVideosAPI.kt
 *
 * Original: object RedditVideosAPI (Kotlin singleton)
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
// Forward declarations for types used in RedditVideosAPI
// ============================================================================

class RRError;
class UriString;
class Context;
class CacheManager;
class CacheRequest;
class CacheRequestCallbacks;
class GenericFactory_SeekableInputStream;  // GenericFactory<SeekableInputStream, IOException>
class TimestampUTC;
class FailedRequestBody;
class ImageInfo;

// Forward: GetImageInfoListener (defined in imgur_api.h or image_imgur_api.h)
class GetImageInfoListener;

// ============================================================================
// MediaInfo — port of Kotlin data class (MPDParser.kt lines 22-26)
//
// Port of:
//   data class MediaInfo(
//       val filename: String,
//       val width: Int?,
//       val height: Int?
//   )
// ============================================================================

struct MediaInfo {
    QString filename;
    std::optional<int> width;
    std::optional<int> height;
};

// ============================================================================
// MPDReadResult — port of Kotlin data class (MPDParser.kt lines 28-31)
//
// Port of:
//   data class MPDReadResult(
//       val video: MediaInfo? = null,
//       val audio: MediaInfo? = null
//   )
// ============================================================================

struct MPDReadResult {
    std::optional<MediaInfo> video;
    std::optional<MediaInfo> audio;
};

// ============================================================================
// RedditVideosAPI — port of Kotlin object RedditVideosAPI (Kotlin line 41)
//
// Port of: org.quantumbadger.redreader.image.RedditVideosAPI
// Kotlin object (singleton) with one function.
// Every field, method ported exactly.
// ============================================================================

class RedditVideosAPI {
public:
    // Prevent instantiation (Kotlin object ↔ static class)
    RedditVideosAPI() = delete;
    ~RedditVideosAPI() = delete;

    // ========================================================================
    // getImageInfo — port of Kotlin fun (Kotlin lines 44-161)
    //
    // Port of:
    //   fun getImageInfo(
    //       context: Context,
    //       imageId: String,
    //       priority: Priority,
    //       listener: GetImageInfoListener)
    //
    // Builds API URL "https://v.redd.it/{imageId}/DASHPlaylist.mpd"
    // Creates CacheRequest via CacheManager with CacheRequestCallbacks.
    // On data stream complete: reads MPD XML, parses with parseMPD,
    //   constructs ImageInfo with video URL, audio URL, etc.
    // On failure: calls listener.onFailure via getGeneralErrorForFailure
    // ========================================================================
    static void getImageInfo(
            Context &context,
            const QString &imageId,
            int priority,                       // Priority as int (see constants.h:Priority)
            GetImageInfoListener &listener);

    // ========================================================================
    // parseMPD — port of Kotlin fun (MPDParser.kt lines 56-75)
    //
    // Port of:
    //   fun parseMPD(mpdContent: String): MPDReadResult
    //
    // Parses DASH MPD XML content to extract video/audio MediaInfo.
    // ========================================================================
    static MPDReadResult parseMPD(const QString &mpdContent);

private:
    // ========================================================================
    // TAG constant — port of Kotlin private const val (Kotlin line 42)
    //
    // Port of:
    //   private const val TAG = "RedditVideosAPI"
    // ========================================================================
    static constexpr const char *TAG = "RedditVideosAPI";
};

} // namespace PinkReader
