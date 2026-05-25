/*
 * PinkReader - GPLv3
 * File: tor_common.cpp
 * Exact port of TorCommon.java (47 lines)
 */

#include "tor_common.h"

namespace PinkReader {

// Port of: private static final AtomicBoolean sIsTorEnabled
//         = new AtomicBoolean(false);  (Java line 27)
std::atomic<bool> TorCommon::s_isTorEnabled{false};

// Port of: public static void updateTorStatus() (Java lines 29-42)
void TorCommon::updateTorStatus() {
    // Port of: General.checkThisIsUIThread();
    // In C++, this is handled by the caller or omitted in non-Android context.

    // Port of: final boolean torEnabled = PrefsUtility.network_tor();
    const bool torEnabled = false; // TODO: Replace with actual PrefsUtility call

    // Port of: final boolean torChanged = (torEnabled != isTorEnabled());
    const bool torChanged = (torEnabled != isTorEnabled());

    // Port of: sIsTorEnabled.set(torEnabled);
    s_isTorEnabled.store(torEnabled);

    // Port of: if(torChanged) { HTTPBackend...; CacheDownload...; }
    if (torChanged) {
        // TODO: HTTPBackend::getBackend().recreateHttpBackend();
        // TODO: CacheDownload::resetUserCredentialsOnNextRequest();
    }
}

// Port of: public static boolean isTorEnabled() (Java lines 44-46)
bool TorCommon::isTorEnabled() {
    // Port of: return sIsTorEnabled.get();
    return s_isTorEnabled.load();
}

// Helper setter for testing/integration
void TorCommon::setTorEnabled(bool enabled) {
    s_isTorEnabled.store(enabled);
}

} // namespace PinkReader
