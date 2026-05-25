// Origin: RedReader image/ImgurAPI.java
#include "imgur_api.h"
#include <stdexcept>

namespace PinkReader {

void ImgurAPI::GetAlbumInfo(
		Context& context,
		const UriString& albumUrl,
		const std::string& albumId,
		Priority priority,
		GetAlbumInfoListener& listener) {

	// final UriString apiUrl = new UriString("https://api.imgur.com/2/album/" + albumId + ".json");
	//
	// CacheManager.getInstance(context).makeRequest(new CacheRequest(
	//   apiUrl, RedditAccountManager.getAnon(), null, priority,
	//   DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
	//   CacheRequest.DownloadQueueType.IMMEDIATE, context,
	//   new CacheRequestJSONParser(context, new Listener() {
	//     onJsonParsed: final JsonObject outer = result.asObject().getObject("album");
	//                  listener.onSuccess(AlbumInfo.parseImgur(albumUrl, outer));
	//       catch t: listener.onFailure(getGeneralErrorForFailure(PARSE, t, null, apiUrl, Optional.of(new FailedRequestBody(result))))
	//     onFailure: listener.onFailure(error)
	//   })
	// ));
}

void ImgurAPI::GetImageInfo(
		Context& context,
		const std::string& imageId,
		Priority priority,
		GetImageInfoListener& listener) {

	// final UriString apiUrl = new UriString("https://api.imgur.com/2/image/" + imageId + ".json");
	//
	// CacheManager.getInstance(context).makeRequest(new CacheRequest(
	//   apiUrl, RedditAccountManager.getAnon(), null, priority,
	//   DownloadStrategyIfNotCached.INSTANCE, Constants.FileType.IMAGE_INFO,
	//   CacheRequest.DownloadQueueType.IMMEDIATE, context,
	//   new CacheRequestJSONParser(context, new Listener() {
	//     onJsonParsed: final JsonObject outer = result.asObject().getObject("image");
	//                  listener.onSuccess(ImageInfo.parseImgur(outer));
	//       catch t: listener.onFailure(getGeneralErrorForFailure(PARSE, t, null, apiUrl, Optional.of(new FailedRequestBody(result))))
	//     onFailure: listener.onFailure(error)
	//   })
	// ));
}

} // namespace PinkReader
