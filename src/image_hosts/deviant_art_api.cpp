// Origin: RedReader image/DeviantArtAPI.java
#include "deviant_art_api.h"
#include <stdexcept>
#include <string>

namespace PinkReader {

void DeviantArtAPI::GetImageInfo(
		Context& context,
		const UriString& url,
		Priority priority,
		GetImageInfoListener& listener) {

	// final UriString apiUrl;
	// try {
	//   apiUrl = new UriString("https://backend.deviantart.com/oembed?url="
	//       + URLEncoder.encode(url.value, "UTF-8"));
	// } catch(UnsupportedEncodingException e) {
	//   throw new RuntimeException(e);
	// }
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
	//     onJsonParsed: listener.onSuccess(ImageInfo.parseDeviantArt(result.asObject()))
	//       catch t: listener.onFailure(getGeneralErrorForFailure(PARSE, t, null, apiUrl, Optional.of(new FailedRequestBody(result))))
	//     onFailure: listener.onFailure(error)
	//   })
	// ));
}

} // namespace PinkReader
