// Origin: RedReader image/ImgurAPIV3.java
#include "imgur_api_v3.h"
#include <stdexcept>

namespace PinkReader {

void ImgurAPIV3::GetAlbumInfo(
		Context& context,
		const UriString& albumUrl,
		const std::string& albumId,
		Priority priority,
		bool withAuth,
		GetAlbumInfoListener& listener) {

	// final UriString apiUrl = new UriString("https://api.imgur.com/3/album/" + albumId);
	//
	// CacheManager.getInstance(context).makeRequest(new CacheRequest(
	//   apiUrl, RedditAccountManager.getAnon(), null, priority,
	//   DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
	//   withAuth ? CacheRequest.DownloadQueueType.IMGUR_API : CacheRequest.DownloadQueueType.IMMEDIATE,
	//   context,
	//   new CacheRequestJSONParser(context, new Listener() {
	//     onJsonParsed: final JsonObject outer = result.asObject().getObject("data");
	//                  final AlbumInfo album = AlbumInfo.parseImgurV3(albumUrl, outer);
	//                  listener.onSuccess(album);
	//       catch t: listener.onFailure(getGeneralErrorForFailure(PARSE, t, null, apiUrl, Optional.of(new FailedRequestBody(result))))
	//     onFailure: listener.onFailure(error)
	//   })
	// ));
}

void ImgurAPIV3::GetImageInfo(
		Context& context,
		const std::string& imageId,
		Priority priority,
		bool withAuth,
		GetImageInfoListener& listener) {

	// final UriString apiUrl = new UriString("https://api.imgur.com/3/image/" + imageId);
	//
	// CacheManager.getInstance(context).makeRequest(new CacheRequest(
	//   apiUrl, RedditAccountManager.getAnon(), null, priority,
	//   DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
	//   withAuth ? CacheRequest.DownloadQueueType.IMGUR_API : CacheRequest.DownloadQueueType.IMMEDIATE,
	//   context,
	//   new CacheRequestJSONParser(context, new Listener() {
	//     onJsonParsed: final JsonObject outer = result.asObject().getObject("data");
	//                  listener.onSuccess(ImageInfo.parseImgurV3(outer));
	//       catch t: listener.onFailure(getGeneralErrorForFailure(PARSE, t, null, apiUrl, Optional.of(new FailedRequestBody(result))))
	//     onFailure: listener.onFailure(error)
	//   })
	// ));
}

} // namespace PinkReader
