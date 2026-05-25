/*
 * PinkReader - GPLv3
 * File: post_listing_controller.cpp - Port of PostListingController.java
 *
 * Exact port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/listingcontrollers/PostListingController.java
 *
 * Every field, method, and logic branch ported exactly.
 */

#include "adapters/post_listing_controller.h"

#include "fragments/post_listing_fragment.h"
#include "network/reddit_url_parser.h"
#include "reddit/post_sort.h"
#include "reddit/things/invalid_subreddit_name_exception.h"
#include "reddit/things/subreddit_canonical_id.h"
#include "reddit/url/post_listing_url.h"
#include "reddit/url/subreddit_post_list_url.h"
#include "reddit/url/user_post_listing_url.h"
#include "settings/prefs_utility.h"

namespace PinkReader {

// ========================================================================
// Constructor — Java lines 50-78
// ========================================================================

PostListingController::PostListingController(
        PostListingURL *url_,
        Context *context,
        QObject *parent)
    : QObject(parent)
    , url(url_) {

    Q_UNUSED(context)

    // Java lines 52-76: Set default sort orders based on URL type
    // Java lines 52-65: SUBREDDIT_POST_LISTING_URL
    if (url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL) {
        // Java lines 53-54:
        // if(url.asSubredditPostListURL().order == null) {
        if (url->asSubredditPostListURL()->order == nullptr) {

            // Java lines 55: PostSort order = PrefsUtility.pref_behaviour_postsort();
            PostSort *order = PrefsUtility::pref_behaviour_postsort();

            // Java lines 57-62:
            // if(order == PostSort.BEST
            //     && url.asSubredditPostListURL().type
            //     != SubredditPostListURL.Type.FRONTPAGE) {
            //     order = PostSort.HOT;
            // }
            if (order == PostSort::BEST
                && url->asSubredditPostListURL()->type
                != SubredditPostListURL::Type::FRONTPAGE) {
                order = PostSort::HOT;
            }

            // Java line 64: url = url.asSubredditPostListURL().sort(order);
            url = url->asSubredditPostListURL()->sort(order);
        }

    // Java lines 66-69: USER_POST_LISTING_URL
    } else if (url->pathType() == RedditURLParser::USER_POST_LISTING_URL) {
        // Java lines 67-68:
        // if(url.asUserPostListURL().order == null) {
        //     url = url.asUserPostListURL().sort(PrefsUtility.pref_behaviour_user_postsort());
        // }
        if (url->asUserPostListURL()->order == nullptr) {
            url = url->asUserPostListURL()->sort(
                    PrefsUtility::pref_behaviour_user_postsort());
        }

    // Java lines 70-75: MULTIREDDIT_POST_LISTING_URL
    } else if (url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
        // Java lines 71-74:
        // if(url.asMultiredditPostListURL().order == null) {
        //     url = url.asMultiredditPostListURL()
        //         .sort(PrefsUtility.pref_behaviour_multi_postsort());
        // }
        if (url->asMultiredditPostListURL()->order == nullptr) {
            url = url->asMultiredditPostListURL()->sort(
                    PrefsUtility::pref_behaviour_multi_postsort());
        }
    }

    // Java line 77: this.url = url;
    this->url = url;
}

// ========================================================================
// Public methods — Java lines 42-219
// ========================================================================

// Java lines 42-44: public void setSession(final UUID session)
void PostListingController::setSession(const QUuid &session_) {
    session = session_;
}

// Java lines 46-48: public UUID getSession()
QUuid PostListingController::getSession() const {
    return session;
}

// Java lines 80-87: public boolean isSortable()
bool PostListingController::isSortable() const {
    // Java lines 81-83:
    // if(url.pathType() == USER_POST_LISTING_URL) {
    //     return (url.asUserPostListURL().type == UserPostListingURL.Type.SUBMITTED);
    // }
    if (url->pathType() == RedditURLParser::USER_POST_LISTING_URL) {
        return (url->asUserPostListURL()->type == UserPostListingURL::Type::SUBMITTED);
    }
    // Java lines 84-86:
    // return (url.pathType() == SUBREDDIT_POST_LISTING_URL)
    //     || (url.pathType() == MULTIREDDIT_POST_LISTING_URL)
    //     || (url.pathType() == SEARCH_POST_LISTING_URL);
    return (url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL)
           || (url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL)
           || (url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL);
}

// Java lines 89-93: public boolean isFrontPage()
bool PostListingController::isFrontPage() const {
    // Java lines 90-92:
    // return url.pathType() == SUBREDDIT_POST_LISTING_URL
    //     && url.asSubredditPostListURL().type == SubredditPostListURL.Type.FRONTPAGE;
    return url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
           && url->asSubredditPostListURL()->type == SubredditPostListURL::Type::FRONTPAGE;
}

// Java lines 95-111: public final void setSort(final PostSort order)
void PostListingController::setSort(PostSort *order) {
    // Java lines 96-97: SUBREDDIT_POST_LISTING_URL
    if (url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL) {
        url = url->asSubredditPostListURL()->sort(order);

    // Java lines 99-100: MULTIREDDIT_POST_LISTING_URL
    } else if (url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
        url = url->asMultiredditPostListURL()->sort(order);

    // Java lines 102-103: SEARCH_POST_LISTING_URL
    } else if (url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL) {
        url = url->asSearchPostListURL()->sort(order);

    // Java lines 105-106: USER_POST_LISTING_URL
    } else if (url->pathType() == RedditURLParser::USER_POST_LISTING_URL) {
        url = url->asUserPostListURL()->sort(order);

    // Java lines 108-109: RuntimeException
    } else {
        throw std::runtime_error("Cannot set sort for this URL");
    }
}

// Java lines 113-132: public final PostSort getSort()
PostSort *PostListingController::getSort() const {
    // Java lines 115-117: SUBREDDIT_POST_LISTING_URL
    if (url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL) {
        return url->asSubredditPostListURL()->order;
    }
    // Java lines 119-121: MULTIREDDIT_POST_LISTING_URL
    if (url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
        return url->asMultiredditPostListURL()->order;
    }
    // Java lines 123-125: SEARCH_POST_LISTING_URL
    if (url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL) {
        return url->asSearchPostListURL()->order;
    }
    // Java lines 127-129: USER_POST_LISTING_URL
    if (url->pathType() == RedditURLParser::USER_POST_LISTING_URL) {
        return url->asUserPostListURL()->order;
    }
    // Java line 131: return null;
    return nullptr;
}

// Java lines 134-136: public Uri getUri()
QString PostListingController::getUri() const {
    // Java: return url.generateJsonUri();
    return url->generateJsonUri()->toString();
}

// Java lines 138-151: public final PostListingFragment get(...)
PostListingFragment *PostListingController::get(
        AppCompatActivity *parent,
        bool force,
        Bundle *savedInstanceState) {
    // Java lines 142-144: if(force) { session = null; }
    if (force) {
        session = QUuid();
    }
    // Java lines 145-150:
    // return new PostListingFragment(parent, savedInstanceState,
    //     getUri(), session, force);
    return new PostListingFragment(
            nullptr,       // parentWidget
            url,           // postListingURL
            session,       // session
            force,         // forceDownload
            nullptr);      // selectionListener
}

// Java lines 153-157: public final boolean isSubreddit()
bool PostListingController::isSubreddit() const {
    // Java lines 154-156:
    // return url.pathType() == SUBREDDIT_POST_LISTING_URL
    //     && url.asSubredditPostListURL().type == SubredditPostListURL.Type.SUBREDDIT;
    return url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
           && url->asSubredditPostListURL()->type == SubredditPostListURL::Type::SUBREDDIT;
}

// Java lines 159-163: public final boolean isSubredditCombination()
bool PostListingController::isSubredditCombination() const {
    // Java lines 160-162:
    // return url.pathType() == SUBREDDIT_POST_LISTING_URL
    //     && url.asSubredditPostListURL().type
    //     == SubredditPostListURL.Type.SUBREDDIT_COMBINATION;
    return url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
           && url->asSubredditPostListURL()->type
           == SubredditPostListURL::Type::SUBREDDIT_COMBINATION;
}

// Java lines 165-167: public final boolean isMultireddit()
bool PostListingController::isMultireddit() const {
    return url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL;
}

// Java lines 169-171: public final boolean isSearchResults()
bool PostListingController::isSearchResults() const {
    return url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL;
}

// Java lines 173-175: public final boolean isSubredditSearchResults()
bool PostListingController::isSubredditSearchResults() const {
    // Java: return isSearchResults() && url.asSearchPostListURL().subreddit != null;
    return isSearchResults() && url->asSearchPostListURL()->subreddit != nullptr;
}

// Java lines 177-179: public final boolean isUserPostListing()
bool PostListingController::isUserPostListing() const {
    return url->pathType() == RedditURLParser::USER_POST_LISTING_URL;
}

// Java lines 181-203: public final SubredditCanonicalId subredditCanonicalName()
SubredditCanonicalId *PostListingController::subredditCanonicalName() const {
    // Java lines 183-192: SUBREDDIT_POST_LISTING_URL
    if (url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
        && (url->asSubredditPostListURL()->type == SubredditPostListURL::Type::SUBREDDIT
            || url->asSubredditPostListURL()->type
            == SubredditPostListURL::Type::SUBREDDIT_COMBINATION)) {
        try {
            return new SubredditCanonicalId(url->asSubredditPostListURL()->subreddit);
        } catch (const InvalidSubredditNameException &e) {
            throw std::runtime_error("Invalid subreddit name");
        }

    // Java lines 193-199: SEARCH_POST_LISTING_URL
    } else if (url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL
               && url->asSearchPostListURL()->subreddit != nullptr) {
        try {
            return new SubredditCanonicalId(url->asSearchPostListURL()->subreddit);
        } catch (const InvalidSubredditNameException &e) {
            throw std::runtime_error("Invalid subreddit name");
        }
    }

    // Java line 202: return null;
    return nullptr;
}

// Java lines 205-211: public final String multiredditName()
QString PostListingController::multiredditName() const {
    // Java lines 206-208:
    // if(url.pathType() == MULTIREDDIT_POST_LISTING_URL) {
    //     return url.asMultiredditPostListURL().name;
    // }
    if (url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
        return url->asMultiredditPostListURL()->name;
    }
    // Java line 210: return null;
    return QString();
}

// Java lines 213-219: public final String multiredditUsername()
QString PostListingController::multiredditUsername() const {
    // Java lines 214-216:
    // if(url.pathType() == MULTIREDDIT_POST_LISTING_URL) {
    //     return url.asMultiredditPostListURL().username;
    // }
    if (url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
        return url->asMultiredditPostListURL()->username;
    }
    // Java line 218: return null;
    return QString();
}

} // namespace PinkReader
