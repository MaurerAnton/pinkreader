// Origin: RedReader image/ImageInfo.kt
#pragma once

#include <string>
#include <optional>
#include <memory>
#include <vector>
#include <cstdint>
#include <climits>

namespace PinkReader {

// Forward declarations
class UriString;
class JsonObject;

// ImageSize: data class with width, height
struct ImageSize {
	int width;
	int height;

	ImageSize(int width, int height) : width(width), height(height) {}

	static std::optional<ImageSize> From(std::optional<int64_t> width, std::optional<int64_t> height);

	static std::optional<ImageSize> FromJson(
		const JsonObject* obj,
		const std::string& fieldWidth = "width",
		const std::string& fieldHeight = "height");

	std::string ToString() const;
};

// ImageUrlInfo: data class with url, size, sizeBytes
struct ImageUrlInfo {
	UriString url;
	std::optional<ImageSize> size;
	std::optional<int64_t> sizeBytes;

	ImageUrlInfo(const UriString& url,
		std::optional<ImageSize> size = std::nullopt,
		std::optional<int64_t> sizeBytes = std::nullopt)
		: url(url), size(std::move(size)), sizeBytes(sizeBytes) {}
};

// ImageInfo: main data class
class ImageInfo {
public:
	enum class MediaType {
		IMAGE,
		VIDEO,
		GIF
	};

	enum class HasAudio {
		HAS_AUDIO,
		MAYBE_AUDIO,
		NO_AUDIO
	};

	static HasAudio HasAudioFromBoolean(std::optional<bool> value);

	// Fields
	ImageUrlInfo original;
	std::optional<ImageUrlInfo> bigSquare;
	std::optional<ImageUrlInfo> preview;
	std::optional<UriString> urlAudioStream;
	std::optional<std::string> title;
	std::optional<std::string> caption;
	std::optional<UriString> outboundUrl;
	std::optional<std::string> type;
	std::optional<bool> isAnimated;
	std::optional<MediaType> mediaType;
	HasAudio hasAudio;

	ImageInfo(
		const ImageUrlInfo& original,
		std::optional<ImageUrlInfo> bigSquare = std::nullopt,
		std::optional<ImageUrlInfo> preview = std::nullopt,
		std::optional<UriString> urlAudioStream = std::nullopt,
		std::optional<std::string> title = std::nullopt,
		std::optional<std::string> caption = std::nullopt,
		std::optional<UriString> outboundUrl = std::nullopt,
		std::optional<std::string> type = std::nullopt,
		std::optional<bool> isAnimated = std::nullopt,
		std::optional<MediaType> mediaType = std::nullopt,
		HasAudio hasAudio = HasAudio::MAYBE_AUDIO);

	// Static parse methods (companion object equivalents)
	static ImageInfo ParseGfycat(const JsonObject& obj);
	static ImageInfo ParseStreamable(const JsonObject& obj);
	static ImageInfo ParseImgur(const JsonObject* obj);
	static ImageInfo ParseImgurV3(const JsonObject* obj);
	static ImageInfo ParseDeviantArt(const JsonObject* obj);
	static ImageInfo ParseRedgifsV2(const JsonObject& obj);
};

// Helper: check if a long is a valid positive int
inline bool IsValidPositiveInt(int64_t value) {
	return value < INT_MAX && value >= 0;
}

} // namespace PinkReader
