// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/Constants.java (lines 1-281)
// Ported from Java final class with static inner classes to C++

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <cstdint>

namespace PinkReader {

// Constants.java line 31: public final class Constants
class Constants {
public:
    // line 33-40: public static String version(final Context context)
    // Simplified: context dependency removed, version set externally
    static std::string version() {
        return sVersion;
    }

    static void setVersion(const std::string& v) {
        sVersion = v;
    }

    // line 42-60: public static final class Mime
    struct Mime {
        // line 44-47: isImage
        static bool isImage(const std::string& mimetype) {
            const std::string lower = toLower(mimetype);
            return lower.find("image/") == 0 && !isImageGif(mimetype);
        }

        // line 49-51: isImageGif
        static bool isImageGif(const std::string& mimetype) {
            return equalsIgnoreCase(mimetype, "image/gif");
        }

        // line 53-55: isVideo
        static bool isVideo(const std::string& mimetype) {
            return mimetype.find("video/") == 0;
        }

        // line 57-59: isOctetStream
        static bool isOctetStream(const std::string& mimetype) {
            return mimetype == "application/octet-stream";
        }
    };

    // line 62-236: public static final class Reddit
    struct Reddit {
        // line 64: DEFAULT_SUBREDDITS
        static const std::vector<std::string>& defaultSubreddits();

        // line 66: BOT_USERNAMES_LOWERCASE
        static const std::unordered_set<std::string>& botUsernamesLowercase();

        // lines 142-161: Reddit API constants
        static constexpr const char* SCHEME_HTTPS = "https";
        static constexpr const char* DOMAIN_HTTPS = "oauth.reddit.com";
        static constexpr const char* DOMAIN_HTTPS_HUMAN = "reddit.com";
        static constexpr const char* PATH_VOTE = "/api/vote";
        static constexpr const char* PATH_SAVE = "/api/save";
        static constexpr const char* PATH_HIDE = "/api/hide";
        static constexpr const char* PATH_UNSAVE = "/api/unsave";
        static constexpr const char* PATH_UNHIDE = "/api/unhide";
        static constexpr const char* PATH_REPORT = "/api/report";
        static constexpr const char* PATH_DELETE = "/api/del";
        static constexpr const char* PATH_SUBSCRIBE = "/api/subscribe";
        static constexpr const char* PATH_SUBREDDITS_MINE_SUBSCRIBER =
            "/subreddits/mine/subscriber.json?limit=100";
        static constexpr const char* PATH_SUBREDDITS_MINE_MODERATOR =
            "/subreddits/mine/moderator.json?limit=100";
        static constexpr const char* PATH_SUBREDDITS_POPULAR = "/subreddits/popular.json";
        static constexpr const char* PATH_MULTIREDDITS_MINE = "/api/multi/mine.json";
        static constexpr const char* PATH_COMMENTS = "/comments/";
        static constexpr const char* PATH_ME = "/api/v1/me";

        // line 162-164: getScheme()
        static std::string getScheme() { return SCHEME_HTTPS; }

        // line 166-168: getDomain()
        static std::string getDomain() { return DOMAIN_HTTPS; }

        // line 170-172: getHumanReadableDomain()
        static std::string getHumanReadableDomain() { return DOMAIN_HTTPS_HUMAN; }

        // line 178-180: getUri(final String path)
        static std::string getUri(const std::string& path) {
            return getScheme() + "://" + getDomain() + path;
        }

        // line 182-184: getNonAPIUri(final String path)
        static std::string getNonAPIUri(const std::string& path) {
            return getScheme() + "://reddit.com" + path;
        }

        // line 186-189: isApiErrorUser
        static bool isApiErrorUser(const std::string& str) {
            return str == ".error.USER_REQUIRED" || str == "please login to do that";
        }

        // line 191-194: isApiErrorCaptcha
        static bool isApiErrorCaptcha(const std::string& str) {
            return str == ".error.BAD_CAPTCHA.field-captcha"
                || str == "care to try these again?";
        }

        // line 196-199: isApiErrorNotAllowed
        static bool isApiErrorNotAllowed(const std::string& str) {
            return str == ".error.SUBREDDIT_NOTALLOWED.field-sr"
                || str == "you aren't allowed to post there.";
        }

        // line 201-204: isApiErrorSubredditRequired
        static bool isApiErrorSubredditRequired(const std::string& str) {
            return str == ".error.SUBREDDIT_REQUIRED.field-sr"
                || str == "you must specify a subreddit";
        }

        // line 206-209: isApiErrorURLRequired
        static bool isApiErrorURLRequired(const std::string& str) {
            return str == ".error.NO_URL.field-url"
                || str == "a url is required";
        }

        // line 211-214: isApiTooFast
        static bool isApiTooFast(const std::string& str) {
            return str == ".error.RATELIMIT.field-ratelimit"
                || (str.find("you are doing that too much") != std::string::npos);
        }

        // line 216-219: isApiTooLong
        static bool isApiTooLong(const std::string& str) {
            return str == "TOO_LONG"
                || (str.find("this is too long") != std::string::npos);
        }

        // line 221-225: isApiAlreadySubmitted
        static bool isApiAlreadySubmitted(const std::string& str) {
            return str == ".error.ALREADY_SUB.field-url"
                || (str.find("that link has already been submitted") != std::string::npos);
        }

        // line 227-231: isPostFlairRequired
        static bool isPostFlairRequired(const std::string& str) {
            return str == ".error.SUBMIT_VALIDATION_FLAIR_REQUIRED.field-flair"
                || (str.find("Your post must contain post flair.") != std::string::npos);
        }

        // line 233-235: isApiError
        static bool isApiError(const std::string& str) {
            return !str.empty() && str.find(".error.") == 0;
        }
    };

    // line 238-242: ua(final Context context)
    static std::string ua();

    // line 244-261: public static final class Priority
    struct Priority {
        static constexpr int CAPTCHA = -600;
        static constexpr int API_ACTION = -500;
        static constexpr int API_MULTIREDDIT_LIST = -200;
        static constexpr int API_SUBREDDIT_LIST = -100;
        static constexpr int API_SUBREDDIT_SEARCH = -500;
        static constexpr int API_SUBREDDIT_INVIDIVUAL = -250;
        static constexpr int API_POST_LIST = -200;
        static constexpr int API_COMMENT_LIST = -300;
        static constexpr int THUMBNAIL = 100;
        static constexpr int INLINE_IMAGE_PREVIEW = 100;
        static constexpr int IMAGE_PRECACHE = 500;
        static constexpr int COMMENT_PRECACHE = 500;
        static constexpr int IMAGE_VIEW = -400;
        static constexpr int API_USER_ABOUT = -500;
        static constexpr int API_INBOX_LIST = -500;
        static constexpr int DEV_ANNOUNCEMENTS = 600;
    };

    // line 263-277: public static final class FileType
    struct FileType {
        static constexpr int NOCACHE = -1;
        static constexpr int SUBREDDIT_LIST = 100;
        static constexpr int SUBREDDIT_ABOUT = 101;
        static constexpr int MULTIREDDIT_LIST = 102;
        static constexpr int POST_LIST = 110;
        static constexpr int COMMENT_LIST = 120;
        static constexpr int USER_ABOUT = 130;
        static constexpr int INBOX_LIST = 140;
        static constexpr int THUMBNAIL = 200;
        static constexpr int IMAGE = 201;
        static constexpr int CAPTCHA = 202;
        static constexpr int INLINE_IMAGE_PREVIEW = 203;
        static constexpr int IMAGE_INFO = 300;
    };

    // line 279-280: OAuth constants
    static constexpr const char* OA_CS = "client_secret";
    static constexpr const char* OA_CI = "client_id";

private:
    static inline std::string sVersion = "0.0.0";

    static std::string toLower(const std::string& str);
    static bool equalsIgnoreCase(const std::string& a, const std::string& b);
};

} // namespace PinkReader
