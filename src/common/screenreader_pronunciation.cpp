// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ScreenreaderPronunciation.java

#include "screenreader_pronunciation.h"
#include "string_utils.h"
#include <algorithm>
#include <cctype>

namespace PinkReader {

const std::string ScreenreaderPronunciation::LANGUAGE_CODE_EN = "en";

std::string ScreenreaderPronunciation::getDefaultLanguage() {
	// In a full port, this would query the system locale.
	// Default to English for now.
	return "en";
}

std::string ScreenreaderPronunciation::getPronunciation(
		std::function<std::string(const std::string&)> getStringFunc,
		const std::string& text) {

	std::string textLowercase = StringUtils::asciiLowercase(text);

	if (textLowercase == "i.redd.it") {
		return getStringFunc("accessibility_subtitle_domain_i_redd_it");
	}

	if (textLowercase == "v.redd.it") {
		return getStringFunc("accessibility_subtitle_domain_v_redd_it");
	}

	if (textLowercase == "imgur.com" || textLowercase == "i.imgur.com") {
		return "imager dot com";
	}

	if (textLowercase == "gfycat.com") {
		return "giffy cat dot com";
	}

	return pronounceSubreddit(textLowercase);
}

std::string ScreenreaderPronunciation::getAccessibilityString(
		std::function<std::string(const std::string&)> getStringFunc,
		const std::string& resKey) {

	// Only override for English
	if (getDefaultLanguage() != LANGUAGE_CODE_EN) {
		return getStringFunc(resKey);
	}

	// Replace "read" with the English homophone "red" to work around bad speech synth handling
	if (resKey == "accessibility_post_already_read_withperiod") {
		return "Red.";
	} else {
		return getStringFunc(resKey);
	}
}

std::string ScreenreaderPronunciation::pronounceSubreddit(const std::string& nameLowercase) {
	if (nameLowercase.size() >= 3
			&& (nameLowercase.compare(0, 3, "/r/") == 0
				|| nameLowercase.compare(0, 3, "/u/") == 0)) {
		std::string result;
		result += nameLowercase[1];
		result += " slash ";
		result += pronounceSubredditStripped(nameLowercase.substr(3));
		return result;

	} else if (nameLowercase.size() >= 2
			&& (nameLowercase.compare(0, 2, "r/") == 0
				|| nameLowercase.compare(0, 2, "u/") == 0)) {
		std::string result;
		result += nameLowercase[0];
		result += " slash ";
		result += pronounceSubredditStripped(nameLowercase.substr(2));
		return result;

	} else {
		return pronounceSubredditStripped(nameLowercase);
	}
}

std::string ScreenreaderPronunciation::pronounceSubredditStripped(
		const std::string& nameLowercase) {

	if (nameLowercase == "iama")                return "i am a";
	if (nameLowercase == "askreddit")           return "ask reddit";
	if (nameLowercase == "redreader")           return "red reader";
	if (nameLowercase == "quantumbadger")       return "quantum badger";
	if (nameLowercase == "automoderator")       return "auto moderator";
	if (nameLowercase == "whatcouldgowrong")    return "what could go wrong";
	if (nameLowercase == "mildlyinteresting")   return "mildly interesting";
	if (nameLowercase == "lifeprotips")         return "life pro tips";
	if (nameLowercase == "listentothis")        return "listen to this";
	if (nameLowercase == "nosleep")             return "no sleep";
	if (nameLowercase == "nottheonion")         return "not the onion";
	if (nameLowercase == "personalfinance")     return "personal finance";
	if (nameLowercase == "tifu")                return "t i f u";
	if (nameLowercase == "todayilearned")       return "today i learned";
	if (nameLowercase == "twoxchromosomes")     return "two x chromosomes";
	if (nameLowercase == "writingprompts")      return "writing prompts";
	if (nameLowercase == "dataisbeautiful")     return "data is beautiful";
	if (nameLowercase == "explainlikeimfive")   return "explain like I'm five";

	return nameLowercase;
}

} // namespace PinkReader
