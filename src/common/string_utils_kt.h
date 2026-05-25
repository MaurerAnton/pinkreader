// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/StringUtilsKt.kt
#pragma once

#include <string>

namespace PinkReader {

/**
 * Kotlin-style extension functions for std::string.
 * Equivalent to: org.quantumbadger.redreader.common.StringUtilsKt
 *
 * In Kotlin these are extension functions on String:
 *   fun String.asciiLowercase() = StringUtils.asciiLowercase(this)
 *   fun String.asciiUppercase() = StringUtils.asciiUppercase(this)
 */

/** Equivalent to Kotlin fun String.asciiLowercase() */
std::string asciiLowercase(const std::string& str);

/** Equivalent to Kotlin fun String.asciiUppercase() */
std::string asciiUppercase(const std::string& str);

} // namespace PinkReader
