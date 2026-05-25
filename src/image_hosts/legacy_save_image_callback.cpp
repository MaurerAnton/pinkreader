// Origin: RedReader image/LegacySaveImageCallback.java
#include "legacy_save_image_callback.h"

namespace PinkReader {

LegacySaveImageCallback::LegacySaveImageCallback(BaseActivity* activity, UriString* uri)
	: mActivity(activity), mUri(uri) {}

void LegacySaveImageCallback::OnPermissionGranted() {
	// FileUtils::downloadImageToSave(activity, uri, callback)
	// callback: (info, cacheFile, mimetype) -> {
	//   filename = General.filenameFromString(info.original.url.value)
	//   dst = Environment.getExternalStoragePublicDirectory(DIRECTORY_PICTURES) / filename
	//   if dst.exists -> increment counter until unique
	//   try: copyFile(cacheFileInputStream, dst)
	//   catch IOException: showResultDialog with STORAGE error
	//   sendBroadcast(ACTION_MEDIA_SCANNER_SCAN_FILE)
	//   quickToast(success message with path)
	// }
}

void LegacySaveImageCallback::OnPermissionDenied() {
	// General::quickToast(activity, R.string.save_image_permission_denied)
}

} // namespace PinkReader
