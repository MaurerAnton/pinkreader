// Origin: org/quantumbadger/redreader/listingcontrollers/CommentListingController.java
#include "comment_listing_controller.h"

#include <stdexcept>

#include "../reddit/url/reddit_url_parser.h"
#include "../reddit/url/comment_listing_url.h"
#include "../reddit/url/post_comment_listing_url.h"
#include "../reddit/url/user_comment_listing_url.h"
#include "../reddit/post_comment_sort.h"
#include "../reddit/user_comment_sort.h"
#include "../settings/prefs_utility.h"
#include "../utils/general.h"
#include "../utils/options_menu_utility.h"
#include "../fragments/comment_listing_fragment.h"

namespace PinkReader {

CommentListingController::CommentListingController(RedditURLParser::RedditURL& url)
	: m_url(nullptr) {

	if(url.pathType() == RedditURLParser::POST_COMMENT_LISTING_URL) {
		PostCommentListingURL* pcl = url.asPostCommentListURL();
		if(pcl->order == nullptr) {
			pcl = pcl->order(defaultOrder());
		}
		url = *pcl;
	} else if(url.pathType() == RedditURLParser::USER_COMMENT_LISTING_URL) {
		UserCommentListingURL* ucl = url.asUserCommentListURL();
		if(ucl->order == nullptr) {
			ucl = ucl->order(defaultUserOrder());
		}
		ucl = ucl->limit(100);
		url = *ucl;
	}

	// In Java: if(!(url instanceof CommentListingURL)) throw RuntimeException
	// In C++, we dynamic_cast:
	CommentListingURL* clUrl = dynamic_cast<CommentListingURL*>(&url);
	if(!clUrl) {
		throw std::runtime_error("Not comment listing URL");
	}

	m_url = clUrl;
}

PostCommentSort CommentListingController::defaultOrder() {
	// Port of: PrefsUtility.pref_behaviour_commentsort()
	// Returns default PostCommentSort
	return PrefsUtility::pref_behaviour_commentsort();
}

UserCommentSort CommentListingController::defaultUserOrder() {
	// Port of: PrefsUtility.pref_behaviour_user_commentsort()
	return PrefsUtility::pref_behaviour_user_commentsort();
}

std::optional<QString> CommentListingController::getSession() const {
	return m_session;
}

void CommentListingController::setSession(const std::optional<QString>& session) {
	m_session = session;
}

void CommentListingController::setSort(const PostCommentSort& s) {
	if(m_url->pathType() == RedditURLParser::POST_COMMENT_LISTING_URL) {
		PostCommentListingURL* pcl = m_url->asPostCommentListURL();
		m_url = pcl->order(s);
	}
}

void CommentListingController::setSort(const UserCommentSort& s) {
	if(m_url->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL) {
		UserCommentListingURL* ucl = m_url->asUserCommentListURL();
		m_url = ucl->order(s);
	}
}

OptionsMenuUtility::Sort* CommentListingController::getSort() {
	if(m_url->pathType() == RedditURLParser::POST_COMMENT_LISTING_URL) {
		return m_url->asPostCommentListURL()->order;
	} else if(m_url->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL) {
		return m_url->asUserCommentListURL()->order;
	}

	return nullptr;
}

void CommentListingController::setSearchString(const QString& searchString) {
	m_searchString = searchString;
}

QString CommentListingController::getSearchString() const {
	return m_searchString;
}

QString CommentListingController::getUri() const {
	return m_url->generateJsonUri().toString();
}

CommentListingURL* CommentListingController::getCommentListingUrl() {
	return m_url;
}

CommentListingFragment* CommentListingController::get(
	AppCompatActivity* parent,
	bool force,
	Bundle* savedInstanceState) {
	if(force) {
		m_session.reset();
	}
	// Port of: new CommentListingFragment(parent, savedInstanceState, General.listOfOne(mUrl), mSession, mSearchString, force)
	return new CommentListingFragment(
		parent,
		savedInstanceState,
		General::listOfOne(m_url),
		m_session,
		m_searchString,
		force);
}

bool CommentListingController::isSortable() const {
	return m_url->pathType() == RedditURLParser::POST_COMMENT_LISTING_URL
		|| m_url->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL;
}

bool CommentListingController::isUserCommentListing() const {
	return m_url->pathType() == RedditURLParser::USER_COMMENT_LISTING_URL;
}

} // namespace PinkReader
