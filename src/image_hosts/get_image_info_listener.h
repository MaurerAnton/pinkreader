// Origin: RedReader image/GetImageInfoListener.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

// Forward declarations
class ImageInfo;
struct RRError;

// Interface for image info callbacks
class GetImageInfoListener {
public:
	virtual ~GetImageInfoListener() = default;

	virtual void OnFailure(const RRError& error) = 0;
	virtual void OnSuccess(const ImageInfo& info) = 0;
	virtual void OnNotAnImage() = 0;
};

} // namespace PinkReader
