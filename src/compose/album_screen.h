// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/AlbumScreen.kt
#pragma once

#include <functional>
#include <optional>
#include <string>

namespace PinkReader {

// Forward declarations
class UriString;
struct AlbumInfo;

// Ported from: @Stable interface TopBarScrollObservable
struct TopBarScrollObservable {
	virtual ~TopBarScrollObservable() = default;
	virtual int firstVisibleItemIndex() const = 0;
	virtual int firstVisibleItemOffset() const = 0;
};

// Ported from: class OverridePaddingValues(parent: PaddingValues, ...)
// PaddingValues equivalent
struct PaddingValues {
	std::optional<float> top;
	std::optional<float> bottom;
	std::optional<float> left;
	std::optional<float> right;

	float calculateBottomPadding(float parentBottom) const;
	float calculateLeftPadding(int layoutDirection, float parentLeft) const;
	float calculateRightPadding(int layoutDirection, float parentRight) const;
	float calculateTopPadding(float parentTop) const;
};

// Ported from: fun PaddingValues.override(top, bottom, left, right)
PaddingValues overridePaddingValues(
	const PaddingValues& parent,
	std::optional<float> top,
	std::optional<float> bottom,
	std::optional<float> left,
	std::optional<float> right
);

// Ported from: @Composable fun AlbumScreen(onBackPressed: () -> Unit, albumUrl: UriString)
// Top-level entry: fetches album data, shows loading/error/content.
void AlbumScreen_Url(
	std::function<void()> onBackPressed,
	const UriString& albumUrl
);

// Ported from: @Composable fun AlbumScreen(onBackPressed: () -> Unit, album: AlbumInfo)
// Renders the album content once data is loaded.
void AlbumScreen_Data(
	std::function<void()> onBackPressed,
	const AlbumInfo& album
);

// Ported from: @Composable fun AlbumSettingsButton(modifier)
void AlbumSettingsButton(
	int modifier = 0   // Modifier placeholder
);

// Ported from: private fun InitialContainer(modifier, content)
void InitialContainer(
	int modifier,
	std::function<void()> content
);

// Ported from: private fun DoOnce(input: AlbumInfo, action)
void DoOnce(
	const AlbumInfo& input,
	std::function<void()> action
);

} // namespace PinkReader
