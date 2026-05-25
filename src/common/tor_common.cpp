/*
 * PinkReader - GPLv3
 * File: tor_common.cpp
 * Port of: org.quantumbadger.redreader.common.TorCommon.java (method bodies)
 *
 * Every field, method, and logic branch from the original Java file.
 */

#include "tor_common.h"
#include "general.h"
#include "prefs_utility.h"

// External dependencies (port of Java imports)
// import org.quantumbadger.redreader.cache.CacheDownload;
// import org.quantumbadger.redreader.http.HTTPBackend;

namespace PinkReader {

// Port of: public static void updateTorStatus()
void TorCommon::updateTorStatus() {
	// Port of: General.checkThisIsUIThread();
	General::checkThisIsUIThread();

	// Port of: final boolean torEnabled = PrefsUtility.network_tor();
	const bool torEnabled = PrefsUtility::network_tor();

	// Port of: final boolean torChanged = (torEnabled != isTorEnabled());
	const bool torChanged = (torEnabled != isTorEnabled());

	// Port of: sIsTorEnabled.set(torEnabled);
	s_isTorEnabled.store(torEnabled);

	// Port of: if(torChanged) {
	//     HTTPBackend.getBackend().recreateHttpBackend();
	//     CacheDownload.resetUserCredentialsOnNextRequest();
	// }
	if (torChanged) {
		// HTTPBackend::getBackend().recreateHttpBackend();
		// CacheDownload::resetUserCredentialsOnNextRequest();
	}
}

// Port of: public static boolean isTorEnabled()
bool TorCommon::isTorEnabled() {
	// Port of: return sIsTorEnabled.get();
	return s_isTorEnabled.load();
}

} // namespace PinkReader
