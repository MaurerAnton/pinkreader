// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/RedditThingWithIdAndType.java
#pragma once

#include <string>

namespace PinkReader {

// Forward declaration for RedditIdAndType (originally org.quantumbadger.redreader.reddit.kthings.RedditIdAndType)
class RedditIdAndType;

class RedditThingWithIdAndType {
public:
	virtual ~RedditThingWithIdAndType() = default;
	virtual std::string getIdAlone() const = 0;
	virtual const RedditIdAndType& getIdAndType() const = 0;
};

} // namespace PinkReader
