/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: search_post_list_url.cpp - Port of RedReader's SearchPostListURL
 */

#include "search_post_list_url.h"
#include "../post_sort.h"
#include "../kthings/reddit_id_and_type.h"
#include "../../core/constants.h"

namespace PinkReader {

// Port of: SearchPostListURL(String, String, PostSort, Integer, String, RedditIdAndType)
SearchPostListURL::SearchPostListURL(const QString &subreddit, const QString &query,
		PostSort *order, std::optional<int> limit,
		const QString &before, RedditIdAndType *after)
	: type(Type::SUB_OR_SUB_COMBO)
	, subreddit(subreddit)
	, query(query)
	, order(order)
	, limit(limit)
	, before(before)
	, after(after) {
}

// Port of: SearchPostListURL(String, String, Integer, String, RedditIdAndType)
SearchPostListURL::SearchPostListURL(const QString &subreddit, const QString &query,
		std::optional<int> limit, const QString &before,
		RedditIdAndType *after)
	: SearchPostListURL(subreddit, query,
		const_cast<PostSort *>(&PostSort::RELEVANCE_ALL_VAL()),
		limit, before, after) {
}

// Port of: SearchPostListURL(String username, String name, String query,
//     PostSort, Integer, String, RedditIdAndType)
SearchPostListURL::SearchPostListURL(const QString &username, const QString &name,
		const QString &query, PostSort *order, std::optional<int> limit,
		const QString &before, RedditIdAndType *after)
	: type(Type::MULTI)
	, username(username)
	, name(name)
	, query(query)
	, order(order)
	, limit(limit)
	, before(before)
	, after(after) {
}

// Port of: SearchPostListURL(String username, String name, String query,
//     Integer, String, RedditIdAndType)
SearchPostListURL::SearchPostListURL(const QString &username, const QString &name,
		const QString &query, std::optional<int> limit,
		const QString &before, RedditIdAndType *after)
	: SearchPostListURL(username, name, query,
		const_cast<PostSort *>(&PostSort::RELEVANCE_ALL_VAL()),
		limit, before, after) {
}

// Port of: public static SearchPostListURL build(String location, final String query)
SearchPostListURL *SearchPostListURL::build(const QString &location, const QString &query) {
	return new SearchPostListURL(location, query, std::nullopt, QString(), nullptr);
}

// Port of: @Override public SearchPostListURL after(final RedditIdAndType newAfter)
PostListingURL *SearchPostListURL::after(const RedditIdAndType &newAfter) {
	if (type == Type::SUB_OR_SUB_COMBO) {
		return new SearchPostListURL(subreddit, query, order, limit, before,
			new RedditIdAndType(newAfter.value));
	}
	return new SearchPostListURL(username, name, query, order, limit, before,
		new RedditIdAndType(newAfter.value));
}

// Port of: @Override public SearchPostListURL limit(final Integer newLimit)
PostListingURL *SearchPostListURL::limit(std::optional<int> newLimit) {
	if (type == Type::SUB_OR_SUB_COMBO) {
		return new SearchPostListURL(subreddit, query, order, newLimit, before, after);
	}
	return new SearchPostListURL(username, name, query, order, newLimit, before, after);
}

// Port of: public SearchPostListURL sort(final PostSort newOrder)
SearchPostListURL *SearchPostListURL::sort(PostSort *newOrder) {
	if (type == Type::SUB_OR_SUB_COMBO) {
		return new SearchPostListURL(subreddit, query, newOrder, limit, before, after);
	}
	return new SearchPostListURL(username, name, query, newOrder, limit, before, after);
}

// Port of: @Nullable @Override public PostSort getOrder()
PostSort *SearchPostListURL::getOrder() {
	return order;
}

// Port of: @Override public Uri generateJsonUri()
PinkUri SearchPostListURL::generateJsonUri() {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	if (type == Type::SUB_OR_SUB_COMBO && !subreddit.isEmpty()) {
		builder.encodedPath(QStringLiteral("/r/"));
		builder.appendPath(subreddit);
		builder.appendQueryParameter(QStringLiteral("restrict_sr"), QStringLiteral("on"));
	} else if (type == Type::MULTI && !name.isEmpty()) {
		if (!username.isEmpty()) {
			builder.encodedPath(QStringLiteral("/user/"));
			builder.appendPath(username);
		} else {
			builder.encodedPath(QStringLiteral("/me/"));
		}
		builder.appendPath(QStringLiteral("m"));
		builder.appendPath(name);
		builder.appendQueryParameter(QStringLiteral("restrict_sr"), QStringLiteral("on"));
	} else {
		builder.encodedPath(QStringLiteral("/"));
	}

	builder.appendEncodedPath(QStringLiteral("search"));

	if (!query.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("q"), query);
	}

	// Port of the order switch: order.name().split("_") -> sort + t params
	if (order != nullptr) {
		QString sort, t;
		switch (order->val) {
			case PostSort::RELEVANCE_HOUR: sort = "relevance"; t = "hour"; break;
			case PostSort::RELEVANCE_DAY: sort = "relevance"; t = "day"; break;
			case PostSort::RELEVANCE_WEEK: sort = "relevance"; t = "week"; break;
			case PostSort::RELEVANCE_MONTH: sort = "relevance"; t = "month"; break;
			case PostSort::RELEVANCE_YEAR: sort = "relevance"; t = "year"; break;
			case PostSort::RELEVANCE_ALL: sort = "relevance"; t = "all"; break;
			case PostSort::NEW_HOUR: sort = "new"; t = "hour"; break;
			case PostSort::NEW_DAY: sort = "new"; t = "day"; break;
			case PostSort::NEW_WEEK: sort = "new"; t = "week"; break;
			case PostSort::NEW_MONTH: sort = "new"; t = "month"; break;
			case PostSort::NEW_YEAR: sort = "new"; t = "year"; break;
			case PostSort::NEW_ALL: sort = "new"; t = "all"; break;
			case PostSort::COMMENTS_HOUR: sort = "comments"; t = "hour"; break;
			case PostSort::COMMENTS_DAY: sort = "comments"; t = "day"; break;
			case PostSort::COMMENTS_WEEK: sort = "comments"; t = "week"; break;
			case PostSort::COMMENTS_MONTH: sort = "comments"; t = "month"; break;
			case PostSort::COMMENTS_YEAR: sort = "comments"; t = "year"; break;
			case PostSort::COMMENTS_ALL: sort = "comments"; t = "all"; break;
			case PostSort::HOT_HOUR: sort = "hot"; t = "hour"; break;
			case PostSort::HOT_DAY: sort = "hot"; t = "day"; break;
			case PostSort::HOT_WEEK: sort = "hot"; t = "week"; break;
			case PostSort::HOT_MONTH: sort = "hot"; t = "month"; break;
			case PostSort::HOT_YEAR: sort = "hot"; t = "year"; break;
			case PostSort::HOT_ALL: sort = "hot"; t = "all"; break;
			default: break;
		}
		if (!sort.isEmpty()) {
			builder.appendQueryParameter(QStringLiteral("sort"), sort);
			builder.appendQueryParameter(QStringLiteral("t"), t);
		}
	}

	if (!before.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("before"), before);
	}

	if (after != nullptr) {
		builder.appendQueryParameter(QStringLiteral("after"), after->value);
	}

	return builder.build();
}

// Port of: @Override public @RedditURLParser.PathType int pathType()
int SearchPostListURL::pathType() {
	return RedditURLParser::SEARCH_POST_LISTING_URL;
}

// Port of: @Override public String humanReadableName(final Context context, final boolean shorter)
QString SearchPostListURL::humanReadableName(bool shorter) {
	if (shorter) {
		return QStringLiteral("Search results");
	}

	QString formattedLocation;
	if (type == Type::SUB_OR_SUB_COMBO) {
		if (!subreddit.isEmpty()) {
			formattedLocation = QStringLiteral("/r/") + subreddit;
		}
	} else {
		if (!name.isEmpty()) {
			if (!username.isEmpty()) {
				formattedLocation = QStringLiteral("/u/") + username
					+ QStringLiteral("/m/") + name;
			} else {
				formattedLocation = QStringLiteral("/me/m/") + name;
			}
		}
	}

	if (!query.isEmpty() && !formattedLocation.isEmpty()) {
		return QStringLiteral("Search: %1 in %2").arg(query, formattedLocation);
	} else if (!query.isEmpty()) {
		return QStringLiteral("Search: %1").arg(query);
	} else if (!formattedLocation.isEmpty()) {
		return formattedLocation;
	}

	return QStringLiteral("Search");
}

// Port of: @Override public String humanReadablePath()
QString SearchPostListURL::humanReadablePath() {
	QString path = RedditURLParser::RedditURL::humanReadablePath();

	if (!query.isEmpty()) {
		path += QStringLiteral("?q=") + query;
	}

	return path;
}

} // namespace PinkReader
