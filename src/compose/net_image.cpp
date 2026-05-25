// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/NetImage.kt
#include "net_image.h"

// Dependencies (placeholder):
// - fetchImage, NetRequestStatus
// - LocalComposeTheme
// - RRErrorView
// - ImageUrlInfo (url, size)
// - Image, Icon, CircularProgressIndicator
// - Modifier extensions: aspectRatio, background, fillMaxWidth, fillMaxSize, padding, matchParentSize, clip
// - CircleShape, ContentScale
// - invokeIf, invokeIfNotNull

namespace PinkReader {

void NetImage(
	int modifier,
	const ImageUrlInfo& image,
	std::optional<float> cropToAspect,
	bool showVideoPlayOverlay,
	int maxCanvasDimension)
{
	// Ported from @Composable fun NetImage(modifier, image, cropToAspect, showVideoPlayOverlay, maxCanvasDimension):
	// 1. val theme = LocalComposeTheme.current
	// 2. val expectedImageAspect = image.size?.takeIf{it.height>0}?.let{it.width.toFloat()/it.height.toFloat()}
	// 3. val url = image.url
	// 4. val data by fetchImage(url, scaleToMaxAxis = maxCanvasDimension)
	// 5. Box(modifier.invokeIfNotNull(cropToAspect, Modifier::aspectRatio).invokeIf(data is Success){ background(theme.postCard.previewImageBackgroundColor) }, contentAlignment=Center) {
	//        if(expectedImageAspect != null && data !is Success && data !is Failed) {
	//            Box(fillMaxWidth().aspectRatio(expectedImageAspect))  // Pad view to desired aspect ratio
	//        }
	//        when(val it = data) {
	//            Connecting -> CircularProgressIndicator(padding(24.dp))
	//            is Downloading -> CircularProgressIndicator(padding(24.dp), progress={it.fractionComplete})
	//            is Failed -> RRErrorView(error = it.error)
	//            is Success -> {
	//                Image(
	//                    modifier = if(cropToAspect == null) {
	//                        val bitmap = it.result.data
	//                        val imageAspect = if(bitmap.height>0) bitmap.width.toDouble()/bitmap.height.toDouble() else 0.0
	//                        fillMaxWidth().aspectRatio(imageAspect.toFloat(), matchHeightConstraintsFirst=true)
	//                    } else { fillMaxSize() },
	//                    bitmap = it.result.data,
	//                    contentDescription = null,
	//                    contentScale = if(cropToAspect==null) ContentScale.Fit else ContentScale.Crop
	//                )
	//                if(showVideoPlayOverlay) {
	//                    Box(background(Color(0f,0f,0f,0.2f)).matchParentSize(), contentAlignment=Center) {
	//                        Box(clip(CircleShape).background(Color(0f,0f,0f,0.7f)).padding(12.dp)) {
	//                            Icon(painter=painterResource(R.drawable.icon_play), contentDescription=null, tint=Color.White)
	//                        }
	//                    }
	//                }
	//            }
	//        }
	//   }
}

} // namespace PinkReader
