// Origin: org/quantumbadger/redreader/cache/CacheRequestCallbacks.java
#pragma once

#include <QByteArray>
#include <QString>
#include <QUuid>

#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>

#include "cache/cache_manager.h"

namespace PinkReader {

class RRError;
class TimestampUTC;

template<typename T, typename E = std::exception>
class GenericFactory {
public:
	virtual ~GenericFactory() = default;
	virtual T create() const = 0;
};

class CacheRequestCallbacks {
public:
	virtual ~CacheRequestCallbacks() = default;

	virtual void onDownloadNecessary() {}

	virtual void onDownloadStarted() {}

	virtual void onDataStreamAvailable(
			const GenericFactory<QByteArray> &streamFactory,
			const TimestampUTC &timestamp,
			const QUuid &session,
			bool fromCache,
			const std::optional<QString> &mimetype) {}

	virtual void onDataStreamComplete(
			const GenericFactory<QByteArray> &streamFactory,
			const TimestampUTC &timestamp,
			const QUuid &session,
			bool fromCache,
			const std::optional<QString> &mimetype) {}

	virtual void onProgress(
			bool authorizationInProgress,
			qint64 bytesRead,
			qint64 totalBytes) {}

	virtual void onFailure(const RRError &error) = 0;

	virtual void onCacheFileWritten(
			const CacheManager::ReadableCacheFile &cacheFile,
			const TimestampUTC &timestamp,
			const QUuid &session,
			bool fromCache,
			const std::optional<QString> &mimetype) {}
};

} // namespace PinkReader
