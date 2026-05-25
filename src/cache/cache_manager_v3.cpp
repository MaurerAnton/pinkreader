// Origin: org.quantumbadger.redreader.cache.CacheManager.java
// Ported from RedReader (GPLv3)

#include "cache/cache_manager_v3.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace PinkReader {

// --- Static members ---

std::mutex CacheManager::s_singletonMutex;
CacheManager* CacheManager::s_singleton = nullptr;
std::atomic<bool> CacheManager::s_isAlreadyInitialized{false};

const std::string CacheManager::s_ext = ".rr_cache_data";
const std::string CacheManager::s_tempExt = ".rr_cache_data_tmp";

// --- Singleton ---

CacheManager* CacheManager::getInstance(void* context) {
    std::lock_guard<std::mutex> lock(s_singletonMutex);
    if (s_singleton == nullptr) {
        s_singleton = new CacheManager(context);
    }
    return s_singleton;
}

// --- Constructor ---

CacheManager::CacheManager(void* context)
    : m_context(context) {
    if (!s_isAlreadyInitialized.compare_exchange_strong(
            *reinterpret_cast<bool*>(new bool(false)),
            true)) {
        throw std::runtime_error("Attempt to initialize the cache twice.");
    }

    // In original:
    // this.context = context;
    // dbManager = new CacheDbManager(context);
    // downloadQueue = new PrioritisedDownloadQueue(context);
    // final RequestHandlerThread requestHandler = new RequestHandlerThread();
    // requestHandler.start();
}

// --- ReadableCacheFile ---

ReadableCacheFile::ReadableCacheFile(long id, CacheCompressionType cacheCompressionType)
    : m_id(id)
    , m_cacheCompressionType(cacheCompressionType) {}

long ReadableCacheFile::getId() const {
    return m_id;
}

void* ReadableCacheFile::getInputStream() {
    return nullptr; // Returns InputStream*
}

void* ReadableCacheFile::getUri() {
    if (m_cachedUri == nullptr) {
        // m_cachedUri = getCacheFileUri(m_id);
    }
    return m_cachedUri;
}

void* ReadableCacheFile::getFile() {
    return nullptr; // Optional<File>
}

// --- WritableCacheFile ---

WritableCacheFile::WritableCacheFile(
        const UriString& url,
        const RedditAccount& user,
        int fileType,
        void* session,
        const std::string* mimetype,
        CacheCompressionType cacheCompressionType)
    : m_url(url)
    , m_user(user)
    , m_fileType(fileType)
    , m_session(session)
    , m_mimetype(mimetype)
    , m_cacheCompressionType(cacheCompressionType) {
    // In original:
    // location = getPreferredCacheLocation();
    // mTmpFile = new File(location, UUID.randomUUID().toString() + tempExt);
    // mOutStream = new FileOutputStream(mTmpFile);
}

ReadableCacheFile* WritableCacheFile::getReadableCacheFile() {
    // return Objects.requireNonNull(readableCacheFile);
    return m_readableCacheFile;
}

void WritableCacheFile::writeWholeFile(const uint8_t* buf, int offset, int length) {
    // In original:
    // if(mCacheCompressionType == CacheCompressionType.NONE) {
    //     mOutStream.write(buf, offset, length);
    //     mCompressedLength += length;
    // } else if(mCacheCompressionType == CacheCompressionType.ZSTD) {
    //     final long maxDestSize = Zstd.compressBound(length);
    //     if(maxDestSize > Integer.MAX_VALUE) throw IOException(...);
    //     final byte[] dst = new byte[(int)maxDestSize];
    //     final int size = (int)Zstd.compressByteArray(dst, 0, dst.length, buf, offset, length, 3);
    //     mOutStream.write(dst, 0, size);
    //     mCompressedLength += size;
    // }
    // mUncompressedLength += length;
    (void)buf;
    (void)offset;
    (void)length;
}

void WritableCacheFile::onWriteFinished() {
    // In original:
    // if(mWriteExternally) {
    //     mCompressedLength = mTmpFile.length();
    //     mUncompressedLength = mCompressedLength;
    // } else {
    //     mOutStream.flush();
    //     mOutStream.close();
    // }
    //
    // final long cacheFileId = dbManager.newEntry(
    //     mUrl, mUser, mFileType, mSession, mMimetype,
    //     mCacheCompressionType, mCompressedLength, mUncompressedLength);
    //
    // final File subdir = getSubdirForCacheFile(location, cacheFileId);
    // FileUtils.mkdirs(subdir);
    // final File dstFile = new File(subdir, cacheFileId + ext);
    // FileUtils.moveFile(mTmpFile, dstFile);
    //
    // dbManager.setEntryDone(cacheFileId);
    // readableCacheFile = new ReadableCacheFile(cacheFileId, mCacheCompressionType);
}

void* WritableCacheFile::writeExternally() {
    m_writeExternally = true;
    // mOutStream.close();
    // return mTmpFile;
    return nullptr;
}

void WritableCacheFile::onWriteCancelled() {
    // In original:
    // try {
    //     mOutStream.close();
    //     if(!mTmpFile.delete()) Log.e(TAG, "Failed to delete temp cache file");
    // } catch(Exception e) { Log.e(TAG, "Exception during cancel", e); }
}

// --- CacheManager public methods ---

void* CacheManager::getPreferredCacheLocation() {
    // return new File(PrefsUtility.pref_cache_location(context));
    return nullptr;
}

std::vector<void*> CacheManager::getCacheDirs(void* context) {
    std::vector<void*> dirs;
    // In original:
    // dirs.add(context.getCacheDir());
    // for(final File dir : context.getExternalCacheDirs()) {
    //     if(dir != null) dirs.add(dir);
    // }
    (void)context;
    return dirs;
}

void CacheManager::pruneTemp() {
    auto dirs = getCacheDirs(m_context);
    for (auto* dir : dirs) {
        pruneTempDir(dir);
    }
}

void CacheManager::pruneCache() {
    // pruneCache(PrefsUtility.pref_cache_maxage());
}

void CacheManager::pruneCache(bool clearListings, bool clearThumbnails, bool clearImages) {
    if (!clearListings && !clearThumbnails && !clearImages) {
        return;
    }

    // Use a maximum age of 0 to clear everything in that category
    // Use a high number as the maximum age to ensure nothing is deleted
    TimeDuration clearEverything = TimeDuration::secs(0);
    TimeDuration clearNothing = TimeDuration::days(365 * 10);

    std::map<int, TimeDuration> maxAge;
    // In original: PrefsUtility.createFileTypeMap(clearEverything/clearNothing...)
    // pruneCache(maxAge);
    (void)clearEverything;
    (void)clearNothing;
    (void)maxAge;
}

void CacheManager::pruneCache(const std::map<int, TimeDuration>& maxAge) {
    // In original:
    // try {
    //     final HashSet<Long> currentFiles = new HashSet<>(1024);
    //     final List<File> dirs = getCacheDirs(context);
    //     for(final File dir : dirs) { getCacheFileList(dir, currentFiles); }
    //     final ArrayList<Long> filesToDelete = dbManager.getFilesToPrune(
    //         currentFiles, maxAge, TimeDuration.hours(72));
    //     Log.i("CacheManager", "Pruning " + filesToDelete.size() + " files");
    //     for(final long id : filesToDelete) {
    //         final File file = getExistingCacheFile(id);
    //         if(file != null) file.delete();
    //     }
    // } catch(Throwable t) { BugReportActivity.handleGlobalError(context, t); }
    (void)maxAge;
}

void CacheManager::emptyTheWholeCache() {
    // dbManager.emptyTheWholeCache();
}

std::map<int, long> CacheManager::getCacheDataUsages() {
    // In original: computes data usage per file type by scanning cache dirs
    std::map<int, long> dataUsagePerType;
    return dataUsagePerType;
}

void CacheManager::makeRequest(CacheRequest* request) {
    // requests.put(request); // PriorityBlockingQueue put
    (void)request;
}

std::vector<CacheEntry> CacheManager::getSessions(const UriString& url, const RedditAccount& user) {
    // return dbManager.select(url, user.username, null);
    (void)url;
    (void)user;
    return {};
}

ReadableCacheFile* CacheManager::getExistingCacheFileById(long cacheId, CacheCompressionType compressionType) {
    return new ReadableCacheFile(cacheId, compressionType);
}

WritableCacheFile* CacheManager::openNewCacheFile(
        const UriString& url,
        const RedditAccount& user,
        int fileType,
        void* session,
        const std::string* mimetype,
        CacheCompressionType cacheCompressionType) {
    return new WritableCacheFile(url, user, fileType, session, mimetype, cacheCompressionType);
}

// --- Private helpers ---

long CacheManager::isCacheFile(void* file) {
    // In original:
    // final String name = file.getName();
    // if(!name.endsWith(ext)) return null;
    // final String[] nameSplit = name.split("\\.");
    // if(nameSplit.length != 2) return null;
    // try { return Long.parseLong(nameSplit[0]); } catch(Exception e) { return null; }
    (void)file;
    return -1;
}

void CacheManager::getCacheFileList(void* dir, std::set<long>& currentFiles) {
    // In original: recursively walks directories, checking filenames against ext pattern
    (void)dir;
    (void)currentFiles;
}

void CacheManager::pruneTempDir(void* dir) {
    // In original:
    // final String[] list = dir.list();
    // if(list == null) return;
    // for(final String file : list) {
    //     if(file.endsWith(tempExt)) new File(dir, file).delete();
    // }
    (void)dir;
}

void* CacheManager::getExistingCacheFile(long id) {
    // In original:
    // First check new format subdirectory, then old format top-level
    // final List<File> dirs = getCacheDirs(context);
    // for(File dir : dirs) {
    //     File f = new File(getSubdirForCacheFile(dir, id), id + ext);
    //     if(f.exists()) return f;
    // }
    // for(File dir : dirs) {
    //     File f = new File(dir, id + ext);
    //     if(f.exists()) return f;
    // }
    // return null;
    (void)id;
    return nullptr;
}

void* CacheManager::getCacheFileInputStream(long id, CacheCompressionType compressionType) {
    // In original:
    // final File cacheFile = getExistingCacheFile(id);
    // if(cacheFile == null) return null;
    // if(compressionType == CacheCompressionType.NONE) {
    //     return new SeekableFileInputStream(cacheFile);
    // } else if(compressionType == CacheCompressionType.ZSTD) {
    //     try(InputStream is = new ZstdInputStream(new FileInputStream(cacheFile))) {
    //         return new MemoryDataStream(General.readWholeStream(is)).getInputStream();
    //     }
    // } else {
    //     throw new RuntimeException("Unhandled compression type " + cacheCompressionType);
    // }
    (void)id;
    (void)compressionType;
    return nullptr;
}

void* CacheManager::getCacheFileUri(long id) {
    // final File cacheFile = getExistingCacheFile(id);
    // if(cacheFile == null) return null;
    // return Uri.fromFile(cacheFile);
    (void)id;
    return nullptr;
}

void* CacheManager::getSubdirForCacheFile(void* cacheRoot, long cacheFileId) {
    // return FileUtils.buildPath(
    //     cacheRoot, "rr_cache_files",
    //     String.format("%02d", cacheFileId % 100),
    //     String.format("%d", (cacheFileId / 100) % 10));
    (void)cacheRoot;
    (void)cacheFileId;
    return nullptr;
}

// --- RequestHandlerThread ---
// In original: extends Thread, runs loop taking requests from PriorityBlockingQueue,
// handles each request based on download strategy:
// - If shouldDownloadWithoutCheckingCache: queueDownload
// - Else: check dbManager.select() for existing entries
//   - If empty: check shouldDownloadIfNotCached, queue or notify cache miss failure
//   - If entries exist: find most recent, check shouldDownloadIfCacheEntryFound,
//     queue or notify from cache (via mDiskCacheThreadPool task)
//
// handleCacheEntryFound creates a PrioritisedCachedThreadPool.Task that:
// 1. Creates stream factory from cache file
// 2. Calls request.notifyDataStreamAvailable()
// 3. Calls request.notifyDataStreamComplete()
// 4. Calls request.notifyCacheFileWritten()

} // namespace PinkReader
