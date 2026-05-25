// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/ui/RRErrorView.kt
#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace PinkReader {

// Forward declaration
struct RRError;

// Ported from: @Composable fun RRErrorView(error: RRError)
// Renders an error message with action buttons (Details, Retry, etc.)
void RRErrorView(const RRError& error);

// Ported from: @Composable @Preview private fun PreviewRRErrorView()
void PreviewRRErrorView();

// Ported from: @Composable @Preview(widthDp=64, heightDp=64) private fun PreviewRRErrorViewSmall()
void PreviewRRErrorViewSmall();

} // namespace PinkReader
