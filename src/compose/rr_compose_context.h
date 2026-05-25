// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ctx/RRComposeContext.kt
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace PinkReader {

// Forward declarations
class ComposeBaseActivity;
class ComposePrefs;
struct AlbumInfo;
struct RRError;
class UriString;
template <typename T> class Preference;

// Ported from: val GlobalNetworkRetry = mutableIntStateOf(0)
// Exposed as a global mutable integer state
struct GlobalNetworkRetryState {
	int intValue = 0;
};
extern GlobalNetworkRetryState GlobalNetworkRetry;

// Ported from: sealed interface Dest
struct Dest {
	// data object Settings
	struct Settings {};
	// data class Link
	struct Link {
		UriString url;
		std::optional<AlbumInfo> albumInfo;
		std::optional<int> albumImageIndex;
	};
	// data class LinkLongClick
	struct LinkLongClick {
		UriString url;
	};
	// data class ResultDialog
	struct ResultDialog {
		RRError error;
	};
	// data class ErrorPropertiesDialog
	struct ErrorPropertiesDialog {
		RRError error;
	};
	// data class SaveMedia
	struct SaveMedia {
		UriString url;
	};
	// data class ShareMedia
	struct ShareMedia {
		UriString url;
	};
	// data class ShareLink
	struct ShareLink {
		UriString url;
	};
	// data object RedditTerms
	struct RedditTerms {};
	// data object AccountsList
	struct AccountsList {};

	using Variant = std::variant<
		Settings,
		Link,
		LinkLongClick,
		ResultDialog,
		ErrorPropertiesDialog,
		SaveMedia,
		ShareMedia,
		ShareLink,
		RedditTerms,
		AccountsList
	>;
};

// Ported from: val LocalRedditUser = staticCompositionLocalOf { RedditAccountId.ANON }
class RedditAccountId;

// Ported from: val LocalLauncher = staticCompositionLocalOf<(Dest) -> Unit>
// Launcher is a function that takes a Dest variant and performs an action
using LocalLauncherFunc = std::function<void(const Dest::Variant&)>;

// Ported from: @Composable fun RRComposeContext(activity: ComposeBaseActivity, content: @Composable () -> Unit)
// Declared as a function. The Composable behavior is platform-specific.
void RRComposeContext(ComposeBaseActivity& activity, std::function<void()> content);

// Ported from: private fun <T> testPref(value: T)
template <typename T>
std::unique_ptr<Preference<T>> testPref(T value);

} // namespace PinkReader
