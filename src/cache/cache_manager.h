/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: cache_manager.h - Port of RedReader's CacheManager.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/cache/CacheManager.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <atomic>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <condition_variable>

namespace PinkReader {

// Forward declarations
class Account;                         // RedditAccount port
class CacheDbManager;
class PrioritisedDownloadQueue;
class PrioritisedCachedThreadPool;
class CacheRequest;
class CacheEntry;
class TimeDuration;
class UriString;

enum class CacheCompressionType {
    NONE = 0,
    ZSTD = 1
};

// ============================================================================
// Priority — comparable priority value used in thread pool
// Port of org.quantumbadger.redreader.common.Priority
// ============================================================================

class TaskPriority {
public:
    explicit TaskPriority(int value) : m_value(value) {}
    int value() const { return m_value; }
    bool isHigherPriorityThan(const TaskPriority &other) const {
        return m_value < other.m_value;
    }
    bool operator<(const TaskPriority &other) const {
        return m_value > other.m_value; // lower numeric = higher priority
    }

private:
    int m_value;
};

// ============================================================================
// GenericFactory — simple factory returning T
// Port of org.quantumbadger.redreader.common.GenericFactory
// ============================================================================

template<typename T, typename E = std::exception>
class GenericFactory {
public:
    using FactoryFunc = std::function<T()>;
    explicit GenericFactory(FactoryFunc func) : m_func(std::move(func)) {}
    T create() const { return m_func(); }

private:
    FactoryFunc m_func;
};

// ============================================================================
// DownloadStrategy — forward, defined in download_strategy.h
// ============================================================================
class DownloadStrategy;

// ============================================================================
// CacheManager — port of Java class CacheManager (Java line 63)
// ============================================================================

class CacheManager {
public:
    // ---- TAG constant (Java line 65) ----
    static constexpr const char *TAG = "CacheManager";

    // ---- File extensions (Java lines 67-68) ----
    static constexpr const char *EXT = ".rr_cache_data";
    static constexpr const char *TEMP_EXT = ".rr_cache_data_tmp";

    // ========================================================================
    // ReadableCacheFile — port of inner class (Java lines 449-512)
    // ========================================================================

    class ReadableCacheFile {
    public:
        ReadableCacheFile(int64_t id, CacheCompressionType compressionType);

        int64_t getId() const;

        // getInputStream — port of Java line 469-478
        // Returns QByteArray instead of InputStream
        QByteArray getInputStream() const;

        // getUri — port of Java line 481-488
        std::optional<QString> getUri() const;

        // getFile — port of Java line 491-493
        std::optional<QString> getFile() const;

        // lookupMimetype — port of Java line 496-506
        std::optional<QString> lookupMimetype() const;

        // toString — port of Java line 509-511
        QString toString() const;

    private:
        int64_t m_id;
        CacheCompressionType m_cacheCompressionType;
        mutable std::optional<QString> m_cachedUri;

        friend class CacheManager;
    };

    // ========================================================================
    // WritableCacheFile — port of inner class (Java lines 303-434)
    // ========================================================================

    class WritableCacheFile {
    public:
        // Constructor — port of Java lines 322-341
        WritableCacheFile(
                const QString &url,
                const Account &user,
                int fileType,
                const QUuid &session,
                const std::optional<QString> &mimetype,
                CacheCompressionType cacheCompressionType);

        ~WritableCacheFile();

        // getReadableCacheFile — port of Java line 344-346
        const ReadableCacheFile &getReadableCacheFile() const;

        // writeWholeFile — port of Java lines 348-382
        void writeWholeFile(const uint8_t *buf, int64_t offset, int64_t length);

        // onWriteFinished — port of Java lines 384-414
        void onWriteFinished();

        // writeExternally — port of Java lines 416-420
        QString writeExternally();

        // onWriteCancelled — port of Java lines 422-433
        void onWriteCancelled();

    private:
        QFile m_outStream;
        ReadableCacheFile *m_readableCacheFile = nullptr;
        QString m_url;
        const Account &m_user;
        int m_fileType;
        QString m_location;
        bool m_writeExternally = false;
        QUuid m_session;
        std::optional<QString> m_mimetype;
        CacheCompressionType m_cacheCompressionType;
        QString m_tmpFile;
        int64_t m_uncompressedLength = 0;
        int64_t m_compressedLength = 0;

        friend class CacheManager;
    };

    // ========================================================================
    // Singleton (Java lines 84-89)
    // ========================================================================

    static CacheManager &getInstance();

    // Initialization — called once with application data directory
    static void initialize(const QString &cacheBasePath);

    // ========================================================================
    // pruneTemp static (Java lines 150-163)
    // ========================================================================

    static void pruneTempDir(const QDir &dir);

    // ========================================================================
    // getCacheDirs (Java lines 166-179)
    // ========================================================================

    static std::vector<QString> getCacheDirs();

    // ========================================================================
    // pruneTemp instance (Java lines 181-186)
    // ========================================================================

    void pruneTemp();

    // ========================================================================
    // pruneCache overloads (Java lines 188-245)
    // ========================================================================

    void pruneCache();
    void pruneCache(bool clearListings, bool clearThumbnails, bool clearImages);
    void pruneCache(const std::map<int, TimeDuration> &maxAge);

    // ========================================================================
    // emptyTheWholeCache (Java lines 247-249)
    // ========================================================================

    void emptyTheWholeCache();

    // ========================================================================
    // getCacheDataUsages (Java lines 251-281)
    // ========================================================================

    std::map<int, int64_t> getCacheDataUsages();

    // ========================================================================
    // makeRequest (Java lines 283-285)
    // ========================================================================

    void makeRequest(CacheRequest &request);

    // ========================================================================
    // getSessions (Java lines 287-289)
    // ========================================================================

    std::vector<CacheEntry> getSessions(const QString &url, const QString &username);

    // ========================================================================
    // getPreferredCacheLocation (Java lines 291-294)
    // ========================================================================

    QString getPreferredCacheLocation() const;

    // ========================================================================
    // getExistingCacheFileById (Java lines 297-301)
    // ========================================================================

    ReadableCacheFile getExistingCacheFileById(int64_t cacheId, CacheCompressionType compressionType);

    // ========================================================================
    // openNewCacheFile (Java lines 515-523)
    // ========================================================================

    std::unique_ptr<WritableCacheFile> openNewCacheFile(
            const QString &url,
            const Account &user,
            int fileType,
            const QUuid &session,
            const std::optional<QString> &mimetype,
            CacheCompressionType cacheCompressionType);

    // ========================================================================
    // Destructor
    // ========================================================================

    ~CacheManager();

private:
    // ---- Singleton instance (Java line 82) ----
    static std::unique_ptr<CacheManager> s_singleton;
    static std::mutex s_singletonMutex;

    // ---- isAlreadyInitialized (Java line 70) ----
    static std::atomic<bool> s_isAlreadyInitialized;

    // ---- Protected constructor (Java lines 91-105) ----
    explicit CacheManager(const QString &cacheBasePath);

    // ---- Fields (Java lines 71-78) ----
    std::unique_ptr<CacheDbManager> m_dbManager;
    std::unique_ptr<PrioritisedDownloadQueue> m_downloadQueue;

    // PriorityBlockingQueue<CacheRequest> (Java line 73-74)
    struct RequestCompare {
        bool operator()(const CacheRequest *a, const CacheRequest *b) const;
    };
    std::priority_queue<CacheRequest *, std::vector<CacheRequest *>, RequestCompare> m_requests;
    std::mutex m_requestsMutex;
    std::condition_variable m_requestsCV;

    // PrioritisedCachedThreadPool (Java lines 77-78)
    std::unique_ptr<PrioritisedCachedThreadPool> m_diskCacheThreadPool;

    // Base path for cache storage
    QString m_cacheBasePath;

    // ---- RequestHandlerThread (Java lines 585-753) ----
    class RequestHandlerThread {
    public:
        explicit RequestHandlerThread(CacheManager &manager);
        ~RequestHandlerThread();
        void start();
        void stop();

    private:
        void run();
        void handleRequest(CacheRequest &request);
        CacheEntry mostRecentFromList(const std::vector<CacheEntry> &list);
        void queueDownload(CacheRequest &request);
        void handleCacheEntryFound(const CacheEntry &entry, CacheRequest &request);

        CacheManager &m_manager;
        std::thread m_thread;
        std::atomic<bool> m_running{false};
    };

    std::unique_ptr<RequestHandlerThread> m_requestHandler;

    // ---- Private methods ----
    std::optional<int64_t> isCacheFile(const QFileInfo &file);
    void getCacheFileList(const QDir &dir, std::set<int64_t> &currentFiles);
    std::optional<QString> getExistingCacheFile(int64_t id);
    std::optional<QByteArray> getCacheFileInputStream(int64_t id, CacheCompressionType compressionType);
    std::optional<QString> getCacheFileUri(int64_t id);

    static QString getSubdirForCacheFile(const QString &cacheRoot, int64_t cacheFileId);

    friend class WritableCacheFile;
    friend class ReadableCacheFile;
    friend class RequestHandlerThread;
};

} // namespace PinkReader
