// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/NetImage.kt
#pragma once

#include <functional>
#include <optional>

namespace PinkReader {

struct ImageUrlInfo;

// Ported from: @Composable fun NetImage(modifier, image, cropToAspect, showVideoPlayOverlay, maxCanvasDimension)
// Renders a network-loaded image with loading/error/success states.
void NetImage(
	int modifier,                           // Modifier placeholder
	const ImageUrlInfo& image,
	std::optional<float> cropToAspect = std::nullopt,
	bool showVideoPlayOverlay = false,
	int maxCanvasDimension = 2048
);

} // namespace PinkReader
