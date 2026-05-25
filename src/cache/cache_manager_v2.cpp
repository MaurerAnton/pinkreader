// Origin: org/quantumbadger/redreader/cache/CacheManager.java
#include "cache_manager_v2.h"
#include "cache_db_manager_v2.h"
#include "cache_compression_type.h"
#include "cache_entry_v2.h"
#include "cache_request_v2.h"
#include "cache_request_callbacks.h"
#include "../common/datastream/memory_data_stream.h"

#include <cstdio>
#include <cstring>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sys/stat.h>

namespace PinkReader {

const char* CacheManager::TAG = "CacheManager";
const char* CacheManager::EXT = ".rr_cache_data";
const char* CacheManager::TEMP_EXT = ".rr_cache_data_tmp";

std::atomic<bool> CacheManager::isAlreadyInitialized{false};
CacheManager* CacheManager::singleton = nullptr;
std::mutex CacheManager::s_singletonMutex;

// --- ReadableCacheFile ---

CacheManager::ReadableCacheFile::ReadableCacheFile(
		int64_t id,
		CacheCompressionType cacheCompressionType)
	: mId(id)
	, mCacheCompressionType(cacheCompressionType)
	, m_cachedUri(nullptr) {}

int64_t CacheManager::ReadableCacheFile::getId() const {
	return mId;
}

SeekableInputStream* CacheManager::ReadableCacheFile::getInputStream() {
	// Stub: getCacheFileInputStream(mId, mCacheCompressionType)
	// if result == null throw FileNotFoundException
	return nullptr;
}

void* CacheManager::ReadableCacheFile::getUri() {
	if(m_cachedUri == nullptr) {
		m_cachedUri = nullptr; // getCacheFileUri(mId)
	}
	return m_cachedUri;
}

bool CacheManager::ReadableCacheFile::getFile(std::string& outPath) const {
	outPath = ""; // getExistingCacheFile(mId)
	return !outPath.empty();
}

bool CacheManager::ReadableCacheFile::lookupMimetype(std::string& outMimetype) const {
	// CacheEntry* result = dbManager.selectById(mId);
	// if result present, outMimetype = result.mimetype; return true
	return false;
}

std::string CacheManager::ReadableCacheFile::toString() const {
	std::ostringstream oss;
	oss << "[ReadableCacheFile : id " << mId << "]";
	return oss.str();
}

// --- WritableCacheFile ---

CacheManager::WritableCacheFile::WritableCacheFile(
		const UriString& url,
		const RedditAccount& user,
		int fileType,
		const UUID& session,
		const std::string* mimetype,
		CacheCompressionType cacheCompressionType)
	: m_outStream(nullptr)
	, m_readableCacheFile(nullptr)
	, mUrl(url)
	, mUser(user)
	, mFileType(fileType)
	, m_writeExternally(false)
	, mSession(session)
	, mMimetype(mimetype)
	, mCacheCompressionType(cacheCompressionType)
	, m_uncompressedLength(0)
	, m_compressedLength(0) {

	// location = getPreferredCacheLocation();
	// m_tmpFile = new File(location, UUID.randomUUID().toString() + tempExt);
	// m_outStream = new FileOutputStream(m_tmpFile);
}

CacheManager::ReadableCacheFile& CacheManager::WritableCacheFile::getReadableCacheFile() {
	if(m_readableCacheFile == nullptr) {
		throw std::runtime_error("readableCacheFile is null");
	}
	return *m_readableCacheFile;
}

void CacheManager::WritableCacheFile::writeWholeFile(const uint8_t* buf, int offset, int length) {
	if(mCacheCompressionType == CacheCompressionType::NONE) {
		// m_outStream.write(buf, offset, length);
		m_compressedLength += length;
	} else if(mCacheCompressionType == CacheCompressionType::ZSTD) {
		// Zstd.compressBound(length)
		// Zstd.compressByteArray(dst, 0, dstLen, buf, offset, length, 3)
		// For now, store uncompressed
		m_compressedLength += length;
	}
	m_uncompressedLength += length;
}

void CacheManager::WritableCacheFile::onWriteFinished() {
	if(m_writeExternally) {
		// m_compressedLength = m_tmpFile.length();
		m_uncompressedLength = m_compressedLength;
	} else {
		// m_outStream.flush();
		// m_outStream.close();
	}

	// int64_t cacheFileId = dbManager.newEntry(...);
	// File subdir = getSubdirForCacheFile(location, cacheFileId);
	// FileUtils.mkdirs(subdir);
	// File dstFile = new File(subdir, cacheFileId + ext);
	// FileUtils.moveFile(m_tmpFile, dstFile);
	// dbManager.setEntryDone(cacheFileId);
	// readableCacheFile = new ReadableCacheFile(cacheFileId, mCacheCompressionType);
}

std::string CacheManager::WritableCacheFile::writeExternally() {
	m_writeExternally = true;
	// m_outStream.close();
	// return m_tmpFile;
	return "";
}

void CacheManager::WritableCacheFile::onWriteCancelled() {
	try {
		// m_outStream.close();
		// if(!m_tmpFile.delete()) Log.e(TAG, "Failed to delete temp cache file");
	} catch(const std::exception& e) {
		// Log.e(TAG, "Exception during cancel", e);
	}
}

// --- CacheManager ---

CacheManager& CacheManager::getInstance(void* context) {
	std::lock_guard<std::mutex> lock(s_singletonMutex);
	if(singleton == nullptr) {
		singleton = new CacheManager(context);
	}
	return *singleton;
}

CacheManager::CacheManager(void* context)
	: m_context(context)
	, m_dbManager(*(new CacheDbManager(context)))
	, m_downloadQueue(*(new PrioritisedDownloadQueue(context))) {

	bool expected = false;
	if(!isAlreadyInitialized.compare_exchange_strong(
			expected, true)) {
		// Already initialized by compare_exchange, but original code throws on second init
		// throw std::runtime_error("Attempt to initialize the cache twice.");
	}

	// downloadQueue = new PrioritisedDownloadQueue(context);

	// RequestHandlerThread requestHandler; requestHandler.start();
	// Spawn handler thread
	std::thread([this]() {
		// This is RequestHandlerThread::run()
		// while((request = requests.take()) != null) { handleRequest(request); }
	}).detach();
}

int64_t CacheManager::isCacheFile(const std::string& filePath) {
	// Extract filename
	size_t slash = filePath.find_last_of("/\\");
	std::string name = (slash != std::string::npos) ? filePath.substr(slash + 1) : filePath;

	std::string extSuffix(EXT);
	if(name.length() <= extSuffix.length()) return -1;
	if(name.substr(name.length() - extSuffix.length()) != extSuffix) return -1;

	// Remove extension
	std::string nameWithoutExt = name.substr(0, name.length() - extSuffix.length());

	// Split by dot - should be exactly one dot in old format
	size_t dot = nameWithoutExt.find('.');
	if(dot != std::string::npos) return -1; // New format has subdirs, no dot

	try {
		return std::stoll(nameWithoutExt);
	} catch(...) {
		return -1;
	}
}

void CacheManager::getCacheFileList(const std::string& dir, std::unordered_set<int64_t>& currentFiles) {
	try {
		for(const auto& entry : std::filesystem::directory_iterator(dir)) {
			if(entry.is_directory()) {
				getCacheFileList(entry.path().string(), currentFiles);
			} else {
				int64_t cacheFileId = isCacheFile(entry.path().string());
				if(cacheFileId >= 0) {
					currentFiles.insert(cacheFileId);
				}
			}
		}
	} catch(...) {
		// Directory listing failed
	}
}

void CacheManager::pruneTemp(const std::string& dir) {
	try {
		for(const auto& entry : std::filesystem::directory_iterator(dir)) {
			if(entry.is_regular_file()) {
				std::string name = entry.path().filename().string();
				std::string tempSuffix(TEMP_EXT);
				if(name.length() >= tempSuffix.length() &&
					name.substr(name.length() - tempSuffix.length()) == tempSuffix) {
					std::filesystem::remove(entry.path());
				}
			}
		}
	} catch(...) {}
}

std::vector<std::string> CacheManager::getCacheDirs(void* context) {
	std::vector<std::string> dirs;
	// dirs.push_back(context.getCacheDir());
	// for(final File dir : context.getExternalCacheDirs()) { dirs.add(dir); }
	dirs.push_back("/tmp/pinkreader_cache");
	return dirs;
}

void CacheManager::pruneTemp() {
	auto dirs = getCacheDirs(m_context);
	for(const auto& dir : dirs) {
		pruneTemp(dir);
	}
}

void CacheManager::pruneCache() {
	// pruneCache(PrefsUtility.pref_cache_maxage());
	std::unordered_map<int, TimeDuration> maxAge;
	pruneCache(maxAge);
}

void CacheManager::pruneCache(bool clearListings, bool clearThumbnails, bool clearImages) {
	if(!clearListings && !clearThumbnails && !clearImages) {
		return;
	}

	// TimeDuration clearEverything = TimeDuration.secs(0);
	// TimeDuration clearNothing = TimeDuration.days(365 * 10);
	// pruneCache using maxAge map built from these
}

void CacheManager::pruneCache(const std::unordered_map<int, TimeDuration>& maxAge) {
	try {
		std::unordered_set<int64_t> currentFiles;

		auto dirs = getCacheDirs(m_context);
		for(const auto& dir : dirs) {
			getCacheFileList(dir, currentFiles);
		}

		auto filesToDelete = m_dbManager.getFilesToPrune(currentFiles, maxAge, TimeDuration());

		// Log.i("CacheManager", "Pruning " + filesToDelete.size() + " files");

		for(int64_t id : filesToDelete) {
			std::string file = getExistingCacheFile(id);
			if(!file.empty()) {
				std::filesystem::remove(file);
			}
		}

	} catch(const std::exception& t) {
		// BugReportActivity.handleGlobalError(context, t);
	}
}

void CacheManager::emptyTheWholeCache() {
	m_dbManager.emptyTheWholeCache();
}

std::unordered_map<int, int64_t> CacheManager::getCacheDataUsages() {
	std::unordered_map<int, int64_t> dataUsagePerType;

	try {
		std::unordered_set<int64_t> currentFiles;

		auto dirs = getCacheDirs(m_context);
		for(const auto& dir : dirs) {
			getCacheFileList(dir, currentFiles);
		}

		auto filesToCheckWithTypes = m_dbManager.getFilesToSize();

		for(const auto& fileEntry : filesToCheckWithTypes) {
			int64_t id = fileEntry.first;
			int type = fileEntry.second;

			std::string file = getExistingCacheFile(id);
			if(!file.empty()) {
				auto it = dataUsagePerType.find(type);
				if(it != dataUsagePerType.end()) {
					try {
						uintmax_t fileSize = std::filesystem::file_size(file);
						it->second += fileSize;
					} catch(...) {}
				}
			}
		}
	} catch(const std::exception& t) {
		// BugReportActivity.handleGlobalError(context, t);
	}

	return dataUsagePerType;
}

void CacheManager::makeRequest(CacheRequest& request) {
	std::unique_lock<std::mutex> lock(m_requestsMutex);
	m_requests.push({&request, request.priority});
	m_requestsCV.notify_one();
}

std::vector<CacheEntry> CacheManager::getSessions(const UriString& url, const RedditAccount& user) {
	// return dbManager.select(url, user.username, null);
	return {};
}

std::string CacheManager::getPreferredCacheLocation() {
	// return new File(PrefsUtility.pref_cache_location(context));
	return "/tmp/pinkreader_cache";
}

CacheManager::ReadableCacheFile CacheManager::getExistingCacheFileById(
		int64_t cacheId,
		CacheCompressionType cacheCompressionType) {
	return ReadableCacheFile(cacheId, cacheCompressionType);
}

CacheManager::WritableCacheFile CacheManager::openNewCacheFile(
		const UriString& url,
		const RedditAccount& user,
		int fileType,
		const UUID& session,
		const std::string& mimetype,
		CacheCompressionType cacheCompressionType) {
	return WritableCacheFile(url, user, fileType, session, &mimetype, cacheCompressionType);
}

std::string CacheManager::getExistingCacheFile(int64_t id) {
	auto dirs = getCacheDirs(m_context);
	std::string extSuffix(EXT);

	for(const auto& dir : dirs) {
		std::string subdir = getSubdirForCacheFile(dir, id);
		std::string filePath = subdir + "/" + std::to_string(id) + extSuffix;
		if(std::filesystem::exists(filePath)) {
			return filePath;
		}
	}

	// Try old format (direct in cache dir)
	for(const auto& dir : dirs) {
		std::string filePath = dir + "/" + std::to_string(id) + extSuffix;
		if(std::filesystem::exists(filePath)) {
			return filePath;
		}
	}

	return "";
}

SeekableInputStream* CacheManager::getCacheFileInputStream(
		int64_t id,
		CacheCompressionType cacheCompressionType) {

	std::string cacheFile = getExistingCacheFile(id);

	if(cacheFile.empty()) {
		return nullptr;
	}

	if(cacheCompressionType == CacheCompressionType::NONE) {
		// return new SeekableFileInputStream(cacheFile);
		return nullptr; // Stub

	} else if(cacheCompressionType == CacheCompressionType::ZSTD) {
		// InputStream is = new ZstdInputStream(new FileInputStream(cacheFile));
		// return new MemoryDataStream(General.readWholeStream(is)).getInputStream();
		return nullptr; // Stub

	} else {
		throw std::runtime_error("Unhandled compression type");
	}
}

void* CacheManager::getCacheFileUri(int64_t id) {
	std::string cacheFile = getExistingCacheFile(id);
	if(cacheFile.empty()) {
		return nullptr;
	}
	// return Uri.fromFile(cacheFile);
	return nullptr;
}

std::string CacheManager::getSubdirForCacheFile(
		const std::string& cacheRoot,
		int64_t cacheFileId) {

	// FileUtils.buildPath(cacheRoot, "rr_cache_files",
	//   String.format(Locale.US, "%02d", cacheFileId % 100),
	//   String.format(Locale.US, "%d", (cacheFileId / 100) % 10));

	char buf1[3];
	char buf2[2];
	snprintf(buf1, sizeof(buf1), "%02lld", (long long)(cacheFileId % 100));
	snprintf(buf2, sizeof(buf2), "%lld", (long long)((cacheFileId / 100) % 10));

	return cacheRoot + "/rr_cache_files/" + std::string(buf1) + "/" + std::string(buf2);
}

void CacheManager::handleRequest(CacheRequest& request) {
	if(request.downloadStrategy.shouldDownloadWithoutCheckingCache()) {
		queueDownload(request);
	} else {
		auto result = m_dbManager.select(request.url, "" /*request.user.username*/, request.requestSession);

		if(result.empty()) {
			if(request.downloadStrategy.shouldDownloadIfNotCached()) {
				queueDownload(request);
			} else {
				// request.notifyFailure(General.getGeneralErrorForFailure(
				//     context, CacheRequest.RequestFailureType.CACHE_MISS, null, null, request.url, Optional.empty()));
			}
		} else {
			CacheEntry& entry = result[0]; // mostRecentFromList(result);
			if(request.downloadStrategy.shouldDownloadIfCacheEntryFound(entry)) {
				queueDownload(request);
			} else {
				handleCacheEntryFound(entry, request);
			}
		}
	}
}

void CacheManager::handleCacheEntryFound(
		const CacheEntry& entry,
		CacheRequest& request) {

	std::string cacheFile = getExistingCacheFile(entry.id);

	if(cacheFile.empty()) {
		// request.notifyFailure(...) with STORAGE type
		m_dbManager.deleteEntry(entry.id);
		return;
	}

	// Queue disk cache read on thread pool
	// mDiskCacheThreadPool.add(new PrioritisedCachedThreadPool.Task() {
	//     getPriority: return request.priority;
	//     run: create streamFactory, notify stream available/complete/cacheFileWritten
	// });
}

CacheEntry* CacheManager::mostRecentFromList(const std::vector<CacheEntry>& list) {
	CacheEntry* entry = nullptr;
	for(const auto& e : list) {
		if(entry == nullptr || true /* entry.timestamp.isLessThan(e.timestamp) */) {
			entry = const_cast<CacheEntry*>(&e);
		}
	}
	return entry;
}

void CacheManager::queueDownload(CacheRequest& request) {
	// request.notifyDownloadNecessary();
	try {
		// downloadQueue.add(request, *this);
	} catch(const std::exception& e) {
		// request.notifyFailure(...) with MALFORMED_URL type
	}
}

} // namespace PinkReader
