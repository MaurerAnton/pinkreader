// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRDropdownMenu.kt
#include "rr_dropdown_menu.h"

#include <algorithm>
#include <cmath>

// Dependencies (placeholder):
// - LocalComposeTheme (dropdownMenu: text, background)
// - StyledText
// - RRIconButton
// - Preference<T>
// - DropdownMenuItem, RadioButton, Checkbox, Slider, HorizontalDivider
// - Popup, PopupPositionProvider, PopupProperties
// - MutableTransitionState, updateTransition, animateFloat, tween, LinearOutSlowInEasing
// - graphicsLayer (scaleX, scaleY, alpha, transformOrigin)
// - Box, Column, Row, Spacer, IntrinsicSize, Modifier extensions
// - RoundedCornerShape, shadow, clip

namespace PinkReader {

// --- RRDropdownMenuState ---

std::unique_ptr<RRDropdownMenuState> rememberRRDropdownMenuState() {
	// Ported from: @Composable fun rememberRRDropdownMenuState() = remember { RRDropdownMenuState() }
	return std::make_unique<RRDropdownMenuState>();
}

// --- RRRadioScope<T> ---

template <typename T>
RRRadioScope<T>::RRRadioScope(Preference<T>& pref, bool dismissOnClick)
	: pref_(pref), dismissOnClick_(dismissOnClick) {}

template <typename T>
void RRRadioScope<T>::Option(RRDropdownMenuScope& scope, const T& value, int text, int icon) {
	// Ported from: @Composable fun RRDropdownMenuScope.Option(value, text, icon):
	// Item(text = text, icon = icon, dismissOnClick = dismissOnClick,
	//      onClick = { pref.value = value }, radioButtonWithValue = pref.value == value)
	scope.Item(
		text,
		icon,
		/*radioButtonWithValue=*/ pref_.getValue() == value,
		std::nullopt,
		[this, &value]() { pref_.setValue(value); },
		dismissOnClick_
	);
}

// --- RRDropdownMenuScope ---

RRDropdownMenuScope::RRDropdownMenuScope(RRDropdownMenuState& menuState)
	: menuState_(menuState) {}

void RRDropdownMenuScope::ItemGroupCollapsible(
	int text,
	std::function<void(RRDropdownMenuScope&)> content)
{
	// Ported from: @Composable fun ItemGroupCollapsible(text, content):
	// var expanded by remember { mutableStateOf(false) }
	// Item(text = text, icon = if(expanded) chevron_down else chevron_right_dark,
	//      onClick = { expanded = !expanded }, dismissOnClick = false)
	// if(expanded) { content() }
	bool expanded = false;  // Per-instance state; simplified for port
	Item(
		text,
		expanded ? 1 : 2,  // chevron_down : chevron_right_dark placeholder
		std::nullopt,
		std::nullopt,
		[expanded]() mutable { expanded = !expanded; },
		false
	);
	if (expanded) {
		content(*this);
	}
}

void RRDropdownMenuScope::Item(
	int text,
	int icon,
	std::optional<bool> radioButtonWithValue,
	std::optional<bool> checkboxWithValue,
	std::function<void()> onClick,
	bool dismissOnClick)
{
	// Ported from: @Composable fun Item(text, icon, radioButtonWithValue, checkboxWithValue, onClick, dismissOnClick):
	// RRDropdownMenuItem(
	//     text = text,
	//     icon = icon,
	//     onClick = if(dismissOnClick) { { menuState.expanded = false; onClick() } } else onClick,
	//     radioButtonWithValue = radioButtonWithValue,
	//     checkboxWithValue = checkboxWithValue
	// )
	auto wrappedClick = onClick;
	if (dismissOnClick) {
		wrappedClick = [this, onClick]() {
			menuState_.expanded = false;
			onClick();
		};
	}
	RRDropdownMenuItem(text, icon, wrappedClick, radioButtonWithValue, checkboxWithValue);
}

void RRDropdownMenuScope::ItemPrefBool(
	int text, int icon, Preference<bool>& pref, bool dismissOnClick)
{
	// Ported from: @Composable fun ItemPrefBool(text, icon, pref, dismissOnClick):
	// Item(text = text, icon = icon, checkboxWithValue = pref.value,
	//      onClick = { pref.value = !pref.value }, dismissOnClick = dismissOnClick)
	Item(
		text,
		icon,
		std::nullopt,
		pref.getValue(),
		[&pref]() { pref.setValue(!pref.getValue()); },
		dismissOnClick
	);
}

template <typename T>
void RRDropdownMenuScope::ItemPrefRadio(
	Preference<T>& pref,
	bool dismissOnClick,
	std::function<void(RRRadioScope<T>&, RRDropdownMenuScope&)> items)
{
	// Ported from: @Composable fun <T> ItemPrefRadio(pref, dismissOnClick, items):
	// items(RRRadioScope(pref, dismissOnClick))
	RRRadioScope<T> scope(pref, dismissOnClick);
	items(scope, *this);
}

void RRDropdownMenuScope::ItemPrefIntSlider(
	int text, Preference<int>& pref, int min, int max, bool continuous)
{
	// Ported from: @Composable fun ItemPrefIntSlider(text, pref, min, max, continuous):
	// val theme = LocalComposeTheme.current
	// Column(fillMaxWidth.padding(top=12.dp, start=18.dp, end=18.dp).sizeIn(minWidth=112.dp, maxWidth=280.dp, minHeight=48.dp)) {
	//     theme.dropdownMenu.text.StyledText(stringResource(text))
	//     Slider(modifier.fillMaxWidth, value=pref.value.toFloat(), onValueChange={pref.value=it.roundToInt()},
	//           valueRange=min.toFloat()..max.toFloat(), steps=((max-min)-1).takeUnless{continuous}?:0)
	// }
}

void RRDropdownMenuScope::ItemDivider() {
	// Ported from: @Composable fun ItemDivider():
	// HorizontalDivider()
}

// --- RRDropdownMenuIconButton ---

void RRDropdownMenuIconButton(
	int modifier, int icon, int contentDescription,
	std::function<void(RRDropdownMenuScope&)> content)
{
	// Ported from: @Composable fun RRDropdownMenuIconButton(modifier, icon, contentDescription, content):
	// Box(modifier = modifier) {
	//     val state = rememberRRDropdownMenuState()
	//     RRIconButton(onClick = { state.expanded = true }, icon = icon, contentDescription = contentDescription)
	//     RRDropdownMenu(state = state, content = content)
	// }
	auto state = rememberRRDropdownMenuState();
	// RRIconButton(...) call omitted (platform-specific)
	RRDropdownMenu(*state, content);
}

// --- RRDropdownMenu ---

void RRDropdownMenu(
	RRDropdownMenuState& state,
	std::function<void(RRDropdownMenuScope&)> content)
{
	// Ported from: @Composable fun RRDropdownMenu(state, content):
	// BaseDropdownMenu(
	//     expanded = state.expanded,
	//     onDismissRequest = { state.expanded = false },
	//     content = { content(RRDropdownMenuScope(state)) },
	// )
	BaseDropdownMenu(
		state.expanded,
		[&state]() { state.expanded = false; },
		0, 0.0f, 0.0f, 0, true,
		[&state, &content]() {
			RRDropdownMenuScope scope(state);
			content(scope);
		}
	);
}

// --- RRDropdownMenuItem ---

void RRDropdownMenuItem(
	int text, int icon,
	std::function<void()> onClick,
	std::optional<bool> radioButtonWithValue,
	std::optional<bool> checkboxWithValue)
{
	// Ported from @Composable private fun RRDropdownMenuItem(text, icon, onClick, radioButtonWithValue, checkboxWithValue):
	// val theme = LocalComposeTheme.current
	// DropdownMenuItem(
	//     modifier = Modifier.semantics(mergeDescendants = true) {},
	//     onClick = onClick,
	//     text = {
	//         Row(verticalAlignment = Alignment.CenterVertically) {
	//             Spacer(6.dp)
	//             if(icon != null) { Icon(painterResource(id=icon), contentDescription=null); Spacer(12.dp) }
	//             theme.dropdownMenu.text.StyledText(stringResource(text))
	//         }
	//     },
	//     trailingIcon = {
	//         if(radioButtonWithValue != null) {
	//             val description = if(radioButtonWithValue) stringResource(R.string.radio_button_selected) else stringResource(R.string.radio_button_not_selected)
	//             RadioButton(modifier.clearAndSetSemantics{contentDescription=description}.focusable(false), selected=radioButtonWithValue, onClick=onClick)
	//         }
	//         if(checkboxWithValue != null) {
	//             val description = if(checkboxWithValue) stringResource(R.string.checkbox_checked) else stringResource(R.string.checkbox_not_checked)
	//             Checkbox(modifier.clearAndSetSemantics{contentDescription=description}.focusable(false), checked=checkboxWithValue, onCheckedChange={onClick()})
	//         }
	//     }
	// )
}

// --- BaseDropdownMenu ---

void BaseDropdownMenu(
	bool expanded,
	std::function<void()> onDismissRequest,
	int modifier,
	float offsetX, float offsetY,
	int scrollState,
	bool focusable,
	std::function<void()> content)
{
	// Ported from @Composable private fun BaseDropdownMenu(expanded, onDismissRequest, ...):
	// val expandedStates = remember { MutableTransitionState(false) }
	// expandedStates.targetState = expanded
	// if(expandedStates.currentState || expandedStates.targetState) {
	//     val transformOriginState = remember { mutableStateOf(TransformOrigin.Center) }
	//     val density = LocalDensity.current
	//     val popupPositionProvider = DropdownMenuPositionProvider(offset, density) { parentBounds, menuBounds ->
	//         transformOriginState.value = calculateTransformOrigin(parentBounds, menuBounds)
	//     }
	//     Popup(onDismissRequest, popupPositionProvider, properties) {
	//         BaseDropdownMenuContent(expandedStates, transformOriginState, scrollState, modifier, content)
	//     }
	// }
	if (expanded && content) {
		content();
	}
}

// --- BaseDropdownMenuContent ---

void BaseDropdownMenuContent(
	bool& expandedStateTarget,
	float& transformOriginX,
	float& transformOriginY,
	int scrollState,
	int modifier,
	std::function<void()> content)
{
	// Ported from @Composable private fun BaseDropdownMenuContent(...):
	// val theme = LocalComposeTheme.current
	// val transition = updateTransition(expandedStates, "DropDownMenu")
	// val scale by transition.animateFloat(...) { if(it) 1f else 0.8f }
	// val alpha by transition.animateFloat(...) { if(it) 1f else 0f }
	// Box(graphicsLayer{scaleX=scale; scaleY=scale; alpha=alpha; transformOrigin=transformOriginState.value}
	//     .shadow(3.dp, RoundedCornerShape(6.dp)).clip(RoundedCornerShape(6.dp)).background(theme.dropdownMenu.background)) {
	//     Column(modifier.padding(vertical=0.dp).width(IntrinsicSize.Max).verticalScroll(scrollState), content = content)
	// }
	if (content) {
		content();
	}
}

// --- DropdownMenuPositionProvider ---

void DropdownMenuPositionProvider::calculatePosition(
	int anchorLeft, int anchorTop, int anchorRight, int anchorBottom,
	int windowWidth, int windowHeight,
	int layoutDirection,   // 0 = Ltr, 1 = Rtl
	int popupWidth, int popupHeight,
	int& outX, int& outY)
{
	// Ported from: override fun calculatePosition(anchorBounds, windowSize, layoutDirection, popupContentSize): IntOffset
	int verticalMargin = static_cast<int>(48.0f * density);   // 48.dp.roundToPx()
	int contentOffsetX_px = static_cast<int>(contentOffsetX * density * (layoutDirection == 0 ? 1 : -1));
	int contentOffsetY_px = static_cast<int>(contentOffsetY * density);

	// Compute horizontal position
	int leftToAnchorLeft = anchorLeft + contentOffsetX_px;
	int rightToAnchorRight = anchorRight - popupWidth + contentOffsetX_px;
	int rightToWindowRight = windowWidth - popupWidth;
	int leftToWindowLeft = 0;

	int x;
	if (layoutDirection == 0) {  // Ltr
		// sequenceOf(leftToAnchorLeft, rightToAnchorRight, if(anchorLeft>=0) rightToWindowRight else leftToWindowLeft)
		auto candidates = {
			leftToAnchorLeft,
			rightToAnchorRight,
			(anchorLeft >= 0) ? rightToWindowRight : leftToWindowLeft
		};
		x = rightToAnchorRight;  // default
		for (int c : candidates) {
			if (c >= 0 && c + popupWidth <= windowWidth) { x = c; break; }
		}
	} else {  // Rtl
		auto candidates = {
			rightToAnchorRight,
			leftToAnchorLeft,
			(anchorRight <= windowWidth) ? leftToWindowLeft : rightToWindowRight
		};
		x = rightToAnchorRight;
		for (int c : candidates) {
			if (c >= 0 && c + popupWidth <= windowWidth) { x = c; break; }
		}
	}

	// Compute vertical position
	int topToAnchorBottom = std::max(anchorBottom + contentOffsetY_px, verticalMargin);
	int bottomToAnchorTop = anchorTop - popupHeight + contentOffsetY_px;
	int centerToAnchorTop = anchorTop - popupHeight / 2 + contentOffsetY_px;
	int bottomToWindowBottom = windowHeight - popupHeight - verticalMargin;

	int y = bottomToAnchorTop;  // default
	for (int c : {topToAnchorBottom, bottomToAnchorTop, centerToAnchorTop, bottomToWindowBottom}) {
		if (c >= verticalMargin && c + popupHeight <= windowHeight - verticalMargin) {
			y = c;
			break;
		}
	}

	outX = x;
	outY = y;

	// onPositionCalculated callback
	if (onPositionCalculated) {
		onPositionCalculated(anchorLeft, anchorTop, anchorRight, anchorBottom,
		                     x, y, x + popupWidth, y + popupHeight);
	}
}

// --- calculateTransformOrigin ---

void calculateTransformOrigin(
	int parentLeft, int parentTop, int parentRight, int parentBottom,
	int menuLeft, int menuTop, int menuRight, int menuBottom,
	float& outPivotX, float& outPivotY)
{
	// Ported from private fun calculateTransformOrigin(parentBounds, menuBounds): TransformOrigin
	// pivotX computation
	if (menuLeft >= parentRight) {
		outPivotX = 0.0f;
	} else if (menuRight <= parentLeft) {
		outPivotX = 1.0f;
	} else if (menuRight - menuLeft == 0) {
		outPivotX = 0.0f;
	} else {
		int intersectionCenter = (std::max(parentLeft, menuLeft) + std::min(parentRight, menuRight)) / 2;
		outPivotX = static_cast<float>(intersectionCenter - menuLeft) / static_cast<float>(menuRight - menuLeft);
	}

	// pivotY computation
	if (menuTop >= parentBottom) {
		outPivotY = 0.0f;
	} else if (menuBottom <= parentTop) {
		outPivotY = 1.0f;
	} else if (menuBottom - menuTop == 0) {
		outPivotY = 0.0f;
	} else {
		int intersectionCenter = (std::max(parentTop, menuTop) + std::min(parentBottom, menuBottom)) / 2;
		outPivotY = static_cast<float>(intersectionCenter - menuTop) / static_cast<float>(menuBottom - menuTop);
	}
}

// --- Explicit template instantiations for common types ---
template class RRRadioScope<int>;
template class RRRadioScope<bool>;
template class RRRadioScope<std::string>;

template void RRDropdownMenuScope::ItemPrefRadio<int>(
	Preference<int>&, bool, std::function<void(RRRadioScope<int>&, RRDropdownMenuScope&)>);
template void RRDropdownMenuScope::ItemPrefRadio<bool>(
	Preference<bool>&, bool, std::function<void(RRRadioScope<bool>&, RRDropdownMenuScope&)>);
template void RRDropdownMenuScope::ItemPrefRadio<std::string>(
	Preference<std::string>&, bool, std::function<void(RRRadioScope<std::string>&, RRDropdownMenuScope&)>);

} // namespace PinkReader
