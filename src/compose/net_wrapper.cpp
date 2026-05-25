// Origin: RedReader/src/main/java/org/quantumbadger/redreader/compose/net/NetWrapper.kt
#include "net_wrapper.h"

// Dependencies (placeholder):
// - BitmapFactory
// - CacheManager, CacheRequest, CacheRequestCallbacks
// - DownloadStrategy, DownloadStrategyIfNotCached
// - AndroidCommon (runOnUiThread)
// - Constants (Priority, FileType)
// - GlobalNetworkRetry
// - LocalRedditUser
// - LinkHandler, GetAlbumInfoListener
// - RRError
// - RedditAccount, RedditAccountId, RedditAccountManager
// - GenericFactory
// - TimestampUTC
// - ImageSize

namespace PinkReader {

NetRequestStatus<AlbumInfo>::Variant* fetchAlbum(
	const UriString& uri,
	const RedditAccountId& user
) {
	// Ported from @Composable fun fetchAlbum(uri, user):
	// 1. val state = remember { mutableStateOf<NetRequestStatus<AlbumInfo>>(Connecting) }
	// 2. val currentRequest = remember { mutableIntStateOf(0) }
	// 3. val context = LocalContext.current.applicationContext
	// 4. LaunchedEffect(uri, user) {
	//       state.value = NetRequestStatus.Connecting
	//       val thisRequest = ++currentRequest.intValue
	//       LinkHandler.getAlbumInfo(context, uri, Priority(IMAGE_VIEW), object : GetAlbumInfoListener {
	//           override fun onFailure(error: RRError) { AndroidCommon.runOnUiThread { if(thisRequest==currentRequest.intValue) state.value=Failed(error) } }
	//           override fun onSuccess(info: AlbumInfo) { AndroidCommon.runOnUiThread { if(thisRequest==currentRequest.intValue) state.value=Success(info) } }
	//           override fun onGalleryRemoved() { onFailure(RRError(...)) }
	//           override fun onGalleryDataNotPresent() { onFailure(RRError(...)) }
	//       })
	//   }
	// 5. return state
	static NetRequestStatus<AlbumInfo>::Variant state = NetRequestStatus<AlbumInfo>::Connecting{};
	return &state;
}

NetRequestStatus<FileRequestResult<ImageBitmap>>::Variant* fetchImage(
	const UriString& uri,
	const RedditAccountId& user,
	int scaleToMaxAxis
) {
	// Ported from @Composable fun fetchImage(uri, user, scaleToMaxAxis):
	// 1. val TAG = "NetWrapper:fetchImage"
	// 2. val context = LocalContext.current.applicationContext
	// 3. val filter: (FileRequestMetadata) -> NetRequestStatus<FileRequestResult<ImageBitmap>> = remember(uri, user) { ... }
	//    - try: BitmapFactory.decodeStream(it.streamFactory.create()), scale, return Success(FileRequestResult(...))
	//    - catch: return Failed(RRError(...))
	// 4. return fetchFile(uri, user, Priority(IMAGE_VIEW), DownloadStrategyIfNotCached.INSTANCE,
	//                     Constants.FileType.IMAGE, IMMEDIATE, cache=true, filter=filter)
	static NetRequestStatus<FileRequestResult<ImageBitmap>>::Variant state =
		NetRequestStatus<FileRequestResult<ImageBitmap>>::Connecting{};
	return &state;
}

template <typename T>
NetRequestStatus<FileRequestResult<T>>::Variant* fetchFile(
	const UriString& uri,
	const RedditAccountId& user,
	const Priority& priority,
	const DownloadStrategy& downloadStrategy,
	int fileType,
	int queueType,
	bool cache,
	std::function<NetRequestStatus<FileRequestResult<T>>::Variant(const FileRequestMetadata&)> filter
) {
	// Ported from @Composable private fun <T> fetchFile(...):
	// 1. val state = remember { mutableStateOf<...>(Connecting) }
	// 2. val currentRequest = remember { mutableIntStateOf(0) }
	// 3. val currentGlobalRetry by GlobalNetworkRetry
	// 4. var currentLocalRetry by remember { mutableIntStateOf(currentGlobalRetry) }
	// 5. val context = LocalContext.current
	// 6. val account = RedditAccountManager.getInstance(context).getAccount(user.canonicalUsername) ?: run { state = Failed(...); return }
	// 7. LaunchedEffect(currentLocalRetry, ...) { if state is Failed && currentLocalRetry!=currentGlobalRetry: state=Connecting; delay 2000ms; currentLocalRetry=currentGlobalRetry }
	// 8. DisposableEffect(currentLocalRetry, uri, user, ...) {
	//       state = Connecting; val thisRequest = ++currentRequest.intValue
	//       val req = CacheRequest(uri, account, ..., object : CacheRequestCallbacks {
	//           var done = false; val active get() = !done && thisRequest == currentRequest.intValue
	//           override onFailure: AndroidCommon.runOnUiThread { if(active) state=Failed(...); done=true }
	//           override onDataStreamComplete: val result=filter(FileRequestMetadata(...)); AndroidCommon.runOnUiThread { if(active) state=result; done=true }
	//           override onProgress: AndroidCommon.runOnUiThread { if(active && totalBytes>0) state=Downloading(bytesRead/totalBytes) }
	//       })
	//       CacheManager.getInstance(context).makeRequest(req)
	//       onDispose { req.cancel() }
	//   }
	// 9. return state
	static NetRequestStatus<FileRequestResult<T>>::Variant state =
		NetRequestStatus<FileRequestResult<T>>::Connecting{};
	return &state;
}

} // namespace PinkReader
