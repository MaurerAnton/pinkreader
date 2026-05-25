// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/AndroidCommon.kt (lines 1-211)
// Implementation

#include "android_common.h"

namespace PinkReader {

// line 48-55: runOnUiThread
void AndroidCommon::runOnUiThread(std::function<void()> runnable) {
    // Origin line 50: if (General.isThisUIThread) { runnable.run() }
    if (isUIThread()) {
        runnable();
    } else {
        // Origin line 53: UI_THREAD_HANDLER.post(runnable)
        std::lock_guard<std::mutex> lock(sMainThreadMutex);
        sMainThreadQueue.push(std::move(runnable));
    }
}

// line 57-82: onTextChanged
void AndroidCommon::onTextChanged(const std::function<void()>& action) {
    // Origin: textBox.addTextChangedListener(...)
    // Simplified: stub for non-Android C++ port
    if (action) {
        action();
    }
}

// line 85-104: onSelectedItemChanged
void AndroidCommon::onSelectedItemChanged(const std::function<void()>& action) {
    // Origin: view.onItemSelectedListener = ...
    // Simplified: stub for non-Android C++ port
    if (action) {
        action();
    }
}

// line 106-122: getPackageInfo
AndroidCommon::PackageInfo AndroidCommon::getPackageInfo() {
    // Origin: context.packageManager.getPackageInfo(...) with signature fetching
    // Simplified: stub for non-Android C++ port
    PackageInfo info;
    info.packageName = "org.quantumbadger.pinkreader";
    info.versionCode = 1;
    info.versionName = "0.1.0";
    return info;
}

// line 131-146: setAutoCompleteTextViewItemsNoFilter
void AndroidCommon::setAutoCompleteTextViewItemsNoFilter(
        const std::vector<std::string>& items) {
    // Origin: view.setSimpleItems(items.toTypedArray())
    //         view.setAdapter(NoFilterAdapter(...))
    // Simplified: stub for non-Android C++ port
    (void)items;
}

// line 148-200: promptForNotificationPermission
void AndroidCommon::promptForNotificationPermission(
        const std::function<void()>& onDisabled) {
    // Origin: checks SDK version, prefs, account, permission,
    //         shows dialog for POST_NOTIFICATIONS
    // Simplified: stub for non-Android C++ port
    if (onDisabled) {
        onDisabled();
    }
}

// line 202-210: removeClickListeners
void AndroidCommon::removeClickListeners() {
    // Origin: view.setOnClickListener(null);
    //         view.setOnLongClickListener(null);
    //         view.isClickable = false;
    //         view.isLongClickable = false;
    // Simplified: stub for non-Android C++ port
}

bool AndroidCommon::isUIThread() {
    // Simplified: always returns true in non-threaded context
    return true;
}

void AndroidCommon::processMainThreadQueue() {
    std::unique_lock<std::mutex> lock(sMainThreadMutex);
    while (!sMainThreadQueue.empty()) {
        auto task = std::move(sMainThreadQueue.front());
        sMainThreadQueue.pop();
        lock.unlock();
        task();
        lock.lock();
    }
}

} // namespace PinkReader
