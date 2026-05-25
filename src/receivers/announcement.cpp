// Origin: RedReader/src/main/java/org/quantumbadger/redreader/receivers/announcements/Announcement.java
#include "announcement.h"
#include "payload.h"
#include <stdexcept>

namespace PinkReader {

// Port of: private Announcement(@NonNull final String id, @NonNull final String title,
//   @Nullable final String message, @NonNull final UriString url, final TimestampUTC showUntil)
Announcement::Announcement(const std::string& id, const std::string& title, const std::string& message,
                           const UriString& url, const TimestampUTC& showUntil)
	: id(id)
	, title(title)
	, message(message)
	, url(url)
	, showUntil(showUntil) {
}

// Port of: @NonNull public static Announcement create(@NonNull final String id,
//   @NonNull final String title, @Nullable final String message,
//   @NonNull final UriString url, final TimeDuration duration)
//   return new Announcement(id, title, message, url, TimestampUTC.now().add(duration));
Announcement Announcement::create(const std::string& id, const std::string& title,
                                  const std::string& message, const UriString& url,
                                  const TimeDuration& duration) {
	return Announcement(id, title, message, url, TimestampUTC::now().add(duration));
}

// Port of: public boolean isExpired() { return showUntil.hasPassed(); }
bool Announcement::isExpired() const {
	return showUntil.hasPassed();
}

// Port of: @NonNull public Payload toPayload()
//   final Payload result = new Payload();
//   result.setString(ENTRY_ID, id);
//   result.setString(ENTRY_TITLE, title);
//   if(message != null) { result.setString(ENTRY_MESSAGE, message); }
//   result.setString(ENTRY_URL, url.value);
//   result.setLong(ENTRY_SHOW_UNTIL, showUntil.toUtcMs());
//   return result;
Payload Announcement::toPayload() const {
	// Port of: final Payload result = new Payload();
	Payload result;

	// Port of: result.setString(ENTRY_ID, id);
	result.setString(ENTRY_ID, id);
	// Port of: result.setString(ENTRY_TITLE, title);
	result.setString(ENTRY_TITLE, title);

	// Port of: if(message != null) { result.setString(ENTRY_MESSAGE, message); }
	if (!message.empty()) {
		result.setString(ENTRY_MESSAGE, message);
	}

	// Port of: result.setString(ENTRY_URL, url.value);
	result.setString(ENTRY_URL, url.value().toStdString());
	// Port of: result.setLong(ENTRY_SHOW_UNTIL, showUntil.toUtcMs());
	result.setLong(ENTRY_SHOW_UNTIL, showUntil.toUtcMs());

	// Port of: return result;
	return result;
}

// Port of: @NonNull public static Announcement fromPayload(@NonNull final Payload payload) throws IOException
Announcement Announcement::fromPayload(const Payload& payload) {
	// Port of: String id = payload.getString(ENTRY_ID);
	std::string id = payload.getString(ENTRY_ID);
	// Port of: final String title = payload.getString(ENTRY_TITLE);
	std::string title = payload.getString(ENTRY_TITLE);
	// Port of: final String message = payload.getString(ENTRY_MESSAGE);
	std::string message = payload.getString(ENTRY_MESSAGE);
	// Port of: final String url = payload.getString(ENTRY_URL);
	std::string urlStr = payload.getString(ENTRY_URL);
	// Port of: final Long showUntil = payload.getLong(ENTRY_SHOW_UNTIL);
	bool foundShowUntil = false;
	int64_t showUntilMs = payload.getLong(ENTRY_SHOW_UNTIL, &foundShowUntil);

	// Port of: if(title == null || url == null || showUntil == null) { throw new IOException("Required entry missing"); }
	if (title.empty() || urlStr.empty() || !foundShowUntil) {
		throw std::runtime_error("Required entry missing");
	}

	// Port of: if(id == null) { id = url; }
	if (id.empty()) {
		id = urlStr;
	}

	// Port of: return new Announcement(id, title, message, new UriString(url), TimestampUTC.fromUtcMs(showUntil));
	return Announcement(id, title, message, UriString(QString::fromStdString(urlStr)),
	                    TimestampUTC::fromUtcMs(showUntilMs));
}

} // namespace PinkReader
