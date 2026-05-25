// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/net/NetWrapper.kt
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace PinkReader {

// Forward declarations
class UriString;
class RedditAccountId;
class Priority;
class DownloadStrategy;
struct RRError;
class SeekableInputStream;
struct AlbumInfo;
struct TimestampUTC;
class GenericFactory;
struct ImageBitmap;

// Ported from: @Immutable sealed interface NetRequestStatus<out R>
template <typename R>
struct NetRequestStatus {
	// data object Connecting : NetRequestStatus<Nothing>
	struct Connecting {};

	// data class Downloading(val fractionComplete: Float) : NetRequestStatus<Nothing>
	struct Downloading {
		float fractionComplete = 0.0f;
	};

	// data class Failed(val error: RRError) : NetRequestStatus<Nothing>
	struct Failed {
		RRError error;
	};

	// data class Success<R>(val result: R) : NetRequestStatus<R>
	struct Success {
		R result;
	};

	using Variant = std::variant<Connecting, Downloading, Failed, Success>;
};

// Ported from: @Immutable class FileRequestMetadata
struct FileRequestMetadata {
	std::shared_ptr<GenericFactory<SeekableInputStream, std::exception>> streamFactory;
	std::optional<TimestampUTC> timestamp;
	std::string session;   // UUID as string
	bool fromCache = false;
	std::optional<std::string> mimetype;
};

// Ported from: @Immutable class FileRequestResult<R>
template <typename R>
struct FileRequestResult {
	std::optional<FileRequestMetadata> metadata;
	R data;
};

// Ported from: @Composable fun fetchAlbum(uri, user)
// Returns state pointer (mimics State<NetRequestStatus<AlbumInfo>>)
NetRequestStatus<AlbumInfo>::Variant* fetchAlbum(
	const UriString& uri,
	const RedditAccountId& user
);

// Ported from: @Composable fun fetchImage(uri, user, scaleToMaxAxis)
NetRequestStatus<FileRequestResult<ImageBitmap>>::Variant* fetchImage(
	const UriString& uri,
	const RedditAccountId& user,
	int scaleToMaxAxis = 2048
);

// Ported from: private fun <T> fetchFile(...)
// Declared private in Kotlin; exposed here for completeness
template <typename T>
NetRequestStatus<FileRequestResult<T>>::Variant* fetchFile(
	const UriString& uri,
	const RedditAccountId& user,
	const Priority& priority,
	const DownloadStrategy& downloadStrategy,
	int fileType,
	int queueType,         // CacheRequest.DownloadQueueType mapped to int
	bool cache,
	std::function<NetRequestStatus<FileRequestResult<T>>::Variant(const FileRequestMetadata&)> filter
);

} // namespace PinkReader
