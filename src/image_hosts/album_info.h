// Origin: RedReader image/AlbumInfo.kt
#pragma once

#include <string>
#include <optional>
#include <vector>
#include <memory>
#include "image_info.h"

namespace PinkReader {

// Forward declarations
class UriString;
class JsonObject;
class RedditPost;
struct ImageMetadata;

// AlbumInfo: represents an image album
class AlbumInfo {
public:
	UriString url;
	std::optional<std::string> title;
	std::optional<std::string> description;
	std::vector<ImageInfo> images;

	AlbumInfo(const UriString& url,
		std::optional<std::string> title,
		std::optional<std::string> description,
		const std::vector<ImageInfo>& images);

	// Static parse methods
	static AlbumInfo ParseImgur(const UriString& url, const JsonObject& obj);
	static AlbumInfo ParseImgurV3(const UriString& url, const JsonObject& obj);
	static std::optional<AlbumInfo> ParseRedditGallery(RedditPost& post);

private:
	static ImageInfo::MediaType StringToMediaType(const std::string* mediaTypeString);
	static std::optional<ImageUrlInfo> GetPreview(
		int minSizePx,
		const std::vector<ImageMetadata>& images);
};

} // namespace PinkReader
