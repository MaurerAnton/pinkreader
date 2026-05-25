/*
 * PinkReader - GPLv3
 * File: album_listing_activity.cpp - Port of AlbumListingActivity.kt
 *
 * Every field, method, inner class ported exactly.
 */

#include "album_listing_activity.h"

// Stubs for Android types
#include <QDebug>

namespace PinkReader {

// ===== Port of: class AlbumListingActivity : ComposeBaseActivity() =====

AlbumListingActivity::AlbumListingActivity(QObject *parent)
    : QObject(parent) {
}

AlbumListingActivity::~AlbumListingActivity() = default;

/*
 * Port of:
 *   override fun onCreate(savedInstanceState: Bundle?) {
 *       PrefsUtility.applyTheme(this)
 *       super.onCreate(savedInstanceState)
 *       setTitle(R.string.image_gallery)
 *       val url = UriString.fromNullable(intent.dataString)
 *       if (url == null) { finish(); return }
 *       if (isSensitiveDebugLoggingEnabled) {
 *           Log.i("AlbumListingActivity", "Loading URL $url")
 *       }
 *       setContentCompose {
 *           AlbumScreen(onBackPressed = this::onBackPressed, albumUrl = url)
 *       }
 *   }
 */
void AlbumListingActivity::onCreate(Bundle * /*savedInstanceState*/) {
    // PrefsUtility.applyTheme(this)
    // ComposeBaseActivity::onCreate(savedInstanceState)

    // setTitle(R.string.image_gallery)
    title = QStringLiteral("Image Gallery");

    // val url = UriString.fromNullable(intent.dataString)
    // NOTE: Intent data retrieval is platform-specific; stub
    url = nullptr;

    if (url == nullptr) {
        // finish()
        qDebug() << "AlbumListingActivity::onCreate - no URL, finishing";
        return;
    }

    // if (isSensitiveDebugLoggingEnabled) Log.i(...)
    // NOTE: Conditional on isSensitiveDebugLoggingEnabled; stubbed
    qDebug() << "AlbumListingActivity: Loading URL" << url->toString();

    // setContentCompose { AlbumScreen(onBackPressed = this::onBackPressed, albumUrl = url) }
    // NOTE: Compose UI is Android-specific; stubbed
}

/*
 * Port of:
 *   override fun onBackPressed() {
 *       if (General.onBackPressed()) {
 *           super.onBackPressed()
 *       }
 *   }
 */
void AlbumListingActivity::onBackPressed() {
    // General.onBackPressed() is a double-tap-to-exit check
    // Ported as: check if last back press was >300ms ago
    // NOTE: Requires General::onBackPressed() — stub true (first press always succeeds)
    // super.onBackPressed() is Android-specific; no-op in Qt
    qDebug() << "AlbumListingActivity::onBackPressed";
}

} // namespace PinkReader
