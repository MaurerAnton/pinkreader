// Origin: RedReader image/ImageInfo.kt
#include "image_info.h"
#include <stdexcept>
#include <sstream>

namespace PinkReader {

// ImageSize

std::optional<ImageSize> ImageSize::From(std::optional<int64_t> width, std::optional<int64_t> height) {
	if (width.has_value() && IsValidPositiveInt(*width) &&
		height.has_value() && IsValidPositiveInt(*height)) {
		return ImageSize(static_cast<int>(*width), static_cast<int>(*height));
	}
	return std::nullopt;
}

std::optional<ImageSize> ImageSize::FromJson(
		const JsonObject* obj,
		const std::string& fieldWidth,
		const std::string& fieldHeight) {
	if (obj == nullptr) return std::nullopt;
	// return From(obj->GetLong(fieldWidth), obj->GetLong(fieldHeight));
	// Deferred: JsonObject GetLong not yet ported
	return std::nullopt;
}

std::string ImageSize::ToString() const {
	return std::to_string(width) + "x" + std::to_string(height);
}

// ImageInfo

ImageInfo::ImageInfo(
		const ImageUrlInfo& original,
		std::optional<ImageUrlInfo> bigSquare,
		std::optional<ImageUrlInfo> preview,
		std::optional<UriString> urlAudioStream,
		std::optional<std::string> title,
		std::optional<std::string> caption,
		std::optional<UriString> outboundUrl,
		std::optional<std::string> type,
		std::optional<bool> isAnimated,
		std::optional<MediaType> mediaType,
		HasAudio hasAudio)
	: original(original)
	, bigSquare(std::move(bigSquare))
	, preview(std::move(preview))
	, urlAudioStream(std::move(urlAudioStream))
	, title(std::move(title))
	, caption(std::move(caption))
	, outboundUrl(std::move(outboundUrl))
	, type(std::move(type))
	, isAnimated(isAnimated)
	, mediaType(mediaType)
	, hasAudio(hasAudio) {}

ImageInfo::HasAudio ImageInfo::HasAudioFromBoolean(std::optional<bool> value) {
	if (!value.has_value()) {
		return HasAudio::MAYBE_AUDIO;
	}
	return *value ? HasAudio::HAS_AUDIO : HasAudio::NO_AUDIO;
}

ImageInfo ImageInfo::ParseGfycat(const JsonObject& obj) {
	// val title = obj.getString("title")
	// val hasAudio = obj.getBoolean("hasAudio")
	// val original = obj.getString("mp4Url")?.let { ... ImageUrlInfo(url, size, mp4Size) }
	// if (original == null) throw RuntimeException("mp4Url field missing from response")
	// val mobilePoster = obj.getObjectAtPath("content_urls", "mobilePoster")
	// val preview = mobilePoster?.getString("url")?.let { ... }
	// return ImageInfo(original, preview, title, type="video/mp4", isAnimated=true, mediaType=VIDEO, hasAudio=fromBoolean(hasAudio))

	// Placeholder - deferred until JsonObject is ported
	throw std::runtime_error("ImageInfo::ParseGfycat not yet implemented");
}

ImageInfo ImageInfo::ParseStreamable(const JsonObject& obj) {
	// var fileObj: JsonObject? = null
	// val files = obj.getObject("files")
	// preferredTypes = ["mp4","webm","mp4-high","webm-high","mp4-mobile","webm-mobile"]
	// for type in preferredTypes: fileObj = files.getObject(type); if fileObj != null break
	// if fileObj == null throw IOException("No suitable Streamable files found")
	// mimeType = "video/" + selectedType.split("-")[0]
	// original = fileObj.getString("url")?.let { ImageUrlInfo(url, size) }
	// if original == null throw RuntimeException("url field missing")
	// return ImageInfo(original, type=mimeType, isAnimated=true, mediaType=VIDEO, hasAudio=MAYBE_AUDIO)

	throw std::runtime_error("ImageInfo::ParseStreamable not yet implemented");
}

ImageInfo ImageInfo::ParseImgur(const JsonObject* obj) {
	// val image = obj?.getObject("image")
	// val links = obj?.getObject("links")
	// val type = image?.getString("type")
	// val isAnimated = "true" == image?.getString("animated")
	// original = links?.getString("original")?.let { url -> if(isAnimated) replace(".gif", ".mp4") else url }
	// if original == null throw RuntimeException("original field missing")
	// bigSquare = links.getString("big_square")?.let { ImageUrlInfo(url=it) }
	// return ImageInfo(original, bigSquare, title, caption, type, isAnimated,
	//   mediaType=(if isAnimated VIDEO else IMAGE), hasAudio=(if isAnimated MAYBE_AUDIO else NO_AUDIO))

	throw std::runtime_error("ImageInfo::ParseImgur not yet implemented");
}

ImageInfo ImageInfo::ParseImgurV3(const JsonObject* obj) {
	// val type = obj?.getString("type")
	// val isAnimated = obj?.getBoolean("animated")
	// var mp4 = false; var hasSound: Boolean? = null
	// originalSize = ImageSize.fromJson(obj)
	// original = obj?.getString("mp4")?.takeIf { !isEmpty }?.let {
	//   hasSound = obj.getBoolean("has_sound"); mp4 = true
	//   ImageUrlInfo(urlMp4, originalSize, mp4_size)
	// } ?: obj?.getString("link")?.takeIf { !isEmpty }?.let { ImageUrlInfo(urlLink, originalSize, size) }
	// if original == null throw RuntimeException("original field missing")
	// bigSquare = obj?.getString("id")?.let { ImageUrlInfo("https://i.imgur.com/" + it + "b.jpg") }
	// return ImageInfo(original, bigSquare, title, caption, type, isAnimated,
	//   mediaType=(if mp4 VIDEO else IMAGE), hasAudio=fromBoolean(hasSound))

	throw std::runtime_error("ImageInfo::ParseImgurV3 not yet implemented");
}

ImageInfo ImageInfo::ParseDeviantArt(const JsonObject* obj) {
	// val original = obj?.getString("url")?.let { ImageUrlInfo(url, ImageSize.fromJson(obj)) }
	// val bigSquare = obj?.getString("thumbnail_url")?.let { ImageUrlInfo(url=it) }
	// if original == null throw RuntimeException("url field missing")
	// return ImageInfo(original, bigSquare, title, caption=tags, type=imagetype, isAnimated=false, mediaType=IMAGE, hasAudio=NO_AUDIO)

	throw std::runtime_error("ImageInfo::ParseDeviantArt not yet implemented");
}

ImageInfo ImageInfo::ParseRedgifsV2(const JsonObject& obj) {
	// val original = obj.getStringAtPath("urls", "hd").orElse(obj.getStringAtPath("urls", "sd")).asNullable()?.let {
	//   ImageUrlInfo(url=UriString(it), size=ImageSize.fromJson(obj))
	// }
	// val hasAudio = obj.getBoolean("hasAudio")
	// val preview = obj.getStringAtPath("urls", "poster").orElseNull()?.let { ImageUrlInfo(url=UriString(url)) }
	// if original == null throw RuntimeException("original field missing")
	// return ImageInfo(original, preview, type="video/mp4", isAnimated=true, mediaType=VIDEO, hasAudio=fromBoolean(hasAudio))

	throw std::runtime_error("ImageInfo::ParseRedgifsV2 not yet implemented");
}

} // namespace PinkReader
