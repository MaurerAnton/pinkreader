// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/NeverAlwaysOrWifiOnly.java
#pragma once

namespace PinkReader {

/**
 * Enum controlling download behavior based on WiFi connectivity.
 * Equivalent to: org.quantumbadger.redreader.common.NeverAlwaysOrWifiOnly
 */
enum class NeverAlwaysOrWifiOnly {
	NEVER,
	WIFIONLY,
	ALWAYS
};

/**
 * Checks whether downloading is enabled given the current WiFi state.
 * Equivalent to the abstract method isEnabled(boolean wifiActive) on each enum value.
 */
inline bool isEnabled(NeverAlwaysOrWifiOnly mode, bool wifiActive) {
	switch (mode) {
		case NeverAlwaysOrWifiOnly::NEVER:
			return false;
		case NeverAlwaysOrWifiOnly::WIFIONLY:
			return wifiActive;
		case NeverAlwaysOrWifiOnly::ALWAYS:
			return true;
	}
	return false; // unreachable
}

} // namespace PinkReader
