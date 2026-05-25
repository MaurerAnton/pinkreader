// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/FileUtils.java (lines 1-718)
// Implementation

#include "file_utils.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace PinkReader {

// line 68-159: MIMETYPE_TO_EXTENSION static initialization
const std::unordered_map<std::string, std::string>& FileUtils::mimetypeToExtension() {
    // Origin lines 70-159: all static put() calls
    static const std::unordered_map<std::string, std::string> map = {
        {"audio/3gpp2", "3g2"}, {"video/3gpp2", "3g2"},
        {"audio/3gpp", "3gp"}, {"video/3gpp", "3gp"},
        {"application/x-7z-compressed", "7z"}, {"audio/aac", "aac"},
        {"application/x-abiword", "abw"}, {"application/x-freearc", "arc"},
        {"video/x-msvideo", "avi"}, {"application/vnd.amazon.ebook", "azw"},
        {"application/octet-stream", "bin"}, {"image/bmp", "bmp"},
        {"application/x-bzip2", "bz2"}, {"application/x-bzip", "bz"},
        {"application/x-csh", "csh"}, {"text/css", "css"},
        {"text/csv", "csv"}, {"application/msword", "doc"},
        {"application/vnd.openxmlformats-officedocument.wordprocessingml.document", "docx"},
        {"application/vnd.ms-fontobject", "eot"}, {"application/epub+zip", "epub"},
        {"image/gif", "gif"}, {"application/gzip", "gz"},
        {"video/h263", "h263"}, {"video/h264", "h264"},
        {"video/h265", "h265"}, {"image/heic ", "heic"},
        {"image/heic-sequence ", "heic"}, {"image/heif ", "heif"},
        {"image/heif-sequence", "heif"}, {"text/html", "html"},
        {"image/vnd.microsoft.icon", "ico"}, {"text/calendar", "ics"},
        {"application/java-archive", "jar"}, {"image/jpeg", "jpg"},
        {"application/json", "json"}, {"application/ld+json", "jsonld"},
        {"text/javascript", "js"}, {"audio/midi audio/x-midi", "mid"},
        {"audio/mpeg", "mp3"}, {"video/mp4", "mp4"},
        {"application/dash+xml", "mpd"}, {"video/mpeg", "mpeg"},
        {"application/vnd.apple.installer+xml", "mpkg"}, {"video/mpv", "mpv"},
        {"application/vnd.oasis.opendocument.presentation", "odp"},
        {"application/vnd.oasis.opendocument.spreadsheet", "ods"},
        {"application/vnd.oasis.opendocument.text", "odt"},
        {"audio/ogg", "oga"}, {"video/ogg", "ogv"},
        {"application/ogg", "ogx"}, {"audio/opus", "opus"},
        {"font/otf", "otf"}, {"application/pdf", "pdf"},
        {"application/x-httpd-php", "php"}, {"image/png", "png"},
        {"application/vnd.ms-powerpoint", "ppt"},
        {"application/vnd.openxmlformats-officedocument.presentationml.presentation", "pptx"},
        {"application/vnd.rar", "rar"}, {"application/rtf", "rtf"},
        {"application/x-sh", "sh"}, {"image/svg+xml", "svg"},
        {"application/x-shockwave-flash", "swf"}, {"application/x-tar", "tar"},
        {"image/tiff", "tiff"}, {"video/mp2t", "ts"},
        {"font/ttf", "ttf"}, {"text/plain", "txt"},
        {"application/vnd.visio", "vsd"}, {"audio/wav", "wav"},
        {"audio/webm", "weba"}, {"video/webm", "webm"},
        {"image/webp", "webp"}, {"font/woff2", "woff2"},
        {"font/woff", "woff"}, {"application/xhtml+xml", "xhtml"},
        {"application/vnd.ms-excel", "xls"},
        {"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", "xlsx"},
        {"application/xml", "xml"}, {"text/xml", "xml"},
        {"application/vnd.mozilla.xul+xml", "xul"}, {"application/zip", "zip"}
    };
    return map;
}

// line 161-174: getExtensionForMimetype
std::optional<std::string> FileUtils::getExtensionForMimetype(const std::string& mimetype) {
    // line 164-169: split on ';'
    std::string splitType;
    size_t semicolon = mimetype.find(';');
    if (semicolon != std::string::npos) {
        splitType = mimetype.substr(0, semicolon);
    } else {
        splitType = mimetype;
    }

    // line 172-173: return Optional.ofNullable(MIMETYPE_TO_EXTENSION.get(StringUtils.asciiLowercase(splitType)));
    std::string lower = splitType;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    const auto& map = mimetypeToExtension();
    auto it = map.find(lower);
    if (it != map.end()) {
        return it->second;
    }
    return std::nullopt;
}

// line 176-186: moveFile
void FileUtils::moveFile(const std::string& src, const std::string& dst) {
    // Origin line 178: if(!src.renameTo(dst))
    std::error_code ec;
    std::filesystem::rename(src, dst, ec);
    if (ec) {
        // Origin line 180: copyFile(src, dst);
        copyFile(src, dst);
        // Origin line 182-184: if(!src.delete()) src.deleteOnExit();
        std::filesystem::remove(src, ec);
    }
}

// line 188-193: copyFile(File, File)
void FileUtils::copyFile(const std::string& src, const std::string& dst) {
    // Origin line 190: try(FileInputStream fis = new FileInputStream(src))
    std::ifstream fis(src, std::ios::binary);
    copyStreamToFile(fis, dst);
}

// line 195-200: copyFile(InputStream, File)
void FileUtils::copyStreamToFile(std::istream& input, const std::string& dst) {
    // Origin line 196: try(FileOutputStream fos = new FileOutputStream(dst))
    std::ofstream fos(dst, std::ios::binary);
    // Origin line 197: General.copyStream(fis, fos);
    fos << input.rdbuf();
    // Origin line 198: fos.flush();
    fos.flush();
}

// line 202-205: isCacheDiskFull
bool FileUtils::isCacheDiskFull(const std::string& cachePath) {
    // Origin line 203: final long space = getFreeSpaceAvailable(PrefsUtility.pref_cache_location(context));
    // Origin line 204: return space < 128 * 1024 * 1024;
    long long space = getFreeSpaceAvailable(cachePath);
    return space < 128LL * 1024 * 1024;
}

// line 208-213: getFreeSpaceAvailable
long long FileUtils::getFreeSpaceAvailable(const std::string& path) {
    // Origin: final StatFs stat = new StatFs(path);
    // Origin: final long availableBlocks = stat.getAvailableBlocksLong();
    // Origin: final long blockSize = stat.getBlockSizeLong();
    // Origin: return availableBlocks * blockSize;
    std::error_code ec;
    auto space = std::filesystem::space(path, ec);
    if (ec) {
        return -1;
    }
    return static_cast<long long>(space.available);
}

// line 654-674: getExtensionFromPath
std::optional<std::string> FileUtils::getExtensionFromPath(const std::string& path) {
    // line 657: final String[] pathSegments = path.split("/");
    size_t lastSlash = path.rfind('/');
    std::string filename = (lastSlash != std::string::npos)
        ? path.substr(lastSlash + 1) : path;

    // line 659-661: if(pathSegments.length == 0) return Optional.empty();
    if (filename.empty()) {
        return std::nullopt;
    }

    // line 663: final String[] dotSegments = pathSegments[pathSegments.length - 1].split("\\.");
    size_t lastDot = filename.rfind('.');
    // line 665-667: if(dotSegments.length < 2) return Optional.empty();
    if (lastDot == std::string::npos) {
        return std::nullopt;
    }

    // line 669-671: if(dotSegments.length == 2 && dotSegments[0].isEmpty()) return Optional.empty();
    if (lastDot == 0) {
        return std::nullopt;
    }

    // line 673: return Optional.of(dotSegments[dotSegments.length - 1]);
    return filename.substr(lastDot + 1);
}

// line 676-688: buildPath
std::string FileUtils::buildPath(
        const std::string& base,
        const std::vector<std::string>& components) {
    // Origin lines 681-687:
    // File result = base;
    // for(final String component : components) { result = new File(result, component); }
    // return result;
    std::string result = base;
    for (const auto& component : components) {
        if (!result.empty() && result.back() != '/') {
            result += '/';
        }
        result += component;
    }
    return result;
}

// line 693-717: mkdirs
void FileUtils::mkdirs(const std::string& path) {
    // Origin line 695: synchronized(sMkdirsLock)
    std::lock_guard<std::mutex> lock(sMkdirsMutex);

    // Origin line 697-699: if(file.isDirectory()) return;
    std::error_code ec;
    if (std::filesystem::is_directory(path, ec)) {
        return;
    }

    // Origin lines 701-709/711-714: Files.createDirectories or file.mkdirs()
    std::filesystem::create_directories(path, ec);
    if (ec) {
        // Origin line 706-708: throw new IOException("Failed to create dirs " + file.getAbsolutePath(), e);
        throw std::runtime_error("Failed to create dirs " + path);
    }
}

// line 580-652: downloadImageToSave - stub
void FileUtils::downloadImageToSave(
        const std::string& uri,
        DownloadImageToSaveSuccessCallback callback) {
    // Origin: LinkHandler.getImageInfo(activity, uri, ...)
    // Origin: CacheManager.getInstance(activity).makeRequest(...)
    // Simplified: stub for non-Android C++ port
    (void)uri;
    if (callback) {
        callback("", "", "");
    }
}

// line 215-251: shareImageAtUri - stub
void FileUtils::shareImageAtUri(const std::string& uri) {
    // Origin: downloadImageToSave then Intent.ACTION_SEND
    // Simplified: stub for non-Android C++ port
    (void)uri;
}

// line 381-450: saveImageAtUri - stub
void FileUtils::saveImageAtUri(const std::string& uri, SaveLocation location) {
    // Origin: switch on saveLocation for PROMPT_EVERY_TIME, SYSTEM_DEFAULT
    // Simplified: stub for non-Android C++ port
    (void)uri;
    (void)location;
}

// line 452-465: showUnexpectedStorageErrorDialog - stub
void FileUtils::showUnexpectedStorageErrorDialog(const std::string& message) {
    // Origin: General.showResultDialog(activity, new RRError(...))
    // Simplified: stub for non-Android C++ port
    (void)message;
}

} // namespace PinkReader
