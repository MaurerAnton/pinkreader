/*
 * PinkReader - GPLv3
 * File: tor_common.h
 * Port of: org.quantumbadger.redreader.common.TorCommon.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include <atomic>

namespace PinkReader {

// Forward declarations for dependencies
class General;
class PrefsUtility;
class HTTPBackend;
class CacheDownload;

/**
 * @brief Port of org.quantumbadger.redreader.common.TorCommon
 *
 * Original: public class TorCommon with static methods.
 * Manages Tor-enabled status and triggers HTTP backend recreation on change.
 */
class TorCommon {
public:
	TorCommon() = delete; // Static-only class

	// Port of: public static void updateTorStatus()
	static void updateTorStatus();

	// Port of: public static boolean isTorEnabled()
	static bool isTorEnabled();

private:
	// Port of: private static final AtomicBoolean sIsTorEnabled = new AtomicBoolean(false);
	static std::atomic<bool> s_isTorEnabled;
};

// Port of: private static final AtomicBoolean sIsTorEnabled = new AtomicBoolean(false);
inline std::atomic<bool> TorCommon::s_isTorEnabled{false};

} // namespace PinkReader
