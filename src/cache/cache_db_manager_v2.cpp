// Origin: org/quantumbadger/redreader/cache/CacheDbManager.java
#include "cache_db_manager_v2.h"
#include "cache_entry_v2.h"
#include "cache_compression_type.h"

#include <algorithm>
#include <cstdio>
#include <ctime>

namespace PinkReader {

const char* CacheDbManager::CACHE_DB_FILENAME = "cache.db";
const char* CacheDbManager::TABLE = "web";

const char* CacheDbManager::FIELD_URL = "url";
const char* CacheDbManager::FIELD_ID = "id";
const char* CacheDbManager::FIELD_TIMESTAMP = "timestamp";
const char* CacheDbManager::FIELD_SESSION = "session";
const char* CacheDbManager::FIELD_USER = "user";
const char* CacheDbManager::FIELD_STATUS = "status";
const char* CacheDbManager::FIELD_TYPE = "type";
const char* CacheDbManager::FIELD_MIMETYPE = "mimetype";
const char* CacheDbManager::FIELD_COMPRESSION_TYPE = "compressionType";
const char* CacheDbManager::FIELD_LENGTH_UNCOMPRESSED = "lengthUncompressed";
const char* CacheDbManager::FIELD_LENGTH_COMPRESSED = "lengthCompressed";

CacheDbManager::CacheDbManager(void* context)
	: m_context(context) {}

void CacheDbManager::createTable() {
	// SQL: CREATE TABLE web (
	//   id INTEGER PRIMARY KEY AUTOINCREMENT,
	//   url TEXT NOT NULL,
	//   user TEXT NOT NULL,
	//   session TEXT NOT NULL,
	//   timestamp INTEGER,
	//   status INTEGER,
	//   type INTEGER,
	//   mimetype TEXT,
	//   compressionType INTEGER,
	//   lengthCompressed INTEGER,
	//   lengthUncompressed INTEGER,
	//   UNIQUE (user, url, session) ON CONFLICT REPLACE)
	std::lock_guard<std::mutex> lock(m_mutex);
	// In-memory store auto-initialized
}

void CacheDbManager::upgradeTable(int oldVersion, int newVersion) {
	std::lock_guard<std::mutex> lock(m_mutex);
	if(oldVersion < 2) {
		// ALTER TABLE web ADD COLUMN compressionType INTEGER NOT NULL DEFAULT 0
		// ALTER TABLE web ADD COLUMN lengthUncompressed INTEGER NOT NULL DEFAULT 0
		// ALTER TABLE web ADD COLUMN lengthCompressed INTEGER NOT NULL DEFAULT 0
		// In in-memory store, columns always present
	}
}

CacheEntry* CacheDbManager::selectById(int64_t id) {
	std::lock_guard<std::mutex> lock(m_mutex);

	for(const auto& row : m_rows) {
		if(row.id == id && row.status == STATUS_DONE) {
			// Return most recent (by timestamp DESC)
			return nullptr; // Stub: requires full UriString/UUID/TimestampUTC types
		}
	}
	return nullptr;
}

std::vector<CacheEntry> CacheDbManager::select(
		const UriString& url,
		const std::string& user,
		const UUID* session) {

	std::lock_guard<std::mutex> lock(m_mutex);
	std::vector<Row> matchingRows;

	for(const auto& row : m_rows) {
		if(row.status != STATUS_DONE) continue;
		// if(url.value != row.url) continue;  // url.value comparison (UriString stub)
		if(row.user != user) continue;
		if(session != nullptr) {
			// session.toString() comparison stub
		}
		matchingRows.push_back(row);
	}

	// Sort by timestamp DESC
	std::sort(matchingRows.begin(), matchingRows.end(),
		[](const Row& a, const Row& b) { return a.timestamp > b.timestamp; });

	return readEntriesFromCursor(matchingRows);
}

int64_t CacheDbManager::newEntry(
		const UriString& url,
		const RedditAccount& user,
		int fileType,
		const UUID& session,
		const std::string& mimetype,
		CacheCompressionType compressionType,
		int64_t lengthCompressed,
		int64_t lengthUncompressed) {

	std::lock_guard<std::mutex> lock(m_mutex);

	Row row;
	row.id = m_nextId++;
	row.url = ""; // url.value
	row.user = ""; // user.username
	row.session = ""; // session.toString()
	row.timestamp = static_cast<int64_t>(std::time(nullptr)) * 1000; // TimestampUTC.now().toUtcMs()
	row.status = STATUS_MOVING;
	row.type = fileType;
	row.mimetype = mimetype;
	row.compressionType = cacheCompressionTypeToDatabaseId(compressionType);
	row.lengthCompressed = lengthCompressed;
	row.lengthUncompressed = lengthUncompressed;

	// Check UNIQUE constraint: (user, url, session) ON CONFLICT REPLACE
	for(auto& existing : m_rows) {
		if(existing.url == row.url && existing.user == row.user && existing.session == row.session) {
			existing = row;
			return row.id;
		}
	}

	m_rows.push_back(row);
	return row.id;
}

void CacheDbManager::setEntryDone(int64_t id) {
	std::lock_guard<std::mutex> lock(m_mutex);
	for(auto& row : m_rows) {
		if(row.id == id) {
			row.status = STATUS_DONE;
			return;
		}
	}
}

int CacheDbManager::deleteEntry(int64_t id) {
	std::lock_guard<std::mutex> lock(m_mutex);
	int deleted = 0;
	m_rows.erase(
		std::remove_if(m_rows.begin(), m_rows.end(),
			[id](const Row& row) { return row.id == id; }),
		m_rows.end());
	return deleted;
}

std::vector<int64_t> CacheDbManager::getFilesToPrune(
		const std::unordered_set<int64_t>& currentFiles,
		const std::unordered_map<int, TimeDuration>& maxAge,
		const TimeDuration& defaultMaxAge) {

	std::lock_guard<std::mutex> lock(m_mutex);

	std::unordered_set<int64_t> currentEntries;
	std::vector<int64_t> entriesToDelete;
	std::vector<int64_t> filesToDelete;

	// TimestampUTC currentTime = TimestampUTC.now() - stubbed
	int64_t currentTimeMs = static_cast<int64_t>(std::time(nullptr)) * 1000;

	for(const auto& row : m_rows) {
		int64_t id = row.id;
		int64_t timestampMs = row.timestamp;
		int type = row.type;

		int64_t pruneIfBeforeMs;

		auto it = maxAge.find(type);
		if(it != maxAge.end()) {
			// currentTime.subtract(maxAge.get(type))
			// pruneIfBeforeMs = currentTimeMs - it->second.toUtcMs();
			pruneIfBeforeMs = currentTimeMs - 3600000; // 1 hour stub
		} else {
			// Log.e("RR DEBUG cache", "Using default age! Filetype " + type);
			pruneIfBeforeMs = currentTimeMs - 3600000; // defaultMaxAge stub
		}

		if(currentFiles.find(id) == currentFiles.end()) {
			entriesToDelete.push_back(id);
		} else if(timestampMs < pruneIfBeforeMs) {
			entriesToDelete.push_back(id);
			filesToDelete.push_back(id);
		} else {
			currentEntries.insert(id);
		}
	}

	for(int64_t id : currentFiles) {
		if(currentEntries.find(id) == currentEntries.end()) {
			filesToDelete.push_back(id);
		}
	}

	// Delete entries from DB
	if(!entriesToDelete.empty()) {
		m_rows.erase(
			std::remove_if(m_rows.begin(), m_rows.end(),
				[&entriesToDelete](const Row& row) {
					return std::find(entriesToDelete.begin(), entriesToDelete.end(), row.id)
						!= entriesToDelete.end();
				}),
			m_rows.end());
	}

	return filesToDelete;
}

void CacheDbManager::emptyTheWholeCache() {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_rows.clear();
}

std::unordered_map<int64_t, int> CacheDbManager::getFilesToSize() {
	std::lock_guard<std::mutex> lock(m_mutex);
	std::unordered_map<int64_t, int> filesToCheck;

	for(const auto& row : m_rows) {
		filesToCheck[row.id] = row.type;
	}

	return filesToCheck;
}

std::vector<CacheEntry> CacheDbManager::readEntriesFromCursor(const std::vector<Row>& rows) {
	std::vector<CacheEntry> result;
	// Stub: requires full UriString, UUID, TimestampUTC types to construct CacheEntry
	// For now, we'd construct CacheEntry objects from Row data
	(void)rows;
	return result;
}

} // namespace PinkReader
