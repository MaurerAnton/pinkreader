// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/Announcement.java
#pragma once

#include <string>
#include <memory>
#include "../common/uri_string.h"
#include "../utils/reddit_time.h"

namespace PinkReader {

// Forward declarations
class Payload;

/**
 * @brief Port of org.quantumbadger.redreader.receivers.announcements.Announcement
 *
 * Represents an announcement with id, title, message, url, and expiration.
 * Every field and method from the original Java file.
 */
class Announcement {
public:
	// Port of: private static final String ENTRY_ID = "i";
	static constexpr const char* ENTRY_ID = "i";
	// Port of: private static final String ENTRY_TITLE = "t";
	static constexpr const char* ENTRY_TITLE = "t";
	// Port of: private static final String ENTRY_MESSAGE = "m";
	static constexpr const char* ENTRY_MESSAGE = "m";
	// Port of: private static final String ENTRY_URL = "u";
	static constexpr const char* ENTRY_URL = "u";
	// Port of: private static final String ENTRY_SHOW_UNTIL = "until";
	static constexpr const char* ENTRY_SHOW_UNTIL = "until";

	// Port of: @NonNull public final String id;
	std::string id;
	// Port of: @NonNull public final String title;
	std::string title;
	// Port of: @Nullable public final String message;
	std::string message; // empty = null
	// Port of: @NonNull public final UriString url;
	UriString url;
	// Port of: public final TimestampUTC showUntil;
	TimestampUTC showUntil;

	// Port of: private Announcement(@NonNull final String id, @NonNull final String title,
	//   @Nullable final String message, @NonNull final UriString url, final TimestampUTC showUntil)
	Announcement(const std::string& id, const std::string& title, const std::string& message,
	             const UriString& url, const TimestampUTC& showUntil);

	// Port of: @NonNull public static Announcement create(@NonNull final String id,
	//   @NonNull final String title, @Nullable final String message,
	//   @NonNull final UriString url, final TimeDuration duration)
	//   return new Announcement(id, title, message, url, TimestampUTC.now().add(duration));
	static Announcement create(const std::string& id, const std::string& title,
	                           const std::string& message, const UriString& url,
	                           const TimeDuration& duration);

	// Port of: public boolean isExpired() { return showUntil.hasPassed(); }
	bool isExpired() const;

	// Port of: @NonNull public Payload toPayload()
	Payload toPayload() const;

	// Port of: @NonNull public static Announcement fromPayload(@NonNull final Payload payload) throws IOException
	static Announcement fromPayload(const Payload& payload);

private:
	// Default constructor for internal use
	Announcement() = default;
};

} // namespace PinkReader
