// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRLinkButton.kt
#include "rr_link_button.h"

// Dependencies (placeholder):
// - LocalLauncher, Dest (Link, LinkLongClick)
// - LocalComposeTheme (linkButton)
// - StyledText
// - combinedClickableWithHaptics
// - ConstraintLayout (createRefs, constrainAs, Dimension)
// - ComposeThemeLinkButton (titleStyle, subtitleStyle, shape, borderColor, borderThickness, iconColor)
// - Icon (ic_action_link_dark, chevron_right_dark)
// - Modifier.fillMaxWidth, .semantics, .border, .clip, .padding

namespace PinkReader {

void RRLinkButton(
	const std::string& title,
	const UriString& link,
	const ComposeThemeLinkButton& theme)
{
	// Ported from @Composable fun RRLinkButton(title, link, theme):
	// val launch = LocalLauncher.current
	// ConstraintLayout(
	//     modifier = Modifier
	//         .fillMaxWidth()
	//         .semantics(mergeDescendants = true) {}
	//         .border(theme.borderThickness, theme.borderColor, theme.shape)
	//         .clip(theme.shape)
	//         .combinedClickableWithHaptics(
	//             role = Role.Button,
	//             onClick = { launch(Dest.Link(link)) },
	//             onLongClick = { launch(Dest.LinkLongClick(link)) }
	//         )
	//         .padding(horizontal = 16.dp, vertical = 12.dp),
	// ) {
	//     val (linkIcon, textBox, arrowIcon) = createRefs()
	//     Icon(modifier.size(20.dp).constrainAs(linkIcon){top/bottom/start linkTo parent},
	//         painter=painterResource(R.drawable.ic_action_link_dark), contentDescription=null, tint=theme.iconColor)
	//     Column(constrainAs(textBox){top/bottom linkTo parent; start.linkTo(linkIcon.end,12.dp); end.linkTo(arrowIcon.start,12.dp); width=Dimension.fillToConstraints}) {
	//         theme.title.StyledText(title)
	//         Spacer(1.dp)
	//         theme.subtitle.StyledText(link.value)
	//     }
	//     Icon(modifier.size(20.dp).constrainAs(arrowIcon){top/bottom/end linkTo parent},
	//         painter=painterResource(R.drawable.chevron_right_dark), contentDescription=null, tint=theme.iconColor)
	// }
}

void PreviewRRLinkButton() {
	// Ported from @Composable @Preview private fun PreviewRRLinkButton():
	// RRComposeContextTest {
	//     val theme = LocalComposeTheme.current
	//     RRLinkButton(title="Test Button", link=UriString("https://redreader.org"), theme=theme.linkButton)
	// }
}

} // namespace PinkReader
