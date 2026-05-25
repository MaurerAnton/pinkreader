/*
 * PinkReader - GPLv3
 * File: base_activity.h
 * Exact port of BaseActivity.java (236 lines)
 *
 * Ports every field, method, interface, and logic branch exactly.
 * Android AppCompatActivity adapted to QObject.
 * Android-specific APIs (WindowManager, PackageManager, Build, etc.)
 * are forward-declared and stubbed.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QHash>
#include <atomic>
#include <functional>
#include <memory>

namespace PinkReader {

// Forward declarations — Android/RedReader types used in signatures
class SharedPrefsWrapper;
class Intent;
class Bundle;

/**
 * @brief Port of org.quantumbadger.redreader.activities.BaseActivity
 *
 * Abstract base class for all activities. Manages shared preferences,
 * permissions, activity results, orientation, and window flags.
 * Implements SharedPrefsWrapper.OnSharedPreferenceChangeListener.
 *
 * Every field, method, interface, and inner interface ported exactly.
 */
class BaseActivity : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Inner interface: PermissionCallback — Java lines 56-60
    // Port of: public interface PermissionCallback
    // ========================================================================
    class PermissionCallback {
    public:
        virtual ~PermissionCallback() = default;

        // Port of: void onPermissionGranted();
        virtual void onPermissionGranted() = 0;

        // Port of: void onPermissionDenied();
        virtual void onPermissionDenied() = 0;
    };

    // ========================================================================
    // Inner interface: ActivityResultCallback — Java lines 62-64
    // Port of: public interface ActivityResultCallback
    // ========================================================================
    class ActivityResultCallback {
    public:
        virtual ~ActivityResultCallback() = default;

        // Port of: void onActivityResult(int resultCode, @Nullable Intent data);
        virtual void onActivityResult(int resultCode, Intent *data) = 0;
    };

    // ========================================================================
    // Constructor / Destructor
    // ========================================================================
    explicit BaseActivity(QObject *parent = nullptr);
    ~BaseActivity() override;

    // ========================================================================
    // closeAllExceptMain — Java lines 66-69
    // Port of: public void closeAllExceptMain()
    // ========================================================================
    /**
     * @brief Sets the closingAll flag and calls closeIfNecessary().
     *
     * Port of:
     *   closingAll = true;
     *   closeIfNecessary();
     */
    void closeAllExceptMain();

    // ========================================================================
    // onCreate — Java lines 72-98
    // Port of: @Override protected void onCreate(final Bundle savedInstanceState)
    // ========================================================================
    /**
     * @brief Activity creation lifecycle method.
     *
     * Sets up shared preferences, window flags (fullscreen, secure, keep-awake),
     * orientation, and registers preference change listener.
     *
     * Port of Java lines 72-98 exactly.
     */
    void onCreate(Bundle *savedInstanceState);

    // ========================================================================
    // onResume — Java lines 100-106
    // Port of: @Override protected void onResume()
    // ========================================================================
    /**
     * @brief Activity resume lifecycle method.
     *
     * Resets orientation, calls closeIfNecessary(), and updates Tor status.
     *
     * Port of Java lines 100-106 exactly.
     */
    void onResume();

    // ========================================================================
    // onDestroy — Java lines 109-113
    // Port of: @Override protected void onDestroy()
    // ========================================================================
    /**
     * @brief Activity destroy lifecycle method.
     *
     * Unregisters the shared preference change listener.
     *
     * Port of Java lines 109-113 exactly.
     */
    void onDestroy();

    // ========================================================================
    // requestPermissionWithCallback — Java lines 125-145
    // Port of: public final void requestPermissionWithCallback(
    //             @NonNull final String permission,
    //             @NonNull final PermissionCallback callback)
    // ========================================================================
    /**
     * @brief Requests a runtime permission with a callback.
     *
     * On Android M+, checks if permission is already granted. If not,
     * stores the callback and calls requestPermissions().
     * On pre-M, immediately calls onPermissionGranted().
     *
     * Port of Java lines 125-145 exactly.
     */
    void requestPermissionWithCallback(
        const QString &permission,
        PermissionCallback *callback);

    // ========================================================================
    // onRequestPermissionsResult — Java lines 147-171
    // Port of: @Override public final void onRequestPermissionsResult(
    //             final int requestCode,
    //             @NonNull final String[] permissions,
    //             @NonNull final int[] grantResults)
    // ========================================================================
    /**
     * @brief Handles the result of a permission request.
     *
     * Looks up and removes the callback from mPermissionRequestCallbacks,
     * then calls onPermissionGranted() or onPermissionDenied().
     *
     * Port of Java lines 147-171 exactly.
     */
    void onRequestPermissionsResult(
        int requestCode,
        const QStringList &permissions,
        const QList<int> &grantResults);

    // ========================================================================
    // startActivityForResultWithCallback — Java lines 173-180
    // Port of: public final void startActivityForResultWithCallback(
    //             @NonNull final Intent intent,
    //             @NonNull final ActivityResultCallback callback)
    // ========================================================================
    /**
     * @brief Starts an activity for result with a callback.
     *
     * Generates a request code, stores the callback, and starts the activity.
     *
     * Port of Java lines 173-180 exactly.
     */
    void startActivityForResultWithCallback(
        Intent *intent,
        ActivityResultCallback *callback);

    // ========================================================================
    // onActivityResult — Java lines 182-198
    // Port of: @Override protected final void onActivityResult(
    //             final int requestCode,
    //             final int resultCode,
    //             @Nullable final Intent data)
    // ========================================================================
    /**
     * @brief Handles the result of a started activity.
     *
     * Looks up and removes the callback from mActivityResultCallbacks,
     * then calls onActivityResult().
     *
     * Port of Java lines 182-198 exactly.
     */
    void onActivityResult(int requestCode, int resultCode, Intent *data);

    // ========================================================================
    // onSharedPreferenceChanged — Java lines 225-235
    // Port of: @Override public void onSharedPreferenceChanged(
    //             @NonNull final SharedPrefsWrapper prefs,
    //             @NonNull final String key)
    // ========================================================================
    /**
     * @brief Handles shared preference changes.
     *
     * Calls onSharedPreferenceChangedInner(), then checks if the
     * screen orientation key changed and updates orientation if so.
     *
     * Port of Java lines 225-235 exactly.
     */
    void onSharedPreferenceChanged(SharedPrefsWrapper *prefs, const QString &key);

protected:
    // ========================================================================
    // onSharedPreferenceChangedInner — Java lines 219-223
    // Port of: protected void onSharedPreferenceChangedInner(
    //             final SharedPrefsWrapper prefs, final String key)
    // ========================================================================
    /**
     * @brief Hook for subclasses to handle preference changes.
     *
     * Port of Java lines 219-223 exactly: default implementation does nothing.
     */
    virtual void onSharedPreferenceChangedInner(
        SharedPrefsWrapper *prefs,
        const QString &key);

    // ========================================================================
    // closeIfNecessary — Java lines 115-123
    // Port of: private void closeIfNecessary()
    // ========================================================================
    /**
     * @brief Closes this activity if closingAll is true, unless it's MainActivity.
     *
     * Port of:
     *   if (closingAll) {
     *       if (this instanceof MainActivity) {
     *           closingAll = false;
     *       } else {
     *           finish();
     *       }
     *   }
     */
    virtual void closeIfNecessary();

    // ========================================================================
    // setOrientationFromPrefs — Java lines 200-216
    // Port of: private void setOrientationFromPrefs()
    // ========================================================================
    /**
     * @brief Sets the activity orientation based on preferences.
     *
     * Port of Java lines 200-216 exactly:
     *   AUTO → SCREEN_ORIENTATION_UNSPECIFIED
     *   PORTRAIT → SCREEN_ORIENTATION_PORTRAIT
     *   LANDSCAPE → SCREEN_ORIENTATION_LANDSCAPE
     */
    virtual void setOrientationFromPrefs();

    // ========================================================================
    // finish — C++ adaptation of Android Activity.finish()
    // ========================================================================
    /**
     * @brief Finishes/closes this activity.
     *
     * In Android, this calls Activity.finish(). In C++, emits a signal
     * that can be connected to close the window.
     */
    virtual void finish();

signals:
    /**
     * @brief Emitted when the activity should be closed.
     *
     * C++ adaptation of Activity.finish().
     */
    void activityFinished();

private:
    // ========================================================================
    // Fields — port exact from Java
    // ========================================================================

    // Port of: private SharedPrefsWrapper mSharedPreferences;
    SharedPrefsWrapper *m_sharedPreferences = nullptr;

    // Port of: private static boolean closingAll = false;
    // Note: Java uses a static field. Since C++ activities may run in
    // different processes/threads, this remains static for cross-activity sharing.
    static bool s_closingAll;

    // Port of: private final AtomicInteger mRequestIdGenerator
    //         = new AtomicInteger(10_000);
    std::atomic<int> m_requestIdGenerator{10000};

    // Port of: private final HashMap<Integer, PermissionCallback>
    //         mPermissionRequestCallbacks = new HashMap<>();
    QHash<int, PermissionCallback *> m_permissionRequestCallbacks;

    // Port of: private final HashMap<Integer, ActivityResultCallback>
    //         mActivityResultCallbacks = new HashMap<>();
    QHash<int, ActivityResultCallback *> m_activityResultCallbacks;

    // ========================================================================
    // isMainActivity — helper for the "this instanceof MainActivity" check
    // ========================================================================
    /**
     * @brief Returns true if this activity is the main activity.
     *
     * Port of: this instanceof MainActivity
     * Subclasses override this to return true/false appropriately.
     */
    virtual bool isMainActivity() const { return false; }
};

} // namespace PinkReader
