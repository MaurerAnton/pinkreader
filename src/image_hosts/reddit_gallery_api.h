// Origin: RedReader image/RedditGalleryAPI.kt
#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "album_info.h"

namespace PinkReader {

class Context;
class UriString;
class GetAlbumInfoListener;
class RedditPost;
enum class Priority;

// RedditGalleryAPI: fetches gallery/album info from Reddit's API
// Includes an LRU cache for parsed gallery data
class RedditGalleryAPI {
public:
	static void AddToCache(RedditPost& post);

	static void GetAlbumInfo(
		Context& context,
		const UriString& albumUrl,
		const std::string& albumId,
		Priority priority,
		GetAlbumInfoListener& listener);

private:
	// LRU cache: LinkedHashMap<String, AlbumInfo> with removeEldestEntry when size > 100
	struct LRUCache {
		std::unordered_map<std::string, AlbumInfo> map;
		std::mutex mutex;
		static constexpr size_t MAX_SIZE = 100;

		void Put(const std::string& key, const AlbumInfo& value);
		std::optional<AlbumInfo> Get(const std::string& key);
		void Remove(const std::string& key);
	};

	static LRUCache sCache;
};

} // namespace PinkReader
