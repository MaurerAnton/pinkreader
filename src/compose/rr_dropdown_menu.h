// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRDropdownMenu.kt
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace PinkReader {

// Forward declarations
template <typename T> class Preference;

// Ported from: @Stable class RRDropdownMenuState
class RRDropdownMenuState {
public:
	bool expanded = false;
};

// Ported from: @Composable fun rememberRRDropdownMenuState()
std::unique_ptr<RRDropdownMenuState> rememberRRDropdownMenuState();

// Forward declare RRDropdownMenuScope
class RRDropdownMenuScope;

// Ported from: class RRRadioScope<T>
template <typename T>
class RRRadioScope {
public:
	RRRadioScope(Preference<T>& pref, bool dismissOnClick);
	~RRRadioScope() = default;

	// Ported from: @Composable fun RRDropdownMenuScope.Option(value, text, icon)
	void Option(RRDropdownMenuScope& scope, const T& value, int text, int icon = 0);

private:
	Preference<T>& pref_;
	bool dismissOnClick_;
};

// Ported from: class RRDropdownMenuScope(menuState)
class RRDropdownMenuScope {
public:
	explicit RRDropdownMenuScope(RRDropdownMenuState& menuState);

	// Ported from: @Composable fun ItemGroupCollapsible(text, content)
	void ItemGroupCollapsible(int text, std::function<void(RRDropdownMenuScope&)> content);

	// Ported from: @Composable fun Item(text, icon, radioButtonWithValue, checkboxWithValue, onClick, dismissOnClick)
	void Item(
		int text,
		int icon,                                    // @DrawableRes int, -1 for null
		std::optional<bool> radioButtonWithValue,
		std::optional<bool> checkboxWithValue,
		std::function<void()> onClick,
		bool dismissOnClick = true
	);

	// Ported from: @Composable fun ItemPrefBool(text, icon, pref, dismissOnClick)
	void ItemPrefBool(
		int text,                                    // @StringRes
		int icon,                                    // @DrawableRes, -1 for null
		Preference<bool>& pref,
		bool dismissOnClick = false
	);

	// Ported from: @Composable fun <T> ItemPrefRadio(pref, dismissOnClick, items)
	template <typename T>
	void ItemPrefRadio(
		Preference<T>& pref,
		bool dismissOnClick,
		std::function<void(RRRadioScope<T>&, RRDropdownMenuScope&)> items
	);

	// Ported from: @Composable fun ItemPrefIntSlider(text, pref, min, max, continuous)
	void ItemPrefIntSlider(
		int text,                                    // @StringRes
		Preference<int>& pref,
		int min,
		int max,
		bool continuous
	);

	// Ported from: @Composable fun ItemDivider()
	void ItemDivider();

private:
	RRDropdownMenuState& menuState_;
};

// Ported from: @Composable fun RRDropdownMenuIconButton(modifier, icon, contentDescription, content)
void RRDropdownMenuIconButton(
	int modifier,                                  // Modifier placeholder
	int icon,                                      // @DrawableRes
	int contentDescription,                        // @StringRes
	std::function<void(RRDropdownMenuScope&)> content
);

// Ported from: @Composable fun RRDropdownMenu(state, content)
void RRDropdownMenu(
	RRDropdownMenuState& state,
	std::function<void(RRDropdownMenuScope&)> content
);

// Ported from: @Composable private fun RRDropdownMenuItem(text, icon, onClick, radioButtonWithValue, checkboxWithValue)
void RRDropdownMenuItem(
	int text,                                      // @StringRes
	int icon,                                      // @DrawableRes, -1 for null
	std::function<void()> onClick,
	std::optional<bool> radioButtonWithValue,
	std::optional<bool> checkboxWithValue
);

// Ported from: @Composable private fun BaseDropdownMenu(expanded, onDismissRequest, modifier, offset, scrollState, properties, content)
void BaseDropdownMenu(
	bool expanded,
	std::function<void()> onDismissRequest,
	int modifier = 0,                              // Modifier placeholder
	float offsetX = 0.0f,                          // DpOffset.x
	float offsetY = 0.0f,                          // DpOffset.y
	int scrollState = 0,                           // ScrollState placeholder
	bool focusable = true,                        // PopupProperties.focusable
	std::function<void()> content = nullptr
);

// Ported from: @Composable private fun BaseDropdownMenuContent(expandedStates, transformOriginState, scrollState, modifier, content)
void BaseDropdownMenuContent(
	bool& expandedStateTarget,                    // MutableTransitionState<Boolean>
	float& transformOriginX,                      // TransformOrigin.pivotFractionX
	float& transformOriginY,                      // TransformOrigin.pivotFractionY
	int scrollState,                               // ScrollState
	int modifier,                                  // Modifier
	std::function<void()> content
);

// Ported from: @Immutable internal data class DropdownMenuPositionProvider
struct DropdownMenuPositionProvider {
	float contentOffsetX = 0.0f;                  // DpOffset
	float contentOffsetY = 0.0f;
	float density = 1.0f;                         // Density (pixels per dp)
	std::function<void(int, int, int, int, int, int, int, int)> onPositionCalculated;

	// calculatePosition returns IntOffset (x, y)
	void calculatePosition(
		int anchorLeft, int anchorTop, int anchorRight, int anchorBottom,
		int windowWidth, int windowHeight,
		int layoutDirection,                        // 0 = Ltr, 1 = Rtl
		int popupWidth, int popupHeight,
		int& outX, int& outY
	);
};

// Ported from: private fun calculateTransformOrigin(parentBounds, menuBounds): TransformOrigin
void calculateTransformOrigin(
	int parentLeft, int parentTop, int parentRight, int parentBottom,
	int menuLeft, int menuTop, int menuRight, int menuBottom,
	float& outPivotX, float& outPivotY
);

} // namespace PinkReader
