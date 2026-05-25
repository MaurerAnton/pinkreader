// Origin: org/quantumbadger/redreader/reddit/RedditFlairChoice.java
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

// Forward declarations
namespace PinkReader {
class JsonArray;
class JsonObject;
class JsonValue;
template<typename T> class Optional;
}

namespace PinkReader {

class RedditFlairChoice {
public:
	const std::string text;
	const std::string templateId;

	RedditFlairChoice(std::string text, std::string templateId)
		: text(std::move(text)), templateId(std::move(templateId)) {}

	// fromJsonList(JsonArray json) -> Optional<List<RedditFlairChoice>>
	static std::optional<std::vector<RedditFlairChoice>> fromJsonList(const JsonArray& json);

	// fromJson(JsonObject json) -> Optional<RedditFlairChoice>
	static std::optional<RedditFlairChoice> fromJson(const JsonObject& json);

	// Parcelable stubs (Android-specific)
	int describeContents() const { return 0; }
	// void writeToParcel(...) - Android-specific, omitted

	std::string toString() const {
		return "RedditFlairChoice(text='" + text + "', templateId='" + templateId + "')";
	}

	bool operator==(const RedditFlairChoice& other) const {
		return text == other.text && templateId == other.templateId;
	}

	bool operator!=(const RedditFlairChoice& other) const {
		return !(*this == other);
	}

	// hashCode equivalent
	size_t hashCode() const {
		return std::hash<std::string>{}(text) + 37 * std::hash<std::string>{}(templateId);
	}
};

} // namespace PinkReader
