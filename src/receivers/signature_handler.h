// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/SignatureHandler.java
#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.receivers.announcements.SignatureHandler
 *
 * ECDSA signature generation and verification utilities.
 * Every constant and method from the original Java file.
 */
class SignatureHandler {
public:
	// Port of: public static class SignatureInvalidException extends Exception {}
	class SignatureInvalidException : public std::runtime_error {
	public:
		SignatureInvalidException() : std::runtime_error("Signature invalid") {}
	};

	// Port of: private static final String ALG = "EC";
	static constexpr const char* ALG = "EC";
	// Port of: private static final String SIGNATURE_ALG = "SHA256withECDSA";
	static constexpr const char* SIGNATURE_ALG = "SHA256withECDSA";

	// Port of: private SignatureHandler() {}
	SignatureHandler() = delete;

	// Port of: @NonNull public static String keyToString(@NonNull final Key key)
	static std::string keyToString(const std::vector<uint8_t>& keyEncoded);

	// Port of: @NonNull public static PrivateKey stringToPrivateKey(@NonNull final String input)
	//   throws NoSuchAlgorithmException, IOException, InvalidKeySpecException
	static std::vector<uint8_t> stringToPrivateKey(const std::string& input);

	// Port of: @NonNull public static PublicKey stringToPublicKey(@NonNull final String input)
	//   throws NoSuchAlgorithmException, IOException, InvalidKeySpecException
	static std::vector<uint8_t> stringToPublicKey(const std::string& input);

	// Port of: @NonNull private static byte[] sign(@NonNull final PrivateKey privateKey, @NonNull final byte[] message)
	static std::vector<uint8_t> sign(const std::vector<uint8_t>& privateKeyEncoded, const std::vector<uint8_t>& message);

	// Port of: private static void verify(...)
	static void verify(const std::vector<uint8_t>& publicKeyEncoded, const std::vector<uint8_t>& message, const std::vector<uint8_t>& signature);

	// Port of: @NonNull public static byte[] generateSignedPayload(...)
	static std::vector<uint8_t> generateSignedPayload(const std::vector<uint8_t>& privateKeyEncoded, const std::vector<uint8_t>& message);

	// Port of: @NonNull public static byte[] readAndVerifySignedPayload(...)
	static std::vector<uint8_t> readAndVerifySignedPayload(const std::vector<uint8_t>& publicKeyEncoded, const std::vector<uint8_t>& payload);
};

} // namespace PinkReader
