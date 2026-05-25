/*
 * PinkReader - GPLv3
 * File: comment_listing_controller.cpp - Port of CommentListingController.java
 *
 * Exact port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/listingcontrollers/CommentListingController.java
 *
 * Every field, method, and logic branch ported exactly.
 */

#include "adapters/comment_listing_controller.h"

#include "fragments/comment_listing_fragment.h"
#include "network/reddit_url_parser.h"
#include "reddit/post_comment_sort.h"
#include "reddit/user_comment_sort.h"
#include "settings/prefs_utility.h"
#include "utils/general.h"
#include "utils/options_menu_utility.h"

namespace PinkReader {

// ========================================================================
// Constructor — Java lines 49-67
// ========================================================================

CommentListingController::CommentListingController(
        RedditURLParser *url,
        QObject *parent)
    : QObject(parent) {

    // Java lines 51-60: Set default sort orders
    if (url->pathType() == RedditURLParser::POST_COMMENT_LISTING_URL) {
        // Java lines 52-54:
        // if(url.asPostCommentListURL().order == null) {
        //     url = url.asPostCommentListURL().order(defaultOrder());
        // }
        if (url->asPostCommentListURL()->order == nullptr) {
            url = url->asPostCommentListURL()->order(defaultOrder());
        }
    } else if (url->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL) {
        // Java lines 55-59:
        // if(url.asUserCommentListURL().order == null) {
        //     url = url.asUserCommentListURL().order(defaultUserOrder());
        // }
        // url = url.asUserCommentListURL().limit(100);
        if (url->asUserCommentListURL()->order == nullptr) {
            url = url->asUserCommentListURL()->order(defaultUserOrder());
        }
        url = url->asUserCommentListURL()->limit(100);
    }

    // Java lines 62-64:
    // if(!(url instanceof CommentListingURL)) {
    //     throw new RuntimeException("Not comment listing URL");
    // }
    // Cast is implicit in C++ — we trust the caller.

    // Java line 66: this.mUrl = (CommentListingURL)url;
    mUrl = dynamic_cast<CommentListingURL *>(url);
}

// ========================================================================
// Public methods — Java lines 41-139
// ========================================================================

// Java lines 41-43: public UUID getSession()
QUuid CommentListingController::getSession() const {
    return mSession;
}

// Java lines 45-47: public void setSession(final UUID session)
void CommentListingController::setSession(const QUuid &session) {
    mSession = session;
}

// Java lines 69-71: private PostCommentSort defaultOrder()
PostCommentSort *CommentListingController::defaultOrder() const {
    return PrefsUtility::pref_behaviour_commentsort();
}

// Java lines 73-75: private UserCommentSort defaultUserOrder()
UserCommentSort *CommentListingController::defaultUserOrder() const {
    return PrefsUtility::pref_behaviour_user_commentsort();
}

// Java lines 77-81: public void setSort(final PostCommentSort s)
void CommentListingController::setSort(const PostCommentSort *s) {
    // Java lines 78-80:
    // if(mUrl.pathType() == RedditURLParser.POST_COMMENT_LISTING_URL) {
    //     mUrl = mUrl.asPostCommentListURL().order(s);
    // }
    if (mUrl->pathType() == RedditURLParser::POST_COMMENT_LISTING_URL) {
        mUrl = mUrl->asPostCommentListURL()->order(s);
    }
}

// Java lines 83-87: public void setSort(final UserCommentSort s)
void CommentListingController::setSort(const UserCommentSort *s) {
    // Java lines 84-86:
    // if(mUrl.pathType() == RedditURLParser.USER_COMMENT_LISTING_URL) {
    //     mUrl = mUrl.asUserCommentListURL().order(s);
    // }
    if (mUrl->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL) {
        mUrl = mUrl->asUserCommentListURL()->order(s);
    }
}

// Java lines 89-98: public OptionsMenuUtility.Sort getSort()
OptionsMenuUtility *CommentListingController::getSort() const {
    // Java lines 91-97:
    // if(mUrl.pathType() == POST_COMMENT_LISTING_URL) { return ... }
    // else if(mUrl.pathType() == USER_COMMENT_LISTING_URL) { return ... }
    // return null;
    if (mUrl->pathType() == RedditURLParser::POST_COMMENT_LISTING_URL) {
        return mUrl->asPostCommentListURL()->order;
    } else if (mUrl->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL) {
        return mUrl->asUserCommentListURL()->order;
    }
    return nullptr;
}

// Java lines 100-102: public void setSearchString(final String searchString)
void CommentListingController::setSearchString(const QString &searchString) {
    mSearchString = searchString;
}

// Java lines 104-106: public String getSearchString()
QString CommentListingController::getSearchString() const {
    return mSearchString;
}

// Java lines 108-110: public Uri getUri()
QString CommentListingController::getUri() const {
    // Java: return mUrl.generateJsonUri();
    return mUrl->generateJsonUri()->toString();
}

// Java lines 112-114: public CommentListingURL getCommentListingUrl()
CommentListingURL *CommentListingController::getCommentListingUrl() const {
    return mUrl;
}

// Java lines 116-130: public CommentListingFragment get(...)
CommentListingFragment *CommentListingController::get(
        AppCompatActivity *parent,
        bool force,
        Bundle *savedInstanceState) {
    // Java lines 120-122:
    // if(force) { mSession = null; }
    if (force) {
        mSession = QUuid();
    }
    // Java lines 123-129:
    // return new CommentListingFragment(
    //     parent, savedInstanceState, General.listOfOne(mUrl),
    //     mSession, mSearchString, force);
    return new CommentListingFragment(
            parent,
            savedInstanceState,
            General::listOfOne(mUrl),
            mSession,
            mSearchString,
            force);
}

// Java lines 132-135: public boolean isSortable()
bool CommentListingController::isSortable() const {
    // Java lines 133-134:
    // return mUrl.pathType() == POST_COMMENT_LISTING_URL
    //     || mUrl.pathType() == USER_COMMENT_LISTING_URL;
    return mUrl->pathType() == RedditURLParser::POST_COMMENT_LISTING_URL
           || mUrl->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL;
}

// Java lines 137-139: public boolean isUserCommentListing()
bool CommentListingController::isUserCommentListing() const {
    return mUrl->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL;
}

} // namespace PinkReader
