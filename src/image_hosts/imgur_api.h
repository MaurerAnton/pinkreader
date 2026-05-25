// Origin: RedReader image/ImgurAPI.java
#pragma once

#include <string>
#include <memory>

namespace PinkReader {

class Context;
class UriString;
class GetAlbumInfoListener;
class GetImageInfoListener;
enum class Priority;

// ImgurAPI v2: fetches album/image info from Imgur's v2 API
class ImgurAPI {
public:
	static void GetAlbumInfo(
		Context& context,
		const UriString& albumUrl,
		const std::string& albumId,
		Priority priority,
		GetAlbumInfoListener& listener);

	static void GetImageInfo(
		Context& context,
		const std::string& imageId,
		Priority priority,
		GetImageInfoListener& listener);
};

} // namespace PinkReader
