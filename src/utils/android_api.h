/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: android_api.h - Port of AndroidApi.java
 *
 * NOTE: AndroidApi.java does not exist in the redreader source tree.
 * This file provides a port of equivalent Android API utilities found in
 * AndroidCommon.kt (the closest match in the original codebase).
 *
 * Port of: org.quantumbadger.redreader.common.AndroidCommon (Kotlin object)
 * Provides cross-platform Android API abstractions.
 *
 * Every method from AndroidCommon.kt is ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <functional>
#include <memory>
#include <vector>

namespace PinkReader {

// ============================================================================
// Forward declarations
// ============================================================================

class Context;
class Runnable;
class TextView;
class MaterialAutoCompleteTextView;
class AdapterView;
class BaseActivity;
class Handler;

// ============================================================================
// Handler stub (port of android.os.Handler)
// ============================================================================

class Handler {
public:
    virtual ~Handler() = default;

    // Port of: handler.post(runnable)
    void post(const std::function<void()> &runnable);

    // Port of: handler.postDelayed(runnable, delayMillis)
    void postDelayed(const std::function<void()> &runnable, int64_t delayMillis);

    // Port of: Handler(Looper.getMainLooper())
    static Handler *forMainLooper();

    // Port of: handler.sendMessage(msg)
    void sendMessage(class Message *msg);

private:
    std::vector<std::function<void()>> m_queue;
};

// ============================================================================
// Looper stub (port of android.os.Looper)
// ============================================================================

class Looper {
public:
    // Port of: Looper.getMainLooper()
    static class MainLooper *getMainLooper();
};

class MainLooper {
};

// ============================================================================
// Message stub (port of android.os.Message)
// ============================================================================

class Message {
public:
    int what = 0;
    void *obj = nullptr;

    // Port of: Message.obtain()
    static Message *obtain();

    // Port of: General.handlerMessage(what, obj)
    static Message *handlerMessage(int what, void *obj);
};

// ============================================================================
// Runnable interface (port of java.lang.Runnable)
// ============================================================================

class Runnable {
public:
    virtual ~Runnable() = default;
    virtual void run() = 0;
};

// ============================================================================
// TextWatcher interface (port of android.text.TextWatcher)
// ============================================================================

class TextWatcher {
public:
    virtual ~TextWatcher() = default;

    // Port of: beforeTextChanged(CharSequence s, int start, int count, int after)
    virtual void beforeTextChanged(const QString &s, int start, int count, int after) = 0;

    // Port of: onTextChanged(CharSequence s, int start, int before, int count)
    virtual void onTextChanged(const QString &s, int start, int before, int count) = 0;

    // Port of: afterTextChanged(Editable s)
    virtual void afterTextChanged(const QString &s) = 0;
};

// ============================================================================
// OnItemSelectedListener interface (port of AdapterView.OnItemSelectedListener)
// ============================================================================

class OnItemSelectedListener {
public:
    virtual ~OnItemSelectedListener() = default;

    // Port of: onItemSelected(AdapterView parent, View view, int position, long id)
    virtual void onItemSelected(void *parent, void *view, int position, int64_t id) = 0;

    // Port of: onNothingSelected(AdapterView parent)
    virtual void onNothingSelected(void *parent) = 0;
};

// ============================================================================
// PackageInfo — port of AndroidCommon.PackageInfo (Java lines 124-129)
// ============================================================================

class PackageInfo {
public:
    // Port of: val packageName: String
    QString packageName;

    // Port of: val versionCode: Int
    int versionCode = 0;

    // Port of: val versionName: String
    QString versionName;

    // Port of: val ids: List<ByteArray>
    std::vector<std::vector<uint8_t>> ids;
};

// ============================================================================
// AndroidApi — port of AndroidCommon.kt (Kotlin object)
//
// Port of: object AndroidCommon { ... }
// Every @JvmStatic method and @JvmField ported exactly.
// ============================================================================

class AndroidApi : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Fields
    // ========================================================================

    // Port of: @JvmField val UI_THREAD_HANDLER = Handler(Looper.getMainLooper()) (line 45-46)
    static Handler *UI_THREAD_HANDLER();

    // ========================================================================
    // Static methods
    // ========================================================================

    // Port of: fun runOnUiThread(runnable: Runnable) (lines 49-55)
    static void runOnUiThread(Runnable *runnable);

    // Port of: fun onTextChanged(textBox: TextView, action: Runnable) (lines 57-83)
    static void onTextChanged(TextView *textBox, Runnable *action);

    // Port of: fun onSelectedItemChanged(view: AdapterView<*>, action: Runnable) (lines 85-104)
    static void onSelectedItemChanged(AdapterView *view, Runnable *action);

    // Port of: fun getPackageInfo(context: Context): PackageInfo (lines 107-122)
    static PackageInfo getPackageInfo(Context *context);

    // Port of: fun setAutoCompleteTextViewItemsNoFilter(
    //     view: MaterialAutoCompleteTextView, items: List<String>) (lines 131-138)
    static void setAutoCompleteTextViewItemsNoFilter(
            MaterialAutoCompleteTextView *view,
            const std::vector<QString> &items);

    // Port of: fun setAutoCompleteTextViewItemsNoFilter(
    //     view: MaterialAutoCompleteTextView, items: Array<String>) (lines 140-146)
    static void setAutoCompleteTextViewItemsNoFilter(
            MaterialAutoCompleteTextView *view,
            const std::vector<QString> &items);

    // Port of: fun promptForNotificationPermission(
    //     activity: BaseActivity, onDisabled: Runnable? = null) (lines 148-200)
    static void promptForNotificationPermission(
            BaseActivity *activity,
            Runnable *onDisabled = nullptr);

    // Port of: fun removeClickListeners(view: View) (lines 202-210)
    static void removeClickListeners(QWidget *view);

    // ========================================================================
    // General utility methods (ported from General.kt for convenience)
    // ========================================================================

    // Port of: General.dpToPixels(context, dp)
    static int dpToPixels(Context *context, float dp);

    // Port of: General.isThisUIThread
    static bool isThisUIThread();

private:
    static Handler *s_uiThreadHandler;

    // Port of: General.kt mPrefs
    static class SharedPrefsWrapper *getSharedPrefs(Context *context);
};

// ============================================================================
// SharedPrefsWrapper stub (port from General.kt)
// ============================================================================

class SharedPrefsWrapper {
public:
    QString getString(const QString &key, const QString &defaultValue) const;
    void setString(const QString &key, const QString &value);
    bool getBoolean(const QString &key, bool defaultValue) const;
    void setBoolean(const QString &key, bool value);
    int getInt(const QString &key, int defaultValue) const;
    void setInt(const QString &key, int value);
};

} // namespace PinkReader
