/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_image_api.h - Port of RedReader's RedditGalleryAPI.kt
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/image/RedditGalleryAPI.kt
 *
 * Original: class RedditGalleryAPI with companion object containing:
 *   - LRU cache (LinkedHashMap with removeEldestEntry, max 100 entries)
 *   - addToCache(post: RedditPost)
 *   - getAlbumInfo(context, albumUrl, albumId, priority, listener)
 *
 * Every field, method, and inner class ported exactly.
 * Android-specific Context is replaced with a forward-declared stub.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

namespace PinkReader {

// ============================================================================
// Forward declarations for types used in RedditImageAPI
// ============================================================================

class RRError;
class UriString;
class Context;
class CacheManager;
class CacheRequest;
class RedditPost;
class GenericFactory_SeekableInputStream;   // GenericFactory<SeekableInputStream, IOException>
class TimestampUTC;

// Forward: AlbumInfo and ImageInfo from image_host port
class AlbumInfo;

// Forward: GetAlbumInfoListener (defined in imgur_api.h)
class GetAlbumInfoListener;

// ============================================================================
// RedditImageAPI — port of Kotlin class RedditGalleryAPI (Kotlin lines 43-152)
//
// Port of: org.quantumbadger.redreader.image.RedditGalleryAPI
//
// The Kotlin companion object holds:
//   - private cache: LRU LinkedHashMap<String, AlbumInfo> (max 100)
//   - fun addToCache(post: RedditPost)
//   - @JvmStatic fun getAlbumInfo(context, albumUrl, albumId, priority, listener)
//
// In C++, the companion object becomes static members.
// ============================================================================

class RedditImageAPI {
public:
    // Prevent instantiation (Kotlin companion object ↔ static class)
    RedditImageAPI() = delete;
    ~RedditImageAPI() = delete;

    // ========================================================================
    // getAlbumInfo — port of Kotlin @JvmStatic fun (Kotlin lines 61-151)
    //
    // Port of:
    //   @JvmStatic fun getAlbumInfo(
    //       context: Context,
    //       albumUrl: UriString,
    //       albumId: String,
    //       priority: Priority,
    //       listener: GetAlbumInfoListener)
    //
    // Checks LRU cache first. On cache miss, fetches from Reddit API
    // using PostCommentListingURL for the album ID.
    // ========================================================================
    static void getAlbumInfo(
            Context &context,
            const UriString &albumUrl,
            const QString &albumId,
            int priority,                       // Priority as int
            GetAlbumInfoListener &listener);

    // ========================================================================
    // addToCache — port of Kotlin fun (Kotlin lines 52-59)
    //
    // Port of:
    //   fun addToCache(post: RedditPost)
    //
    // Parses gallery from a RedditPost and adds to LRU cache.
    // ========================================================================
    static void addToCache(RedditPost &post);

private:
    // ========================================================================
    // LRU Cache — port of Kotlin private cache (Kotlin lines 46-50)
    //
    // Port of:
    //   private val cache = object : LinkedHashMap<String, AlbumInfo>() {
    //       override fun removeEldestEntry(
    //           eldest: MutableMap.MutableEntry<String, AlbumInfo>?
    //       ): Boolean {
    //           return this.size > 100
    //       }
    //   }
    //
    // Max entries: 100
    // Thread-safe via mutex
    // ========================================================================
    static constexpr int MAX_CACHE_SIZE = 100;

    // LRU cache: ordered map with explicit eviction
    // std::map is ordered; we manually evict eldest when size > MAX_CACHE_SIZE
    struct LRUCache {
        std::map<QString, std::shared_ptr<AlbumInfo>> entries;
        std::mutex mutex;

        std::shared_ptr<AlbumInfo> get(const QString &key);
        void put(const QString &key, std::shared_ptr<AlbumInfo> info);
        void remove(const QString &key);
    };

    static LRUCache s_cache;
};

} // namespace PinkReader
