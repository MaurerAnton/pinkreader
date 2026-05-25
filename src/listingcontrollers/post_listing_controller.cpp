// Origin: org/quantumbadger/redreader/listingcontrollers/PostListingController.java
#include "post_listing_controller.h"

#include <stdexcept>

#include "../reddit/url/reddit_url_parser.h"
#include "../reddit/url/post_listing_url.h"
#include "../reddit/url/subreddit_post_list_url.h"
#include "../reddit/url/user_post_listing_url.h"
#include "../reddit/url/multireddit_post_list_url.h"
#include "../reddit/post_sort.h"
#include "../reddit/things/subreddit_canonical_id.h"
#include "../reddit/things/invalid_subreddit_name_exception.h"
#include "../settings/prefs_utility.h"
#include "../fragments/post_listing_fragment.h"

namespace PinkReader {

PostListingController::PostListingController(PostListingURL& url)
	: m_url(nullptr) {

	if(url.pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL) {
		SubredditPostListURL* sp = url.asSubredditPostListURL();
		if(sp->order == nullptr) {
			PostSort* order = PrefsUtility::pref_behaviour_postsort();

			if(order && order->value == PostSort::BEST
				&& sp->type != SubredditPostListURL::Type::FRONTPAGE) {
				order = const_cast<PostSort*>(&PostSort::HOT_VAL());
			}

			sp = sp->sort(*order);
			url = *sp;
		}
	} else if(url.pathType() == RedditURLParser::USER_POST_LISTING_URL) {
		UserPostListingURL* up = url.asUserPostListURL();
		if(up->order == nullptr) {
			PostSort* order = PrefsUtility::pref_behaviour_user_postsort();
			up = up->sort(*order);
			url = *up;
		}
	} else if(url.pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
		MultiredditPostListURL* mp = url.asMultiredditPostListURL();
		if(mp->order == nullptr) {
			PostSort* order = PrefsUtility::pref_behaviour_multi_postsort();
			mp = mp->sort(*order);
			url = *mp;
		}
	}

	m_url = &url;
}

void PostListingController::setSession(const std::optional<QString>& session) {
	m_session = session;
}

std::optional<QString> PostListingController::getSession() const {
	return m_session;
}

bool PostListingController::isSortable() const {
	if(m_url->pathType() == RedditURLParser::USER_POST_LISTING_URL) {
		return (m_url->asUserPostListURL()->type == UserPostListingURL::Type::SUBMITTED);
	}
	return (m_url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL)
		|| (m_url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL)
		|| (m_url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL);
}

bool PostListingController::isFrontPage() const {
	return m_url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
		&& m_url->asSubredditPostListURL()->type
			== SubredditPostListURL::Type::FRONTPAGE;
}

void PostListingController::setSort(const PostSort& order) {
	if(m_url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL) {
		SubredditPostListURL* sp = m_url->asSubredditPostListURL();
		m_url = sp->sort(order);

	} else if(m_url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
		MultiredditPostListURL* mp = m_url->asMultiredditPostListURL();
		m_url = mp->sort(order);

	} else if(m_url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL) {
		SearchPostListURL* sp = m_url->asSearchPostListURL();
		m_url = sp->sort(order);

	} else if(m_url->pathType() == RedditURLParser::USER_POST_LISTING_URL) {
		UserPostListingURL* up = m_url->asUserPostListURL();
		m_url = up->sort(order);

	} else {
		throw std::runtime_error("Cannot set sort for this URL");
	}
}

PostSort* PostListingController::getSort() {
	if(m_url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL) {
		return m_url->asSubredditPostListURL()->order;
	}

	if(m_url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
		return m_url->asMultiredditPostListURL()->order;
	}

	if(m_url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL) {
		return m_url->asSearchPostListURL()->order;
	}

	if(m_url->pathType() == RedditURLParser::USER_POST_LISTING_URL) {
		return m_url->asUserPostListURL()->order;
	}

	return nullptr;
}

QString PostListingController::getUri() const {
	return m_url->generateJsonUri().toString();
}

PostListingFragment* PostListingController::get(
	AppCompatActivity* parent,
	bool force,
	Bundle* savedInstanceState) {
	if(force) {
		m_session.reset();
	}
	return new PostListingFragment(
		parent,
		savedInstanceState,
		getUri(),
		m_session,
		force);
}

bool PostListingController::isSubreddit() const {
	return m_url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
		&& m_url->asSubredditPostListURL()->type
			== SubredditPostListURL::Type::SUBREDDIT;
}

bool PostListingController::isSubredditCombination() const {
	return m_url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
		&& m_url->asSubredditPostListURL()->type
			== SubredditPostListURL::Type::SUBREDDIT_COMBINATION;
}

bool PostListingController::isMultireddit() const {
	return m_url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL;
}

bool PostListingController::isSearchResults() const {
	return m_url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL;
}

bool PostListingController::isSubredditSearchResults() const {
	return isSearchResults() && m_url->asSearchPostListURL()->subreddit.has_value();
}

bool PostListingController::isUserPostListing() const {
	return m_url->pathType() == RedditURLParser::USER_POST_LISTING_URL;
}

SubredditCanonicalId* PostListingController::subredditCanonicalName() {
	if(m_url->pathType() == RedditURLParser::SUBREDDIT_POST_LISTING_URL
		&& (m_url->asSubredditPostListURL()->type
			== SubredditPostListURL::Type::SUBREDDIT
			|| m_url->asSubredditPostListURL()->type
			== SubredditPostListURL::Type::SUBREDDIT_COMBINATION)) {
		try {
			return new SubredditCanonicalId(m_url->asSubredditPostListURL()->subreddit);
		} catch(const InvalidSubredditNameException& e) {
			throw std::runtime_error(e.what());
		}
	} else if(m_url->pathType() == RedditURLParser::SEARCH_POST_LISTING_URL
		&& m_url->asSearchPostListURL()->subreddit.has_value()) {
		try {
			return new SubredditCanonicalId(*m_url->asSearchPostListURL()->subreddit);
		} catch(const InvalidSubredditNameException& e) {
			throw std::runtime_error(e.what());
		}
	}

	return nullptr;
}

QString PostListingController::multiredditName() const {
	if(m_url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
		return m_url->asMultiredditPostListURL()->name;
	}

	return QString();
}

QString PostListingController::multiredditUsername() const {
	if(m_url->pathType() == RedditURLParser::MULTIREDDIT_POST_LISTING_URL) {
		return m_url->asMultiredditPostListURL()->username;
	}

	return QString();
}

} // namespace PinkReader
