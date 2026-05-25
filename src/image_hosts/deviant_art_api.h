// Origin: RedReader image/DeviantArtAPI.java
#pragma once

#include <string>
#include <memory>

namespace PinkReader {

// Forward declarations
class Context;
class UriString;
class GetImageInfoListener;
enum class Priority;

// DeviantArtAPI: fetches image info from DeviantArt's oembed API
class DeviantArtAPI {
public:
	static void GetImageInfo(
		Context& context,
		const UriString& url,
		Priority priority,
		GetImageInfoListener& listener);
};

} // namespace PinkReader
