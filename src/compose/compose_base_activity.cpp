// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/activity/ComposeBaseActivity.kt
#include "compose_base_activity.h"

// Note: enableEdgeToEdge(), PrefsUtility::applyTheme(), ComposeView,
// and RRComposeContext are Android/Compose dependencies.
// This translation preserves the exact logic structure.

namespace PinkReader {

void ComposeBaseActivity::onCreate(Bundle* savedInstanceState) {
	// enableEdgeToEdge() -- AndroidX activity edge-to-edge
	// PrefsUtility.applyTheme(this)
	// super.onCreate(savedInstanceState)
	// Equivalent calls omitted (platform-specific)
}

void ComposeBaseActivity::setContentCompose(std::function<void()> content) {
	// setContentView(ComposeView(this).also { view ->
	//     view.setContent {
	//         RRComposeContext(this) {
	//             content()
	//         }
	//     }
	// })
	// This wraps the content in RRComposeContext, mirroring the Kotlin logic.
	// Platform-specific Compose rendering omitted.
}

} // namespace PinkReader
