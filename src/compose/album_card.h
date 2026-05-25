// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/AlbumCard.kt
#pragma once

#include <functional>

namespace PinkReader {

struct ImageInfo;

// Ported from: @Composable fun AlbumCard(index, image, onClick, onLongClick)
// AlbumCard renders a single image card in the album view.
// Preserves exact parameter structure:
//   index: Int, image: ImageInfo, onClick: (Int) -> Unit, onLongClick: (Int) -> Unit
void AlbumCard(
	int index,
	const ImageInfo& image,
	std::function<void(int)> onClick,
	std::function<void(int)> onLongClick
);

// Ported from: @Composable @Preview fun PreviewAlbumCard()
// Creates a test preview with sample data
void PreviewAlbumCard();

} // namespace PinkReader
