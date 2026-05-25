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
 *   org.quantumbadger.redreader.reddit.url.UserPostListingURL (lines 1-281)
 */

#include "user_post_listing_url.h"
#include "../../core/constants.h"
#include "../kthings/reddit_id_and_type.h"

// Placeholder Forward for PostSort
namespace PinkReader {

// Minimal placeholder: PostSort enum/class
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

// ---- Static factory methods (Java lines 34-52) ----

UserPostListingURL *UserPostListingURL::getSaved(const QString &username) {
	return new UserPostListingURL(Type::SAVED, username, nullptr, std::nullopt, QString(), nullptr);
}

UserPostListingURL *UserPostListingURL::getHidden(const QString &username) {
	return new UserPostListingURL(Type::HIDDEN, username, nullptr, std::nullopt, QString(), nullptr);
}

UserPostListingURL *UserPostListingURL::getLiked(const QString &username) {
	return new UserPostListingURL(Type::UPVOTED, username, nullptr, std::nullopt, QString(), nullptr);
}

UserPostListingURL *UserPostListingURL::getDisliked(const QString &username) {
	return new UserPostListingURL(Type::DOWNVOTED, username, nullptr, std::nullopt, QString(), nullptr);
}

UserPostListingURL *UserPostListingURL::getSubmitted(const QString &username) {
	return new UserPostListingURL(Type::SUBMITTED, username, nullptr, std::nullopt, QString(), nullptr);
}

// Port of: UserPostListingURL(Type, String, PostSort, Integer, String, RedditIdAndType) (Java lines 61-74)
UserPostListingURL::UserPostListingURL(Type type, const QString &user, PostSort *order,
		std::optional<int> limit, const QString &before, RedditIdAndType *after)
	: type(type)
	, user(user)
	// Port of: this.order = order == PostSort.RISING ? PostSort.NEW : order; (Java line 70)
	, order(order && order->val == PostSort::RISING ? new PostSort(PostSort::NEW) : order)
	, limit(limit)
	, before(before)
	, after(after) {}

// Port of: @Override public UserPostListingURL after(final RedditIdAndType newAfter) (Java lines 81-83)
PostListingURL *UserPostListingURL::after(const RedditIdAndType &newAfter) {
	return new UserPostListingURL(type, user, order, limit, before, new RedditIdAndType(newAfter.value));
}

// Port of: @Override public UserPostListingURL limit(final Integer newLimit) (Java lines 85-88)
PostListingURL *UserPostListingURL::limit(std::optional<int> newLimit) {
	return new UserPostListingURL(type, user, order, newLimit, before, after);
}

// Port of: public UserPostListingURL sort(final PostSort newOrder) (Java lines 90-92)
UserPostListingURL *UserPostListingURL::sort(PostSort *newOrder) {
	return new UserPostListingURL(type, user, newOrder, limit, before, after);
}

// Port of: @Override public PostSort getOrder() (Java lines 94-97)
PostSort *UserPostListingURL::getOrder() {
	return order;
}

// Port of: public static UserPostListingURL parse(final Uri uri) (Java lines 99-173)
UserPostListingURL *UserPostListingURL::parse(const PinkUri &uri) {
	std::optional<int> limit;
	QString before;
	RedditIdAndType *after = nullptr;

	// Port of: for(final String parameterKey : General.getUriQueryParameterNames(uri)) (Java lines 105-119)
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

	// Port of: final String[] pathSegments; (Java lines 121-141)
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

	// Port of: final PostSort order; (Java lines 143-150)
	PostSort *order = nullptr;
	if (pathSegmentsFiltered.size() > 0) {
		order = PostSort::parse(
				uri.getQueryParameter(QStringLiteral("sort")),
				uri.getQueryParameter(QStringLiteral("t")));
	}

	// Port of: if(pathSegments.length < 3) return null; (Java lines 152-154)
	if (pathSegmentsFiltered.size() < 3) {
		return nullptr;
	}

	// Port of: if(!pathSegments[0].equalsIgnoreCase("user") && !pathSegments[0].equalsIgnoreCase("u")) (Java lines 156-159)
	if (!pathSegmentsFiltered[0].compare(QStringLiteral("user"), Qt::CaseInsensitive) == 0
			&& !pathSegmentsFiltered[0].compare(QStringLiteral("u"), Qt::CaseInsensitive) == 0) {
		return nullptr;
	}

	// Port of: // TODO validate username with regex (Java line 161)
	// Port of: final String username = pathSegments[1]; (Java line 162)
	const QString username = pathSegmentsFiltered[1];

	// Port of: final String typeName = StringUtils.asciiUppercase(pathSegments[2]); (Java line 163)
	const QString typeName = pathSegmentsFiltered[2].toUpper();

	// Port of: final Type type; try { type = Type.valueOf(typeName); } (Java lines 164-170)
	Type type;
	if (typeName == QStringLiteral("SAVED")) type = Type::SAVED;
	else if (typeName == QStringLiteral("HIDDEN")) type = Type::HIDDEN;
	else if (typeName == QStringLiteral("UPVOTED")) type = Type::UPVOTED;
	else if (typeName == QStringLiteral("DOWNVOTED")) type = Type::DOWNVOTED;
	else if (typeName == QStringLiteral("SUBMITTED")) type = Type::SUBMITTED;
	else return nullptr;

	// Port of: return new UserPostListingURL(type, username, order, limit, before, after); (Java line 172)
	return new UserPostListingURL(type, username, order, limit, before, after);
}

// Port of: @Override public Uri generateJsonUri() (Java lines 176-205)
PinkUri UserPostListingURL::generateJsonUri() {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	// Port of: builder.appendEncodedPath("user"); (Java line 182)
	builder.appendEncodedPath(QStringLiteral("user"));

	// Port of: builder.appendPath(user); (Java line 183)
	builder.appendPath(user);

	// Port of: builder.appendEncodedPath(StringUtils.asciiLowercase(type.name())); (Java line 184)
	switch (type) {
		case Type::SAVED: builder.appendEncodedPath(QStringLiteral("saved")); break;
		case Type::HIDDEN: builder.appendEncodedPath(QStringLiteral("hidden")); break;
		case Type::UPVOTED: builder.appendEncodedPath(QStringLiteral("upvoted")); break;
		case Type::DOWNVOTED: builder.appendEncodedPath(QStringLiteral("downvoted")); break;
		case Type::SUBMITTED: builder.appendEncodedPath(QStringLiteral("submitted")); break;
	}

	// Port of: if(order != null) order.addToUserPostListingUri(builder); (Java lines 186-188)
	if (order != nullptr) {
		order->addToUserPostListingUri(builder);
	}

	// Port of: if(before != null) builder.appendQueryParameter("before", before); (Java lines 190-192)
	if (!before.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("before"), before);
	}

	// Port of: if(after != null) builder.appendQueryParameter("after", after.getValue()); (Java lines 194-196)
	if (after != nullptr) {
		builder.appendQueryParameter(QStringLiteral("after"), after->value);
	}

	// Port of: if(limit != null) builder.appendQueryParameter("limit", String.valueOf(limit)); (Java lines 198-200)
	if (limit.has_value()) {
		builder.appendQueryParameter(QStringLiteral("limit"), QString::number(limit.value()));
	}

	// Port of: builder.appendEncodedPath(".json"); (Java line 202)
	builder.appendEncodedPath(QStringLiteral(".json"));

	return builder.build();
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 207-211)
int UserPostListingURL::pathType() {
	return RedditURLParser::USER_POST_LISTING_URL;
}

// Port of: @Override public String humanReadablePath() (Java lines 214-242)
QString UserPostListingURL::humanReadablePath() {
	QString path = RedditURLParser::RedditURL::humanReadablePath();

	if (order == nullptr || type != Type::SUBMITTED) {
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
			return path + QStringLiteral("?sort=") + parts[0].toLower()
					+ QStringLiteral("&t=") + parts[1].toLower();
		}
		default:
			return path + QStringLiteral("?sort=") + order->name().toLower();
	}
}

// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 244-280)
QString UserPostListingURL::humanReadableName(bool shorter) {
	QString name;
	switch (type) {
		case Type::SAVED: name = QStringLiteral("Saved"); break;
		case Type::HIDDEN: name = QStringLiteral("Hidden"); break;
		case Type::UPVOTED: name = QStringLiteral("Upvoted"); break;
		case Type::DOWNVOTED: name = QStringLiteral("Downvoted"); break;
		case Type::SUBMITTED: name = QStringLiteral("Submitted"); break;
		default: return RedditURLParser::RedditURL::humanReadableName(shorter);
	}

	if (shorter) {
		return name;
	} else {
		return QStringLiteral("%1 (%2)").arg(name, user);
	}
}

} // namespace PinkReader
