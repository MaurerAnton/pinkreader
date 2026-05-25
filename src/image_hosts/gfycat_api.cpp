// Origin: RedReader image/GfycatAPI.java
#include "gfycat_api.h"
#include <stdexcept>

namespace PinkReader {

void GfycatAPI::GetImageInfo(
		Context& context,
		const std::string& imageId,
		Priority priority,
		GetImageInfoListener& listener) {

	// final UriString apiUrl = new UriString("https://api.gfycat.com/v1/gfycats/" + imageId);
	//
	// CacheManager.getInstance(context).makeRequest(new CacheRequest(
	//   apiUrl,
	//   RedditAccountManager.getAnon(),
	//   null,
	//   priority,
	//   DownloadStrategyIfNotCached.INSTANCE,
	//   Constants.FileType.IMAGE_INFO,
	//   CacheRequest.DownloadQueueType.IMMEDIATE,
	//   context,
	//   new CacheRequestJSONParser(context, new CacheRequestJSONParser.Listener() {
	//     onJsonParsed: final JsonObject outer = result.asObject().getObject("gfyItem");
	//                  listener.onSuccess(ImageInfo.parseGfycat(outer));
	//       catch t: listener.onFailure(getGeneralErrorForFailure(PARSE, t, null, apiUrl, Optional.of(new FailedRequestBody(result))))
	//     onFailure: listener.onFailure(error)
	//   })
	// ));
}

} // namespace PinkReader
