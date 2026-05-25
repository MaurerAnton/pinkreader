// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/PrefsBackup.java

#include "prefs_backup.h"
#include "serialize_utils.h"
#include <any>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <chrono>

namespace PinkReader {

const std::string PrefsBackup::TAG = "PrefsBackup";

const std::string PrefsBackup::MAGIC_HEADER_STR = "RedReader preferences backup\r\n";
const std::vector<uint8_t> PrefsBackup::MAGIC_HEADER(
	MAGIC_HEADER_STR.begin(), MAGIC_HEADER_STR.end());

const std::string PrefsBackup::FIELD_TYPE = "type";
const std::string PrefsBackup::FIELD_FILE_VERSION = "file_version";
const std::string PrefsBackup::FIELD_VERSION_CODE = "version_code";
const std::string PrefsBackup::FIELD_VERSION_NAME = "version_name";
const std::string PrefsBackup::FIELD_IS_ALPHA = "is_alpha";
const std::string PrefsBackup::FIELD_TIMESTAMP_UTC = "timestamp_utc";
const std::string PrefsBackup::FIELD_PREFS = "prefs";

const std::string PrefsBackup::FILE_TYPE = "redreader_prefs_backup";
const int32_t PrefsBackup::FILE_VERSION = 1;

const std::set<std::string> PrefsBackup::IGNORED_PREFS = {
	"AnnouncementDownloader_LastReadId",
	"AnnouncementDownloader_PayloadStorageHex",
	"NewMessageChecker_SavedMessageId",
	"NewMessageChecker_SavedMessageTimestamp",
	"FeatureFlagHandler_LastVersion",
	"FeatureFlagHandler_FirstRunMessageShown"
};

// --- MapReader ---

PrefsBackup::MapReader::MapReader(const AnyMap& map)
	: mMap(map) {}

std::string PrefsBackup::MapReader::anyToString(const std::any& value) {
	if (!value.has_value()) return "null";
	if (value.type() == typeid(std::string))
		return std::any_cast<std::string>(value);
	if (value.type() == typeid(int32_t))
		return std::to_string(std::any_cast<int32_t>(value));
	if (value.type() == typeid(int64_t))
		return std::to_string(std::any_cast<int64_t>(value));
	if (value.type() == typeid(bool))
		return std::any_cast<bool>(value) ? "true" : "false";
	return "?";
}

const std::any& PrefsBackup::MapReader::getRequired(const std::string& key) const {
	std::any lookupKey(key);

	auto it = mMap.find(lookupKey);
	if (it == mMap.end()) {
		throw std::runtime_error("Missing field: '" + key + "'");
	}
	return it->second;
}

std::string PrefsBackup::MapReader::getRequiredString(const std::string& key) const {
	const std::any& result = getRequired(key);

	if (result.type() != typeid(std::string)) {
		throw std::runtime_error("Expecting string for key '" + key + "', got "
			+ std::string(result.type().name()));
	}

	return std::any_cast<std::string>(result);
}

const AnyMap& PrefsBackup::MapReader::getRequiredMap(const std::string& key) const {
	const std::any& result = getRequired(key);

	if (result.type() != typeid(AnyMap)) {
		throw std::runtime_error("Expecting map for key '" + key + "', got "
			+ std::string(result.type().name()));
	}

	return std::any_cast<const AnyMap&>(result);
}

int32_t PrefsBackup::MapReader::getRequiredInt(const std::string& key) const {
	const std::any& result = getRequired(key);

	if (result.type() != typeid(int32_t)) {
		throw std::runtime_error("Expecting integer for key '" + key + "', got "
			+ std::string(result.type().name()));
	}

	return std::any_cast<int32_t>(result);
}

int64_t PrefsBackup::MapReader::getRequiredLong(const std::string& key) const {
	const std::any& result = getRequired(key);

	if (result.type() != typeid(int64_t)) {
		throw std::runtime_error("Expecting long for key '" + key + "', got "
			+ std::string(result.type().name()));
	}

	return std::any_cast<int64_t>(result);
}

bool PrefsBackup::MapReader::getRequiredBoolean(const std::string& key) const {
	const std::any& result = getRequired(key);

	if (result.type() != typeid(bool)) {
		throw std::runtime_error("Expecting boolean for key '" + key + "', got "
			+ std::string(result.type().name()));
	}

	return std::any_cast<bool>(result);
}

// --- Backup ---

void PrefsBackup::backup(
		SharedPrefsAccess& prefs,
		BackupDestination& destination,
		std::function<void()> onSuccess,
		std::function<void(const std::string&)> onError,
		int32_t versionCode,
		const std::string& versionName,
		bool isAlpha) {

	try {
		std::map<std::string, std::any> prefMap = prefs.getAllClone();

		// Remove ignored prefs
		for (const auto& ignoredPref : IGNORED_PREFS) {
			prefMap.erase(ignoredPref);
		}

		// Convert string-keyed map to any-keyed AnyMap for serialization
		AnyMap prefMapAny;
		for (const auto& entry : prefMap) {
			prefMapAny.emplace(std::any(std::string(entry.first)), entry.second);
		}

		// Build backup metadata
		AnyMap rootMap;
		rootMap.emplace(std::any(std::string(FIELD_TYPE)), std::any(std::string(FILE_TYPE)));
		rootMap.emplace(std::any(std::string(FIELD_FILE_VERSION)), std::any(FILE_VERSION));
		rootMap.emplace(std::any(std::string(FIELD_VERSION_CODE)), std::any(versionCode));
		rootMap.emplace(std::any(std::string(FIELD_VERSION_NAME)), std::any(versionName));
		rootMap.emplace(std::any(std::string(FIELD_IS_ALPHA)), std::any(isAlpha));

		// Current timestamp in milliseconds since epoch
		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			now.time_since_epoch()).count();
		rootMap.emplace(std::any(std::string(FIELD_TIMESTAMP_UTC)),
			std::any(static_cast<int64_t>(ms)));

		rootMap.emplace(std::any(std::string(FIELD_PREFS)), std::any(prefMapAny));

		// Open destination and serialize
		auto outStream = destination.openOutputStream();

		// Write magic header
		outStream->write(MAGIC_HEADER.data(), MAGIC_HEADER.size());

		// Serialize the root map
		SerializeUtils::serialize(*outStream, std::any(rootMap));
		outStream->flush();

		onSuccess();

	} catch (const std::exception& e) {
		onError(e.what());
	}
}

// --- Restore ---

void PrefsBackup::restore(
		SharedPrefsAccess& prefs,
		BackupSource& source,
		std::function<void()> onSuccess,
		std::function<void(const std::string&)> onError,
		int32_t currentVersionCode,
		std::function<void(std::function<void()>)> onVersionWarning) {

	try {
		auto inStream = source.openInputStream();

		// Read and verify magic header
		std::vector<uint8_t> magicHeader(MAGIC_HEADER.size());
		inStream->readFully(magicHeader.data(), MAGIC_HEADER.size());

		if (std::memcmp(magicHeader.data(), MAGIC_HEADER.data(), MAGIC_HEADER.size()) != 0) {
			onError("Invalid file: wrong magic header");
			return;
		}

		// Deserialize root map
		std::any rootObj = SerializeUtils::deserialize(*inStream);

		if (!rootObj.has_value()) {
			onError("Expecting Map, got null");
			return;
		}

		if (rootObj.type() != typeid(AnyMap)) {
			onError("Expecting Map, got " + std::string(rootObj.type().name()));
			return;
		}

		const auto& rootMap = std::any_cast<const AnyMap&>(rootObj);
		MapReader root(rootMap);

		std::string type = root.getRequiredString(FIELD_TYPE);
		int32_t fileVersion = root.getRequiredInt(FIELD_FILE_VERSION);
		int32_t versionCode = root.getRequiredInt(FIELD_VERSION_CODE);
		std::string versionName = root.getRequiredString(FIELD_VERSION_NAME);
		bool isAlpha = root.getRequiredBoolean(FIELD_IS_ALPHA);
		int64_t timestampUtc = root.getRequiredLong(FIELD_TIMESTAMP_UTC);
		const auto& restorePrefs = root.getRequiredMap(FIELD_PREFS);

		if (type != FILE_TYPE) {
			onError("Invalid file type: " + type);
			return;
		}

		if (fileVersion > FILE_VERSION) {
			onError("File version " + std::to_string(fileVersion)
				+ " is newer than supported (" + std::to_string(FILE_VERSION) + ")");
			return;
		}

		// The actual restore action
		auto doRestore = [&prefs, &restorePrefs, onSuccess]() {
			prefs.performActionWithWriteLock([&](std::map<std::string, std::any>& sharedPrefs) {
				// Build set of existing keys to remove
				std::set<std::string> keysToRemove;
				for (const auto& entry : sharedPrefs) {
					keysToRemove.insert(entry.first);
				}
				for (const auto& ignoredPref : IGNORED_PREFS) {
					keysToRemove.erase(ignoredPref);
				}

				// Restore preferences from AnyMap
				for (const auto& entry : restorePrefs) {
					if (entry.first.type() != typeid(std::string)) {
						continue; // Skip non-string keys
					}

					std::string key = std::any_cast<std::string>(entry.first);

					if (IGNORED_PREFS.count(key) > 0) {
						continue;
					}

					keysToRemove.erase(key);

					const auto& value = entry.second;

					if (value.type() == typeid(std::string)) {
						sharedPrefs[key] = std::any_cast<std::string>(value);
					} else if (value.type() == typeid(int32_t)) {
						sharedPrefs[key] = std::any_cast<int32_t>(value);
					} else if (value.type() == typeid(bool)) {
						sharedPrefs[key] = std::any_cast<bool>(value);
					} else if (value.type() == typeid(int64_t)) {
						sharedPrefs[key] = std::any_cast<int64_t>(value);
					} else if (value.type() == typeid(float)) {
						sharedPrefs[key] = std::any_cast<float>(value);
					} else {
						// For sets/lists, just store as-is
						sharedPrefs[key] = value;
					}
				}

				// Remove old keys
				for (const auto& key : keysToRemove) {
					sharedPrefs.erase(key);
				}
			});

			onSuccess();
		};

		// Version check
		if (versionCode > currentVersionCode) {
			onVersionWarning(doRestore);
		} else {
			doRestore();
		}

	} catch (const std::exception& e) {
		onError(e.what());
	}
}

} // namespace PinkReader
