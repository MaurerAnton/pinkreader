// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRErrorView.kt
#include "rr_error_view.h"

// Dependencies (placeholder):
// - LocalComposeTheme (error theme: title, message, border, background, primaryButton, secondaryButton)
// - LocalLauncher, Dest (ResultDialog, ErrorPropertiesDialog, RedditTerms, AccountsList)
// - GlobalNetworkRetry
// - RRButton
// - StyledText
// - RRError (title, message, resolution, Resolution.RETRY, ACCEPT_REDDIT_TERMS, ACCOUNTS_LIST, buttonText)
// - ConstraintLayout (createRefs, constrainAs, Dimension)
// - Icon (alert_circle_outline)
// - Modifier.padding, .fillMaxWidth, .onSizeChanged, .border, .clip, .background, .invokeIf, .clickable, .fillMaxHeight
// - IntSize
// - Row, Column, Spacer, Box

namespace PinkReader {

void RRErrorView(const RRError& error) {
	// Ported from @Composable fun RRErrorView(error):
	// val theme = LocalComposeTheme.current.error
	// val launch = LocalLauncher.current
	// var size by remember { mutableStateOf(IntSize.Zero) }
	//
	// Box(modifier.padding(8.dp).fillMaxWidth().onSizeChanged{size=it}, contentAlignment=Center) {
	//     val smallWidth = with(LocalDensity.current) { size.width.toDp() < 150.dp }
	//
	//     Box(fillMaxWidth().border(1.dp, theme.border, RoundedCornerShape(6.dp)).clip(RoundedCornerShape(6.dp))
	//         .background(theme.background).invokeIf(smallWidth){fillMaxHeight().clickable(onClick={launch(Dest.ResultDialog(error))})}.padding(12.dp), contentAlignment=Center) {
	//
	//         if(smallWidth) {
	//             Icon(painter=painterResource(R.drawable.alert_circle_outline), contentDescription=stringResource(R.string.error_title), tint=theme.border)
	//         } else {
	//             ConstraintLayout(fillMaxWidth) {
	//                 val (icon, text) = createRefs()
	//                 Icon(painter=painterResource(R.drawable.alert_circle_outline), contentDescription=null,
	//                     modifier.constrainAs(icon){start.linkTo(parent.start); top.linkTo(parent.top)}, tint=theme.border)
	//                 Column(constrainAs(text){start.linkTo(icon.end, 16.dp); end.linkTo(parent.end); top.linkTo(parent.top); bottom.linkTo(parent.bottom); width=Dimension.fillToConstraints}) {
	//                     theme.title.StyledText(error.title ?: stringResource(R.string.error_title))
	//                     error.message?.let { Spacer(3.dp); theme.message.StyledText(it) }
	//                     Spacer(14.dp)
	//                     Row(fillMaxWidth, horizontalArrangement=Arrangement.End) {
	//                         RRButton(onClick={launch(Dest.ErrorPropertiesDialog(error))}, text=stringResource(R.string.button_error_details),
	//                             theme=if(error.resolution==null) theme.primaryButton else theme.secondaryButton)
	//                         if(error.resolution != null) {
	//                             Spacer(8.dp)
	//                             RRButton(onClick={
	//                                 when(error.resolution) {
	//                                     RETRY -> GlobalNetworkRetry.intValue++
	//                                     ACCEPT_REDDIT_TERMS -> launch(Dest.RedditTerms)
	//                                     ACCOUNTS_LIST -> launch(Dest.AccountsList)
	//                                 }
	//                             }, text=stringResource(error.resolution.buttonText), theme=theme.primaryButton)
	//                         }
	//                     }
	//                 }
	//             }
	//         }
	//     }
	// }
}

void PreviewRRErrorView() {
	// Ported from @Composable @Preview private fun PreviewRRErrorView():
	// RRComposeContextTest { RRErrorView(error=RRError(title="Error title", message="Error message")) }
}

void PreviewRRErrorViewSmall() {
	// Ported from @Composable @Preview(widthDp=64, heightDp=64) private fun PreviewRRErrorViewSmall():
	// RRComposeContextTest { RRErrorView(error=RRError(title="Error title", message="Error message")) }
}

} // namespace PinkReader
