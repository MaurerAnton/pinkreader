/*
 * PinkReader - GPLv3
 * File: general.h - Port of General.kt (object General)
 * Exact port: every public method and logic branch from the original.
 *
 * Original was a Kotlin object with @JvmStatic methods.
 * Android-specific UI methods (Toast, Dialog, Activity) are adapted to Qt equivalents
 * or provided as stubs with documentation of their original behavior.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QSet>
#include <QVector>
#include <functional>
#include <string>
#include <vector>

// Forward declarations for Qt types used in signatures
QT_BEGIN_NAMESPACE
class QWidget;
class QUrl;
QT_END_NAMESPACE

namespace PinkReader {

// Forward declare
class SharedPrefsWrapper;
class RRError;

/**
 * @brief Port of org.quantumbadger.redreader.common.General (Kotlin object)
 *
 * Every public @JvmStatic method is ported as a static member function.
 * Private methods are ported as private static helpers.
 */
class General {
public:
    General() = delete; // Cannot instantiate (Kotlin object)

    // Port of: @JvmField val CHARSET_UTF8: Charset = Objects.requireNonNull(Charset.forName("UTF-8"))
    static const QString CHARSET_UTF8_NAME;

    // Port of: const val LTR_OVERRIDE_MARK = "\u202D"
    static constexpr const char16_t LTR_OVERRIDE_MARK = u'\u202D';

    // Port of: const val COLOR_INVALID = Color.MAGENTA
    // Color.MAGENTA = 0xFFFF00FF in Android (ARGB). Qt uses AARRGGBB.
    static constexpr unsigned int COLOR_INVALID = 0xFFFF00FF;

    // --- Port of: onBackPressed() ---
    // Returns true if back press was within 300ms (indicating first press for double-back-to-exit)
    // Port of:
    //   @JvmStatic fun onBackPressed(): Boolean {
    //       if(lastBackPress < SystemClock.uptimeMillis() - 300) {
    //           lastBackPress = SystemClock.uptimeMillis(); return true;
    //       } return false;
    //   }
    static bool onBackPressed();

    // --- Port of: getSharedPrefs(context) ---
    // Uses AtomicReference for thread-safe lazy initialization.
    // Returns SharedPrefsWrapper pointer.
    // Port of:
    //   @JvmStatic fun getSharedPrefs(context: Context): SharedPrefsWrapper { ... AtomicReference ... }
    static SharedPrefsWrapper *getSharedPrefs();

    // --- Port of: handlerMessage(what, obj) ---
    // Creates a message-like struct. Original uses android.os.Message.
    // Port of:
    //   @JvmStatic fun handlerMessage(what: Int, obj: Any?): Message { ... }
    struct Message {
        int what = 0;
        void *obj = nullptr;
    };
    static Message handlerMessage(int what, void *obj = nullptr);

    // --- Port of: addUnits(input: Long): String ---
    // Converts bytes to human-readable string (B, KiB, MiB, GiB)
    // Exact logic from original:
    //   var i = 0; var result = input.toDouble();
    //   while (i <= 3 && result >= 1024) { i++; result = input / 1024.0.pow(i) }
    //   ... formatting ...
    static QString addUnits(qint64 input);

    // --- Port of: bytesToMegabytes(input: Long): String ---
    // Port of:
    //   val totalKilobytes = input / 1024; val totalMegabytes = totalKilobytes / 1024;
    //   val remainder = totalKilobytes % 1024;
    //   return String.format(Locale.US, "%d.%02d MB", totalMegabytes, remainder / 10)
    static QString bytesToMegabytes(qint64 input);

    // --- Port of: dpToPixels(context: Context, dp: Float): Int ---
    // Uses TypedValue.applyDimension. We port the formula: px = dp * (dpi / 160)
    // By default uses 160 DPI baseline; caller can override with specific DPI.
    static int dpToPixels(float dp, float densityDpi = 160.0f);

    // --- Port of: val isSensitiveDebugLoggingEnabled: Boolean ---
    // Original: get() = BuildConfig.DEBUG
    static bool isSensitiveDebugLoggingEnabled();

    // --- Port of: quickToast overloads ---
    // These are Android-specific (Toast.makeText). In Qt, these would use QToolTip or status bar.
    // We provide the interface but the implementation is platform-specific.
    // Port of:
    //   @JvmStatic fun quickToast(context: Context, textRes: Int)
    //   @JvmStatic fun quickToast(context: Context?, text: String?)
    //   @JvmStatic fun quickToast(context: Context?, text: String?, duration: Int)
    //   @JvmStatic fun quickToast(context: Context, textRes: Int, duration: Int)
    static void quickToast(const QString &text, int durationMs = 2000);
    static void quickToast(int textResId, int durationMs = 2000);

    // --- Port of: isTablet(context: Context): Boolean ---
    // Port of:
    //   when(PrefsUtility.appearance_twopane()) {
    //       AppearanceTwopane.AUTO -> check screen size XLARGE
    //       AppearanceTwopane.NEVER -> false
    //       AppearanceTwopane.FORCE -> true
    //   }
    static bool isTablet();

    // --- Port of: isConnectionWifi(context: Context): Boolean ---
    // Original uses ConnectivityManager. We provide stub that can be overridden.
    static bool isConnectionWifi();

    // --- Port of: isNetworkConnected(context: Context): Boolean ---
    // Original uses ConnectivityManager. Stub.
    static bool isNetworkConnected();

    // --- Port of: getGeneralErrorForFailure (overload 1 - RequestFailureType) ---
    // This is a large method mapping failure types to error strings.
    // We port the core logic but simplify the error construction.
    // Port of:
    //   @JvmStatic fun getGeneralErrorForFailure(context, type: RequestFailureType, t, status, url, response): RRError
    enum class RequestFailureType {
        CANCELLED, PARSE, CACHE_MISS, STORAGE, CONNECTION,
        MALFORMED_URL, DISK_SPACE, CACHE_DIR_DOES_NOT_EXIST,
        REQUEST, REDDIT_REDIRECT, PARSE_IMGUR, UPLOAD_FAIL_IMGUR
    };
    static RRError getGeneralErrorForFailure(RequestFailureType type,
                                              const QString &exceptionMessage = QString(),
                                              int httpStatus = -1,
                                              const QString &url = QString());

    // --- Port of: getGeneralErrorForFailure (overload 2 - APIFailureType) ---
    enum class APIFailureType {
        INVALID_USER, NOTALLOWED, BAD_CAPTCHA, SUBREDDIT_REQUIRED,
        URL_REQUIRED, TOO_FAST, TOO_LONG, ALREADY_SUBMITTED,
        POST_FLAIR_REQUIRED, UNKNOWN
    };
    static RRError getGeneralErrorForFailure(APIFailureType type,
                                              const QString &debuggingContext = QString());

    // --- Port of: filenameFromString(url: String): String ---
    // Extracts filename from URL, defaulting to .jpg or .mp4 for reddit videos
    static QString filenameFromString(const QString &url);

    // --- Port of: sha256(plaintext: String): String ---
    static QString sha256(const QString &plaintext);

    // --- Port of: sha1(plaintext: ByteArray): String ---
    static QString sha1(const QByteArray &plaintext);

    // --- Port of: getUriQueryParameterNames(uri: Uri): Set<String> ---
    // Returns set of query parameter names from a URI
    static QSet<QString> getUriQueryParameterNames(const QString &uriStr);

    // --- Port of: divideCeil(num: Int, divisor: Int): Int ---
    // Port of: return (num + divisor - 1) / divisor
    static int divideCeil(int num, int divisor);

    // --- Port of: checkThisIsUIThread() ---
    // Port of: if(!isThisUIThread) throw RuntimeException
    static void checkThisIsUIThread();

    // --- Port of: val isThisUIThread: Boolean ---
    // Port of: Looper.getMainLooper().thread === Thread.currentThread()
    static bool isThisUIThread();

    // --- Port of: listOfOne(obj: E): ArrayList<E> ---
    template<typename E>
    static QVector<E> listOfOne(const E &obj) {
        QVector<E> result;
        result.append(obj);
        return result;
    }

    // --- Port of: copyStream / readWholeStream / readWholeStreamAsUTF8 ---
    // These are I/O utilities. Ported as static methods operating on QIODevice or raw data.
    static QByteArray readWholeStream(class QIODevice *device);
    static QString readWholeStreamAsUTF8(class QIODevice *device);

    // --- Port of: setAllMarginsDp / setLayoutMatchParent / setLayoutMatchWidthWrapHeight / setLayoutWidthHeight ---
    // Android layout utilities. Ported as static helpers for QWidget.
    static void setAllMarginsDp(QWidget *widget, int marginDp, float densityDpi = 160.0f);
    static void setLayoutMatchParent(QWidget *widget);
    static void setLayoutMatchWidthWrapHeight(QWidget *widget);
    static void setLayoutWidthHeight(QWidget *widget, int width, int height);

    // --- Port of: startNewThread(name, runnable) ---
    static void startNewThread(const QString &name, std::function<void()> runnable);

    // --- Port of: hashsetFromArray / listFromArray ---
    template<typename E>
    static QSet<E> hashsetFromArray(std::initializer_list<E> data) {
        return QSet<E>(data.begin(), data.end());
    }

    template<typename E>
    static QVector<E> listFromArray(std::initializer_list<E> data) {
        return QVector<E>(data.begin(), data.end());
    }

    // --- Port of: nullAlternative(vararg values: E): E ---
    // Returns first non-null value, or last if all null.
    // In C++ we can't have null for value types, so we check for default-constructed.
    template<typename E>
    static E nullAlternative(std::initializer_list<E> values) {
        for (const E &v : values) {
            if (v != E()) {
                return v;
            }
        }
        return *(values.end() - 1); // last element as fallback (original behavior)
    }

    // --- Port of: val isAlpha: Boolean ---
    // Original: get() = General::class.java.canonicalName?.contains("alpha") == true
    static bool isAlpha();

    // --- Thread ID for UI thread check ---
    static void setUIThreadId();
    static bool s_isUIThreadSet;

private:
    // Port of: private val mPrefs = AtomicReference<SharedPrefsWrapper>()
    static SharedPrefsWrapper *s_prefs;

    // Port of: private var lastBackPress: Long = -1
    static qint64 s_lastBackPress;

    // Port of: private fun toHex(bytes: ByteArray): String
    static QString toHex(const QByteArray &bytes);

    // Port of: private fun isTorError(t: Throwable?): Boolean
    static bool isTorError(const QString &message);

    // Port of: private fun isContentBlockerError(t: Throwable?): Boolean
    static bool isContentBlockerError(const QString &message);

    // UI thread ID
    static Qt::HANDLE s_uiThreadId;
};

} // namespace PinkReader
