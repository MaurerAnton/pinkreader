// Origin: org/quantumbadger/redreader/cache/CacheContentProvider.java
// Android ContentProvider ported to C++ (heavily stubbed - Android-specific)
#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <optional>

namespace PinkReader {

class CacheManager;
enum class CacheCompressionType;

class CacheContentProvider {
private:
	static const char* TAG;
	static const std::string COLUMNS[2]; // {OpenableColumns.DISPLAY_NAME, OpenableColumns.SIZE}

	CacheManager* m_cacheManager;

public:
	CacheContentProvider();

	// Static helpers
	static std::string generateFilename(
			int64_t cacheId,
			const std::string& mimetype,
			const std::string& defaultExtension);

	static std::optional<int64_t> getCacheIdFromFilename(const std::string& filename);

	static std::string getUriForFile(
			int64_t cacheId,
			const std::string& mimetype,
			const std::string& defaultExtension);

	// ContentProvider overrides
	bool onCreate();

	// openFile equivalent
	std::string openFile(const std::string& uri, const std::string& mode);

	// query equivalent
	struct QueryRow {
		std::string displayName;
		int64_t size;
		std::string mimeType;
	};

	std::vector<QueryRow> query(
			const std::string& uri,
			const std::vector<std::string>& projection);

	// getType equivalent
	std::string getType(const std::string& uri);

	// Not supported operations
	void insert(const std::string& uri);
	int deleteEntry(const std::string& uri);
	int update(const std::string& uri);

private:
	std::optional<CacheManager::ReadableCacheFile> getReadableCacheFile(const std::string& uri);
	std::optional<std::string> getFile(const std::string& uri);
};

} // namespace PinkReader
