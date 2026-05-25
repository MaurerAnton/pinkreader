// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/AlbumListItem.kt
#include "album_list_item.h"

// Dependencies (placeholder):
// - LocalComposePrefs, LocalComposeTheme
// - LocalLauncher, Dest
// - NetImage
// - RRIconButton, RRLinkButton
// - StyledText, combinedClickableWithHaptics
// - ConstraintLayout (createRefs, constrainAs, Dimension)
// - AnimatedVisibility, slideInHorizontally, slideOutHorizontally
// - ImageInfo (bigSquare, preview, title, caption, type, original, outboundUrl, size, sizeBytes)
// - ImageSize, ImageUrlInfo

namespace PinkReader {

void AlbumListItem(
	int index,
	const ImageInfo& image,
	std::function<void(int)> onClick,
	std::function<void(int)> onLongClick
) {
	// Ported from @Composable fun AlbumListItem(index, image, onClick, onLongClick):
	// 1. val prefs = LocalComposePrefs.current; val theme = LocalComposeTheme.current
	// 2. val thumbnailImage = image.bigSquare ?: image.preview
	// 3. val thumbnailSize = prefs.albumListThumbnailSize.value.dp
	// 4. ConstraintLayout(fillMaxWidth, background(theme.postCard.backgroundColor), combinedClickableWithHaptics(onClick, onLongClick)) {
	//        val (thumbnail, text, buttons) = createRefs()
	//        // Thumbnail box
	//        Box(constrainAs(thumbnail){top/bottom/start linkTo parent, height=Dimension.fillToConstraints.atLeast(thumbnailSize)}, background, contentAlignment=Center) {
	//            if(prefs.albumListShowThumbnails.value && thumbnailImage != null)
	//                NetImage(modifier=Modifier.width(thumbnailSize), image=thumbnailImage, cropToAspect=1f)
	//        }
	//        // Text column
	//        val title = remember(image) { image.title?.trim()?.takeUnless{it.isEmpty()} }
	//        val caption = remember(image) { image.caption?.trim()?.takeUnless{it.isEmpty()} ?: listOfNotNull(
	//            image.type?.run { when{ "image/png"->"PNG"; "image/gif"->"GIF"; "image/jpg"/"image/jpeg"->"JPEG"; else->this } },
	//            image.original.size?.run{"${width}x$height"},
	//            image.original.sizeBytes?.let { if(it<512*1024) String.format("%.1f kB", it/1024f) else String.format("%.1f MB", it/(1024f*1024f)) }
	//        ).joinToString(", ").takeUnless{it.isEmpty()} }
	//        Column(padding(vertical=8.dp, horizontal=12.dp), constrainAs(text){...}) {
	//            theme.postCard.title.StyledText(title ?: stringResource(R.string.album_image_default_text, index+1))
	//            if(caption != null) { Spacer(4.dp); theme.postCard.subtitle.StyledText(caption) }
	//            if(image.outboundUrl != null) { Spacer(12.dp); RRLinkButton(title, link, theme.linkButton) }
	//        }
	//        // Buttons box
	//        Box(constrainAs(buttons){...}) {
	//            AnimatedVisibility(visible=prefs.albumListShowButtons.value, enter=slideInHorizontally, exit=slideOutHorizontally) {
	//                val launch = LocalLauncher.current
	//                RRIconButton(onClick={launch(Dest.LinkLongClick(image.original.url))}, icon=R.drawable.dots_vertical_dark, contentDescription=R.string.three_dots_menu)
	//            }
	//        }
	//   }
}

void PreviewAlbumListItem() {
	// Ported from @Composable @Preview fun PreviewAlbumListItem():
	// RRComposeContextTest {
	//     AlbumListItem(2, ImageInfo(
	//         original = ImageUrlInfo(UriString("testimage"), size = ImageSize(100, 100)),
	//         title = "Test title which is very long",
	//         caption = null,
	//         hasAudio = ImageInfo.HasAudio.NO_AUDIO,
	//         outboundUrl = UriString("https://redreader.org")
	//     ), {}, {})
	// }
}

} // namespace PinkReader
