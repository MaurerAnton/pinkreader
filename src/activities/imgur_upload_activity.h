/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: imgur_upload_activity.h
 *   Port of ImgurUploadActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/ImgurUploadActivity.java
 *
 * Original: public class ImgurUploadActivity extends ViewsBaseActivity
 *
 * Every field, method, and constant ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>

namespace PinkReader {

// ============================================================================
// Forward declarations for Android/RedReader types
// ============================================================================

class Bundle;
class Intent;
class Uri;
class View;
class TextView;
class ImageView;
class Button;
class FrameLayout;
class LinearLayout;
class ScrollView;
class LoadingSpinnerView;
class ViewsBaseActivity;
class Context;

// RedReader common
class General;
class PrefsUtility;
class OptionalBase;
template<typename T> class Optional;
class RRError;

// RedReader cache/download
class CacheManager;
class CacheRequest;
class CacheRequestJSONParser;
class Priority;
class Constants;
class UriString;

// RedReader account
class RedditAccountManager;
class RedditAccount;

// RedReader JSON
class JsonValue;
class JsonObject;

// RedReader HTTP
class HTTPRequestBody;
class FailedRequestBody;

// RedReader image
class ThumbnailScaler;

// RedReader time
class TimestampUTC;

// ============================================================================
// ImgurUploadActivity — port of Java class (Java lines 65-362)
//
// Port of: org.quantumbadger.redreader.activities.ImgurUploadActivity
//
// Extends ViewsBaseActivity. Every field, method, and constant ported exactly.
// ============================================================================

class ImgurUploadActivity : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Constructor / Destructor
    // ========================================================================
    explicit ImgurUploadActivity(QObject *parent = nullptr);
    ~ImgurUploadActivity() override;

    // ========================================================================
    // Public lifecycle methods
    // ========================================================================

    // Port of: @Override protected void onCreate(final Bundle savedInstanceState) (Java lines 77-157)
    void onCreate(Bundle *savedInstanceState);

    // Port of: @Override public void onBackPressed() (Java lines 356-361)
    void onBackPressed();

private:
    // ========================================================================
    // Private methods
    // ========================================================================

    // Port of: private void showLoadingOverlay() (Java lines 159-161)
    void showLoadingOverlay();

    // Port of: private void hideLoadingOverlay() (Java lines 163-165)
    void hideLoadingOverlay();

    // Port of: private void updateUploadButtonVisibility() (Java lines 167-172)
    void updateUploadButtonVisibility();

    // Port of: private void onImageSelected(final Uri uri) (Java lines 174-272)
    void onImageSelected(Uri *uri);

    // Port of: private void uploadImage() (Java lines 274-354)
    void uploadImage();

    // ========================================================================
    // Private fields (Java lines 67-75)
    // ========================================================================

    // Port of: private TextView mTextView;
    TextView *mTextView = nullptr;

    // Port of: private ImageView mThumbnailView;
    ImageView *mThumbnailView = nullptr;

    // Port of: private byte[] mImageData;
    // C++: represented as unsigned char vector
    unsigned char *mImageData = nullptr;
    size_t mImageDataLength = 0;

    // Port of: private Button mUploadButton;
    Button *mUploadButton = nullptr;

    // Port of: private View mLoadingOverlay;
    View *mLoadingOverlay = nullptr;

    // Reference to ViewsBaseActivity base class
    ViewsBaseActivity *m_base = nullptr;
};

} // namespace PinkReader
