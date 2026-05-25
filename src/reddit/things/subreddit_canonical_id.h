// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/SubredditCanonicalId.java
#pragma once

#include <string>
#include <stdexcept>
#include "invalid_subreddit_name_exception.h"

namespace PinkReader {

// Forward declarations
class JsonObject;
class RedditSubreddit;

class SubredditCanonicalId {
public:
	explicit SubredditCanonicalId(const std::string& name);

	const std::string& getId() const { return mId; }

	std::string getDisplayNameLowercase() const;

	std::string toString() const { return mId; }

	bool operator==(const SubredditCanonicalId& other) const {
		return mId == other.mId;
	}

	bool operator!=(const SubredditCanonicalId& other) const {
		return mId != other.mId;
	}

	bool operator<(const SubredditCanonicalId& other) const {
		return mId < other.mId;
	}

	std::size_t hashCode() const {
		return std::hash<std::string>{}(mId);
	}

private:
	std::string mId;
};

} // namespace PinkReader

// Specialize std::hash for SubredditCanonicalId
namespace std {
template<>
struct hash<PinkReader::SubredditCanonicalId> {
	std::size_t operator()(const PinkReader::SubredditCanonicalId& id) const {
		return id.hashCode();
	}
};
} // namespace std
