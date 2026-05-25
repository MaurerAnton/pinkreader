// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/SignatureHandler.java
#include "signature_handler.h"
#include "../common/hex_utils.h"
#include <cstring>

namespace PinkReader {

// Port of: @NonNull public static String keyToString(@NonNull final Key key)
//   return HexUtils.toHex(key.getEncoded());
std::string SignatureHandler::keyToString(const std::vector<uint8_t>& keyEncoded) {
	return HexUtils::toHex(keyEncoded);
}

// Port of: @NonNull public static PrivateKey stringToPrivateKey(@NonNull final String input)
//   throws NoSuchAlgorithmException, IOException, InvalidKeySpecException
//   return KeyFactory.getInstance(ALG).generatePrivate(new PKCS8EncodedKeySpec(HexUtils.fromHex(input)));
std::vector<uint8_t> SignatureHandler::stringToPrivateKey(const std::string& input) {
	// In C++, we just return the decoded key bytes.
	// The actual crypto operations use these raw bytes + OpenSSL/alternate library.
	// Port of: HexUtils.fromHex(input) -> PKCS8EncodedKeySpec
	return HexUtils::fromHex(input);
}

// Port of: @NonNull public static PublicKey stringToPublicKey(@NonNull final String input)
//   throws NoSuchAlgorithmException, IOException, InvalidKeySpecException
//   return KeyFactory.getInstance(ALG).generatePublic(new X509EncodedKeySpec(HexUtils.fromHex(input)));
std::vector<uint8_t> SignatureHandler::stringToPublicKey(const std::string& input) {
	// Port of: HexUtils.fromHex(input) -> X509EncodedKeySpec
	return HexUtils::fromHex(input);
}

// Port of: @NonNull private static byte[] sign(@NonNull final PrivateKey privateKey, @NonNull final byte[] message)
//   throws NoSuchAlgorithmException, InvalidKeyException, SignatureException
//   final Signature signer = Signature.getInstance(SIGNATURE_ALG);
//   signer.initSign(privateKey);
//   signer.update(message);
//   return signer.sign();
std::vector<uint8_t> SignatureHandler::sign(const std::vector<uint8_t>& privateKeyEncoded, const std::vector<uint8_t>& message) {
	// Port of: Signature.getInstance(SIGNATURE_ALG) -> initSign -> update -> sign
	// In production, use OpenSSL EC signing with SHA256.
	// For port correctness, we preserve the logic: sign the message with ECDSA.
	// Placeholder: return empty vector as signing requires external crypto library.
	(void)privateKeyEncoded;
	(void)message;
	throw std::runtime_error("SignatureHandler::sign requires OpenSSL/ECDSA support");
}

// Port of: private static void verify(@NonNull final PublicKey publicKey,
//   @NonNull final byte[] message, @NonNull final byte[] signature)
//   final Signature signer = Signature.getInstance(SIGNATURE_ALG);
//   signer.initVerify(publicKey);
//   signer.update(message);
//   if(!signer.verify(signature)) { throw new SignatureInvalidException(); }
void SignatureHandler::verify(const std::vector<uint8_t>& publicKeyEncoded, const std::vector<uint8_t>& message, const std::vector<uint8_t>& signature) {
	// Port of: Signature.getInstance -> initVerify -> update -> verify
	// Placeholder: requires OpenSSL/ECDSA support
	(void)publicKeyEncoded;
	(void)message;
	(void)signature;
	throw std::runtime_error("SignatureHandler::verify requires OpenSSL/ECDSA support");
}

// Port of: @NonNull public static byte[] generateSignedPayload(
//   @NonNull final PrivateKey privateKey, @NonNull final byte[] message)
//   final byte[] signature = sign(privateKey, message);
//   final ByteArrayOutputStream result = new ByteArrayOutputStream();
//   final DataOutputStream dos = new DataOutputStream(result);
//   try { dos.writeInt(message.length); dos.write(message);
//         dos.writeInt(signature.length); dos.write(signature);
//         dos.flush(); dos.close(); }
//   catch(final IOException e) { throw new RuntimeException(e); }
//   return result.toByteArray();
std::vector<uint8_t> SignatureHandler::generateSignedPayload(const std::vector<uint8_t>& privateKeyEncoded, const std::vector<uint8_t>& message) {
	// Port of: final byte[] signature = sign(privateKey, message);
	std::vector<uint8_t> sig = sign(privateKeyEncoded, message);

	// Port of: final ByteArrayOutputStream result = new ByteArrayOutputStream();
	std::vector<uint8_t> result;

	// Port of: dos.writeInt(message.length);
	int32_t msgLen = static_cast<int32_t>(message.size());
	result.push_back(static_cast<uint8_t>((msgLen >> 24) & 0xFF));
	result.push_back(static_cast<uint8_t>((msgLen >> 16) & 0xFF));
	result.push_back(static_cast<uint8_t>((msgLen >> 8) & 0xFF));
	result.push_back(static_cast<uint8_t>(msgLen & 0xFF));

	// Port of: dos.write(message);
	result.insert(result.end(), message.begin(), message.end());

	// Port of: dos.writeInt(signature.length);
	int32_t sigLen = static_cast<int32_t>(sig.size());
	result.push_back(static_cast<uint8_t>((sigLen >> 24) & 0xFF));
	result.push_back(static_cast<uint8_t>((sigLen >> 16) & 0xFF));
	result.push_back(static_cast<uint8_t>((sigLen >> 8) & 0xFF));
	result.push_back(static_cast<uint8_t>(sigLen & 0xFF));

	// Port of: dos.write(signature);
	result.insert(result.end(), sig.begin(), sig.end());

	// Port of: dos.flush(); dos.close(); (no-op)

	// Port of: return result.toByteArray();
	return result;
}

// Port of: @NonNull public static byte[] readAndVerifySignedPayload(
//   @NonNull final PublicKey publicKey, @NonNull final byte[] payload)
//   try(DataInputStream payloadStream = new DataInputStream(new ByteArrayInputStream(payload)))
//     final int msgLength = payloadStream.readInt();
//     final byte[] msg = new byte[msgLength]; payloadStream.readFully(msg);
//     final int sigLength = payloadStream.readInt();
//     final byte[] sig = new byte[sigLength]; payloadStream.readFully(sig);
//     verify(publicKey, msg, sig);
//     return msg;
std::vector<uint8_t> SignatureHandler::readAndVerifySignedPayload(const std::vector<uint8_t>& publicKeyEncoded, const std::vector<uint8_t>& payload) {
	// Port of: try(DataInputStream payloadStream = new DataInputStream(new ByteArrayInputStream(payload)))
	size_t pos = 0;

	// Port of: final int msgLength = payloadStream.readInt();
	if (pos + 4 > payload.size()) throw std::runtime_error("readAndVerifySignedPayload: unexpected end");
	int32_t msgLength = (static_cast<int32_t>(payload[pos]) << 24)
		| (static_cast<int32_t>(payload[pos + 1]) << 16)
		| (static_cast<int32_t>(payload[pos + 2]) << 8)
		| static_cast<int32_t>(payload[pos + 3]);
	pos += 4;

	// Port of: final byte[] msg = new byte[msgLength]; payloadStream.readFully(msg);
	if (pos + msgLength > payload.size()) throw std::runtime_error("readAndVerifySignedPayload: msg too long");
	std::vector<uint8_t> msg(payload.begin() + pos, payload.begin() + pos + msgLength);
	pos += msgLength;

	// Port of: final int sigLength = payloadStream.readInt();
	if (pos + 4 > payload.size()) throw std::runtime_error("readAndVerifySignedPayload: unexpected end");
	int32_t sigLength = (static_cast<int32_t>(payload[pos]) << 24)
		| (static_cast<int32_t>(payload[pos + 1]) << 16)
		| (static_cast<int32_t>(payload[pos + 2]) << 8)
		| static_cast<int32_t>(payload[pos + 3]);
	pos += 4;

	// Port of: final byte[] sig = new byte[sigLength]; payloadStream.readFully(sig);
	if (pos + sigLength > payload.size()) throw std::runtime_error("readAndVerifySignedPayload: sig too long");
	std::vector<uint8_t> sig(payload.begin() + pos, payload.begin() + pos + sigLength);

	// Port of: // (any trailing bytes in payloadStream are safely ignored)

	// Port of: verify(publicKey, msg, sig);
	verify(publicKeyEncoded, msg, sig);

	// Port of: return msg;
	return msg;
}

} // namespace PinkReader
