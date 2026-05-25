// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/Payload.java
#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.receivers.announcements.Payload
 *
 * Binary-serializable key-value store for Strings, Longs, and Booleans.
 * Every field, constant, and method from the original Java file.
 */
class Payload {
public:
	// Port of: private static final byte HEADER_EOF = 0;
	static constexpr uint8_t HEADER_EOF = 0;
	// Port of: private static final byte HEADER_ENTRY_STRING = 1;
	static constexpr uint8_t HEADER_ENTRY_STRING = 1;
	// Port of: private static final byte HEADER_ENTRY_LONG = 2;
	static constexpr uint8_t HEADER_ENTRY_LONG = 2;
	// Port of: private static final byte HEADER_ENTRY_BOOLEAN = 3;
	static constexpr uint8_t HEADER_ENTRY_BOOLEAN = 3;

	// Port of: public void setString(@NonNull final String key, @NonNull final String value)
	void setString(const std::string& key, const std::string& value);

	// Port of: public void setLong(@NonNull final String key, final long value)
	void setLong(const std::string& key, int64_t value);

	// Port of: public void setBoolean(@NonNull final String key, final boolean value)
	void setBoolean(const std::string& key, bool value);

	// Port of: @Nullable public String getString(@NonNull final String key)
	std::string getString(const std::string& key) const;

	// Port of: @Nullable public Long getLong(@NonNull final String key)
	int64_t getLong(const std::string& key, bool* found = nullptr) const;

	// Port of: @Nullable public Boolean getBoolean(@NonNull final String key)
	bool getBoolean(const std::string& key, bool* found = nullptr) const;

	// Port of: @NonNull public byte[] toBytes()
	std::vector<uint8_t> toBytes() const;

	// Port of: @NonNull public static Payload fromBytes(@NonNull final byte[] data) throws IOException
	static Payload fromBytes(const std::vector<uint8_t>& data);

private:
	// Port of: private final HashMap<String, String> mStrings = new HashMap<>();
	std::unordered_map<std::string, std::string> mStrings;
	// Port of: private final HashMap<String, Long> mLongs = new HashMap<>();
	std::unordered_map<std::string, int64_t> mLongs;
	// Port of: private final HashMap<String, Boolean> mBooleans = new HashMap<>();
	std::unordered_map<std::string, bool> mBooleans;
};

} // namespace PinkReader
