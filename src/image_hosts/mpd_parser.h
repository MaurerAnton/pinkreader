// Origin: RedReader image/MPDParser.kt
#pragma once

#include <string>
#include <optional>
#include <vector>

namespace PinkReader {

// Data structures for MPD (Media Presentation Description) parsing
// Used to extract video/audio stream info from DASH manifests

struct MediaInfo {
	std::string filename;
	std::optional<int> width;
	std::optional<int> height;

	MediaInfo(std::string filename,
		std::optional<int> width = std::nullopt,
		std::optional<int> height = std::nullopt)
		: filename(std::move(filename)), width(width), height(height) {}
};

struct MPDReadResult {
	std::optional<MediaInfo> video;
	std::optional<MediaInfo> audio;

	MPDReadResult(
		std::optional<MediaInfo> video = std::nullopt,
		std::optional<MediaInfo> audio = std::nullopt)
		: video(std::move(video)), audio(std::move(audio)) {}
};

// Internal MPD structure representations
struct Representation {
	std::optional<std::string> id;
	int bandwidth;
	std::optional<std::string> codecs;
	std::optional<int> width;
	std::optional<int> height;
	std::optional<std::string> audioSamplingRate;
	std::optional<std::string> baseURL;
};

struct AdaptationSet {
	std::optional<std::string> contentType;
	std::vector<Representation> representations;
};

struct Period {
	std::vector<AdaptationSet> adaptationSets;
};

struct MPD {
	std::vector<Period> periods;
};

// Main parsing function
MPDReadResult ParseMPD(const std::string& mpdContent);

// Internal parsing functions
MPD ParseMPDDocument(const std::string& content);
Period ParsePeriod(const std::string& content, size_t& pos);
AdaptationSet ParseAdaptationSet(const std::string& content, size_t& pos);
Representation ParseRepresentation(const std::string& content, size_t& pos);

// Selection functions
std::optional<MediaInfo> SelectVideoRepresentation(const AdaptationSet& adaptationSet);
std::optional<MediaInfo> SelectAudioRepresentation(const AdaptationSet& adaptationSet);

} // namespace PinkReader
