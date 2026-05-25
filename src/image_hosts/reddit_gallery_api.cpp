// Origin: RedReader image/RedditGalleryAPI.kt
#include "reddit_gallery_api.h"
#include <stdexcept>

namespace PinkReader {

RedditGalleryAPI::LRUCache RedditGalleryAPI::sCache;

void RedditGalleryAPI::LRUCache::Put(const std::string& key, const AlbumInfo& value) {
	std::lock_guard<std::mutex> lock(mutex);
	map[key] = value;
	// LRU eviction: if size > MAX_SIZE, remove eldest
	// Deferred: proper LRU tracking
}

std::optional<AlbumInfo> RedditGalleryAPI::LRUCache::Get(const std::string& key) {
	std::lock_guard<std::mutex> lock(mutex);
	auto it = map.find(key);
	if (it != map.end()) {
		return it->second;
	}
	return std::nullopt;
}

void RedditGalleryAPI::LRUCache::Remove(const std::string& key) {
	std::lock_guard<std::mutex> lock(mutex);
	map.erase(key);
}

void RedditGalleryAPI::AddToCache(RedditPost& post) {
	// AlbumInfo.parseRedditGallery(post)?.apply {
	//   synchronized(cache) {
	//     cache.remove(post.id)
	//     cache.put(post.id, this)
	//   }
	// }
	// Deferred: RedditPost parsing not yet implemented
}

void RedditGalleryAPI::GetAlbumInfo(
		Context& context,
		const UriString& albumUrl,
		const std::string& albumId,
		Priority priority,
		GetAlbumInfoListener& listener) {

	// val cacheEntry = synchronized(cache) { cache[albumId] }
	// cacheEntry?.apply { listener.onSuccess(this); return }
	//
	// val apiUrl = PostCommentListingURL(null, albumId, null, null, null, null, false).generateJsonUri()
	//
	// CacheManager.getInstance(context).makeRequest(CacheRequest(
	//   UriString.from(apiUrl),
	//   RedditAccountManager.getInstance(context).defaultAccount,
	//   null, priority, DownloadStrategyIfNotCached.INSTANCE,
	//   Constants.FileType.IMAGE_INFO, CacheRequest.DownloadQueueType.REDDIT_API, context,
	//   object : CacheRequestCallbacks {
	//     onDataStreamComplete:
	//       val thingResponse = JsonUtils.decodeRedditThingResponseFromStream(streamFactory.create())
	//       val responseMultiple = (thingResponse as? RedditThingResponse.Multiple) ?: throw RuntimeException(...)
	//       val listing = (responseMultiple.things.firstOrNull() as? RedditThing.Listing)?.data ?: throw RuntimeException(...)
	//       val firstItem = listing.children.firstOrNull()?.ok()
	//       val post = (firstItem as? RedditThing.Post)?.data ?: throw RuntimeException(...)
	//       val album = AlbumInfo.parseRedditGallery(post)
	//       if album == null:
	//         if post.removed_by_category != null: listener.onGalleryRemoved()
	//         else: listener.onGalleryDataNotPresent()
	//       else: listener.onSuccess(album)
	//       catch t: onFailure(getGeneralErrorForFailure(PARSE, t, null, albumUrl, FailedRequestBody.from(streamFactory)))
	//     onFailure: listener.onFailure(error)
	//   }
	// ))
}

} // namespace PinkReader
