// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/RedditUser.java
#pragma once

#include <string>
#include <cstdint>
#include <optional>

namespace PinkReader {

class UriString;
class JsonObject;

class RedditUser {
public:
	std::optional<int32_t> comment_karma;
	std::optional<int32_t> link_karma;

	std::optional<int64_t> created;
	std::optional<int64_t> created_utc;

	std::optional<bool> has_mail;
	std::optional<bool> has_mod_mail;
	std::optional<bool> is_friend;
	std::optional<bool> is_gold;
	std::optional<bool> is_mod;
	std::optional<bool> is_suspended;
	std::optional<bool> over_18;
	std::optional<bool> is_blocked;

	std::optional<std::string> id;
	std::string name;
	std::optional<std::string> icon_img;

	std::optional<bool> is_employee;

	RedditUser() = default;

	// Returns icon URL if present
	// UriString getIconUrl();

	// Fullname "t2_id"
	std::string fullname() const {
		// NOTE: Uses RedditThing::KIND_USER which is "t2"
		return std::string("t2_") + id.value_or("");
	}
};

} // namespace PinkReader
