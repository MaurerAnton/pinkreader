// Origin: org.quantumbadger.redreader.cache.CacheManager.java
// Ported from RedReader (GPLv3)
#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>

namespace PinkReader {

// Forward declarations
class UriString;
class RedditAccount;
class CacheRequest;
class CacheEntry;
class CacheDbManager;
class PrioritisedCachedThreadPool;
class PrioritisedDownloadQueue;
class SeekableInputStream;
class ReadableCacheFile;
class WritableCacheFile;

// File type enum
using FileType = int;

// Compression type enum (matches CacheCompressionType)
enum class CacheCompressionType {
    NONE = 0,
    ZSTD = 1
};

// Duration in seconds
struct TimeDuration {
    long seconds = 0;
    static TimeDuration secs(long s) { return TimeDuration{s}; }
    static TimeDuration hours(long h) { return TimeDuration{h * 3600}; }
    static TimeDuration days(long d) { return TimeDuration{d * 86400}; }
};

// Priority wrapper
struct Priority {
    int value = 0;
};

// --- ReadableCacheFile (public inner class in original) ---

class ReadableCacheFile {
public:
    ReadableCacheFile(long id, CacheCompressionType cacheCompressionType);

    long getId() const;
    void* getInputStream(); // InputStream*
    void* getUri();         // Uri*
    void* getFile();        // Optional<File>

private:
    long m_id;
    CacheCompressionType m_cacheCompressionType;
    void* m_cachedUri = nullptr; // Uri*
};

// --- WritableCacheFile ---

class WritableCacheFile {
public:
    WritableCacheFile(
            const UriString& url,
            const RedditAccount& user,
            int fileType,
            void* session,       // UUID*
            const std::string* mimetype,
            CacheCompressionType cacheCompressionType);

    ReadableCacheFile* getReadableCacheFile();

    void writeWholeFile(const uint8_t* buf, int offset, int length);
    void onWriteFinished();
    void* writeExternally();   // File*
    void onWriteCancelled();

private:
    void* m_outStream = nullptr;    // OutputStream*
    ReadableCacheFile* m_readableCacheFile = nullptr;
    const UriString& m_url;
    const RedditAccount& m_user;
    int m_fileType;
    void* m_location = nullptr;     // File*
    bool m_writeExternally = false;
    void* m_session = nullptr;      // UUID*
    const std::string* m_mimetype = nullptr;
    CacheCompressionType m_cacheCompressionType;
    void* m_tmpFile = nullptr;      // File*
    long m_uncompressedLength = 0;
    long m_compressedLength = 0;
};

// --- CacheManager (singleton) ---

class CacheManager {
public:
    // --- Singleton ---
    static CacheManager* getInstance(void* context);

    // --- Public API ---
    void pruneTemp();
    void pruneCache();
    void pruneCache(bool clearListings, bool clearThumbnails, bool clearImages);
    void pruneCache(const std::map<int, TimeDuration>& maxAge);
    void emptyTheWholeCache();
    std::map<int, long> getCacheDataUsages();

    void makeRequest(CacheRequest* request);

    std::vector<CacheEntry> getSessions(const UriString& url, const RedditAccount& user);

    void* getPreferredCacheLocation(); // File*

    ReadableCacheFile* getExistingCacheFileById(long cacheId, CacheCompressionType compressionType);

    WritableCacheFile* openNewCacheFile(
            const UriString& url,
            const RedditAccount& user,
            int fileType,
            void* session,       // UUID*
            const std::string* mimetype,
            CacheCompressionType cacheCompressionType);

    static std::vector<void*> getCacheDirs(void* context); // ArrayList<File>

private:
    // --- Singleton pattern ---
    static std::mutex s_singletonMutex;
    static CacheManager* s_singleton;
    static std::atomic<bool> s_isAlreadyInitialized;

    // --- Constructor ---
    explicit CacheManager(void* context);

    // --- Private helpers ---
    long isCacheFile(void* file); // File* -> Long or null
    void getCacheFileList(void* dir, std::set<long>& currentFiles);
    static void pruneTempDir(void* dir);
    void* getExistingCacheFile(long id); // File*
    void* getCacheFileInputStream(long id, CacheCompressionType compressionType); // SeekableInputStream*
    void* getCacheFileUri(long id); // Uri*
    static void* getSubdirForCacheFile(void* cacheRoot, long cacheFileId); // File*

    // --- Inner RequestHandlerThread ---
    class RequestHandlerThread;
    friend class RequestHandlerThread;

    // --- Constants ---
    static const std::string s_ext;        // ".rr_cache_data"
    static const std::string s_tempExt;    // ".rr_cache_data_tmp"

    // --- Fields ---
    CacheDbManager* m_dbManager = nullptr;
    void* m_requestQueue = nullptr;        // PriorityBlockingQueue<CacheRequest>
    PrioritisedDownloadQueue* m_downloadQueue = nullptr;
    PrioritisedCachedThreadPool* m_diskCacheThreadPool = nullptr;
    void* m_context = nullptr;
};

} // namespace PinkReader
