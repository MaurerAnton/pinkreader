// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/Constants.java (lines 1-281)
// Implementation for static methods and data

#include "constants.h"
#include <algorithm>
#include <cctype>

namespace PinkReader {

// line 69-119: Default subreddit strings (static initialization block)
static const std::vector<std::string>& getDefaultSubredditStrings() {
    static const std::vector<std::string> list = {
        "/r/Art", "/r/AskReddit", "/r/askscience", "/r/aww",
        "/r/books", "/r/creepy", "/r/dataisbeautiful", "/r/DIY",
        "/r/Documentaries", "/r/EarthPorn", "/r/explainlikeimfive",
        "/r/Fitness", "/r/food", "/r/funny", "/r/Futurology",
        "/r/gadgets", "/r/gaming", "/r/GetMotivated", "/r/gifs",
        "/r/history", "/r/IAmA", "/r/InternetIsBeautiful",
        "/r/Jokes", "/r/LifeProTips", "/r/listentothis",
        "/r/mildlyinteresting", "/r/movies", "/r/Music",
        "/r/news", "/r/nosleep", "/r/nottheonion",
        "/r/oldschoolcool", "/r/personalfinance", "/r/philosophy",
        "/r/photoshopbattles", "/r/pics", "/r/reddit",
        "/r/science", "/r/Showerthoughts", "/r/space",
        "/r/sports", "/r/television", "/r/tifu",
        "/r/todayilearned", "/r/TwoXChromosomes",
        "/r/UpliftingNews", "/r/videos", "/r/worldnews",
        "/r/writingprompts"
    };
    return list;
}

// line 64: DEFAULT_SUBREDDITS
const std::vector<std::string>& Constants::Reddit::defaultSubreddits() {
    // Origin line 121-123: DEFAULT_SUBREDDITS = new CollectionStream<>(defaultSubredditStrings)
    //     .mapRethrowExceptions(SubredditCanonicalId::new)
    //     .collect(new ArrayList<>(defaultSubredditStrings.length));
    // Simplified: return raw strings since SubredditCanonicalId is not ported
    return getDefaultSubredditStrings();
}

// line 66: BOT_USERNAMES_LOWERCASE
const std::unordered_set<std::string>& Constants::Reddit::botUsernamesLowercase() {
    // Origin lines 125-140: static block adding bot usernames
    static const std::unordered_set<std::string> bots = {
        "automoderator", "qualityvote", "visualmod", "a-mirror-bot",
        "unexbot", "rfauxmoi", "ukbot-nicolabot", "qualityvote2",
        "trendingtattler", "cannabun", "pcmrbot", "spotlight-app",
        "flairassistant", "sponge-tron"
    };
    return bots;
}

// line 238-242: ua()
std::string Constants::ua() {
    // Origin line 239-241:
    // final String canonicalName = RedReader.class.getCanonicalName();
    // return canonicalName.substring(0, canonicalName.lastIndexOf('.')) + "/" + version(context);
    return "org.quantumbadger.redreader/" + version();
}

// Helper: toLower
std::string Constants::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Helper: equalsIgnoreCase
bool Constants::equalsIgnoreCase(const std::string& a, const std::string& b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char ca, char cb) {
                          return std::tolower(static_cast<unsigned char>(ca))
                              == std::tolower(static_cast<unsigned char>(cb));
                      });
}

} // namespace PinkReader
