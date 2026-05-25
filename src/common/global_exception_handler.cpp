// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/GlobalExceptionHandler.kt (lines 1-115)
// Implementation

#include "global_exception_handler.h"
#include <fstream>
#include <filesystem>

namespace PinkReader {

// line 59-61: private fun getFileLocation(context: Context): File
std::string GlobalExceptionHandler::getFileLocation(const std::string& filesDir) {
    // Origin line 60: return File(context.filesDir, "unhandled_exception.txt")
    return filesDir + "/unhandled_exception.txt";
}

// line 63-65: fun init(context: Context)
void GlobalExceptionHandler::init(const std::string& filesDir) {
    // Origin line 64: Thread.setDefaultUncaughtExceptionHandler(GlobalExceptionHandler(context))
    sFileLocation = getFileLocation(filesDir);
}

// line 38-54: override fun uncaughtException(t: Thread, e: Throwable)
void GlobalExceptionHandler::uncaughtException(
        const std::string& message,
        const std::string& stackTrace) {
    // Origin line 39: Log.e(TAG, "Got unhandled exception", e)
    // Origin lines 41-46: PrintWriter(FileWriter(mFile, true)).use { writer -> ... }
    try {
        std::ofstream writer(sFileLocation, std::ios::app);
        writer << "--- Begin stack trace ---" << std::endl;
        writer << message << std::endl;
        writer << stackTrace << std::endl;
        writer << "--- End stack trace ---" << std::endl;
        writer.flush();
    } catch (...) {
        // Origin line 48: Log.e(TAG, "Failed writing exception to disk", e2)
    }
}

// line 67-113: fun handleLastCrash(activity: BaseActivity)
void GlobalExceptionHandler::handleLastCrash(const std::string& activityContext) {
    // Origin line 69: checkThisIsUIThread()
    // Origin line 70-73: if (sCheckedForLastCrash) return; sCheckedForLastCrash = true;
    if (sCheckedForLastCrash) {
        return;
    }
    sCheckedForLastCrash = true;

    // Origin line 74: val file = getFileLocation(activity)
    std::string filePath = sFileLocation;

    // Origin line 77-78: if (!file.exists()) return@Runnable
    if (!std::filesystem::exists(filePath)) {
        return;
    }

    // Origin lines 81-83: FileInputStream(file).use { inStr -> fileText = readWholeStreamAsUTF8(inStr) }
    std::string fileText;
    try {
        std::ifstream inStr(filePath, std::ios::binary);
        fileText.assign(std::istreambuf_iterator<char>(inStr),
                        std::istreambuf_iterator<char>());
    } catch (...) {
        // Origin line 84: Log.e(TAG, "Got exception when reading file", e)
        return;
    }

    // Origin line 87-89: if (!file.delete()) Log.e(TAG, "Unable to delete file")
    std::filesystem::remove(filePath);

    // Origin line 90-111: runOnUiThread { DialogUtils.showDialogPositiveNegative(...) }
    // Stub: In C++ port, crash reporting is done differently
    (void)activityContext;
    (void)fileText;
}

} // namespace PinkReader
