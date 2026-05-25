// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/AndroidCommon.kt (lines 1-211)
// Ported from Kotlin object to C++

#pragma once

#include <functional>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>

namespace PinkReader {

// AndroidCommon.kt line 44: object AndroidCommon
class AndroidCommon {
public:
    // line 45-46: UI_THREAD_HANDLER
    // line 48-55: runOnUiThread
    // Simplified: posts runnable to main thread queue
    static void runOnUiThread(std::function<void()> runnable);

    // line 57-82: onTextChanged - simplified stub
    static void onTextChanged(
            const std::function<void()>& action);

    // line 85-104: onSelectedItemChanged - simplified stub
    static void onSelectedItemChanged(
            const std::function<void()>& action);

    // line 106-122: getPackageInfo
    struct PackageInfo {
        std::string packageName;
        int versionCode = 0;
        std::string versionName;
        std::vector<std::vector<uint8_t>> ids;
    };

    static PackageInfo getPackageInfo();

    // line 124-129: class PackageInfo (inner class, defined above)

    // line 131-146: setAutoCompleteTextViewItemsNoFilter - stub
    static void setAutoCompleteTextViewItemsNoFilter(
            const std::vector<std::string>& items);

    // line 148-200: promptForNotificationPermission - stub
    static void promptForNotificationPermission(
            const std::function<void()>& onDisabled = nullptr);

    // line 202-210: removeClickListeners - stub
    static void removeClickListeners();

    // Main thread processing (called by platform loop)
    static void processMainThreadQueue();

private:
    static inline std::mutex sMainThreadMutex;
    static inline std::queue<std::function<void()>> sMainThreadQueue;

    static bool isUIThread();
};

} // namespace PinkReader
