// Origin: org/quantumbadger/redreader/cache/CacheContentProvider.java
#include "cache_content_provider.h"
#include "cache_manager_v2.h"
#include "cache_compression_type.h"

#include <cstdio>
#include <cctype>

namespace PinkReader {

const char* CacheContentProvider::TAG = "CacheContentProvider";
const std::string CacheContentProvider::COLUMNS[2] = {"_display_name", "_size"};

CacheContentProvider::CacheContentProvider()
	: m_cacheManager(nullptr) {}

std::string CacheContentProvider::generateFilename(
		int64_t cacheId,
		const std::string& mimetype,
		const std::string& defaultExtension) {

	// FileUtils.getExtensionForMimetype(mimetype).orElse(defaultExtension)
	std::string extension = defaultExtension;

	// return String.format(Locale.US, "redreader_dl_%d.%s", cacheId, extension);
	char buf[128];
	snprintf(buf, sizeof(buf), "redreader_dl_%lld.%s",
		static_cast<long long>(cacheId), extension.c_str());
	return std::string(buf);
}

std::optional<int64_t> CacheContentProvider::getCacheIdFromFilename(const std::string& filename) {
	// Split by dot
	size_t dot = filename.find_last_of('.');
	if(dot == std::string::npos) {
		// Log.e(TAG, "Expecting one dot in filename: " + filename);
		return std::nullopt;
	}

	std::string namePart = filename.substr(0, dot);
	std::string extPart = filename.substr(dot + 1);

	// Check for exactly 2 parts (one dot only)
	if(filename.find('.', dot + 1) != std::string::npos) {
		// More dots than expected
		return std::nullopt;
	}

	const char* prefix = "redreader_dl_";
	size_t prefixLen = 9; // strlen("redreader_dl_")

	if(namePart.length() < prefixLen || namePart.substr(0, prefixLen) != prefix) {
		// Log.e(TAG, "Expecting redreader_dl_ prefix in filename: " + filename);
		return std::nullopt;
	}

	std::string numStr = namePart.substr(prefixLen);

	try {
		return std::stoll(numStr);
	} catch(...) {
		// Log.e(TAG, "Invalid number in filename: " + filename, e);
		return std::nullopt;
	}
}

std::string CacheContentProvider::getUriForFile(
		int64_t cacheId,
		const std::string& mimetype,
		const std::string& defaultExtension) {

	// return new Uri.Builder()
	//     .scheme("content")
	//     .authority("org.quantumbadger.redreader.cacheprovider")
	//     .encodedPath(generateFilename(cacheId, mimetype, defaultExtension))
	//     .build();

	return "content://org.quantumbadger.redreader.cacheprovider/" +
		generateFilename(cacheId, mimetype, defaultExtension);
}

std::optional<CacheManager::ReadableCacheFile> CacheContentProvider::getReadableCacheFile(
		const std::string& uri) {

	// String filename = General.filenameFromString(uri.toString());
	size_t lastSlash = uri.find_last_of('/');
	std::string filename = (lastSlash != std::string::npos) ? uri.substr(lastSlash + 1) : uri;

	auto cacheId = getCacheIdFromFilename(filename);

	if(!cacheId.has_value()) {
		return std::nullopt;
	}

	// return Optional.of(m_cacheManager.getExistingCacheFileById(
	//     cacheId.get(), CacheCompressionType.NONE));
	if(m_cacheManager != nullptr) {
		return m_cacheManager->getExistingCacheFileById(
			cacheId.value(), CacheCompressionType::NONE);
	}

	return std::nullopt;
}

std::optional<std::string> CacheContentProvider::getFile(const std::string& uri) {
	auto readableCacheFile = getReadableCacheFile(uri);

	if(!readableCacheFile.has_value()) {
		return std::nullopt;
	}

	std::string path;
	if(readableCacheFile.value().getFile(path)) {
		return path;
	}

	return std::nullopt;
}

bool CacheContentProvider::onCreate() {
	// m_cacheManager = CacheManager.getInstance(getContext());
	return true;
}

std::string CacheContentProvider::openFile(const std::string& uri, const std::string& mode) {
	// return ParcelFileDescriptor.open(
	//     getFile(uri).orThrow(() -> new FileNotFoundException(uri.toString())),
	//     ParcelFileDescriptor.MODE_READ_ONLY);

	auto file = getFile(uri);
	if(!file.has_value()) {
		throw std::runtime_error("File not found: " + uri);
	}

	return file.value();
}

std::vector<CacheContentProvider::QueryRow> CacheContentProvider::query(
		const std::string& uri,
		const std::vector<std::string>& projection) {

	std::vector<QueryRow> result;

	auto readableCacheFile = getReadableCacheFile(uri);
	if(!readableCacheFile.has_value()) {
		// Log.e(TAG, "Couldn't get readable cache file: " + uri);
		return result;
	}

	std::string filePath;
	if(!readableCacheFile.value().getFile(filePath)) {
		// Log.e(TAG, "Couldn't get underlying file: " + uri);
		return result;
	}

	std::string mimetype;
	if(!readableCacheFile.value().lookupMimetype(mimetype)) {
		// Log.e(TAG, "Couldn't get mimetype: " + uri);
		return result;
	}

	// Build row based on projection
	QueryRow row;

	for(const auto& col : (projection.empty() ? std::vector<std::string>{"_display_name", "_size"} : projection)) {
		if(col == "_display_name") {
			row.displayName = generateFilename(
				readableCacheFile.value().getId(), mimetype, "jpg");
		} else if(col == "_size") {
			row.size = 0; // file.get().length() - requires actual file
		} else if(col == "mime_type") {
			row.mimeType = mimetype;
		}
	}

	result.push_back(row);
	return result;
}

std::string CacheContentProvider::getType(const std::string& uri) {
	auto readableCacheFile = getReadableCacheFile(uri);
	if(!readableCacheFile.has_value()) {
		// Log.e(TAG, "Couldn't get readable cache file: " + uri);
		return "";
	}

	std::string mimetype;
	if(readableCacheFile.value().lookupMimetype(mimetype)) {
		return mimetype;
	}

	return "";
}

void CacheContentProvider::insert(const std::string& uri) {
	throw std::runtime_error("No external inserts");
}

int CacheContentProvider::deleteEntry(const std::string& uri) {
	(void)uri;
	return 0;
}

int CacheContentProvider::update(const std::string& uri) {
	throw std::runtime_error("No external updates");
}

} // namespace PinkReader
