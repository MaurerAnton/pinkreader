// Origin: RedReader/src/main/java/org/quantumbadger/redreader/activities/SessionChangeListener.java

#pragma once

#include <string>
#include <cstdint>

namespace PinkReader {

// Forward declaration
class TimestampUTC;

/**
 * @brief Port of org.quantumbadger.redreader.activities.SessionChangeListener
 *
 * Interface for listening to session changes (post/comment cache sessions).
 * Every field, enum, and method from the original Java file.
 */
class SessionChangeListener {
public:
	// Port of: enum SessionChangeType { POSTS, COMMENTS }
	enum class SessionChangeType {
		POSTS,
		COMMENTS
	};

	virtual ~SessionChangeListener() = default;

	// Port of: void onSessionSelected(UUID session, SessionChangeType type);
	// UUID represented as string
	virtual void onSessionSelected(const std::string& session, SessionChangeType type) = 0;

	// Port of: void onSessionRefreshSelected(SessionChangeType type);
	virtual void onSessionRefreshSelected(SessionChangeType type) = 0;

	// Port of: void onSessionChanged(UUID session, SessionChangeType type, TimestampUTC timestamp);
	virtual void onSessionChanged(const std::string& session, SessionChangeType type, const TimestampUTC& timestamp) = 0;
};

} // namespace PinkReader
