// Origin: org/quantumbadger/redreader/cache/CacheRequestJSONParser.java
#pragma once

#include <string>
#include <cstdint>
#include <atomic>
#include <memory>
#include <stdexcept>

#include "cache_request_callbacks.h"

namespace PinkReader {

class Context;
class TimestampUTC;
class UUID;
class JsonValue;
class RRError;

class CacheRequestJSONParser : public CacheRequestCallbacks {
public:
	class Listener {
	public:
		virtual ~Listener() = default;

		virtual void onJsonParsed(
				const JsonValue& result,
				const TimestampUTC& timestamp,
				const UUID& session,
				bool fromCache) = 0;

		virtual void onFailure(const RRError& error) = 0;

		virtual void onDownloadNecessary() {}
	};

private:
	void* m_context; // Opaque context pointer
	Listener& m_listener;

	std::atomic<bool> m_notifiedFailure{false};

public:
	CacheRequestJSONParser(void* context, Listener& listener);

	// CacheRequestCallbacks overrides
	void onDataStreamAvailable(
			const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
			const TimestampUTC& timestamp,
			const UUID& session,
			bool fromCache,
			const std::string* mimetype) override;

	void onFailure(const RRError& error) override;
};

} // namespace PinkReader
