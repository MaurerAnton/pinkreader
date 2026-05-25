// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRIconButton.kt
#pragma once

#include <cstdint>
#include <functional>

namespace PinkReader {

// Ported from: @Composable fun RRIconButton(onClick, icon, contentDescription, modifier, tint)
// A themed icon button. icon and contentDescription use @DrawableRes/@StringRes which are ints in Android.
void RRIconButton(
	std::function<void()> onClick,
	int icon,                                 // @DrawableRes int
	int contentDescription,                   // @StringRes int
	int modifier = 0,                         // Modifier placeholder
	uint32_t tint = 0                         // Color (ARGB), 0 = use theme default
);

} // namespace PinkReader
