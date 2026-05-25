// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/PrefsBackup.java
#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>
#include <cstdint>
#include <any>

namespace PinkReader {

// Forward declarations
class DataOutputStream;
class DataInputStream;
struct AnyHash;
struct AnyEqual;

/** Map with any-comparable keys (from serialize_utils.h). */
using AnyMap = std::unordered_map<std::any, std::any, AnyHash, AnyEqual>;

/**
 * Preferences backup/restore utility.
 * Equivalent to: org.quantumbadger.redreader.common.PrefsBackup
 */
class PrefsBackup {
public:
	/** Interface for opening an output stream for backup. */
	class BackupDestination {
	public:
		virtual ~BackupDestination() = default;
		virtual std::unique_ptr<DataOutputStream> openOutputStream() = 0;
	};

	/** Interface for opening an input stream for restore. */
	class BackupSource {
	public:
		virtual ~BackupSource() = default;
		virtual std::unique_ptr<DataInputStream> openInputStream() = 0;
	};

	/** Interface for shared preferences access. */
	class SharedPrefsAccess {
	public:
		virtual ~SharedPrefsAccess() = default;
		virtual std::map<std::string, std::any> getAllClone() = 0;
		virtual void performActionWithWriteLock(
			std::function<void(std::map<std::string, std::any>&)> action) = 0;
	};

private:
	PrefsBackup() = delete;

	static const std::string TAG;
	static const std::string MAGIC_HEADER_STR;
	static const std::vector<uint8_t> MAGIC_HEADER;
	static const std::string FIELD_TYPE;
	static const std::string FIELD_FILE_VERSION;
	static const std::string FIELD_VERSION_CODE;
	static const std::string FIELD_VERSION_NAME;
	static const std::string FIELD_IS_ALPHA;
	static const std::string FIELD_TIMESTAMP_UTC;
	static const std::string FIELD_PREFS;
	static const std::string FILE_TYPE;
	static const int32_t FILE_VERSION;
	static const std::set<std::string> IGNORED_PREFS;

	class MapReader {
	private:
		const AnyMap& mMap;
		static std::string anyToString(const std::any& value);

	public:
		explicit MapReader(const AnyMap& map);
		const std::any& getRequired(const std::string& key) const;
		std::string getRequiredString(const std::string& key) const;
		const AnyMap& getRequiredMap(const std::string& key) const;
		int32_t getRequiredInt(const std::string& key) const;
		int64_t getRequiredLong(const std::string& key) const;
		bool getRequiredBoolean(const std::string& key) const;
	};

public:
	static void backup(
		SharedPrefsAccess& prefs,
		BackupDestination& destination,
		std::function<void()> onSuccess,
		std::function<void(const std::string&)> onError,
		int32_t versionCode,
		const std::string& versionName,
		bool isAlpha);

	static void restore(
		SharedPrefsAccess& prefs,
		BackupSource& source,
		std::function<void()> onSuccess,
		std::function<void(const std::string&)> onError,
		int32_t currentVersionCode,
		std::function<void(std::function<void()>)> onVersionWarning);
};

} // namespace PinkReader
