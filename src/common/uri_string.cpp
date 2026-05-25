/*
 * PinkReader - GPLv3
 * File: uri_string.cpp
 * Port of: org.quantumbadger.redreader.common.UriString.kt (method bodies)
 *
 * Every field, method, companion object method from the original Kotlin file.
 */

#include "uri_string.h"

// Placeholder for PinkUri (from reddit/url/pink_uri.h)
// #include "../reddit/url/pink_uri.h"

namespace PinkReader {

// Port of: fun toUri() = Uri.parse(value)
PinkUri UriString::toUri() const {
	// PinkUri is not yet fully ported; return a default-constructed object
	// TODO: integrate with PinkUri::parse(m_value)
	return PinkUri();
}

// Port of: @JvmStatic fun from(value: Uri) = UriString(value.toString())
UriString UriString::from(const PinkUri &uri) {
	// TODO: integrate with PinkUri::toString()
	return UriString(QString());
}

} // namespace PinkReader
