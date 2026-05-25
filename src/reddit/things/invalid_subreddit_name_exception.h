// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/InvalidSubredditNameException.java
#pragma once

#include <exception>
#include <string>

namespace PinkReader {

class InvalidSubredditNameException : public std::exception {
public:
	explicit InvalidSubredditNameException(const std::string& subredditName)
		: m_message(std::string("Invalid subreddit name '")
			+ (subredditName.empty() ? "NULL" : subredditName) + "'") {}

	const char* what() const noexcept override {
		return m_message.c_str();
	}

private:
	std::string m_message;
};

} // namespace PinkReader
