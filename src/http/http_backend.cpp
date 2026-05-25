// Origin: RedReader http/HTTPBackend.kt
#include "http_backend.h"
#include "okhttp_backend.h"

namespace PinkReader {

HTTPBackend& HTTPBackend::GetBackend() {
	return OKHTTPBackend::GetHttpBackend();
}

} // namespace PinkReader
