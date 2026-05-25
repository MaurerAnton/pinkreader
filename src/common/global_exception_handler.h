// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/GlobalExceptionHandler.kt (lines 1-115)
// Ported from Kotlin to C++

#pragma once

#include <string>

namespace PinkReader {

// GlobalExceptionHandler.kt line 29-30:
// class GlobalExceptionHandler private constructor(context: Context) :
//     Thread.UncaughtExceptionHandler
class GlobalExceptionHandler {
public:
    // line 56-57: companion object
    // line 63-65: fun init(context: Context)
    static void init(const std::string& filesDir);

    // line 67-113: fun handleLastCrash(activity: BaseActivity)
    static void handleLastCrash(const std::string& activityContext);

private:
    // line 59-61: private fun getFileLocation(context: Context): File
    static std::string getFileLocation(const std::string& filesDir);

    // line 38-54: override fun uncaughtException(t: Thread, e: Throwable)
    static void uncaughtException(const std::string& message, const std::string& stackTrace);

    // line 57: private const val TAG = "GlobalExceptionHandler"
    static constexpr const char* TAG = "GlobalExceptionHandler";

    // line 58: private var sCheckedForLastCrash = false
    static inline bool sCheckedForLastCrash = false;
    static inline std::string sFileLocation;
};

} // namespace PinkReader
