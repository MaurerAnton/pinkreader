// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/AnnouncementDownloader.java
#pragma once

#include <string>
#include "../common/optional.h"
#include "../utils/reddit_time.h"

namespace PinkReader {

// Forward declarations
class Announcement;
struct SharedPrefsWrapper;

/**
 * @brief Port of org.quantumbadger.redreader.receivers.announcements.AnnouncementDownloader
 *
 * Downloads and manages announcements from Reddit.
 * Every field, constant, and static method from the original Java file.
 * NOTE: Android-specific dependencies (Context, CacheManager, etc.) are stubbed.
 */
class AnnouncementDownloader {
public:
	// Port of: private static final String TAG = "AnnouncementDownloader";
	static constexpr const char* TAG = "AnnouncementDownloader";

	// Port of: private static final String PUBLIC_KEY = "3059301306072A8648CE3D020106082A8648CE3D0301070342000"
	//   + "4F74D436746282E6080F0EE9FB80DCDCA06667F701A0266F2F14C15C204B6E48414444BD9D0C1170E6B0"
	//   + "C257B3DE1AE23F4BA965D8CEB055A3C374DA927415C5D";
	static constexpr const char* PUBLIC_KEY =
		"3059301306072A8648CE3D020106082A8648CE3D0301070342000"
		"4F74D436746282E6080F0EE9FB80DCDCA06667F701A0266F2F14C15C204B6E48414444BD9D0C1170E6B0"
		"C257B3DE1AE23F4BA965D8CEB055A3C374DA927415C5D";

	// Port of: public static final String PREF_KEY_PAYLOAD_STORAGE_HEX = "AnnouncementDownloaderPayload";
	static constexpr const char* PREF_KEY_PAYLOAD_STORAGE_HEX = "AnnouncementDownloaderPayload";
	// Port of: public static final String PREF_KEY_LAST_READ_ID = "AnnouncementDownloaderLastReadId";
	static constexpr const char* PREF_KEY_LAST_READ_ID = "AnnouncementDownloaderLastReadId";

	// Port of: private AnnouncementDownloader() {} (implicit in final class)
	AnnouncementDownloader() = delete;

	// Port of: public static void performDownload(@NonNull final Context context)
	// Stub: Android-specific, calls CacheManager, etc.
	static void performDownload();

	// Port of: @NonNull public static Optional<Announcement> getMostRecentUnreadAnnouncement(
	//   @NonNull final SharedPrefsWrapper prefs)
	static Optional<Announcement> getMostRecentUnreadAnnouncement(SharedPrefsWrapper* prefs);

	// Port of: public static void markAsRead(
	//   @NonNull final Context context, @NonNull final Announcement announcement)
	// Stub: writes to shared prefs
	static void markAsRead(SharedPrefsWrapper* prefs, const Announcement& announcement);
};

} // namespace PinkReader
