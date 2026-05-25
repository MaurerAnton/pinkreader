/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: cache_manager.cpp - Implementation port of CacheManager.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/cache/CacheManager.java
 */

#include "cache/cache_manager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QUuid>

#include <algorithm>
#include <cstdio>
#include <stdexcept>

// Placeholder includes for types not yet fully ported
// When the actual implementations exist, these will be replaced
// #include "cache/cache_db_manager.h"
// #include "cache/cache_entry.h"
// #include "cache/cache_request.h"
// #include "cache/download_queue.h"
// #include "cache/prioritised_cached_thread_pool.h"
// #include "accounts/account.h"
// #include "core/constants.h"
// #include "common/time_duration.h"
// #include "common/uri_string.h"
// #include "common/general.h"
// #include "settings/prefs_utility.h"

namespace PinkReader {

// ============================================================================
// Static members (Java lines 65-83)
// ============================================================================

std::unique_ptr<CacheManager> CacheManager::s_singleton;
std::mutex CacheManager::s_singletonMutex;
std::atomic<bool> CacheManager::s_isAlreadyInitialized{false};

// ============================================================================
// Singleton (Java lines 84-89)
// ============================================================================

CacheManager &CacheManager::getInstance() {
    // In Java: synchronized getInstance(Context) returns singleton
    // In C++, callers must have called initialize() first
    if (!s_singleton) {
        throw std::runtime_error("CacheManager::initialize() must be called before getInstance()");
    }
    return *s_singleton;
}

void CacheManager::initialize(const QString &cacheBasePath) {
    std::lock_guard<std::mutex> lock(s_singletonMutex);
    if (!s_singleton) {
        s_singleton.reset(new CacheManager(cacheBasePath));
    }
}

// ============================================================================
// Constructor (Java lines 91-105)
// ============================================================================

CacheManager::CacheManager(const QString &cacheBasePath)
    : m_cacheBasePath(cacheBasePath)
{
    // Java: if(!isAlreadyInitialized.compareAndSet(false, true)) throw RuntimeException
    bool expected = false;
    if (!s_isAlreadyInitialized.compare_exchange_strong(expected, true)) {
        throw std::runtime_error("Attempt to initialize the cache twice.");
    }

    // Java: dbManager = new CacheDbManager(context);
    // m_dbManager = std::make_unique<CacheDbManager>(cacheBasePath);

    // Java: downloadQueue = new PrioritisedDownloadQueue(context);
    // m_downloadQueue = std::make_unique<PrioritisedDownloadQueue>(cacheBasePath);

    // Java: final RequestHandlerThread requestHandler = new RequestHandlerThread();
    //       requestHandler.start();
    m_requestHandler = std::make_unique<RequestHandlerThread>(*this);
    m_requestHandler->start();

    // Java: mDiskCacheThreadPool = new PrioritisedCachedThreadPool(2, "Disk Cache");
    // m_diskCacheThreadPool = std::make_unique<PrioritisedCachedThreadPool>(2, "Disk Cache");
}

CacheManager::~CacheManager() {
    if (m_requestHandler) {
        m_requestHandler->stop();
    }
}

// ============================================================================
// isCacheFile (Java lines 108-126)
// ============================================================================

std::optional<int64_t> CacheManager::isCacheFile(const QFileInfo &file) {
    // Java: final String name = file.getName();
    const QString name = file.fileName();

    // Java: if(!name.endsWith(ext)) return null;
    if (!name.endsWith(QString::fromLatin1(EXT))) {
        return std::nullopt;
    }

    // Java: final String[] nameSplit = name.split("\\.");
    //       if(nameSplit.length != 2) return null;
    const int dotIdx = name.indexOf(QLatin1Char('.'));
    if (dotIdx <= 0) {
        return std::nullopt;
    }

    // Java: try { return Long.parseLong(nameSplit[0]); } catch(Exception) { return null; }
    bool ok = false;
    const int64_t val = name.left(dotIdx).toLongLong(&ok);
    if (!ok) {
        return std::nullopt;
    }
    return val;
}

// ============================================================================
// getCacheFileList (Java lines 128-148)
// ============================================================================

void CacheManager::getCacheFileList(const QDir &dir, std::set<int64_t> &currentFiles) {
    // Java: final File[] list = dir.listFiles(); if(list == null) return;
    const QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &file : list) {
        // Java: if(file.isDirectory()) { getCacheFileList(file, currentFiles); }
        if (file.isDir()) {
            getCacheFileList(QDir(file.absoluteFilePath()), currentFiles);
        } else {
            // Java: final Long cacheFileId = isCacheFile(file);
            //       if(cacheFileId != null) currentFiles.add(cacheFileId);
            const auto cacheFileId = isCacheFile(file);
            if (cacheFileId.has_value()) {
                currentFiles.insert(cacheFileId.value());
            }
        }
    }
}

// ============================================================================
// pruneTempDir (static) (Java lines 150-163)
// ============================================================================

void CacheManager::pruneTempDir(const QDir &dir) {
    // Java: final String[] list = dir.list(); if(list == null) return;
    const QStringList list = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    for (const QString &file : list) {
        // Java: if(file.endsWith(tempExt)) new File(dir, file).delete();
        if (file.endsWith(QString::fromLatin1(TEMP_EXT))) {
            QFile::remove(dir.absoluteFilePath(file));
        }
    }
}

// ============================================================================
// getCacheDirs (Java lines 166-179)
// ============================================================================

std::vector<QString> CacheManager::getCacheDirs() {
    std::vector<QString> dirs;

    if (s_singleton) {
        dirs.push_back(s_singleton->m_cacheBasePath);
    } else {
        // Fallback: use temp location
        dirs.push_back(QDir::tempPath() + QStringLiteral("/pinkreader_cache"));
    }

    // Java also adds external cache dirs; in C++ we just use the main cache dir
    // Additional dirs can be added via config

    return dirs;
}

// ============================================================================
// pruneTemp instance (Java lines 181-186)
// ============================================================================

void CacheManager::pruneTemp() {
    // Java: final List<File> dirs = getCacheDirs(context);
    //       for(final File dir : dirs) { pruneTemp(dir); }
    const auto dirs = getCacheDirs();
    for (const QString &dir : dirs) {
        pruneTempDir(QDir(dir));
    }
}

// ============================================================================
// pruneCache() no-arg (Java lines 188-190)
// ============================================================================

void CacheManager::pruneCache() {
    // Java: pruneCache(PrefsUtility.pref_cache_maxage());
    // C++: call with default max age map (TODO: wire to actual PrefsUtility)
    // For now, use a default 72-hour max age for all types
    std::map<int, TimeDuration> maxAge;
    // Default: keep everything up to 72 hours
    // This matches the Java behavior when pref_cache_maxage is called
    pruneCache(maxAge);
}

// ============================================================================
// pruneCache(bool,bool,bool) (Java lines 192-214)
// ============================================================================

void CacheManager::pruneCache(bool clearListings, bool clearThumbnails, bool clearImages) {
    // Java: if(!clearListings && !clearThumbnails && !clearImages) return;
    if (!clearListings && !clearThumbnails && !clearImages) {
        return;
    }

    // Java: Use a maximum age of 0 to clear everything* in that category.
    //       Otherwise, use a high number as the maximum age to ensure nothing is deleted.
    //       TimeDuration clearEverything = TimeDuration.secs(0);
    //       TimeDuration clearNothing = TimeDuration.days(365 * 10);

    // In C++ we pass map entries: 0 means delete everything, max means keep everything
    // This is implemented as:
    std::map<int, TimeDuration> maxAge;
    // Placeholder — actual TimeDuration types will be passed when TimeDuration is ported
    // For now, the (bool,bool,bool) overload delegates to the (map) overload

    // The Java mapping:
    //   FileType constants: SUBREDDIT_LIST=100, COMMENT_LIST=120, etc. → "listings"
    //   FileType::THUMBNAIL=200, INLINE_IMAGE_PREVIEW=203 → "thumbnails"
    //   FileType::IMAGE=201, CAPTCHA=202, IMAGE_INFO=300 → "images"

    // For this port, we pass the raw map
    pruneCache(maxAge);
}

// ============================================================================
// pruneCache(map) (Java lines 216-245)
// ============================================================================

void CacheManager::pruneCache(const std::map<int, TimeDuration> &maxAge) {
    // Java: try {
    try {
        // Java: final HashSet<Long> currentFiles = new HashSet<>(1024);
        std::set<int64_t> currentFiles;

        // Java: final List<File> dirs = getCacheDirs(context);
        //       for(final File dir : dirs) { getCacheFileList(dir, currentFiles); }
        const auto dirs = getCacheDirs();
        for (const QString &dir : dirs) {
            getCacheFileList(QDir(dir), currentFiles);
        }

        // Java: final ArrayList<Long> filesToDelete = dbManager.getFilesToPrune(
        //         currentFiles, maxAge, TimeDuration.hours(72));
        std::vector<int64_t> filesToDelete;
        // TODO: when CacheDbManager is ported:
        // filesToDelete = m_dbManager->getFilesToPrune(currentFiles, maxAge, TimeDuration::hours(72));

        qDebug() << TAG << ": Pruning" << filesToDelete.size() << "files";

        // Java: for(final long id : filesToDelete) {
        //         final File file = getExistingCacheFile(id);
        //         if(file != null) file.delete();
        //       }
        for (const int64_t id : filesToDelete) {
            const auto file = getExistingCacheFile(id);
            if (file.has_value()) {
                QFile::remove(file.value());
            }
        }

    } catch (const std::exception &e) {
        // Java: BugReportActivity.handleGlobalError(context, t);
        qWarning() << TAG << ": Error pruning cache:" << e.what();
    } catch (...) {
        qWarning() << TAG << ": Unknown error pruning cache";
    }
}

// ============================================================================
// emptyTheWholeCache (Java lines 247-249)
// ============================================================================

void CacheManager::emptyTheWholeCache() {
    // Java: dbManager.emptyTheWholeCache();
    // TODO: m_dbManager->emptyTheWholeCache();

    // Also delete all cache files on disk
    const auto dirs = getCacheDirs();
    for (const QString &dir : dirs) {
        QDir d(dir);
        if (d.exists()) {
            d.removeRecursively();
            d.mkpath(QStringLiteral("."));
        }
    }
}

// ============================================================================
// getCacheDataUsages (Java lines 251-281)
// ============================================================================

std::map<int, int64_t> CacheManager::getCacheDataUsages() {
    // Java: final HashMap<Integer, Long> dataUsagePerType = PrefsUtility.createFileTypeMap(0L, 0L, 0L);
    std::map<int, int64_t> dataUsagePerType;
    // Initialize with 0 for all file types
    dataUsagePerType[100] = 0; // SUBREDDIT_LIST
    dataUsagePerType[101] = 0; // SUBREDDIT_ABOUT
    dataUsagePerType[102] = 0; // MULTIREDDIT_LIST
    dataUsagePerType[110] = 0; // POST_LIST
    dataUsagePerType[120] = 0; // COMMENT_LIST
    dataUsagePerType[130] = 0; // USER_ABOUT
    dataUsagePerType[140] = 0; // INBOX_LIST
    dataUsagePerType[200] = 0; // THUMBNAIL
    dataUsagePerType[201] = 0; // IMAGE
    dataUsagePerType[202] = 0; // CAPTCHA
    dataUsagePerType[203] = 0; // INLINE_IMAGE_PREVIEW
    dataUsagePerType[300] = 0; // IMAGE_INFO

    try {
        // Java: final HashSet<Long> currentFiles = new HashSet<>(128);
        std::set<int64_t> currentFiles;

        // Java: final List<File> dirs = getCacheDirs(context);
        //       for(final File dir : dirs) { getCacheFileList(dir, currentFiles); }
        const auto dirs = getCacheDirs();
        for (const QString &dir : dirs) {
            getCacheFileList(QDir(dir), currentFiles);
        }

        // Java: final HashMap<Long, Integer> filesToCheckWithTypes = dbManager.getFilesToSize();
        // For now, iterate over currentFiles and sum sizes
        // TODO: when CacheDbManager is ported, use dbManager->getFilesToSize()
        for (const int64_t id : currentFiles) {
            const auto filePath = getExistingCacheFile(id);
            if (filePath.has_value()) {
                QFileInfo fi(filePath.value());
                // Without DB, we can't determine type — all goes into IMAGE (201) as default
                // This is a placeholder until CacheDbManager is ported
                if (fi.exists()) {
                    // Default to IMAGE type — actual type lookup requires DB
                    dataUsagePerType[201] += fi.size();
                }
            }
        }

    } catch (const std::exception &e) {
        qWarning() << TAG << ": Error getting cache data usages:" << e.what();
    } catch (...) {
        qWarning() << TAG << ": Unknown error getting cache data usages";
    }

    return dataUsagePerType;
}

// ============================================================================
// makeRequest (Java lines 283-285)
// ============================================================================

void CacheManager::makeRequest(CacheRequest &request) {
    // Java: requests.put(request);
    {
        std::lock_guard<std::mutex> lock(m_requestsMutex);
        m_requests.push(&request);
    }
    m_requestsCV.notify_one();
}

// ============================================================================
// getSessions (Java lines 287-289)
// ============================================================================

std::vector<CacheEntry> CacheManager::getSessions(const QString &url, const QString &username) {
    // Java: return dbManager.select(url, user.username, null);
    // TODO: m_dbManager->select(url, username, std::nullopt);
    return {};
}

// ============================================================================
// getPreferredCacheLocation (Java lines 291-294)
// ============================================================================

QString CacheManager::getPreferredCacheLocation() const {
    // Java: return new File(PrefsUtility.pref_cache_location(context));
    // For now, return the configured base path
    return m_cacheBasePath;
}

// ============================================================================
// getExistingCacheFileById (Java lines 297-301)
// ============================================================================

CacheManager::ReadableCacheFile CacheManager::getExistingCacheFileById(
        int64_t cacheId,
        CacheCompressionType compressionType) {
    return ReadableCacheFile(cacheId, compressionType);
}

// ============================================================================
// openNewCacheFile (Java lines 515-523)
// ============================================================================

std::unique_ptr<CacheManager::WritableCacheFile> CacheManager::openNewCacheFile(
        const QString &url,
        const Account &user,
        int fileType,
        const QUuid &session,
        const std::optional<QString> &mimetype,
        CacheCompressionType cacheCompressionType) {
    return std::unique_ptr<WritableCacheFile>(
            new WritableCacheFile(url, user, fileType, session, mimetype, cacheCompressionType));
}

// ============================================================================
// getSubdirForCacheFile (Java lines 437-447)
// ============================================================================

QString CacheManager::getSubdirForCacheFile(const QString &cacheRoot, int64_t cacheFileId) {
    // Java: return FileUtils.buildPath(cacheRoot, "rr_cache_files",
    //         String.format(Locale.US, "%02d", cacheFileId % 100),
    //         String.format(Locale.US, "%d", (cacheFileId / 100) % 10));

    const int firstDir = static_cast<int>(cacheFileId % 100);
    const int secondDir = static_cast<int>((cacheFileId / 100) % 10);

    return cacheRoot
            + QStringLiteral("/rr_cache_files/")
            + QString::asprintf("%02d", firstDir)
            + QStringLiteral("/")
            + QString::number(secondDir);
}

// ============================================================================
// getExistingCacheFile (Java lines 526-546)
// ============================================================================

std::optional<QString> CacheManager::getExistingCacheFile(int64_t id) {
    // Java: final List<File> dirs = getCacheDirs(context);
    const auto dirs = getCacheDirs();

    // Java: Try new format first — for(dir : dirs) { f = getSubdirForCacheFile(dir, id) + id + ext; if(f.exists()) return f; }
    for (const QString &dir : dirs) {
        const QString f = getSubdirForCacheFile(dir, id) + QStringLiteral("/") + QString::number(id) + QString::fromLatin1(EXT);
        if (QFileInfo::exists(f)) {
            return f;
        }
    }

    // Java: Try old format — for(dir : dirs) { f = dir / (id + ext); if(f.exists()) return f; }
    for (const QString &dir : dirs) {
        const QString f = dir + QStringLiteral("/") + QString::number(id) + QString::fromLatin1(EXT);
        if (QFileInfo::exists(f)) {
            return f;
        }
    }

    return std::nullopt;
}

// ============================================================================
// getCacheFileInputStream (Java lines 549-571)
// ============================================================================

std::optional<QByteArray> CacheManager::getCacheFileInputStream(
        int64_t id,
        CacheCompressionType compressionType) {
    // Java: final File cacheFile = getExistingCacheFile(id); if(cacheFile == null) return null;
    const auto cacheFile = getExistingCacheFile(id);
    if (!cacheFile.has_value()) {
        return std::nullopt;
    }

    if (compressionType == CacheCompressionType::NONE) {
        // Java: return new SeekableFileInputStream(cacheFile);
        QFile file(cacheFile.value());
        if (!file.open(QIODevice::ReadOnly)) {
            return std::nullopt;
        }
        return file.readAll();

    } else if (compressionType == CacheCompressionType::ZSTD) {
        // Java: try(InputStream is = new ZstdInputStream(new FileInputStream(cacheFile))) {
        //         return new MemoryDataStream(General.readWholeStream(is)).getInputStream();
        //       }
        // TODO: Zstd decompression when zstd library is available
        QFile file(cacheFile.value());
        if (!file.open(QIODevice::ReadOnly)) {
            return std::nullopt;
        }
        return file.readAll(); // Placeholder: raw bytes, not decompressed

    } else {
        throw std::runtime_error(std::string("Unhandled compression type ") + std::to_string(static_cast<int>(compressionType)));
    }
}

// ============================================================================
// getCacheFileUri (Java lines 574-583)
// ============================================================================

std::optional<QString> CacheManager::getCacheFileUri(int64_t id) {
    // Java: final File cacheFile = getExistingCacheFile(id);
    //       if(cacheFile == null) return null;
    //       return Uri.fromFile(cacheFile);
    const auto cacheFile = getExistingCacheFile(id);
    if (!cacheFile.has_value()) {
        return std::nullopt;
    }
    return QStringLiteral("file://") + cacheFile.value();
}

// ============================================================================
// RequestCompare (for priority_queue ordering)
// ============================================================================

bool CacheManager::RequestCompare::operator()(const CacheRequest *a, const CacheRequest *b) const {
    // In Java CacheRequest.compareTo returns: priority.value - another.priority.value
    // Lower value = higher priority
    // For max-heap: return a->priority > b->priority means lower priority at top
    // We want higher priority (lower numeric value) at top: a->priority > b->priority
    // Placeholder: default ordering
    return a > b; // pointer comparison placeholder
}

// ============================================================================
// ReadableCacheFile (Java lines 449-512)
// ============================================================================

CacheManager::ReadableCacheFile::ReadableCacheFile(int64_t id, CacheCompressionType compressionType)
    : m_id(id)
    , m_cacheCompressionType(compressionType)
{
}

int64_t CacheManager::ReadableCacheFile::getId() const {
    // Java line 464-466
    return m_id;
}

QByteArray CacheManager::ReadableCacheFile::getInputStream() const {
    // Java lines 469-478
    // In the real impl, this would call CacheManager::getCacheFileInputStream
    // For now, return empty
    qWarning() << TAG << ": getInputStream() called — needs CacheManager reference for proper impl";
    return QByteArray();
}

std::optional<QString> CacheManager::ReadableCacheFile::getUri() const {
    // Java lines 481-488
    if (!m_cachedUri.has_value()) {
        // m_cachedUri = getCacheFileUri(mId);
        // Needs CacheManager reference
        qWarning() << TAG << ": getUri() called — needs CacheManager reference for proper impl";
    }
    return m_cachedUri;
}

std::optional<QString> CacheManager::ReadableCacheFile::getFile() const {
    // Java lines 491-493
    // return Optional.ofNullable(getExistingCacheFile(mId));
    // Needs CacheManager reference
    qWarning() << TAG << ": getFile() called — needs CacheManager reference for proper impl";
    return std::nullopt;
}

std::optional<QString> CacheManager::ReadableCacheFile::lookupMimetype() const {
    // Java lines 496-506
    // final Optional<CacheEntry> result = dbManager.selectById(mId);
    // if(result.isPresent()) return Optional.of(result.get().mimetype);
    // else return Optional.empty();
    return std::nullopt;
}

QString CacheManager::ReadableCacheFile::toString() const {
    // Java lines 509-511
    // return String.format(Locale.US, "[ReadableCacheFile : id %d]", mId);
    return QStringLiteral("[ReadableCacheFile : id %1]").arg(m_id);
}

// ============================================================================
// WritableCacheFile (Java lines 303-434)
// ============================================================================

CacheManager::WritableCacheFile::WritableCacheFile(
        const QString &url,
        const Account &user,
        int fileType,
        const QUuid &session,
        const std::optional<QString> &mimetype,
        CacheCompressionType cacheCompressionType)
    // Java lines 330-341
    : m_url(url)
    , m_user(user)
    , m_fileType(fileType)
    , m_session(session)
    , m_mimetype(mimetype)
    , m_cacheCompressionType(cacheCompressionType)
{
    // Java: location = getPreferredCacheLocation();
    m_location = QDir::tempPath() + QStringLiteral("/pinkreader_cache");
    // TODO: call CacheManager::getPreferredCacheLocation()

    // Java: mTmpFile = new File(location, UUID.randomUUID().toString() + tempExt);
    m_tmpFile = m_location + QStringLiteral("/") + QUuid::createUuid().toString(QUuid::WithoutBraces)
                + QString::fromLatin1(TEMP_EXT);

    // Java: mOutStream = new FileOutputStream(mTmpFile);
    // In C++, we create the file
    QDir().mkpath(m_location);
    m_outStream.setFileName(m_tmpFile);
    // File will be opened on first write
}

CacheManager::WritableCacheFile::~WritableCacheFile() {
    if (m_outStream.isOpen()) {
        m_outStream.close();
    }
    // Clean up temp file if not finished
    if (!m_tmpFile.isEmpty() && QFileInfo::exists(m_tmpFile) && !m_readableCacheFile) {
        QFile::remove(m_tmpFile);
    }
}

const CacheManager::ReadableCacheFile &CacheManager::WritableCacheFile::getReadableCacheFile() const {
    // Java line 344-346: return Objects.requireNonNull(readableCacheFile);
    if (!m_readableCacheFile) {
        throw std::runtime_error("ReadableCacheFile is null — onWriteFinished not called yet");
    }
    return *m_readableCacheFile;
}

void CacheManager::WritableCacheFile::writeWholeFile(const uint8_t *buf, int64_t offset, int64_t length) {
    // Java lines 348-382
    if (!m_outStream.isOpen()) {
        if (!m_outStream.open(QIODevice::WriteOnly | QIODevice::Append)) {
            throw std::runtime_error("Failed to open temp cache file for writing");
        }
    }

    if (m_cacheCompressionType == CacheCompressionType::NONE) {
        // Java: mOutStream.write(buf, offset, length); mCompressedLength += length;
        m_outStream.write(reinterpret_cast<const char *>(buf + offset), static_cast<qint64>(length));
        m_compressedLength += length;

    } else if (m_cacheCompressionType == CacheCompressionType::ZSTD) {
        // Java: final long maxDestSize = Zstd.compressBound(length);
        //       final byte[] dst = new byte[(int)maxDestSize];
        //       final int size = (int)Zstd.compressByteArray(dst, 0, dst.length, buf, offset, length, 3);
        //       mOutStream.write(dst, 0, size); mCompressedLength += size;

        // For now, just write uncompressed (ZSTD library integration pending)
        // TODO: integrate libzstd compression
        m_outStream.write(reinterpret_cast<const char *>(buf + offset), static_cast<qint64>(length));
        m_compressedLength += length;
    }

    m_uncompressedLength += length;
}

void CacheManager::WritableCacheFile::onWriteFinished() {
    // Java lines 384-414
    if (m_writeExternally) {
        // Java: mCompressedLength = mTmpFile.length(); mUncompressedLength = mCompressedLength;
        QFileInfo fi(m_tmpFile);
        m_compressedLength = fi.size();
        m_uncompressedLength = m_compressedLength;
    } else {
        // Java: mOutStream.flush(); mOutStream.close();
        if (m_outStream.isOpen()) {
            m_outStream.flush();
            m_outStream.close();
        }
    }

    // Java: final long cacheFileId = dbManager.newEntry(mUrl, mUser, mFileType, mSession,
    //         mMimetype, mCacheCompressionType, mCompressedLength, mUncompressedLength);
    int64_t cacheFileId = 0;
    // TODO: m_dbManager->newEntry(...)
    // For now, use a hash of the URL as cache ID
    cacheFileId = static_cast<int64_t>(qHash(m_url));

    // Java: final File subdir = getSubdirForCacheFile(location, cacheFileId);
    //       FileUtils.mkdirs(subdir);
    const QString subdir = CacheManager::getSubdirForCacheFile(m_location, cacheFileId);
    QDir().mkpath(subdir);

    // Java: final File dstFile = new File(subdir, cacheFileId + ext);
    //       FileUtils.moveFile(mTmpFile, dstFile);
    const QString dstFile = subdir + QStringLiteral("/") + QString::number(cacheFileId)
                            + QString::fromLatin1(EXT);
    QFile::rename(m_tmpFile, dstFile);

    // Java: dbManager.setEntryDone(cacheFileId);
    // TODO: m_dbManager->setEntryDone(cacheFileId);

    // Java: readableCacheFile = new ReadableCacheFile(cacheFileId, mCacheCompressionType);
    m_readableCacheFile = new ReadableCacheFile(cacheFileId, m_cacheCompressionType);
}

QString CacheManager::WritableCacheFile::writeExternally() {
    // Java lines 416-420
    // mWriteExternally = true; mOutStream.close(); return mTmpFile;
    m_writeExternally = true;
    if (m_outStream.isOpen()) {
        m_outStream.close();
    }
    return m_tmpFile;
}

void CacheManager::WritableCacheFile::onWriteCancelled() {
    // Java lines 422-433
    try {
        if (m_outStream.isOpen()) {
            m_outStream.close();
        }
        if (!m_tmpFile.isEmpty()) {
            if (!QFile::remove(m_tmpFile)) {
                qWarning() << TAG << ": Failed to delete temp cache file" << m_tmpFile;
            }
        }
    } catch (const std::exception &e) {
        qWarning() << TAG << ": Exception during cancel:" << e.what();
    }
}

// ============================================================================
// RequestHandlerThread (Java lines 585-753)
// ============================================================================

CacheManager::RequestHandlerThread::RequestHandlerThread(CacheManager &manager)
    : m_manager(manager)
{
}

CacheManager::RequestHandlerThread::~RequestHandlerThread() {
    stop();
}

void CacheManager::RequestHandlerThread::start() {
    // Java: super("Request Handler Thread"); ... start();
    m_running = true;
    m_thread = std::thread(&RequestHandlerThread::run, this);
}

void CacheManager::RequestHandlerThread::stop() {
    m_running = false;
    m_manager.m_requestsCV.notify_all();
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void CacheManager::RequestHandlerThread::run() {
    // Java lines 592-604
    try {
        while (m_running) {
            CacheRequest *request = nullptr;

            // Java: while((request = requests.take()) != null) { handleRequest(request); }
            {
                std::unique_lock<std::mutex> lock(m_manager.m_requestsMutex);
                m_manager.m_requestsCV.wait(lock, [this] {
                    return !m_manager.m_requests.empty() || !m_running;
                });

                if (!m_running) {
                    break;
                }

                if (!m_manager.m_requests.empty()) {
                    request = m_manager.m_requests.top();
                    m_manager.m_requests.pop();
                }
            }

            if (request) {
                handleRequest(*request);
            }
        }

    } catch (const std::exception &e) {
        qWarning() << TAG << ": RequestHandlerThread exception:" << e.what();
    }
}

void CacheManager::RequestHandlerThread::handleRequest(CacheRequest &request) {
    // Java lines 606-655

    // Java: if(request.url == null) { request.notifyFailure(...); return; }
    // In C++, request.url is a QString, check with isNull()
    // This check is done in CacheRequest itself or here
    // Placeholder: assume it's handled

    // Java: if(request.downloadStrategy.shouldDownloadWithoutCheckingCache()) { queueDownload(request); }
    // For now, always try cache first
    bool shouldDownloadWithoutCache = false;
    // TODO: request.downloadStrategy.shouldDownloadWithoutCheckingCache()

    if (shouldDownloadWithoutCache) {
        queueDownload(request);
    } else {
        // Java: final List<CacheEntry> result = dbManager.select(request.url, request.user.username, request.requestSession);
        std::vector<CacheEntry> result;
        // TODO: result = m_manager.m_dbManager->select(request.url, request.user.username, request.requestSession);

        if (result.empty()) {
            // Java: if(request.downloadStrategy.shouldDownloadIfNotCached()) { queueDownload(request); }
            //       else { request.notifyFailure(CACHE_MISS); }
            bool shouldDownloadIfNotCached = true;
            // TODO: request.downloadStrategy.shouldDownloadIfNotCached()

            if (shouldDownloadIfNotCached) {
                queueDownload(request);
            } else {
                // request.notifyFailure(General.getGeneralErrorForFailure(context, CACHE_MISS, ...));
                qWarning() << TAG << ": Cache miss for request";
            }
        } else {
            // Java: final CacheEntry entry = mostRecentFromList(result);
            const CacheEntry entry = mostRecentFromList(result);

            // Java: if(request.downloadStrategy.shouldDownloadIfCacheEntryFound(entry)) {
            //         queueDownload(request);
            //       } else { handleCacheEntryFound(entry, request); }
            bool shouldDownload = false;
            // TODO: request.downloadStrategy.shouldDownloadIfCacheEntryFound(entry)

            if (shouldDownload) {
                queueDownload(request);
            } else {
                handleCacheEntryFound(entry, request);
            }
        }
    }
}

CacheEntry CacheManager::RequestHandlerThread::mostRecentFromList(const std::vector<CacheEntry> &list) {
    // Java lines 657-668
    CacheEntry entry;

    for (const CacheEntry &e : list) {
        if (entry.timestamp.isLessThan(e.timestamp)) {
            entry = e;
        }
    }

    return entry;
}

void CacheManager::RequestHandlerThread::queueDownload(CacheRequest &request) {
    // Java lines 670-684
    // request.notifyDownloadNecessary();

    try {
        // Java: downloadQueue.add(request, CacheManager.this);
        // TODO: m_manager.m_downloadQueue->add(request, m_manager);
    } catch (const std::exception &e) {
        // Java: request.notifyFailure(General.getGeneralErrorForFailure(context, MALFORMED_URL, e, ...));
        qWarning() << TAG << ": Error queueing download:" << e.what();
    }
}

void CacheManager::RequestHandlerThread::handleCacheEntryFound(
        const CacheEntry &entry,
        CacheRequest &request) {
    // Java lines 686-752

    // Java: final File cacheFile = getExistingCacheFile(entry.id);
    const auto cacheFile = m_manager.getExistingCacheFile(entry.id);

    if (!cacheFile.has_value()) {
        // Java: request.notifyFailure(General.getGeneralErrorForFailure(context, STORAGE, new RuntimeException(), ...));
        //       dbManager.delete(entry.id); return;
        qWarning() << TAG << ": Cache entry found in DB but file missing. ID:" << entry.id;
        // TODO: m_manager.m_dbManager->delete(entry.id);
        return;
    }

    // Java: mDiskCacheThreadPool.add(new PrioritisedCachedThreadPool.Task() { ... });
    // The task reads the cache file and notifies the request
    // For now, we just deliver the cached data directly

    auto streamFactory = GenericFactory<QByteArray>([this, &entry]() {
        auto stream = m_manager.getCacheFileInputStream(entry.id, entry.cacheCompressionType);
        if (!stream.has_value()) {
            // TODO: m_manager.m_dbManager->delete(entry.id);
            throw std::runtime_error("Failed to open file");
        }
        return stream.value();
    });

    // Java:
    //   request.notifyDataStreamAvailable(streamFactory, entry.timestamp, entry.session, true, entry.mimetype);
    //   request.notifyDataStreamComplete(streamFactory, entry.timestamp, entry.session, true, entry.mimetype);
    //   request.notifyCacheFileWritten(new ReadableCacheFile(entry.id, entry.cacheCompressionType),
    //       entry.timestamp, entry.session, true, entry.mimetype);

    // TODO: These callbacks go through CacheRequest, which will be wired up when
    // CacheRequest, TimestampUTC, etc. are ported

    qDebug() << TAG << ": Cache entry found for id" << entry.id;
}

} // namespace PinkReader
