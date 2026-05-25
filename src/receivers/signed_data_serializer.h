// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/SignedDataSerializer.java
#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.receivers.announcements.SignedDataSerializer
 *
 * Serializes signed data with START/END markers and hex encoding.
 * Every constant and method from the original Java file.
 */
class SignedDataSerializer {
public:
	// Port of: private static final String MARKER_START = "START";
	static constexpr const char* MARKER_START = "START";
	// Port of: private static final String MARKER_END = "END";
	static constexpr const char* MARKER_END = "END";

	// Port of: private SignedDataSerializer() {}
	SignedDataSerializer() = delete;

	// Port of: @NonNull public static String serialize(
	//   @NonNull final PrivateKey privateKey, @NonNull final byte[] data)
	static std::string serialize(const std::vector<uint8_t>& privateKeyEncoded, const std::vector<uint8_t>& data);

	// Port of: @NonNull public static byte[] deserialize(
	//   @NonNull final PublicKey publicKey, @NonNull final String data)
	static std::vector<uint8_t> deserialize(const std::vector<uint8_t>& publicKeyEncoded, const std::string& data);
};

} // namespace PinkReader
