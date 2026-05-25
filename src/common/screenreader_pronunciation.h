// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/ScreenreaderPronunciation.java
#pragma once

#include <string>
#include <functional>

namespace PinkReader {

/**
 * Provides pronunciation guidance for screen readers (accessibility).
 * Equivalent to: org.quantumbadger.redreader.common.ScreenreaderPronunciation
 *
 * Overrides default text-to-speech pronunciation for domain names,
 * subreddit names, and specific accessibility strings.
 */
class ScreenreaderPronunciation {
private:
	ScreenreaderPronunciation() = delete;

	static const std::string LANGUAGE_CODE_EN;

	/**
	 * Returns the current device language code.
	 * In Android: Locale.getDefault().getLanguage()
	 * In portable C++: returns "en" by default (caller should override).
	 */
	static std::string getDefaultLanguage();

public:
	/**
	 * Returns pronunciation text for the given domain/subreddit text.
	 * Equivalent to: ScreenreaderPronunciation.getPronunciation(Context, String)
	 *
	 * @param getStringFunc function to look up localized strings by key
	 * @param text the text to get pronunciation for
	 * @return pronunciation text for screen readers
	 */
	static std::string getPronunciation(
		std::function<std::string(const std::string&)> getStringFunc,
		const std::string& text);

	/**
	 * Returns an accessibility-friendly version of the string resource.
	 * Overrides the pronunciation for specific English strings.
	 * Equivalent to: ScreenreaderPronunciation.getAccessibilityString(Context, int)
	 *
	 * @param getStringFunc function to look up localized strings by key
	 * @param resKey the string resource key
	 * @return accessibility string
	 */
	static std::string getAccessibilityString(
		std::function<std::string(const std::string&)> getStringFunc,
		const std::string& resKey);

private:
	/**
	 * Pronounces a subreddit or user name (with or without r/ or u/ prefix).
	 */
	static std::string pronounceSubreddit(const std::string& nameLowercase);

	/**
	 * Pronounces a subreddit name without the r/ or u/ prefix.
	 */
	static std::string pronounceSubredditStripped(const std::string& nameLowercase);
};

} // namespace PinkReader
