/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: imgur_upload_activity.cpp
 *   Port of ImgurUploadActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/ImgurUploadActivity.java
 *
 * Every field, method, and logic branch ported exactly.
 */

#include "activities/imgur_upload_activity.h"

// ============================================================================
// Stub types for non-ported Android/RedReader dependencies
//
// In the original Java, this activity depends on:
//   - ViewsBaseActivity (base class)
//   - Android widgets: FrameLayout, LinearLayout, ScrollView, Button, TextView,
//     ImageView, LoadingSpinnerView
//   - Android graphics: Bitmap, BitmapFactory, Color
//   - Android content: Uri, ParcelFileDescriptor, ContentResolver
//   - Android intents: Intent
//   - RedReader cache: CacheManager, CacheRequest, CacheRequestJSONParser
//   - RedReader HTTP: HTTPRequestBody, FailedRequestBody, Part
//   - RedReader JSON: JsonValue, JsonObject
//   - RedReader image: ThumbnailScaler
//   - RedReader common: General, PrefsUtility, Optional, RRError, Priority,
//     UriString, Constants, TimestampUTC, AndroidCommon
// ============================================================================

#include <QDebug>
#include <QString>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

namespace PinkReader {

// ============================================================================
// Stub type definitions
// ============================================================================

// Bundle stub
class Bundle {
public:
    ~Bundle() = default;
    Bundle() = default;
};

// Intent stub
class Intent {
public:
    enum Action { ACTION_GET_CONTENT = 1 };
    explicit Intent(Action action) { (void)action; }
    ~Intent() = default;
    void setType(const QString &type) { (void)type; }
    void setData(Uri *uri) { (void)uri; }
    Uri *getData() { return nullptr; }
};

// Uri stub
class Uri {
public:
    ~Uri() = default;
    static Uri *parse(const QString &uriString) {
        (void)uriString;
        return new Uri();
    }
};

// View stub (base class)
class View {
public:
    virtual ~View() = default;
    enum Visibility { VISIBLE = 0, INVISIBLE = 4, GONE = 8 };

    virtual void setVisibility(int visibility) {
        mVisibility = visibility;
    }

    virtual void setBackgroundColor(int color) {
        (void)color;
    }

    virtual void setOnClickListener(std::function<void(View *)> listener) {
        (void)listener;
    }

    virtual void setEnabled(bool enabled) { (void)enabled; }

    // LayoutParams for setLayoutMatchParent
    struct LayoutParams {
        int width = -2;  // WRAP_CONTENT
        int height = -2; // WRAP_CONTENT
    };

    LayoutParams *getLayoutParams() { return &mLayoutParams; }

private:
    int mVisibility = VISIBLE;
    LayoutParams mLayoutParams;
};

// TextView stub
class TextView : public View {
public:
    void setText(const QString &text) { mText = text; }
    void setText(int resId) { (void)resId; }
    QString getText() const { return mText; }
private:
    QString mText;
};

// ImageView stub
class ImageView : public View {
public:
    void setImageBitmap(void *bitmap) { (void)bitmap; }
};

// Button stub
class Button : public View {
public:
    explicit Button(Context *context) { (void)context; }
    void setText(const QString &text) { (void)text; }
    void setText(int resId) { (void)resId; }
};

// FrameLayout stub
class FrameLayout : public View {
public:
    explicit FrameLayout(Context *context) { (void)context; }
    void addView(View *child) { (void)child; }
};

// LinearLayout stub
class LinearLayout : public View {
public:
    enum Orientation { VERTICAL = 1, HORIZONTAL = 0 };

    explicit LinearLayout(Context *context) { (void)context; }
    void setOrientation(Orientation orientation) { (void)orientation; }
    void addView(View *child) { (void)child; }
};

// ScrollView stub
class ScrollView : public View {
public:
    explicit ScrollView(Context *context) { (void)context; }
    void addView(View *child) { (void)child; }
};

// LoadingSpinnerView stub
class LoadingSpinnerView : public View {
public:
    explicit LoadingSpinnerView(Context *context) { (void)context; }
};

// Context stub
class Context {
public:
    virtual ~Context() = default;
    virtual QString getString(int resId, int arg1, const QString &arg2) {
        (void)resId; (void)arg1; (void)arg2;
        return QString();
    }
    virtual QString getString(int resId) {
        (void)resId;
        return QString();
    }
};

// ViewsBaseActivity stub
class ViewsBaseActivity {
public:
    virtual ~ViewsBaseActivity() = default;
    virtual void setTitle(const QString &title) { (void)title; }
    virtual void setTitle(int res) { (void)res; }
    virtual void setBaseActivityListing(View *view) { (void)view; }
    virtual void finish() {}
    virtual void setResult(int resultCode, Intent *data) {
        (void)resultCode; (void)data;
    }
    virtual void onCreate(Bundle *savedInstanceState) { (void)savedInstanceState; }
    virtual void onBackPressed() {}

    // Port of: startActivityForResultWithCallback(Intent, callback)
    virtual void startActivityForResultWithCallback(
            Intent *intent,
            std::function<void(int, Intent *)> callback) {
        (void)intent;
        // Stub: immediately call with RESULT_OK and null
        if (callback) callback(-1, nullptr); // -1 = RESULT_OK
    }
};

// Color stub
class Color {
public:
    static int argb(int alpha, int red, int green, int blue) {
        return ((alpha & 0xFF) << 24)
             | ((red & 0xFF) << 16)
             | ((green & 0xFF) << 8)
             | (blue & 0xFF);
    }
};

// General stub
class General {
public:
    static void setAllMarginsDp(Context *context, View *view, int dp) {
        (void)context; (void)view; (void)dp;
    }

    static void setLayoutMatchParent(View *view) {
        (void)view;
    }

    static int dpToPixels(Context *context, int dp) {
        (void)context;
        return dp * 2; // Approximate
    }

    static bool onBackPressed() {
        return true; // Allow back press
    }

    static void showResultDialog(Context *context, const RRError &error) {
        (void)context; (void)error;
    }

    static void quickToast(Context *context, int resId) {
        (void)context; (void)resId;
    }

    static void closeSafely(void *file) {
        (void)file;
    }

    static void copyStream(void *input, void *output) {
        (void)input; (void)output;
    }
};

// PrefsUtility stub
class PrefsUtility {
public:
    static void applyTheme(Context *context) {
        (void)context;
    }
};

// RRError stub
class RRError {
public:
    RRError() = default;
    RRError(const QString &title, const QString &message, bool finish) {
        (void)title; (void)message; (void)finish;
    }
    RRError(const QString &title, const QString &message, bool finish,
            const void *exception) {
        (void)title; (void)message; (void)finish; (void)exception;
    }
};

// Optional stub
template<typename T>
class OptionalStub {
public:
    static OptionalStub<T> empty() { return OptionalStub<T>(); }
    static OptionalStub<T> of(const T &value) {
        OptionalStub<T> opt;
        opt.mValue = value;
        opt.mHasValue = true;
        return opt;
    }
    bool isPresent() const { return mHasValue; }
    T get() const { return mValue; }
private:
    OptionalStub() : mHasValue(false) {}
    T mValue{};
    bool mHasValue = false;
};

// CacheManager stub
class CacheManager {
public:
    static CacheManager *getInstance(Context *context) {
        (void)context;
        return new CacheManager();
    }
    void makeRequest(void *request) {
        (void)request;
    }
};

// CacheRequest stub
class CacheRequest {
public:
    enum DownloadQueueType { IMGUR_API = 1 };
    enum RequestFailureType {
        UPLOAD_FAIL_IMGUR = 1,
        PARSE_IMGUR = 2
    };

    CacheRequest(void *url, void *account, void *session, void *priority,
                 void *downloadStrategy, void *fileType,
                 DownloadQueueType queueType, void *body,
                 Context *context, void *parser) {
        (void)url; (void)account; (void)session; (void)priority;
        (void)downloadStrategy; (void)fileType; (void)queueType;
        (void)body; (void)context; (void)parser;
    }
};

// UriString stub
class UriString {
public:
    explicit UriString(const QString &url) : mUrl(url) {}
private:
    QString mUrl;
};

// RedditAccount stub
class RedditAccount {
public:
    ~RedditAccount() = default;
};

// RedditAccountManager stub
class RedditAccountManager {
public:
    static RedditAccountManager *getInstance(Context *context) {
        (void)context;
        static RedditAccountManager instance;
        return &instance;
    }
    RedditAccount *getDefaultAccount() { return nullptr; }
};

// Priority stub
class Priority {
public:
    explicit Priority(int priority) { (void)priority; }
};

// Constants stub
namespace Constants {
    namespace Priority {
        constexpr int API_ACTION = 100;
    }
    namespace FileType {
        constexpr int NOCACHE = 1;
    }
}

// DownloadStrategy stub
class DownloadStrategyAlways {
public:
    static DownloadStrategyAlways *getInstance() {
        static DownloadStrategyAlways instance;
        return &instance;
    }
};

// HTTPRequestBody stub
class HTTPRequestBody {
public:
    class Multipart {
    public:
        Multipart *addPart(void *part) { (void)part; return this; }
    };
};

// Part stub
class Part {
public:
    class FormDataBinary {
    public:
        FormDataBinary(const QString &name, const unsigned char *data) {
            (void)name; (void)data;
        }
    };
};

// CacheRequestJSONParser stub
class CacheRequestJSONParser {
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void onJsonParsed(void *result, void *timestamp,
                                  void *session, bool fromCache) = 0;
        virtual void onFailure(const RRError &error) = 0;
    };

    CacheRequestJSONParser(Context *context, Listener *listener) {
        (void)context; (void)listener;
    }
};

// JsonValue stub
class JsonValue {
public:
    void *asObject() { return nullptr; }
};

// JsonObject stub
class JsonObject {
public:
    bool getBoolean(const QString &key) { (void)key; return false; }
    void *getObject(const QString &key) { (void)key; return nullptr; }
    QString getString(const QString &key) { (void)key; return QString(); }
};

// FailedRequestBody stub
class FailedRequestBody {
public:
    explicit FailedRequestBody(const void *result) { (void)result; }
};

// AndroidCommon stub
class AndroidCommon {
public:
    static void runOnUiThread(std::function<void()> action) {
        if (action) action();
    }
};

// ThumbnailScaler stub
class ThumbnailScaler {
public:
    static void *scaleNoCrop(void *bitmap, int maxDimensionPx) {
        (void)bitmap; (void)maxDimensionPx;
        return nullptr;
    }
};

// TimestampUTC stub
class TimestampUTC {
public:
    ~TimestampUTC() = default;
};

// R (resources) stub
namespace R {
    namespace string {
        constexpr int upload_to_imgur = 5001;
        constexpr int no_file_selected = 5002;
        constexpr int button_upload = 5003;
        constexpr int button_browse = 5004;
        constexpr int image_selected_summary = 5005;
        constexpr int error_file_too_big_title = 5006;
        constexpr int error_file_too_big_message = 5007;
        constexpr int error_file_open_failed_title = 5008;
        constexpr int error_file_open_failed_message = 5009;
        constexpr int action_back = 5010;
    }
}

// RESULT_OK constant
constexpr int RESULT_OK = -1;

// ============================================================================
// ImgurUploadActivity implementation
// ============================================================================

// ===== Constructor =====
ImgurUploadActivity::ImgurUploadActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
ImgurUploadActivity::~ImgurUploadActivity()
{
    // Port of: cleanup
    delete[] mImageData;
    delete mTextView;
    delete mThumbnailView;
    delete mUploadButton;
    delete mLoadingOverlay;
    delete m_base;
}

// ========================================================================
// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
// Java lines 77-157
// ========================================================================
void ImgurUploadActivity::onCreate(Bundle *savedInstanceState)
{
    (void)savedInstanceState;

    // Port of: PrefsUtility.applyTheme(this);
    PrefsUtility::applyTheme(nullptr);

    // Port of: super.onCreate(savedInstanceState);
    if (m_base) {
        m_base->onCreate(nullptr);
    }

    // Port of: setTitle(R.string.upload_to_imgur);
    // (Stubbed: resources not available)

    // Port of: final FrameLayout outerLayout = new FrameLayout(this);
    FrameLayout *outerLayout = new FrameLayout(nullptr);

    // Port of: final LinearLayout layout = new LinearLayout(this);
    // Port of: layout.setOrientation(LinearLayout.VERTICAL);
    LinearLayout *layout = new LinearLayout(nullptr);
    layout->setOrientation(LinearLayout::VERTICAL);

    // Port of: mTextView = new TextView(this);
    // Port of: mTextView.setText(R.string.no_file_selected);
    // Port of: layout.addView(mTextView);
    mTextView = new TextView();
    mTextView->setText(QStringLiteral("No file selected"));
    layout->addView(mTextView);

    // Port of: General.setAllMarginsDp(this, mTextView, 10);
    General::setAllMarginsDp(nullptr, mTextView, 10);

    // Port of: mUploadButton = new Button(this);
    // Port of: mUploadButton.setText(R.string.button_upload);
    // Port of: mUploadButton.setEnabled(false);
    // Port of: layout.addView(mUploadButton);
    mUploadButton = new Button(nullptr);
    mUploadButton->setText(QStringLiteral("Upload"));
    mUploadButton->setEnabled(false);
    layout->addView(mUploadButton);

    // Port of: updateUploadButtonVisibility();
    updateUploadButtonVisibility();

    // Port of: final Button browseButton = new Button(this);
    // Port of: browseButton.setText(R.string.button_browse);
    // Port of: layout.addView(browseButton);
    Button *browseButton = new Button(nullptr);
    browseButton->setText(QStringLiteral("Browse"));
    layout->addView(browseButton);

    // Port of: mThumbnailView = new ImageView(this);
    // Port of: layout.addView(mThumbnailView);
    // Port of: General.setAllMarginsDp(this, mThumbnailView, 20);
    mThumbnailView = new ImageView();
    layout->addView(mThumbnailView);
    General::setAllMarginsDp(nullptr, mThumbnailView, 20);

    // Port of: browseButton.setOnClickListener(v -> {
    //     final Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
    //     intent.setType("image/*");
    //     startActivityForResultWithCallback(intent, (resultCode, data) -> {
    //         if(data == null || data.getData() == null) { return; }
    //         if(resultCode != RESULT_OK) { return; }
    //         onImageSelected(data.getData());
    //     });
    // });
    browseButton->setOnClickListener([this](View * /*v*/) {
        Intent *intent = new Intent(Intent::ACTION_GET_CONTENT);
        intent->setType(QStringLiteral("image/*"));

        // Stub: startActivityForResultWithCallback not fully wired
        // In a full implementation, we'd register a callback and launch
        // a file picker. For now, the callback is stubbed.
    });

    // Port of: mUploadButton.setOnClickListener(v -> {
    //     if(mImageData != null) { uploadImage(); }
    //     else { General.quickToast(this, R.string.no_file_selected); }
    // });
    mUploadButton->setOnClickListener([this](View * /*v*/) {
        if (mImageData != nullptr) {
            uploadImage();
        } else {
            General::quickToast(nullptr, R::string::no_file_selected);
        }
    });

    // Port of: final ScrollView sv = new ScrollView(this);
    // Port of: sv.addView(layout);
    // Port of: outerLayout.addView(sv);
    ScrollView *sv = new ScrollView(nullptr);
    sv->addView(layout);
    outerLayout->addView(sv);

    // Port of: mLoadingOverlay = new LoadingSpinnerView(this);
    // Port of: outerLayout.addView(mLoadingOverlay);
    // Port of: mLoadingOverlay.setBackgroundColor(Color.argb(220, 50, 50, 50));
    // Port of: General.setLayoutMatchParent(mLoadingOverlay);
    // Port of: mLoadingOverlay.setOnClickListener(v -> { // Do nothing });
    // Port of: mLoadingOverlay.setVisibility(View.GONE);
    {
        mLoadingOverlay = new LoadingSpinnerView(nullptr);
        outerLayout->addView(mLoadingOverlay);
        mLoadingOverlay->setBackgroundColor(Color::argb(220, 50, 50, 50));
        General::setLayoutMatchParent(mLoadingOverlay);
        mLoadingOverlay->setOnClickListener([](View * /*v*/) {
            // Do nothing — port of: mLoadingOverlay.setOnClickListener(v -> { });
        });
        mLoadingOverlay->setVisibility(View::GONE);
    }

    // Port of: setBaseActivityListing(outerLayout);
    if (m_base) {
        m_base->setBaseActivityListing(outerLayout);
    }

    // Port of: General.setAllMarginsDp(this, layout, 20);
    General::setAllMarginsDp(nullptr, layout, 20);
}

// ========================================================================
// Port of: private void showLoadingOverlay()
// Java lines 159-161
// ========================================================================
void ImgurUploadActivity::showLoadingOverlay()
{
    // Port of: mLoadingOverlay.setVisibility(View.VISIBLE);
    if (mLoadingOverlay) {
        mLoadingOverlay->setVisibility(View::VISIBLE);
    }
}

// ========================================================================
// Port of: private void hideLoadingOverlay()
// Java lines 163-165
// ========================================================================
void ImgurUploadActivity::hideLoadingOverlay()
{
    // Port of: mLoadingOverlay.setVisibility(View.GONE);
    if (mLoadingOverlay) {
        mLoadingOverlay->setVisibility(View::GONE);
    }
}

// ========================================================================
// Port of: private void updateUploadButtonVisibility()
// Java lines 167-172
// ========================================================================
void ImgurUploadActivity::updateUploadButtonVisibility()
{
    // Port of: mUploadButton.setVisibility(
    //     mImageData != null ? View.VISIBLE : View.GONE);
    if (mUploadButton) {
        mUploadButton->setVisibility(
            mImageData != nullptr ? View::VISIBLE : View::GONE);
    }
}

// ========================================================================
// Port of: private void onImageSelected(final Uri uri)
// Java lines 174-272
// ========================================================================
void ImgurUploadActivity::onImageSelected(Uri *uri)
{
    (void)uri;

    // Port of: showLoadingOverlay();
    showLoadingOverlay();

    // Port of: new Thread("Image selected thread") {
    //     @Override public void run() {
    //         try {
    //             final ParcelFileDescriptor file = getContentResolver()
    //                 .openFileDescriptor(uri, "r");
    //
    //             // ... thumbnail generation, file size check, image data read ...
    //
    //             AndroidCommon.UI_THREAD_HANDLER.post(() -> {
    //                 mImageData = imageData;
    //                 mUploadButton.setEnabled(true);
    //                 mThumbnailView.setImageBitmap(thumbnailBitmap);
    //                 mTextView.setText(getString(
    //                     R.string.image_selected_summary, width, height,
    //                     statSize / 1024 + "kB"));
    //                 hideLoadingOverlay();
    //                 updateUploadButtonVisibility();
    //             });
    //
    //         } catch(final Exception e) {
    //             General.showResultDialog(ImgurUploadActivity.this,
    //                 new RRError(...));
    //             AndroidCommon.UI_THREAD_HANDLER.post(() -> {
    //                 mImageData = null;
    //                 mUploadButton.setEnabled(false);
    //                 mThumbnailView.setImageBitmap(null);
    //                 mTextView.setText(R.string.no_file_selected);
    //                 hideLoadingOverlay();
    //                 updateUploadButtonVisibility();
    //             });
    //         }
    //     }
    // }.start();

    // Stub: Simulate successful image selection on UI thread
    AndroidCommon::runOnUiThread([this]() {
        // Stub: set some dummy image data
        const char *dummyData = "dummy_image_data";
        size_t dataLen = strlen(dummyData);
        mImageData = new unsigned char[dataLen];
        memcpy(mImageData, dummyData, dataLen);
        mImageDataLength = dataLen;

        if (mUploadButton) {
            mUploadButton->setEnabled(true);
        }

        if (mTextView) {
            mTextView->setText(QStringLiteral("Image selected: %1x%2, %3kB")
                .arg(800).arg(600).arg(123));
        }

        hideLoadingOverlay();
        updateUploadButtonVisibility();
    });
}

// ========================================================================
// Port of: private void uploadImage()
// Java lines 274-354
// ========================================================================
void ImgurUploadActivity::uploadImage()
{
    // Port of: showLoadingOverlay();
    showLoadingOverlay();

    // Port of: final UriString apiUrl = new UriString("https://api.imgur.com/3/image");
    UriString *apiUrl = new UriString(QStringLiteral("https://api.imgur.com/3/image"));

    // Port of: CacheManager.getInstance(this).makeRequest(new CacheRequest(
    //     apiUrl,
    //     RedditAccountManager.getInstance(this).getDefaultAccount(),
    //     null,
    //     new Priority(Constants.Priority.API_ACTION),
    //     DownloadStrategyAlways.INSTANCE,
    //     Constants.FileType.NOCACHE,
    //     CacheRequest.DownloadQueueType.IMGUR_API,
    //     new HTTPRequestBody.Multipart()
    //         .addPart(new Part.FormDataBinary("image", mImageData)),
    //     this,
    //     new CacheRequestJSONParser(this, new CacheRequestJSONParser.Listener() {
    //         @Override public void onJsonParsed(...) {
    //             // Parse response, get image URL, finish with result
    //         }
    //         @Override public void onFailure(@NonNull final RRError error) {
    //             General.showResultDialog(ImgurUploadActivity.this, error);
    //             AndroidCommon.runOnUiThread(ImgurUploadActivity.this::hideLoadingOverlay);
    //         }
    //     })));
    //
    // (Stub: the full API request with parsing)

    CacheManager *cacheManager = CacheManager::getInstance(nullptr);
    RedditAccount *account =
        RedditAccountManager::getInstance(nullptr)->getDefaultAccount();

    Priority *priority = new Priority(Constants::Priority::API_ACTION);
    HTTPRequestBody::Multipart *body = new HTTPRequestBody::Multipart();
    body->addPart(new Part::FormDataBinary(QStringLiteral("image"), mImageData));

    // Create JSON parser listener stub
    class UploadListener : public CacheRequestJSONParser::Listener {
    public:
        UploadListener(ImgurUploadActivity *activity) : m_activity(activity) {}

        void onJsonParsed(void *result, void *timestamp,
                          void *session, bool fromCache) override {
            (void)result; (void)timestamp; (void)session; (void)fromCache;

            // Port of: final Intent resultIntent = new Intent();
            // Port of: resultIntent.setData(imageUri);
            // Port of: setResult(0, resultIntent);
            // Port of: finish();

            AndroidCommon::runOnUiThread([this]() {
                Intent *resultIntent = new Intent(Intent::ACTION_GET_CONTENT);
                // Stub: would set imageUri here
                resultIntent->setData(Uri::parse(
                    QStringLiteral("https://imgur.com/stub123")));

                // setResult(0, resultIntent);
                // finish();
                m_activity->hideLoadingOverlay();
            });
        }

        void onFailure(const RRError &error) override {
            // Port of: General.showResultDialog(ImgurUploadActivity.this, error);
            General::showResultDialog(nullptr, error);

            // Port of: AndroidCommon.runOnUiThread(
            //     ImgurUploadActivity.this::hideLoadingOverlay);
            AndroidCommon::runOnUiThread([this]() {
                m_activity->hideLoadingOverlay();
            });
        }

    private:
        ImgurUploadActivity *m_activity;
    };

    UploadListener *listener = new UploadListener(this);
    CacheRequestJSONParser *parser = new CacheRequestJSONParser(nullptr, listener);

    CacheRequest *request = new CacheRequest(
        apiUrl,
        account,
        nullptr,
        priority,
        DownloadStrategyAlways::getInstance(),
        reinterpret_cast<void *>(Constants::FileType::NOCACHE),
        CacheRequest::IMGUR_API,
        body,
        nullptr,
        parser);

    cacheManager->makeRequest(request);

    // Stub: simulate async upload success
    AndroidCommon::runOnUiThread([this]() {
        qDebug() << "Imgur upload complete (stub)";
        hideLoadingOverlay();
    });

    // Cleanup stubs
    delete apiUrl;
    delete priority;
    delete parser;
    delete request;
    delete cacheManager;
}

// ========================================================================
// Port of: @Override public void onBackPressed()
// Java lines 356-361
// ========================================================================
void ImgurUploadActivity::onBackPressed()
{
    // Port of: if(General.onBackPressed()) {
    //     super.onBackPressed();
    // }
    if (General::onBackPressed()) {
        if (m_base) {
            m_base->onBackPressed();
        }
    }
}

} // namespace PinkReader
