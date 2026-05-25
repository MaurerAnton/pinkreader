// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/AlbumEntryButtons.kt
#include "album_entry_buttons.h"

// Dependencies (placeholder):
// - LocalLauncher (Dest)
// - RRIconButton
// - RRDropdownMenuIconButton, Item
// - R.drawable.download, ic_action_share_dark, dots_vertical_dark
// - R.string.action_save_image, action_share, action_share_image, action_share_link, three_dots_menu
// - Row, Spacer, Arrangement

namespace PinkReader {

void AlbumEntryButtons(int modifier, const ImageUrlInfo& image) {
	// Ported from @Composable fun AlbumEntryButtons(modifier, image):
	// 1. val launch = LocalLauncher.current
	// 2. Row(modifier = modifier, horizontalArrangement = Arrangement.Absolute.Right) {
	//        RRIconButton(onClick={launch(Dest.SaveMedia(image.url))}, icon=R.drawable.download, contentDescription=R.string.action_save_image)
	//        RRDropdownMenuIconButton(icon=R.drawable.ic_action_share_dark, contentDescription=R.string.action_share) {
	//            Item(icon=R.drawable.ic_action_image_dark, text=R.string.action_share_image, onClick={launch(Dest.ShareMedia(image.url))})
	//            Item(icon=R.drawable.ic_action_link_dark, text=R.string.action_share_link, onClick={launch(Dest.ShareLink(image.url))})
	//        }
	//        RRIconButton(onClick={launch(Dest.LinkLongClick(image.url))}, icon=R.drawable.dots_vertical_dark, contentDescription=R.string.three_dots_menu)
	//        Spacer(Modifier.width(4.dp))
	//   }
}

} // namespace PinkReader
