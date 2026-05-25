/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: imgur_api.h - Port of RedReader's ImgurAPI.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/ImgurAPI.java
 *
 * Original: public final class ImgurAPI
 * Two static methods: getAlbumInfo, getImageInfo
 *
 * Every field, method, and inner class ported exactly.
 * Android-specific Context is replaced with a forward-declared stub.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

namespace PinkReader {

// ============================================================================
// Forward declarations for types used in ImgurAPI
// ============================================================================

class RRError;           // Port of org.quantumbadger.redreader.common.RRError
class UriString;         // Port of org.quantumbadger.redreader.common.UriString
class Context;           // Android Context stub
class RedditAccount;     // Port of org.quantumbadger.redreader.account.RedditAccount
class CacheManager;      // Port of org.quantumbadger.redreader.cache.CacheManager
class CacheRequest;      // Port of org.quantumbadger.redreader.cache.CacheRequest
class TimestampUTC;      // Port of org.quantumbadger.redreader.common.time.TimestampUTC

// ============================================================================
// GetAlbumInfoListener — port of Java interface (Java lines 23-32)
// Port of: org.quantumbadger.redreader.image.GetAlbumInfoListener
// ============================================================================

class GetAlbumInfoListener {
public:
    virtual ~GetAlbumInfoListener() = default;

    // Port of: void onFailure(@NonNull RRError error); (Java line 25)
    virtual void onFailure(const RRError &error) = 0;

    // Port of: void onSuccess(@NonNull AlbumInfo info); (Java line 27)
    // AlbumInfo is forward-declared; we use void* since full type isn't ported here
    virtual void onSuccess(const class AlbumInfo &info) = 0;

    // Port of: void onGalleryRemoved(); (Java line 29)
    virtual void onGalleryRemoved() = 0;

    // Port of: void onGalleryDataNotPresent(); (Java line 31)
    virtual void onGalleryDataNotPresent() = 0;
};

// ============================================================================
// GetImageInfoListener — port of Java interface (Java lines 23-29)
// Port of: org.quantumbadger.redreader.image.GetImageInfoListener
// ============================================================================

class GetImageInfoListener {
public:
    virtual ~GetImageInfoListener() = default;

    // Port of: void onFailure(@NonNull RRError error); (Java line 24)
    virtual void onFailure(const RRError &error) = 0;

    // Port of: void onSuccess(ImageInfo info); (Java line 26)
    virtual void onSuccess(const class ImageInfo &info) = 0;

    // Port of: void onNotAnImage(); (Java line 28)
    virtual void onNotAnImage() = 0;
};

// ============================================================================
// ImgurAPI — port of public final class ImgurAPI (Java line 42)
//
// Port of: org.quantumbadger.redreader.image.ImgurAPI
// Public final class with two static methods.
// Every field, method ported exactly.
// ============================================================================

class ImgurAPI {
public:
    // Prevent instantiation (Java: final class)
    ImgurAPI() = delete;
    ~ImgurAPI() = delete;

    // ========================================================================
    // getAlbumInfo — port of Java static method (Java lines 44-93)
    //
    // Port of:
    //   public static void getAlbumInfo(
    //       final Context context,
    //       final UriString albumUrl,
    //       final String albumId,
    //       @NonNull final Priority priority,
    //       final GetAlbumInfoListener listener)
    //
    // Builds API URL "https://api.imgur.com/2/album/{albumId}.json"
    // Creates CacheRequest via CacheManager with CacheRequestJSONParser callback.
    // On success: calls AlbumInfo.parseImgur(albumUrl, outer) then listener.onSuccess
    // On failure: calls General.getGeneralErrorForFailure then listener.onFailure
    // ========================================================================
    static void getAlbumInfo(
            Context &context,
            const UriString &albumUrl,
            const QString &albumId,
            int priority,                       // Priority as int (see constants.h:Priority)
            GetAlbumInfoListener &listener);

    // ========================================================================
    // getImageInfo — port of Java static method (Java lines 96-141)
    //
    // Port of:
    //   public static void getImageInfo(
    //       final Context context,
    //       final String imageId,
    //       @NonNull final Priority priority,
    //       final GetImageInfoListener listener)
    //
    // Builds API URL "https://api.imgur.com/2/image/{imageId}.json"
    // Creates CacheRequest via CacheManager with CacheRequestJSONParser callback.
    // On success: calls ImageInfo.parseImgur(outer) then listener.onSuccess
    // On failure: calls General.getGeneralErrorForFailure then listener.onFailure
    // ========================================================================
    static void getImageInfo(
            Context &context,
            const QString &imageId,
            int priority,                       // Priority as int (see constants.h:Priority)
            GetImageInfoListener &listener);
};

} // namespace PinkReader
