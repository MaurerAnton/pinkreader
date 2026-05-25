/*******************************************************************************
 * This file is part of RedReader.
 *
 * RedReader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RedReader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RedReader.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/*
 * PinkReader C++ port of:
 *   org.quantumbadger.redreader.reddit.url.SubredditPostListURL (lines 1-431)
 */

#include "subreddit_post_list_url.h"
#include "../../core/constants.h"
#include "../kthings/reddit_id_and_type.h"

#include <QRegularExpression>

// Include PostSort placeholder (same as in user_post_listing_url.cpp)
namespace PinkReader {

class PostSort {
public:
	enum Value {
		HOT, NEW, RISING, CONTROVERSIAL, TOP,
		HOT_HOUR, HOT_DAY, HOT_WEEK, HOT_MONTH, HOT_YEAR, HOT_ALL,
		NEW_HOUR, NEW_DAY, NEW_WEEK, NEW_MONTH, NEW_YEAR, NEW_ALL,
		CONTROVERSIAL_HOUR, CONTROVERSIAL_DAY, CONTROVERSIAL_WEEK,
		CONTROVERSIAL_MONTH, CONTROVERSIAL_YEAR, CONTROVERSIAL_ALL,
		TOP_HOUR, TOP_DAY, TOP_WEEK, TOP_MONTH, TOP_YEAR, TOP_ALL,
		RELEVANCE_HOUR, RELEVANCE_DAY, RELEVANCE_WEEK, RELEVANCE_MONTH,
		RELEVANCE_YEAR, RELEVANCE_ALL,
		COMMENTS_HOUR, COMMENTS_DAY, COMMENTS_WEEK, COMMENTS_MONTH,
		COMMENTS_YEAR, COMMENTS_ALL
	};

	Value val;
	explicit PostSort(Value v) : val(v) {}

	static PostSort *parse(const QString &sort, const QString &time) {
		if (sort.isEmpty()) return nullptr;
		QString lower = sort.toLower();
		if (lower == QStringLiteral("hot")) return new PostSort(HOT);
		if (lower == QStringLiteral("new")) return new PostSort(NEW);
		if (lower == QStringLiteral("rising")) return new PostSort(RISING);
		if (lower == QStringLiteral("controversial")) return new PostSort(CONTROVERSIAL);
		if (lower == QStringLiteral("top")) return new PostSort(TOP);
		return nullptr;
	}

	static PostSort *parseSearch(const QString &sort, const QString &time) {
		return parse(sort, time);
	}

	QString name() const {
		switch (val) {
			case HOT: return QStringLiteral("HOT");
			case NEW: return QStringLiteral("NEW");
			case RISING: return QStringLiteral("RISING");
			case CONTROVERSIAL: return QStringLiteral("CONTROVERSIAL");
			case TOP: return QStringLiteral("TOP");
			default: return QStringLiteral("HOT");
		}
	}

	void addToSubredditListingUri(PinkUri::Builder &builder) const {
		QString n = name().toLower();
		builder.appendEncodedPath(n);
	}

	void addToUserPostListingUri(PinkUri::Builder &builder) const {
		QString n = name().toLower();
		builder.appendQueryParameter(QStringLiteral("sort"), n);
	}
};

} // namespace PinkReader

namespace PinkReader {

// Port of: public static SubredditPostListURL getFrontPage() (Java lines 38-40)
SubredditPostListURL *SubredditPostListURL::getFrontPage() {
	return new SubredditPostListURL(Type::FRONTPAGE, QString(), nullptr, std::nullopt, QString(), nullptr);
}

// Port of: public static SubredditPostListURL getPopular() (Java lines 42-44)
SubredditPostListURL *SubredditPostListURL::getPopular() {
	return new SubredditPostListURL(Type::POPULAR, QString(), nullptr, std::nullopt, QString(), nullptr);
}

// Port of: public static SubredditPostListURL getAll() (Java lines 46-48)
SubredditPostListURL *SubredditPostListURL::getAll() {
	return new SubredditPostListURL(Type::ALL, QString(), nullptr, std::nullopt, QString(), nullptr);
}

// Port of: public static RedditURLParser.RedditURL getSubreddit(final String) (Java lines 50-53)
RedditURLParser::RedditURL *SubredditPostListURL::getSubreddit(const QString &subreddit) {
	// Port of: return getSubreddit(new SubredditCanonicalId(subreddit)); (Java line 52)
	return getSubredditCanonical(QStringLiteral("/r/") + subreddit);
}

// Port of: public static RedditURLParser.RedditURL getSubreddit(final SubredditCanonicalId) (Java lines 55-61)
RedditURLParser::RedditURL *SubredditPostListURL::getSubredditCanonical(const QString &subredditPath) {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());
	builder.encodedPath(subredditPath);
	return RedditURLParser::parse(builder.build());
}

// Port of: private SubredditPostListURL(Type, String, PostSort, Integer, String, RedditIdAndType) (Java lines 75-89)
SubredditPostListURL::SubredditPostListURL(Type type, const QString &subreddit, PostSort *order,
		std::optional<int> limit, const QString &before, RedditIdAndType *after)
	: type(type)
	, subreddit(subreddit)
	, order(order)
	, limit(limit)
	, before(before)
	, after(after) {}

// Port of: @Override public SubredditPostListURL after(final RedditIdAndType newAfter) (Java lines 91-94)
PostListingURL *SubredditPostListURL::after(const RedditIdAndType &newAfter) {
	return new SubredditPostListURL(type, subreddit, order, limit, before, new RedditIdAndType(newAfter.value));
}

// Port of: @Override public SubredditPostListURL limit(final Integer newLimit) (Java lines 96-99)
PostListingURL *SubredditPostListURL::limit(std::optional<int> newLimit) {
	return new SubredditPostListURL(type, subreddit, order, newLimit, before, after);
}

// Port of: public SubredditPostListURL sort(final PostSort newOrder) (Java lines 101-103)
SubredditPostListURL *SubredditPostListURL::sort(PostSort *newOrder) {
	return new SubredditPostListURL(type, subreddit, newOrder, limit, before, after);
}

// Port of: @Nullable @Override public PostSort getOrder() (Java lines 105-109)
PostSort *SubredditPostListURL::getOrder() {
	return order;
}

// Port of: @Override public Uri generateJsonUri() (Java lines 111-159)
PinkUri SubredditPostListURL::generateJsonUri() {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	// Port of: switch(type) (Java lines 118-138)
	switch (type) {
		case Type::FRONTPAGE:
			builder.encodedPath(QStringLiteral("/"));
			break;
		case Type::ALL:
			builder.encodedPath(QStringLiteral("/r/all"));
			break;
		case Type::SUBREDDIT:
		case Type::SUBREDDIT_COMBINATION:
		case Type::ALL_SUBTRACTION:
			builder.encodedPath(QStringLiteral("/r/"));
			builder.appendPath(subreddit);
			break;
		case Type::POPULAR:
			builder.encodedPath(QStringLiteral("/r/popular"));
			break;
	}

	// Port of: if(order != null) order.addToSubredditListingUri(builder); (Java lines 140-142)
	if (order != nullptr) {
		order->addToSubredditListingUri(builder);
	}

	// Port of: if(before != null) builder.appendQueryParameter("before", before); (Java lines 144-146)
	if (!before.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("before"), before);
	}

	// Port of: if(after != null) builder.appendQueryParameter("after", after.getValue()); (Java lines 148-150)
	if (after != nullptr) {
		builder.appendQueryParameter(QStringLiteral("after"), after->value);
	}

	// Port of: if(limit != null) builder.appendQueryParameter("limit", String.valueOf(limit)); (Java lines 152-154)
	if (limit.has_value()) {
		builder.appendQueryParameter(QStringLiteral("limit"), QString::number(limit.value()));
	}

	// Port of: builder.appendEncodedPath(".json"); (Java line 156)
	builder.appendEncodedPath(QStringLiteral(".json"));

	return builder.build();
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 161-165)
int SubredditPostListURL::pathType() {
	return RedditURLParser::SUBREDDIT_POST_LISTING_URL;
}

// Port of: public static SubredditPostListURL parse(final Uri uri) (Java lines 167-367)
SubredditPostListURL *SubredditPostListURL::parse(const PinkUri &uri) {
	std::optional<int> limit;
	QString before;
	RedditIdAndType *after = nullptr;

	// Port of: for(final String parameterKey : General.getUriQueryParameterNames(uri)) (Java lines 173-188)
	QStringList paramNames = uri.getQueryParameterNames();
	for (const QString &parameterKey : paramNames) {
		if (parameterKey.compare(QStringLiteral("after"), Qt::CaseInsensitive) == 0) {
			after = new RedditIdAndType(uri.getQueryParameter(parameterKey));
		} else if (parameterKey.compare(QStringLiteral("before"), Qt::CaseInsensitive) == 0) {
			before = uri.getQueryParameter(parameterKey);
		} else if (parameterKey.compare(QStringLiteral("limit"), Qt::CaseInsensitive) == 0) {
			bool ok = false;
			int val = uri.getQueryParameter(parameterKey).toInt(&ok);
			if (ok) limit = val;
		}
	}

	// Port of: final String[] pathSegments; (Java lines 190-209)
	QStringList pathSegmentsList = uri.getPathSegments();
	QStringList pathSegmentsFiltered;
	pathSegmentsFiltered.reserve(pathSegmentsList.size());
	for (QString segment : pathSegmentsList) {
		while (segment.toLower().endsWith(QStringLiteral(".json"))
				|| segment.toLower().endsWith(QStringLiteral(".xml"))) {
			segment = segment.left(segment.lastIndexOf('.'));
		}
		if (!segment.isEmpty()) {
			pathSegmentsFiltered.append(segment);
		}
	}

	// Port of: final PostSort order; (Java lines 211-218)
	PostSort *order = nullptr;
	if (pathSegmentsFiltered.size() > 0) {
		order = PostSort::parse(
				pathSegmentsFiltered.last(),
				uri.getQueryParameter(QStringLiteral("t")));
	}

	// Port of: switch(pathSegments.length) (Java lines 220-366)
	switch (pathSegmentsFiltered.size()) {
		case 0:
			// Port of: return new SubredditPostListURL(Type.FRONTPAGE, null, null, limit, before, after); (Java lines 222-228)
			return new SubredditPostListURL(Type::FRONTPAGE, QString(), nullptr, limit, before, after);

		case 1: {
			// Port of: if(order != null) { return ...FRONTPAGE... } else return null; (Java lines 231-242)
			if (order != nullptr) {
				return new SubredditPostListURL(Type::FRONTPAGE, QString(), order, limit, before, after);
			} else {
				return nullptr;
			}
		}

		case 2:
		case 3: {
			// Port of: if(!pathSegments[0].equals("r")) return null; (Java lines 247-249)
			if (pathSegmentsFiltered[0] != QStringLiteral("r")) {
				return nullptr;
			}

			// Port of: final String subreddit = StringUtils.asciiLowercase(pathSegments[1]); (Java line 251)
			const QString sub = pathSegmentsFiltered[1].toLower();

			// Port of: if(subreddit.equals("all")) (Java lines 253-275)
			if (sub == QStringLiteral("all")) {
				if (pathSegmentsFiltered.size() == 2) {
					return new SubredditPostListURL(Type::ALL, QString(), nullptr, limit, before, after);
				} else if (order != nullptr) {
					return new SubredditPostListURL(Type::ALL, QString(), order, limit, before, after);
				} else {
					return nullptr;
				}
			// Port of: else if(subreddit.equals("popular")) (Java lines 277-285)
			} else if (sub == QStringLiteral("popular")) {
				return new SubredditPostListURL(Type::POPULAR, QString(), order, limit, before, after);

			// Port of: else if(subreddit.matches("all(\\-[\\w\\.]+)+")) (Java lines 287-308)
			} else if (QRegularExpression(QStringLiteral("all(-[\\w\\.]+)+")).match(sub).hasMatch()) {
				if (pathSegmentsFiltered.size() == 2) {
					return new SubredditPostListURL(Type::ALL_SUBTRACTION, sub, nullptr, limit, before, after);
				} else if (order != nullptr) {
					return new SubredditPostListURL(Type::ALL_SUBTRACTION, sub, order, limit, before, after);
				} else {
					return nullptr;
				}

			// Port of: else if(subreddit.matches("\\w+(\\+[\\w\\.]+)+")) (Java lines 311-333)
			} else if (QRegularExpression(QStringLiteral("\\w+(\\+[\\w\\.]+)+")).match(sub).hasMatch()) {
				if (pathSegmentsFiltered.size() == 2) {
					return new SubredditPostListURL(Type::SUBREDDIT_COMBINATION, sub, nullptr, limit, before, after);
				} else if (order != nullptr) {
					return new SubredditPostListURL(Type::SUBREDDIT_COMBINATION, sub, order, limit, before, after);
				} else {
					return nullptr;
				}

			// Port of: else if(subreddit.matches("[\\w\\.]+")) (Java lines 335-357)
			} else if (QRegularExpression(QStringLiteral("[\\w\\.]+")).match(sub).hasMatch()) {
				if (pathSegmentsFiltered.size() == 2) {
					return new SubredditPostListURL(Type::SUBREDDIT, sub, nullptr, limit, before, after);
				} else if (order != nullptr) {
					return new SubredditPostListURL(Type::SUBREDDIT, sub, order, limit, before, after);
				} else {
					return nullptr;
				}

			} else {
				return nullptr;
			}
		}

		default:
			return nullptr;
	}
}

// Port of: @Override public String humanReadablePath() (Java lines 369-396)
QString SubredditPostListURL::humanReadablePath() {
	QString path = RedditURLParser::RedditURL::humanReadablePath();

	if (order == nullptr) {
		return path;
	}

	switch (order->val) {
		case PostSort::CONTROVERSIAL_HOUR:
		case PostSort::CONTROVERSIAL_DAY:
		case PostSort::CONTROVERSIAL_WEEK:
		case PostSort::CONTROVERSIAL_MONTH:
		case PostSort::CONTROVERSIAL_YEAR:
		case PostSort::CONTROVERSIAL_ALL:
		case PostSort::TOP_HOUR:
		case PostSort::TOP_DAY:
		case PostSort::TOP_WEEK:
		case PostSort::TOP_MONTH:
		case PostSort::TOP_YEAR:
		case PostSort::TOP_ALL: {
			QStringList parts = order->name().split('_');
			return path + QStringLiteral("?t=") + parts[1].toLower();
		}
		default:
			return path;
	}
}

// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 398-426)
QString SubredditPostListURL::humanReadableName(bool /*shorter*/) {
	switch (type) {
		case Type::FRONTPAGE:
			return QStringLiteral("Front Page");
		case Type::ALL:
			return QStringLiteral("All");
		case Type::POPULAR:
			return QStringLiteral("Popular");
		case Type::SUBREDDIT:
			return subreddit;
		case Type::SUBREDDIT_COMBINATION:
		case Type::ALL_SUBTRACTION:
			return subreddit;
		default:
			return RedditURLParser::RedditURL::humanReadableName(false);
	}
}

// Port of: public SubredditPostListURL changeSubreddit(final String newSubreddit) (Java lines 428-430)
SubredditPostListURL *SubredditPostListURL::changeSubreddit(const QString &newSubreddit) {
	return new SubredditPostListURL(type, newSubreddit, order, limit, before, after);
}

} // namespace PinkReader
