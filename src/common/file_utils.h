// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/FileUtils.java (lines 1-718)
// Ported from Java to C++

#pragma once

#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <istream>
#include <ostream>

namespace PinkReader {

// FileUtils.java line 64: public class FileUtils
class FileUtils {
public:
    // line 66: private static final String TAG = "FileUtils";
    static constexpr const char* TAG = "FileUtils";

    // line 161-174: getExtensionForMimetype
    // Origin: @NonNull public static Optional<String> getExtensionForMimetype(@NonNull final String mimetype)
    static std::optional<std::string> getExtensionForMimetype(const std::string& mimetype);

    // line 176-186: moveFile
    static void moveFile(const std::string& src, const std::string& dst);

    // line 188-193: copyFile(File, File)
    static void copyFile(const std::string& src, const std::string& dst);

    // line 195-200: copyFile(InputStream, File)
    static void copyStreamToFile(std::istream& input, const std::string& dst);

    // line 202-205: isCacheDiskFull
    static bool isCacheDiskFull(const std::string& cachePath);

    // line 208-213: getFreeSpaceAvailable
    static long long getFreeSpaceAvailable(const std::string& path);

    // line 654-674: getExtensionFromPath
    // Origin: @NonNull public static Optional<String> getExtensionFromPath(@NonNull final String path)
    static std::optional<std::string> getExtensionFromPath(const std::string& path);

    // line 676-688: buildPath
    // Origin: @NonNull public static File buildPath(@NonNull final File base, @NonNull final String... components)
    static std::string buildPath(
            const std::string& base,
            const std::vector<std::string>& components);

    // line 693-717: mkdirs
    static void mkdirs(const std::string& path);

    // line 467-472: DownloadImageToSaveSuccessCallback
    using DownloadImageToSaveSuccessCallback = std::function<void(
            const std::string& info,
            const std::string& cacheFileId,
            const std::string& mimetype)>;

    // line 580-652: downloadImageToSave - stub
    static void downloadImageToSave(
            const std::string& uri,
            DownloadImageToSaveSuccessCallback callback);

    // line 215-251: shareImageAtUri - stub
    static void shareImageAtUri(const std::string& uri);

    // line 381-450: saveImageAtUri - stub
    enum class SaveLocation {
        PROMPT_EVERY_TIME,
        SYSTEM_DEFAULT
    };
    static void saveImageAtUri(const std::string& uri, SaveLocation location);

    // line 452-465: showUnexpectedStorageErrorDialog - stub
    static void showUnexpectedStorageErrorDialog(
            const std::string& message);

    // line 253-276: private interface/class FileDataSource
    using FileDataSource = std::function<void(std::ostream&)>;

private:
    FileUtils() = default;

    // line 68: private static final HashMap<String, String> MIMETYPE_TO_EXTENSION
    static const std::unordered_map<std::string, std::string>& mimetypeToExtension();

    // line 691: private static final Object sMkdirsLock
    static inline std::mutex sMkdirsMutex;
};

} // namespace PinkReader
