// Origin: RedReader/src/main/java/org/quantumbadger/redreader/RedReader.kt

#pragma once

#include <QObject>
#include <QString>
#include <QThread>
#include <functional>

namespace PinkReader {

// Forward declarations
class CacheManager;

/**
 * @brief Port of org.quantumbadger.redreader.RedReader (Application class)
 *
 * Main Application class. Handles initialization of global services,
 * cache pruning, and background threads.
 * Every field, method, companion object, and logic branch from the
 * original Kotlin file (89 lines).
 */
class PinkReaderApp : public QObject {
	Q_OBJECT

public:
	// Port of: companion object { const val TAG = "RedReader" }
	static constexpr const char* TAG = "RedReader";

	// Port of: @JvmStatic fun getInstance(context: Context) = context.applicationContext as RedReader
	static PinkReaderApp* getInstance();

	// Port of: lateinit var packageInfo: AndroidCommon.PackageInfo
	// Package info structure (version name, version code, etc.)
	struct PackageInfo {
		QString versionName;
		int versionCode = 0;
		QString packageName;
	};

	PackageInfo m_packageInfo;

	explicit PinkReaderApp(QObject* parent = nullptr);
	virtual ~PinkReaderApp() = default;

	// Port of: override fun onCreate()
	//   super.onCreate()
	//   Log.i(TAG, "Application created.")
	//   packageInfo = AndroidCommon.getPackageInfo(this)
	//   GlobalExceptionHandler.init(this)
	//   PrefsUtility.init(this)
	//   ComposePrefsSingleton.init(this)
	//   Fonts.onAppCreate(assets)
	//   RedditOAuth.init(this)
	//   Log.i(TAG, "Config: " + GlobalConfig.appName + " (" + GlobalConfig.appBuildType + ")")
	//   val cm = CacheManager.getInstance(this)
	//   object : Thread() { override fun run() {
	//     Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND)
	//     cm.pruneTemp()
	//     cm.pruneCache()
	//   }}.start()
	//   object : Thread() { override fun run() {
	//     RedditChangeDataIO.getInstance(this@RedReader).runInitialReadInThisThread()
	//     RedditChangeDataManager.pruneAllUsersDefaultMaxAge()
	//   }}.start()
	//   Alarms.onBoot(this)
	//   AnnouncementDownloader.performDownload(this)
	//   NewMessageChecker.checkForNewMessages(this)
	void initialize();

signals:
	void initialized();
};

} // namespace PinkReader
