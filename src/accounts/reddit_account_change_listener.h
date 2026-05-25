// Origin: RedReader/src/main/java/org/quantumbadger/redreader/account/RedditAccountChangeListener.java
#pragma once

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.account.RedditAccountChangeListener
 *
 * Original: public interface RedditAccountChangeListener {
 *   void onRedditAccountChanged();
 * }
 */
class RedditAccountChangeListener {
public:
	virtual ~RedditAccountChangeListener() = default;

	// Port of: void onRedditAccountChanged();
	virtual void onRedditAccountChanged() = 0;
};

} // namespace PinkReader
