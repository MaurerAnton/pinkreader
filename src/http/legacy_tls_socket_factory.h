// Origin: RedReader http/LegacyTLSSocketFactory.java
#pragma once

#include <string>
#include <memory>
#include <stdexcept>

namespace PinkReader {

// LegacyTLSSocketFactory wraps an SSLSocketFactory to enforce TLS 1.2
// This is a port of the Java SSLSocketFactory wrapper that restricts
// enabled protocols to TLSv1.2 only.
// 
// Since C++ doesn't have a direct equivalent of Java's SSLSocketFactory,
// this provides a conceptual port with a factory pattern.

class LegacyTLSSocketFactory {
private:
	static const std::string TLS_V1_2_ONLY[];
	void* mDelegate; // SSLSocketFactory* equivalent - actual implementation delegated

public:
	explicit LegacyTLSSocketFactory(void* base)
		: mDelegate(base) {}

	// The following methods delegate to the underlying factory
	// and apply TLS 1.2 enforcement on created sockets.

	// In a real implementation, these would be wrapped SSL calls:
	// - getDefaultCipherSuites() -> delegate.getDefaultCipherSuites()
	// - getSupportedCipherSuites() -> delegate.getSupportedCipherSuites()
	// - createSocket variants -> delegate.createSocket + enableTLS1_2

private:
	void* EnableTLS1_2(void* socket) {
		// If socket is SSLSocket, set enabled protocols to TLS_V1_2_ONLY
		// In C++, this would use the SSL library's API
		return socket;
	}
};

} // namespace PinkReader
