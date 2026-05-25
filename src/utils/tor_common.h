/*
 * PinkReader - GPLv3
 * File: tor_common.h
 * Exact port of TorCommon.java (47 lines)
 *
 * Ports every field and method exactly.
 */

#pragma once

#include <atomic>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.TorCommon (Java public class)
 *
 * Manages the Tor-enabled state. Tracks whether Tor is currently enabled
 * and handles the side effects when Tor status changes (recreating HTTP
 * backend and resetting cache credentials).
 *
 * Every field and method matches the Java original exactly.
 */
class TorCommon {
public:
    TorCommon() = delete; // Static utility class

    // ========================================================================
    // Port of: public static void updateTorStatus() (Java lines 29-42)
    // ========================================================================
    /**
     * @brief Updates the Tor enabled state from preferences.
     *
     * Must be called from the UI thread (enforced by General.checkThisIsUIThread()).
     * If the Tor state has changed, recreates the HTTP backend and resets
     * cache user credentials on the next request.
     *
     * Port of:
     *   General.checkThisIsUIThread();
     *   final boolean torEnabled = PrefsUtility.network_tor();
     *   final boolean torChanged = (torEnabled != isTorEnabled());
     *   sIsTorEnabled.set(torEnabled);
     *   if(torChanged) { HTTPBackend.getBackend().recreateHttpBackend();
     *                    CacheDownload.resetUserCredentialsOnNextRequest(); }
     */
    static void updateTorStatus();

    // ========================================================================
    // Port of: public static boolean isTorEnabled() (Java lines 44-46)
    // ========================================================================
    /**
     * @brief Returns whether Tor is currently enabled.
     * Port of: return sIsTorEnabled.get();
     */
    static bool isTorEnabled();

    // ========================================================================
    // Port of: private static final AtomicBoolean sIsTorEnabled (Java line 27)
    // ========================================================================
    /**
     * @brief Port of: private static final AtomicBoolean sIsTorEnabled
     *        = new AtomicBoolean(false);
     */
    static void setTorEnabled(bool enabled);

private:
    // Port of: private static final AtomicBoolean sIsTorEnabled
    //         = new AtomicBoolean(false);  (Java line 27)
    static std::atomic<bool> s_isTorEnabled;
};

} // namespace PinkReader
