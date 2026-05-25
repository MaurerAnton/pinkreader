// Origin: org/quantumbadger/redreader/cache/CacheDbManager.java
// SQLiteOpenHelper subclass ported to C++ (uses stubbed SQLite or in-memory store)
#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <stdexcept>

namespace PinkReader {

class UriString;
class RedditAccount;
class TimestampUTC;
class TimeDuration;
class UUID;
class CacheEntry;
enum class CacheCompressionType;
struct Optional;

class CacheDbManager {
public:
	static const char* FIELD_URL;
	static const char* FIELD_ID;
	static const char* FIELD_TIMESTAMP;
	static const char* FIELD_SESSION;
	static const char* FIELD_USER;
	static const char* FIELD_STATUS;
	static const char* FIELD_TYPE;
	static const char* FIELD_MIMETYPE;
	static const char* FIELD_COMPRESSION_TYPE;
	static const char* FIELD_LENGTH_UNCOMPRESSED;
	static const char* FIELD_LENGTH_COMPRESSED;

private:
	static const char* CACHE_DB_FILENAME;
	static const char* TABLE;

	static const int STATUS_MOVING = 1;
	static const int STATUS_DONE = 2;
	static const int CACHE_DB_VERSION = 2;

	void* m_context; // Opaque context
	std::mutex m_mutex;

	// In-memory database simulation
	struct Row {
		int64_t id = 0;
		std::string url;
		std::string user;
		std::string session;
		int64_t timestamp = 0;
		int status = 0;
		int type = 0;
		std::string mimetype;
		int compressionType = 0;
		int64_t lengthCompressed = 0;
		int64_t lengthUncompressed = 0;
	};

	std::vector<Row> m_rows;
	int64_t m_nextId = 1;

public:
	explicit CacheDbManager(void* context);

	// onCreate equivalent
	void createTable();

	// onUpgrade equivalent
	void upgradeTable(int oldVersion, int newVersion);

	// selectById
	CacheEntry* selectById(int64_t id);

	// select
	std::vector<CacheEntry> select(
			const UriString& url,
			const std::string& user,
			const UUID* session);

	// newEntry
	int64_t newEntry(
			const UriString& url,
			const RedditAccount& user,
			int fileType,
			const UUID& session,
			const std::string& mimetype,
			CacheCompressionType compressionType,
			int64_t lengthCompressed,
			int64_t lengthUncompressed);

	// setEntryDone
	void setEntryDone(int64_t id);

	// delete
	int deleteEntry(int64_t id);

	// getFilesToPrune
	std::vector<int64_t> getFilesToPrune(
			const std::unordered_set<int64_t>& currentFiles,
			const std::unordered_map<int, TimeDuration>& maxAge,
			const TimeDuration& defaultMaxAge);

	// emptyTheWholeCache
	void emptyTheWholeCache();

	// getFilesToSize
	std::unordered_map<int64_t, int> getFilesToSize();

private:
	std::vector<CacheEntry> readEntriesFromCursor(const std::vector<Row>& cursor);
};

} // namespace PinkReader
