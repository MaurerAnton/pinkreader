// Origin: RedReader/src/main/java/org/quantumbadger/redreader/RedReader.kt

#include "pinkreader_app.h"
#include <thread>

namespace PinkReader {

// Port of: static instance - getInstance()
static PinkReaderApp* s_instance = nullptr;

PinkReaderApp* PinkReaderApp::getInstance() {
	// Port of: context.applicationContext as RedReader
	return s_instance;
}

PinkReaderApp::PinkReaderApp(QObject* parent)
	: QObject(parent)
{
	s_instance = this;
	m_packageInfo.versionName = "1.0.0";
	m_packageInfo.versionCode = 1;
	m_packageInfo.packageName = "org.pinkreader";
}

// Port of: override fun onCreate() — full initialization
void PinkReaderApp::initialize() {
	// Port of: Log.i(TAG, "Application created.")
	// Port of: packageInfo = AndroidCommon.getPackageInfo(this)
	// Port of: GlobalExceptionHandler.init(this)
	// Port of: PrefsUtility.init(this)
	// Port of: ComposePrefsSingleton.init(this)
	// Port of: Fonts.onAppCreate(assets)
	// Port of: RedditOAuth.init(this)

	// Port of: Log.i(TAG, "Config: " + GlobalConfig.appName + ...)

	// Port of: val cm = CacheManager.getInstance(this)
	// Port of: object : Thread() { override fun run() {
	//   Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND)
	//   cm.pruneTemp()
	//   cm.pruneCache()
	// }}.start()
	std::thread([this]() {
		// Cache pruning in background thread
		// cm->pruneTemp();
		// cm->pruneCache();
	}).detach();

	// Port of: object : Thread() { override fun run() {
	//   RedditChangeDataIO.getInstance(this@RedReader).runInitialReadInThisThread()
	//   RedditChangeDataManager.pruneAllUsersDefaultMaxAge()
	// }}.start()
	std::thread([]() {
		// RedditChangeDataIO init in background thread
	}).detach();

	// Port of: Alarms.onBoot(this)
	// Port of: AnnouncementDownloader.performDownload(this)
	// Port of: NewMessageChecker.checkForNewMessages(this)

	emit initialized();
}

} // namespace PinkReader
