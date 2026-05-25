// Origin: RedReader image/AlbumInfo.kt
#include "album_info.h"
#include <stdexcept>
#include <algorithm>

namespace PinkReader {

AlbumInfo::AlbumInfo(
		const UriString& url,
		std::optional<std::string> title,
		std::optional<std::string> description,
		const std::vector<ImageInfo>& images)
	: url(url)
	, title(std::move(title))
	, description(std::move(description))
	, images(images) {}

ImageInfo::MediaType AlbumInfo::StringToMediaType(const std::string* mediaTypeString) {
	if (mediaTypeString == nullptr) {
		return ImageInfo::MediaType::IMAGE;
	}

	if (*mediaTypeString == "AnimatedImage") {
		return ImageInfo::MediaType::GIF;
	} else if (*mediaTypeString == "Video") {
		return ImageInfo::MediaType::VIDEO;
	} else {
		return ImageInfo::MediaType::IMAGE;
	}
}

std::optional<ImageUrlInfo> AlbumInfo::GetPreview(
		int minSizePx,
		const std::vector<ImageMetadata>& images) {

	std::optional<int64_t> bestSizeMinAxis;
	std::optional<std::string> bestUrl;
	std::optional<ImageSize> bestSize;

	for (const auto& image : images) {
		if (image.u.empty()) {
			continue;
		}

		int64_t x = image.x;
		int64_t y = image.y;
		int64_t minAxis = std::min(x, y);

		if (!bestSizeMinAxis.has_value() ||
			(*bestSizeMinAxis < minSizePx && minAxis > *bestSizeMinAxis) ||
			(minAxis >= minSizePx && minAxis < *bestSizeMinAxis)) {

			bestSizeMinAxis = minAxis;
			bestUrl = image.u; // decoded URL string
			bestSize = ImageSize::From(x, y);
		}
	}

	if (bestUrl.has_value()) {
		return ImageUrlInfo(
			UriString(*bestUrl), // Deferred: UriString construction
			bestSize);
	}

	return std::nullopt;
}

AlbumInfo AlbumInfo::ParseImgur(const UriString& url, const JsonObject& obj) {
	// var title = obj.getString("title") -> unescapeHtml4
	// var description = obj.getString("description") -> unescapeHtml4
	// val imagesJson = obj.getArray("images")
	// for imageJson in imagesJson: images.add(ImageInfo.parseImgur(imageJson.asObject()))
	// return AlbumInfo(url, title, description, images)

	throw std::runtime_error("AlbumInfo::ParseImgur not yet implemented");
}

AlbumInfo AlbumInfo::ParseImgurV3(const UriString& url, const JsonObject& obj) {
	// Same as ParseImgur but uses ImageInfo.parseImgurV3 for each image
	throw std::runtime_error("AlbumInfo::ParseImgurV3 not yet implemented");
}

std::optional<AlbumInfo> AlbumInfo::ParseRedditGallery(RedditPost& post) {
	// val galleryItems = post.gallery_data?.items ?: return null
	// val images = galleryItems.mapNotNull { (it as? MaybeParseError.Ok)?.value }
	//   .mapNotNull { item ->
	//     val mediaMetadataEntry = (post.media_metadata?.get(item.media_id) as? MaybeParseError.Ok)?.value ?: return null
	//     val standardImage = mediaMetadataEntry.s
	//     val mediaType = stringToMediaType(mediaMetadataEntry.e)
	//     val urlEscaped = standardImage.u ?: standardImage.mp4 ?: standardImage.gif
	//     val original = urlEscaped?.let { ImageUrlInfo(UriString(it.decoded), ImageSize.from(...)) }
	//     if original == null throw RuntimeException("url missing from response")
	//     var bigSquare / preview from mediaMetadataEntry.p
	//     ImageInfo(original, preview, bigSquare, title=item.caption?.decoded,
	//       outboundUrl=..., type=mediaMetadataEntry.m, isAnimated, mediaType, hasAudio=MAYBE_AUDIO)
	//   }.toList()
	// val title = post.title?.decoded
	// return AlbumInfo(post.findUrl()!!, title, null, images)

	std::optional<AlbumInfo> result;
	// Deferred: full RedditPost parsing
	return result;
}

} // namespace PinkReader
