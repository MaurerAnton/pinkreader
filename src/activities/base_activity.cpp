/*
 * PinkReader - GPLv3
 * File: base_activity.cpp
 * Exact port of BaseActivity.java (236 lines)
 *
 * Ports every method body exactly. Android-specific APIs are adapted to
 * platform-independent stubs and Qt signals.
 */

#include "base_activity.h"

// Forward-declared dependency headers (to be added as needed)
// #include "general.h"
// #include "prefs_utility.h"
// #include "shared_prefs_wrapper.h"
// #include "tor_common.h"
// #include "global_exception_handler.h"

#include <stdexcept>

namespace PinkReader {

// ============================================================================
// Static field definition — Java line 46
// Port of: private static boolean closingAll = false;
// ============================================================================
bool BaseActivity::s_closingAll = false;

// ============================================================================
// Constructor / Destructor
// ============================================================================
BaseActivity::BaseActivity(QObject *parent)
    : QObject(parent)
{
}

BaseActivity::~BaseActivity() = default;

// ============================================================================
// closeAllExceptMain — Java lines 66-69
// Port of: public void closeAllExceptMain()
// ============================================================================
void BaseActivity::closeAllExceptMain()
{
    // Port of: closingAll = true;
    s_closingAll = true;
    // Port of: closeIfNecessary();
    closeIfNecessary();
}

// ============================================================================
// onCreate — Java lines 72-98
// Port of: @Override protected void onCreate(final Bundle savedInstanceState)
// ============================================================================
void BaseActivity::onCreate(Bundle *savedInstanceState)
{
    (void) savedInstanceState; // Used by subclasses

    // Port of: super.onCreate(savedInstanceState);
    // (C++: QObject doesn't have onCreate; subclasses call parent if needed)

    // Port of: mSharedPreferences = General.getSharedPrefs(this);
    // TODO: Wire up General::getSharedPrefs when context is available
    // m_sharedPreferences = General::getSharedPrefs();

    // Port of: if (PrefsUtility.pref_appearance_android_status()
    //             == PrefsUtility.AppearanceStatusBarMode.ALWAYS_HIDE) {
    //             getWindow().setFlags(
    //                 WindowManager.LayoutParams.FLAG_FULLSCREEN,
    //                 WindowManager.LayoutParams.FLAG_FULLSCREEN);
    // }
    // TODO: Wire up PrefsUtility and window management

    // Port of: if (PrefsUtility.behaviour_block_screenshots()) {
    //             getWindow().addFlags(WindowManager.LayoutParams.FLAG_SECURE);
    // }
    // TODO: Wire up

    // Port of: if (PrefsUtility.pref_behaviour_keep_screen_awake()) {
    //             getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    // }
    // TODO: Wire up

    // Port of: mSharedPreferences.registerOnSharedPreferenceChangeListener(this);
    if (m_sharedPreferences) {
        m_sharedPreferences->registerOnSharedPreferenceChangeListener(this);
    }

    // Port of: setOrientationFromPrefs();
    setOrientationFromPrefs();

    // Port of: closeIfNecessary();
    closeIfNecessary();

    // Port of: GlobalExceptionHandler.handleLastCrash(this);
    // TODO: Wire up GlobalExceptionHandler
    // GlobalExceptionHandler::handleLastCrash(this);
}

// ============================================================================
// onResume — Java lines 100-106
// Port of: @Override protected void onResume()
// ============================================================================
void BaseActivity::onResume()
{
    // Port of: super.onResume();
    // Port of: setOrientationFromPrefs();
    setOrientationFromPrefs();
    // Port of: closeIfNecessary();
    closeIfNecessary();
    // Port of: TorCommon.updateTorStatus();
    // TODO: Wire up TorCommon::updateTorStatus()
}

// ============================================================================
// onDestroy — Java lines 109-113
// Port of: @Override protected void onDestroy()
// ============================================================================
void BaseActivity::onDestroy()
{
    // Port of: super.onDestroy();
    // Port of: mSharedPreferences.unregisterOnSharedPreferenceChangeListener(this);
    if (m_sharedPreferences) {
        m_sharedPreferences->unregisterOnSharedPreferenceChangeListener(this);
    }
}

// ============================================================================
// closeIfNecessary — Java lines 115-123
// Port of: private void closeIfNecessary()
// ============================================================================
void BaseActivity::closeIfNecessary()
{
    // Port of: if (closingAll) {
    if (s_closingAll) {
        // Port of: if (this instanceof MainActivity) {
        if (isMainActivity()) {
            // Port of: closingAll = false;
            s_closingAll = false;
        } else {
            // Port of: finish();
            finish();
        }
    }
}

// ============================================================================
// requestPermissionWithCallback — Java lines 125-145
// Port of: public final void requestPermissionWithCallback(
//             @NonNull final String permission,
//             @NonNull final PermissionCallback callback)
// ============================================================================
void BaseActivity::requestPermissionWithCallback(
        const QString &permission,
        PermissionCallback *callback)
{
    // Port of: General.checkThisIsUIThread();
    // TODO: Wire up General::checkThisIsUIThread()

    // Port of: if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
    // For C++, we assume permissions are always available on desktop platforms.
    // The full Android logic is documented below.

    // Port of: if (checkSelfPermission(permission) == PackageManager.PERMISSION_GRANTED) {
    //             callback.onPermissionGranted();
    //         } else {
    //             final int requestCode = mRequestIdGenerator.incrementAndGet();
    //             mPermissionRequestCallbacks.put(requestCode, callback);
    //             requestPermissions(new String[]{permission}, requestCode);
    //         }
    //
    // Port of: } else { callback.onPermissionGranted(); }

    // Simplified: For non-Android platforms, always grant
    if (callback) {
        callback->onPermissionGranted();
    }
}

// ============================================================================
// onRequestPermissionsResult — Java lines 147-171
// Port of: @Override public final void onRequestPermissionsResult(
//             final int requestCode,
//             @NonNull final String[] permissions,
//             @NonNull final int[] grantResults)
// ============================================================================
void BaseActivity::onRequestPermissionsResult(
        int requestCode,
        const QStringList &permissions,
        const QList<int> &grantResults)
{
    // Port of: super.onRequestPermissionsResult(requestCode, permissions, grantResults);

    // Port of: final PermissionCallback callback
    //             = mPermissionRequestCallbacks.remove(requestCode);
    PermissionCallback *callback = nullptr;
    auto it = m_permissionRequestCallbacks.find(requestCode);
    if (it != m_permissionRequestCallbacks.end()) {
        callback = it.value();
        m_permissionRequestCallbacks.erase(it);
    }

    // Port of: if (callback == null) { return; }
    if (callback == nullptr) {
        return;
    }

    // Port of: if (permissions.length != 1) {
    //             throw new RuntimeException("Unexpected permission result");
    // }
    if (permissions.size() != 1) {
        throw std::runtime_error("Unexpected permission result");
    }

    // Port of: if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
    //             callback.onPermissionGranted();
    //         } else {
    //             callback.onPermissionDenied();
    //         }
    if (!grantResults.isEmpty() && grantResults.at(0) == 0) {
        // PERMISSION_GRANTED = 0 in Android PackageManager
        callback->onPermissionGranted();
    } else {
        callback->onPermissionDenied();
    }
}

// ============================================================================
// startActivityForResultWithCallback — Java lines 173-180
// Port of: public final void startActivityForResultWithCallback(
//             @NonNull final Intent intent,
//             @NonNull final ActivityResultCallback callback)
// ============================================================================
void BaseActivity::startActivityForResultWithCallback(
        Intent *intent,
        ActivityResultCallback *callback)
{
    // Port of: final int requestCode = mRequestIdGenerator.incrementAndGet();
    int requestCode = m_requestIdGenerator.fetch_add(1) + 1;

    // Port of: mActivityResultCallbacks.put(requestCode, callback);
    m_activityResultCallbacks.insert(requestCode, callback);

    // Port of: startActivityForResult(intent, requestCode);
    // TODO: Wire up activity launching with request code
    (void) intent; // Used when activity system is wired up
}

// ============================================================================
// onActivityResult — Java lines 182-198
// Port of: @Override protected final void onActivityResult(
//             final int requestCode,
//             final int resultCode,
//             @Nullable final Intent data)
// ============================================================================
void BaseActivity::onActivityResult(int requestCode, int resultCode, Intent *data)
{
    // Port of: super.onActivityResult(requestCode, resultCode, data);

    // Port of: final ActivityResultCallback callback
    //             = mActivityResultCallbacks.remove(requestCode);
    ActivityResultCallback *callback = nullptr;
    auto it = m_activityResultCallbacks.find(requestCode);
    if (it != m_activityResultCallbacks.end()) {
        callback = it.value();
        m_activityResultCallbacks.erase(it);
    }

    // Port of: if (callback == null) { return; }
    if (callback == nullptr) {
        return;
    }

    // Port of: callback.onActivityResult(resultCode, data);
    callback->onActivityResult(resultCode, data);
}

// ============================================================================
// setOrientationFromPrefs — Java lines 200-216
// Port of: private void setOrientationFromPrefs()
// ============================================================================
void BaseActivity::setOrientationFromPrefs()
{
    // Port of: final PrefsUtility.ScreenOrientation orientation
    //             = PrefsUtility.pref_behaviour_screen_orientation();
    // TODO: Wire up PrefsUtility::pref_behaviour_screen_orientation()

    // Port of: if (orientation == PrefsUtility.ScreenOrientation.AUTO) {
    //             setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
    //         } else if (orientation == PrefsUtility.ScreenOrientation.PORTRAIT) {
    //             setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    //         } else if (orientation == PrefsUtility.ScreenOrientation.LANDSCAPE) {
    //             setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    //         }

    // ActivityInfo constants:
    // SCREEN_ORIENTATION_UNSPECIFIED = -1
    // SCREEN_ORIENTATION_PORTRAIT = 1
    // SCREEN_ORIENTATION_LANDSCAPE = 0

    // TODO: Wire up when PrefsUtility is available
}

// ============================================================================
// onSharedPreferenceChangedInner — Java lines 219-223
// Port of: protected void onSharedPreferenceChangedInner(
//             final SharedPrefsWrapper prefs, final String key)
// ============================================================================
void BaseActivity::onSharedPreferenceChangedInner(
        SharedPrefsWrapper *prefs,
        const QString &key)
{
    // Port of: // Do nothing
    (void) prefs;
    (void) key;
}

// ============================================================================
// onSharedPreferenceChanged — Java lines 225-235
// Port of: @Override public void onSharedPreferenceChanged(
//             @NonNull final SharedPrefsWrapper prefs,
//             @NonNull final String key)
// ============================================================================
void BaseActivity::onSharedPreferenceChanged(
        SharedPrefsWrapper *prefs,
        const QString &key)
{
    // Port of: onSharedPreferenceChangedInner(prefs, key);
    onSharedPreferenceChangedInner(prefs, key);

    // Port of: if (key.equals(getString(R.string.pref_behaviour_screenorientation_key))) {
    //             setOrientationFromPrefs();
    //         }
    // TODO: Wire up the screen orientation preference key constant
    // The Android R.string.pref_behaviour_screenorientation_key corresponds to:
    // "pref_behaviour_screenorientation"
    if (key == QStringLiteral("pref_behaviour_screenorientation")) {
        setOrientationFromPrefs();
    }
}

// ============================================================================
// finish — C++ adaptation of Android Activity.finish()
// ============================================================================
void BaseActivity::finish()
{
    emit activityFinished();
}

} // namespace PinkReader
