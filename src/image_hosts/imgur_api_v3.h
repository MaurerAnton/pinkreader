// Origin: RedReader image/ImgurAPIV3.java
#pragma once

#include <string>
#include <memory>

namespace PinkReader {

class Context;
class UriString;
class GetAlbumInfoListener;
class GetImageInfoListener;
enum class Priority;

// ImgurAPI v3: fetches album/image info from Imgur's v3 API
// Supports authenticated requests via withAuth flag
class ImgurAPIV3 {
public:
	static void GetAlbumInfo(
		Context& context,
		const UriString& albumUrl,
		const std::string& albumId,
		Priority priority,
		bool withAuth,
		GetAlbumInfoListener& listener);

	static void GetImageInfo(
		Context& context,
		const std::string& imageId,
		Priority priority,
		bool withAuth,
		GetImageInfoListener& listener);
};

} // namespace PinkReader
