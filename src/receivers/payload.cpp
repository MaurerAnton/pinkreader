// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/Payload.java
#include "payload.h"
#include <cstring>
#include <sstream>

namespace PinkReader {

// Port of: public void setString(@NonNull final String key, @NonNull final String value)
void Payload::setString(const std::string& key, const std::string& value) {
	// Port of: mStrings.put(key, value);
	mStrings[key] = value;
}

// Port of: public void setLong(@NonNull final String key, final long value)
void Payload::setLong(const std::string& key, int64_t value) {
	// Port of: mLongs.put(key, value);
	mLongs[key] = value;
}

// Port of: public void setBoolean(@NonNull final String key, final boolean value)
void Payload::setBoolean(const std::string& key, bool value) {
	// Port of: mBooleans.put(key, value);
	mBooleans[key] = value;
}

// Port of: @Nullable public String getString(@NonNull final String key)
std::string Payload::getString(const std::string& key) const {
	// Port of: return mStrings.get(key);
	auto it = mStrings.find(key);
	if (it != mStrings.end()) {
		return it->second;
	}
	return ""; // Java: returns null, C++ returns empty string (nullable)
}

// Port of: @Nullable public Long getLong(@NonNull final String key)
int64_t Payload::getLong(const std::string& key, bool* found) const {
	// Port of: return mLongs.get(key);
	auto it = mLongs.find(key);
	if (it != mLongs.end()) {
		if (found) *found = true;
		return it->second;
	}
	if (found) *found = false;
	return 0;
}

// Port of: @Nullable public Boolean getBoolean(@NonNull final String key)
bool Payload::getBoolean(const std::string& key, bool* found) const {
	// Port of: return mBooleans.get(key);
	auto it = mBooleans.find(key);
	if (it != mBooleans.end()) {
		if (found) *found = true;
		return it->second;
	}
	if (found) *found = false;
	return false;
}

// Port of: @NonNull public byte[] toBytes()
std::vector<uint8_t> Payload::toBytes() const {
	// Port of: final ByteArrayOutputStream result = new ByteArrayOutputStream();
	// Port of: final DataOutputStream dos = new DataOutputStream(result);
	std::vector<uint8_t> result;

	// Helper: write UTF string to vector (Java DataOutputStream.writeUTF format)
	auto writeUTF = [&result](const std::string& str) {
		// Java DataOutputStream.writeUTF: 2-byte length + modified UTF-8
		// For simplicity, we use the same format: 2-byte big-endian length + bytes
		uint16_t len = static_cast<uint16_t>(str.size());
		result.push_back(static_cast<uint8_t>((len >> 8) & 0xFF));
		result.push_back(static_cast<uint8_t>(len & 0xFF));
		result.insert(result.end(), str.begin(), str.end());
	};

	try {
		// Port of: for(final Map.Entry<String, String> entry : mStrings.entrySet())
		for (const auto& entry : mStrings) {
			// Port of: if(entry.getValue() == null) { continue; }
			// (strings are never null in our map)
			// Port of: dos.writeByte(HEADER_ENTRY_STRING);
			result.push_back(HEADER_ENTRY_STRING);
			// Port of: dos.writeUTF(entry.getKey());
			writeUTF(entry.first);
			// Port of: dos.writeUTF(entry.getValue());
			writeUTF(entry.second);
		}

		// Port of: for(final Map.Entry<String, Long> entry : mLongs.entrySet())
		for (const auto& entry : mLongs) {
			// Port of: if(entry.getValue() == null) { continue; }
			// Port of: dos.writeByte(HEADER_ENTRY_LONG);
			result.push_back(HEADER_ENTRY_LONG);
			// Port of: dos.writeUTF(entry.getKey());
			writeUTF(entry.first);
			// Port of: dos.writeLong(entry.getValue());
			int64_t val = entry.second;
			for (int i = 7; i >= 0; i--) {
				result.push_back(static_cast<uint8_t>((val >> (i * 8)) & 0xFF));
			}
		}

		// Port of: for(final Map.Entry<String, Boolean> entry : mBooleans.entrySet())
		for (const auto& entry : mBooleans) {
			// Port of: if(entry.getValue() == null) { continue; }
			// Port of: dos.writeByte(HEADER_ENTRY_BOOLEAN);
			result.push_back(HEADER_ENTRY_BOOLEAN);
			// Port of: dos.writeUTF(entry.getKey());
			writeUTF(entry.first);
			// Port of: dos.writeBoolean(entry.getValue());
			result.push_back(entry.second ? 1 : 0);
		}

		// Port of: dos.writeByte(HEADER_EOF);
		result.push_back(HEADER_EOF);

		// Port of: dos.flush(); dos.close();
		// (no-op in C++)

	} catch (const std::exception& e) {
		// Port of: catch(final IOException e) { throw new RuntimeException(e); }
		throw std::runtime_error(std::string("Payload::toBytes failed: ") + e.what());
	}

	// Port of: return result.toByteArray();
	return result;
}

// Port of: @NonNull public static Payload fromBytes(@NonNull final byte[] data) throws IOException
Payload Payload::fromBytes(const std::vector<uint8_t>& data) {
	// Port of: try(DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data)))
	size_t pos = 0;

	// Helper: read UTF string from vector
	auto readUTF = [&data, &pos]() -> std::string {
		if (pos + 2 > data.size()) {
			throw std::runtime_error("Payload::fromBytes: unexpected end of data");
		}
		uint16_t len = (static_cast<uint16_t>(data[pos]) << 8) | data[pos + 1];
		pos += 2;
		if (pos + len > data.size()) {
			throw std::runtime_error("Payload::fromBytes: unexpected end of data in string");
		}
		std::string result(data.begin() + pos, data.begin() + pos + len);
		pos += len;
		return result;
	};

	// Helper: read long from vector
	auto readLong = [&data, &pos]() -> int64_t {
		if (pos + 8 > data.size()) {
			throw std::runtime_error("Payload::fromBytes: unexpected end of data");
		}
		int64_t val = 0;
		for (int i = 0; i < 8; i++) {
			val = (val << 8) | data[pos + i];
		}
		pos += 8;
		return val;
	};

	// Port of: final Payload result = new Payload();
	Payload result;

	while (true) {
		// Port of: final byte header = dis.readByte();
		if (pos >= data.size()) {
			throw std::runtime_error("Payload::fromBytes: unexpected end of data");
		}
		uint8_t header = data[pos++];

		// Port of: switch(header)
		switch (header) {
			// Port of: case HEADER_EOF: return result;
			case HEADER_EOF:
				return result;

			// Port of: case HEADER_ENTRY_STRING: result.setString(dis.readUTF(), dis.readUTF()); break;
			case HEADER_ENTRY_STRING: {
				std::string key = readUTF();
				std::string value = readUTF();
				result.setString(key, value);
				break;
			}

			// Port of: case HEADER_ENTRY_LONG: result.setLong(dis.readUTF(), dis.readLong()); break;
			case HEADER_ENTRY_LONG: {
				std::string key = readUTF();
				int64_t value = readLong();
				result.setLong(key, value);
				break;
			}

			// Port of: case HEADER_ENTRY_BOOLEAN: result.setBoolean(dis.readUTF(), dis.readBoolean()); break;
			case HEADER_ENTRY_BOOLEAN: {
				std::string key = readUTF();
				if (pos >= data.size()) {
					throw std::runtime_error("Payload::fromBytes: unexpected end of data");
				}
				bool value = (data[pos++] != 0);
				result.setBoolean(key, value);
				break;
			}

			// Port of: default: throw new IOException("Unknown entry header " + header);
			default:
				throw std::runtime_error("Unknown entry header " + std::to_string(header));
		}
	}
}

} // namespace PinkReader
