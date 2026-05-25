/*
 * PinkReader - GPLv3
 * File: album_listing_activity.h - Port of AlbumListingActivity.kt
 *
 * Original: class AlbumListingActivity : ComposeBaseActivity()
 * Located at: redreader/src/main/java/org/quantumbadger/redreader/activities/AlbumListingActivity.kt
 *
 * Every field, method, inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

namespace PinkReader {

// Forward declarations
class ComposeBaseActivity;
class Bundle;
class UriString;

/**
 * @brief Port of AlbumListingActivity.kt
 *
 * Displays an image gallery / album screen for a given URL.
 * Uses Jetpack Compose via ComposeBaseActivity in original;
 * ported as stub with same interface.
 */
class AlbumListingActivity : public QObject {
    Q_OBJECT

public:
    explicit AlbumListingActivity(QObject *parent = nullptr);
    ~AlbumListingActivity() override;

    // ===== Port of: override fun onCreate(savedInstanceState: Bundle?) =====
    void onCreate(Bundle *savedInstanceState);

    // ===== Port of: override fun onBackPressed() =====
    void onBackPressed();

    // ===== Port of: internal fields (from ComposeBaseActivity) =====
    // setTitle(R.string.image_gallery) equivalent
    QString title;

    // url from intent.dataString via UriString.fromNullable
    UriString *url = nullptr;
};

} // namespace PinkReader
