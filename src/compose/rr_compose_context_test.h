// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ctx/RRComposeContextTest.kt
#pragma once

#include <functional>
#include <memory>

namespace PinkRenderer {
class ComposePrefs;
template <typename T> class Preference;
} // namespace PinkRenderer

namespace PinkReader {

// Ported from: @Composable fun RRComposeContextTest(content: @Composable () -> Unit)
void RRComposeContextTest(std::function<void()> content);

// Ported from: private fun <T> testPref(value: T)
// Moved to rr_compose_context.h

} // namespace PinkReader
