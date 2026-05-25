// Origin: RedReader image/GfycatAPI.java
#pragma once

#include <string>
#include <memory>

namespace PinkReader {

class Context;
class GetImageInfoListener;
enum class Priority;

// GfycatAPI: fetches image info from Gfycat's v1 API
class GfycatAPI {
public:
	static void GetImageInfo(
		Context& context,
		const std::string& imageId,
		Priority priority,
		GetImageInfoListener& listener);
};

} // namespace PinkReader
