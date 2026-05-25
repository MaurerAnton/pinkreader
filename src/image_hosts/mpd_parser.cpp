// Origin: RedReader image/MPDParser.kt
#include "mpd_parser.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <string>

namespace PinkReader {

// Simple XML pull parser for MPD manifests
// Full implementation would use XmlPullParser logic:
// Parse MPD document -> get first period -> find video/audio adaptation sets
// -> select best video/audio representation

MPDReadResult ParseMPD(const std::string& mpdContent) {
	// factory = XmlPullParserFactory.newInstance()
	// factory.isNamespaceAware = true
	// parser = factory.newPullParser()
	// parser.setInput(mpdContent.reader())
	// mpd = parseMPDDocument(parser)
	// firstPeriod = mpd.periods.firstOrNull() ?: return MPDReadResult()
	// videoAdaptationSet = firstPeriod.adaptationSets.firstOrNull { it.contentType == "video" }
	// audioAdaptationSet = firstPeriod.adaptationSets.firstOrNull { it.contentType == "audio" }
	// selectedVideo = videoAdaptationSet?.let { selectVideoRepresentation(it) }
	// selectedAudio = audioAdaptationSet?.let { selectAudioRepresentation(it) }
	// return MPDReadResult(video = selectedVideo, audio = selectedAudio)

	MPDReadResult result;

	// Deferred: XML parsing requires an XML library
	// For now, return empty result
	return result;
}

MPD ParseMPDDocument(const std::string& content) {
	MPD mpd;
	// Parse Period elements from MPD document
	// while eventType != END_DOCUMENT:
	//   if START_TAG && name == "Period": periods.add(parsePeriod(parser))
	//   eventType = parser.next()
	return mpd;
}

Period ParsePeriod(const std::string& content, size_t& pos) {
	Period period;
	// Parse AdaptationSet elements within a Period
	// while eventType != END_DOCUMENT:
	//   when START_TAG: if name == "AdaptationSet": add parseAdaptationSet
	//   when END_TAG: if name == "Period" && depth == periodDepth: break
	return period;
}

AdaptationSet ParseAdaptationSet(const std::string& content, size_t& pos) {
	// contentType = parser.getAttributeValue(null, "contentType")
	// while eventType != END_DOCUMENT:
	//   when START_TAG: if name == "Representation": add parseRepresentation
	//   when END_TAG: if name == "AdaptationSet" && depth == periodDepth: break
	// return AdaptationSet(contentType, representations)
	AdaptationSet set;
	return set;
}

Representation ParseRepresentation(const std::string& content, size_t& pos) {
	Representation rep{};
	// id = parser.getAttributeValue(null, "id")
	// bandwidth = parser.getAttributeValue(null, "bandwidth")?.toIntOrNull() ?: 0
	// codecs = parser.getAttributeValue(null, "codecs")
	// width = parser.getAttributeValue(null, "width")?.toIntOrNull()
	// height = parser.getAttributeValue(null, "height")?.toIntOrNull()
	// audioSamplingRate = parser.getAttributeValue(null, "audioSamplingRate")
	// Parse BaseURL child element
	// return Representation(id, bandwidth, codecs, width, height, audioSamplingRate, baseURL)
	return rep;
}

std::optional<MediaInfo> SelectVideoRepresentation(const AdaptationSet& adaptationSet) {
	// Filter representations with baseURL != null
	// Sort by comparator:
	//   compareBy<Representation> { rep ->
	//     val smallestDim = minOf(rep.width ?: 0, rep.height ?: 0)
	//     when { smallestDim == 480 -> 0; smallestDim == 720 -> 1;
	//            smallestDim > 480 -> 2 + (smallestDim - 481);
	//            else -> 10000 - smallestDim }
	//   }.thenByDescending { rep -> if (rep.codecs?.startsWith("avc1.") == true) 1 else 0 }
	//    .thenByDescending { rep -> rep.bandwidth }
	// Select first sorted representation
	// Return MediaInfo(filename = baseURL!!, width, height)

	const auto& reps = adaptationSet.representations;
	for (auto it = reps.begin(); it != reps.end(); ++it) {
		if (it->baseURL.has_value()) {
			return MediaInfo(*it->baseURL, it->width, it->height);
		}
	}

	return std::nullopt;
}

std::optional<MediaInfo> SelectAudioRepresentation(const AdaptationSet& adaptationSet) {
	// Filter representations with baseURL != null
	// Select highest bandwidth: validReps.maxByOrNull { it.bandwidth }
	// Return MediaInfo(filename = baseURL!!, width=null, height=null)

	const auto& reps = adaptationSet.representations;
	int maxBandwidth = -1;
	const Representation* best = nullptr;

	for (const auto& rep : reps) {
		if (rep.baseURL.has_value() && rep.bandwidth > maxBandwidth) {
			maxBandwidth = rep.bandwidth;
			best = &rep;
		}
	}

	if (best != nullptr) {
		return MediaInfo(*best->baseURL);
	}

	return std::nullopt;
}

} // namespace PinkReader
