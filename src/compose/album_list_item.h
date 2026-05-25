// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/AlbumListItem.kt
#pragma once

#include <functional>

namespace PinkReader {

struct ImageInfo;

// Ported from: @Composable fun AlbumListItem(index, image, onClick, onLongClick)
// Renders a single row in the album list view with thumbnail, text, and actions.
void AlbumListItem(
	int index,
	const ImageInfo& image,
	std::function<void(int)> onClick,
	std::function<void(int)> onLongClick
);

// Ported from: @Composable @Preview fun PreviewAlbumListItem()
void PreviewAlbumListItem();

} // namespace PinkReader
