// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/RedditThing.java
#pragma once

#include <string>
#include <map>
#include <memory>
#include <stdexcept>

namespace PinkReader {

// Forward declarations for RedditSubreddit, RedditUser, JsonObject
class RedditSubreddit;
class RedditUser;
class JsonObject;

class RedditThing {
public:
	static constexpr const char* KIND_USER = "t2";

	enum class Kind {
		POST,
		USER,
		COMMENT,
		MESSAGE,
		SUBREDDIT,
		MORE_COMMENTS,
		LISTING
	};

	std::string kind;
	// JsonObject data; -- originally JsonObject, forward-declared

	Kind getKind() const {
		static const std::map<std::string, Kind> kinds = {
			{"t1", Kind::COMMENT},
			{KIND_USER, Kind::USER},
			{"t3", Kind::POST},
			{"t4", Kind::MESSAGE},
			{"t5", Kind::SUBREDDIT},
			{"more", Kind::MORE_COMMENTS},
			{"Listing", Kind::LISTING}
		};

		auto it = kinds.find(kind);
		if (it == kinds.end()) {
			throw std::runtime_error(std::string("Unknown thing type: ") + kind);
		}
		return it->second;
	}

	// RedditSubreddit asSubreddit(); -- depends on JsonObject infrastructure
	// RedditUser asUser(); -- depends on JsonObject infrastructure
};

} // namespace PinkReader
