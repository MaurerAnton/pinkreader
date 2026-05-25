// Origin: RedReader image/GetAlbumInfoListener.java
#pragma once

#include <memory>
#include <string>

namespace PinkReader {

// Forward declarations
class AlbumInfo;
struct RRError;

// Interface for album info callbacks
class GetAlbumInfoListener {
public:
	virtual ~GetAlbumInfoListener() = default;

	virtual void OnFailure(const RRError& error) = 0;
	virtual void OnSuccess(const AlbumInfo& info) = 0;
	virtual void OnGalleryRemoved() = 0;
	virtual void OnGalleryDataNotPresent() = 0;
};

} // namespace PinkReader
