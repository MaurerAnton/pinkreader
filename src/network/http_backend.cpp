/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: http_backend.cpp
 * Port of RedReader's HTTPBackend.kt
 */

#include "network/http_backend.h"

namespace PinkReader {

// ============================================================================
// Static backend singleton (companion object, Kotlin lines 60-63)
// In Kotlin: @JvmStatic val backend: HTTPBackend
//           get() = OKHTTPBackend.getHttpBackend()
// ============================================================================
HTTPBackend *HTTPBackend::s_backend = nullptr;

HTTPBackend &HTTPBackend::getBackend() {
    if (!s_backend) {
        // In the original, this delegates to OKHTTPBackend.getHttpBackend().
        // Here we throw if not set — the application must call setBackend()
        // during initialization.
        throw std::runtime_error(
            "HTTPBackend not initialized — call setBackend() first");
    }
    return *s_backend;
}

void HTTPBackend::setBackend(HTTPBackend *backend) {
    s_backend = backend;
}

} // namespace PinkReader
