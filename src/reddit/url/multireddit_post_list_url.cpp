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
 *   org.quantumbadger.redreader.reddit.url.MultiredditPostListURL (lines 1-287)
 */

#include "multireddit_post_list_url.h"
#include "../../core/constants.h"
#include "../kthings/reddit_id_and_type.h"

// Include PostSort placeholder (same as in subreddit_post_list_url.cpp)
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

// Port of: public static RedditURLParser.RedditURL getMultireddit(@NonNull final String name) (Java lines 36-47)
RedditURLParser::RedditURL *MultiredditPostListURL::getMultireddit(const QString &name) {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());
	builder.encodedPath(QStringLiteral("/me/m/"));
	builder.appendPath(name);
	return RedditURLParser::parse(builder.build());
}

// Port of: public static RedditURLParser.RedditURL getMultireddit(
//            @NonNull final String username, @NonNull final String name) (Java lines 49-63)
RedditURLParser::RedditURL *MultiredditPostListURL::getMultireddit(const QString &username, const QString &name) {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());
	builder.encodedPath(QStringLiteral("/user/"));
	builder.appendPath(username);
	builder.appendPath(QStringLiteral("/m/"));
	builder.appendPath(name);
	return RedditURLParser::parse(builder.build());
}

// Port of: private MultiredditPostListURL(...) (Java lines 73-87)
MultiredditPostListURL::MultiredditPostListURL(const QString &username, const QString &name,
		PostSort *order, std::optional<int> limit,
		const QString &before, RedditIdAndType *after)
	: username(username)
	, name(name)
	, order(order)
	, limit(limit)
	, before(before)
	, after(after) {}

// Port of: @Override public MultiredditPostListURL after(final RedditIdAndType newAfter) (Java lines 89-92)
PostListingURL *MultiredditPostListURL::after(const RedditIdAndType &newAfter) {
	return new MultiredditPostListURL(username, name, order, limit, before, new RedditIdAndType(newAfter.value));
}

// Port of: @Override public MultiredditPostListURL limit(final Integer newLimit) (Java lines 94-97)
PostListingURL *MultiredditPostListURL::limit(std::optional<int> newLimit) {
	return new MultiredditPostListURL(username, name, order, newLimit, before, after);
}

// Port of: public MultiredditPostListURL sort(final PostSort newOrder) (Java lines 99-101)
MultiredditPostListURL *MultiredditPostListURL::sort(PostSort *newOrder) {
	return new MultiredditPostListURL(username, name, newOrder, limit, before, after);
}

// Port of: @Nullable @Override public PostSort getOrder() (Java lines 103-107)
PostSort *MultiredditPostListURL::getOrder() {
	return order;
}

// Port of: @Override public Uri generateJsonUri() (Java lines 109-145)
PinkUri MultiredditPostListURL::generateJsonUri() {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	// Port of: if(username != null) { builder.encodedPath("/user/"); builder.appendPath(username); }
	//          else { builder.encodedPath("/me/"); } (Java lines 116-121)
	if (!username.isEmpty()) {
		builder.encodedPath(QStringLiteral("/user/"));
		builder.appendPath(username);
	} else {
		builder.encodedPath(QStringLiteral("/me/"));
	}

	// Port of: builder.appendPath("m"); builder.appendPath(name); (Java lines 123-124)
	builder.appendPath(QStringLiteral("m"));
	builder.appendPath(name);

	// Port of: if(order != null) order.addToSubredditListingUri(builder); (Java lines 126-128)
	if (order != nullptr) {
		order->addToSubredditListingUri(builder);
	}

	// Port of: if(before != null) builder.appendQueryParameter("before", before); (Java lines 130-132)
	if (!before.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("before"), before);
	}

	// Port of: if(after != null) builder.appendQueryParameter("after", after.getValue()); (Java lines 134-136)
	if (after != nullptr) {
		builder.appendQueryParameter(QStringLiteral("after"), after->value);
	}

	// Port of: if(limit != null) builder.appendQueryParameter("limit", String.valueOf(limit)); (Java lines 138-140)
	if (limit.has_value()) {
		builder.appendQueryParameter(QStringLiteral("limit"), QString::number(limit.value()));
	}

	// Port of: builder.appendEncodedPath(".json"); (Java line 142)
	builder.appendEncodedPath(QStringLiteral(".json"));

	return builder.build();
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 147-151)
int MultiredditPostListURL::pathType() {
	return RedditURLParser::MULTIREDDIT_POST_LISTING_URL;
}

// Port of: public static MultiredditPostListURL parse(final Uri uri) (Java lines 153-246)
MultiredditPostListURL *MultiredditPostListURL::parse(const PinkUri &uri) {
	std::optional<int> limit;
	QString before;
	RedditIdAndType *after = nullptr;

	// Port of: for(final String parameterKey : General.getUriQueryParameterNames(uri)) (Java lines 159-174)
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

	// Port of: final String[] pathSegments; (Java lines 176-196)
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

	// Port of: final PostSort order; (Java lines 198-205)
	PostSort *order = nullptr;
	if (pathSegmentsFiltered.size() > 0) {
		order = PostSort::parse(
				pathSegmentsFiltered.last(),
				uri.getQueryParameter(QStringLiteral("t")));
	}

	// Port of: if(pathSegments.length < 3) return null; (Java lines 207-209)
	if (pathSegmentsFiltered.size() < 3) {
		return nullptr;
	}

	// Port of: if(pathSegments[pathSegments.length - 1].equalsIgnoreCase("search")) return null; (Java lines 211-213)
	if (pathSegmentsFiltered.last().compare(QStringLiteral("search"), Qt::CaseInsensitive) == 0) {
		return nullptr;
	}

	// Port of: if(pathSegments[0].equalsIgnoreCase("me")) (Java lines 215-227)
	if (pathSegmentsFiltered[0].compare(QStringLiteral("me"), Qt::CaseInsensitive) == 0) {
		// Port of: if(!pathSegments[1].equalsIgnoreCase("m")) return null; (Java lines 217-219)
		if (!(pathSegmentsFiltered[1].compare(QStringLiteral("m"), Qt::CaseInsensitive) == 0)) {
			return nullptr;
		}

		return new MultiredditPostListURL(QString(), pathSegmentsFiltered[2], order, limit, before, after);

	// Port of: else (Java lines 229-245)
	} else {
		// Port of: if(!(pathSegments[0].equalsIgnoreCase("user") || pathSegments[0].equalsIgnoreCase("u"))
		//            || !pathSegments[2].equalsIgnoreCase("m") || pathSegments.length < 4) (Java lines 231-236)
		if ((!(pathSegmentsFiltered[0].compare(QStringLiteral("user"), Qt::CaseInsensitive) == 0
				|| pathSegmentsFiltered[0].compare(QStringLiteral("u"), Qt::CaseInsensitive) == 0))
				|| !(pathSegmentsFiltered[2].compare(QStringLiteral("m"), Qt::CaseInsensitive) == 0)
				|| pathSegmentsFiltered.size() < 4) {
			return nullptr;
		}

		return new MultiredditPostListURL(
				pathSegmentsFiltered[1], pathSegmentsFiltered[3], order, limit, before, after);
	}
}

// Port of: @Override public String humanReadablePath() (Java lines 248-275)
QString MultiredditPostListURL::humanReadablePath() {
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

// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 277-286)
QString MultiredditPostListURL::humanReadableName(bool /*shorter*/) {
	// Port of: if(username == null) return name; else return String.format(Locale.US, "%s (%s)", name, username); (Java lines 280-285)
	if (username.isEmpty()) {
		return name;
	} else {
		return QStringLiteral("%1 (%2)").arg(name, username);
	}
}

} // namespace PinkReader
