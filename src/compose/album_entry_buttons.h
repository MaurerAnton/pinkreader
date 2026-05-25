// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/AlbumEntryButtons.kt
#pragma once

#include <functional>

namespace PinkReader {

struct ImageUrlInfo;

// Ported from: @Composable fun AlbumEntryButtons(modifier: Modifier = Modifier, image: ImageUrlInfo)
// Renders the row of action buttons (save, share dropdown, more options) for an album entry.
void AlbumEntryButtons(
	int modifier,              // Modifier (placeholder)
	const ImageUrlInfo& image
);

} // namespace PinkReader
