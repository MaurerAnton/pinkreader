// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditParsedComment.java
#pragma once

#include <string>
#include <memory>
#include <optional>
#include "../things/reddit_thing_with_id_and_type.h"

namespace PinkReader {

// Forward declarations
class RedditComment;
class BodyElement;
class BetterSSB;
class RedditIdAndType;

class RedditParsedComment : public RedditThingWithIdAndType {
public:
	// Constructor takes RedditComment and an activity context reference
	RedditParsedComment(const RedditComment& comment, void* activity);

	const BodyElement& getBody() const;
	BetterSSB* getFlair() const;

	std::string getIdAlone() const override;
	const RedditIdAndType& getIdAndType() const override;

	const RedditComment& getRawComment() const;

private:
	const RedditComment* mSrc;
	std::unique_ptr<BodyElement> mBody;
	std::unique_ptr<BetterSSB> mFlair;

	void getFlairEmotes(void* flairRichtext, void* activity);
};

} // namespace PinkReader
