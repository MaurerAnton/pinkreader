// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/SignedDataSerializer.java
#include "signed_data_serializer.h"
#include "signature_handler.h"
#include "../common/hex_utils.h"
#include <cstring>
#include <stdexcept>

namespace PinkReader {

// Port of: @NonNull public static String serialize(
//   @NonNull final PrivateKey privateKey, @NonNull final byte[] data)
//   return MARKER_START
//     + HexUtils.toHex(SignatureHandler.generateSignedPayload(privateKey, data))
//     + MARKER_END;
std::string SignedDataSerializer::serialize(const std::vector<uint8_t>& privateKeyEncoded, const std::vector<uint8_t>& data) {
	// Port of: MARKER_START + HexUtils.toHex(SignatureHandler.generateSignedPayload(privateKey, data)) + MARKER_END
	return std::string(MARKER_START)
		+ HexUtils::toHex(SignatureHandler::generateSignedPayload(privateKeyEncoded, data))
		+ std::string(MARKER_END);
}

// Port of: @NonNull public static byte[] deserialize(
//   @NonNull final PublicKey publicKey, @NonNull final String data)
std::vector<uint8_t> SignedDataSerializer::deserialize(const std::vector<uint8_t>& publicKeyEncoded, const std::string& data) {
	// Port of: final int startMarkerIndex = data.indexOf(MARKER_START);
	size_t startMarkerIndex = data.find(MARKER_START);
	// Port of: final int endMarkerIndex = data.indexOf(MARKER_END);
	size_t endMarkerIndex = data.find(MARKER_END);

	// Port of: if(startMarkerIndex == -1) { throw new IOException("Start marker not found"); }
	if (startMarkerIndex == std::string::npos) {
		throw std::runtime_error("Start marker not found");
	}

	// Port of: if(endMarkerIndex == -1) { throw new IOException("End marker not found"); }
	if (endMarkerIndex == std::string::npos) {
		throw std::runtime_error("End marker not found");
	}

	// Port of: final int start = startMarkerIndex + MARKER_START.length();
	size_t start = startMarkerIndex + strlen(MARKER_START);
	// Port of: final int length = endMarkerIndex - start;
	// Port of: if(length < 0) { throw new IOException("Negative length"); }
	if (endMarkerIndex < start) {
		throw std::runtime_error("Negative length");
	}

	// Port of: final String hexData = data.substring(start, endMarkerIndex);
	std::string hexData = data.substr(start, endMarkerIndex - start);

	// Port of: final byte[] signedPayload = HexUtils.fromHex(hexData);
	std::vector<uint8_t> signedPayload = HexUtils::fromHex(hexData);

	// Port of: return SignatureHandler.readAndVerifySignedPayload(publicKey, signedPayload);
	return SignatureHandler::readAndVerifySignedPayload(publicKeyEncoded, signedPayload);
}

} // namespace PinkReader
