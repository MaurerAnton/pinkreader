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
 *   org.quantumbadger.redreader.reddit.url.UserCommentListingURL (lines 1-177)
 */

#include "user_comment_listing_url.h"
#include "../../core/constants.h"

// Placeholder for UserCommentSort until properly ported
namespace PinkReader {

// Minimal placeholder: UserCommentSort
class UserCommentSort {
public:
	QString key;
	QString timeKey;
	UserCommentSort(const QString &k, const QString &t) : key(k), timeKey(t) {}

	static UserCommentSort *parse(const QString &sort, const QString &time) {
		return new UserCommentSort(sort, time);
	}
	void addToUserCommentListingUri(PinkUri::Builder &builder) const {
		if (!key.isEmpty()) builder.appendQueryParameter(QStringLiteral("sort"), key);
		if (!timeKey.isEmpty()) builder.appendQueryParameter(QStringLiteral("t"), timeKey);
	}
};

} // namespace PinkReader

namespace PinkReader {

// Port of: UserCommentListingURL(String, UserCommentSort, Integer, String) (Java lines 38-47)
UserCommentListingURL::UserCommentListingURL(const QString &user, UserCommentSort *order,
		std::optional<int> limit, const QString &after)
	: user(user)
	, order(order)
	, limit(limit)
	, after(after) {}

// Port of: @Override public UserCommentListingURL after(final String newAfter) (Java lines 50-52)
CommentListingURL *UserCommentListingURL::after(const QString &newAfter) {
	return new UserCommentListingURL(user, order, limit, newAfter);
}

// Port of: @Override public UserCommentListingURL limit(final Integer newLimit) (Java lines 54-57)
CommentListingURL *UserCommentListingURL::limit(std::optional<int> newLimit) {
	return new UserCommentListingURL(user, order, newLimit, after);
}

// Port of: public UserCommentListingURL order(final UserCommentSort newOrder) (Java lines 59-61)
UserCommentListingURL *UserCommentListingURL::order(UserCommentSort *newOrder) {
	return new UserCommentListingURL(user, newOrder, limit, after);
}

// Port of: public static UserCommentListingURL parse(final Uri uri) (Java lines 63-129)
UserCommentListingURL *UserCommentListingURL::parse(const PinkUri &uri) {
	// Port of: final String[] pathSegments; (Java lines 65-85)
	QStringList pathSegmentsList = uri.getPathSegments();

	QStringList pathSegmentsFiltered;
	pathSegmentsFiltered.reserve(pathSegmentsList.size());
	for (QString segment : pathSegmentsList) {
		// Port of: while(StringUtils.asciiLowercase(segment).endsWith(".json") || ...endsWith(".xml")) (Java lines 73-76)
		while (segment.toLower().endsWith(QStringLiteral(".json"))
				|| segment.toLower().endsWith(QStringLiteral(".xml"))) {
			segment = segment.left(segment.lastIndexOf('.'));
		}

		// Port of: if(!segment.isEmpty()) pathSegmentsFiltered.add(segment); (Java lines 78-80)
		if (!segment.isEmpty()) {
			pathSegmentsFiltered.append(segment);
		}
	}

	// Port of: final UserCommentSort order; (Java lines 87-93)
	UserCommentSort *order = nullptr;
	if (pathSegmentsFiltered.size() > 0) {
		order = UserCommentSort::parse(
				uri.getQueryParameter(QStringLiteral("sort")),
				uri.getQueryParameter(QStringLiteral("t")));
	}

	// Port of: if(pathSegments.length < 3) return null; (Java lines 95-97)
	if (pathSegmentsFiltered.size() < 3) {
		return nullptr;
	}

	// Port of: if(!pathSegments[0].equalsIgnoreCase("user") && !pathSegments[0].equalsIgnoreCase("u")) (Java lines 99-102)
	if (!pathSegmentsFiltered[0].compare(QStringLiteral("user"), Qt::CaseInsensitive) == 0
			&& !pathSegmentsFiltered[0].compare(QStringLiteral("u"), Qt::CaseInsensitive) == 0) {
		return nullptr;
	}

	// Port of: // TODO validate username with regex (Java line 104)
	// Port of: final String username = pathSegments[1]; (Java line 105)
	const QString username = pathSegmentsFiltered[1];

	// Port of: final String typeName = pathSegments[2]; (Java line 106)
	const QString typeName = pathSegmentsFiltered[2];

	// Port of: if(!typeName.equalsIgnoreCase("comments")) return null; (Java lines 108-110)
	if (typeName.compare(QStringLiteral("comments"), Qt::CaseInsensitive) != 0) {
		return nullptr;
	}

	std::optional<int> limit;
	QString after;

	// Port of: for(final String parameterKey : General.getUriQueryParameterNames(uri)) (Java lines 115-126)
	QStringList paramNames = uri.getQueryParameterNames();
	for (const QString &parameterKey : paramNames) {
		// Port of: if(parameterKey.equalsIgnoreCase("after")) (Java lines 117-118)
		if (parameterKey.compare(QStringLiteral("after"), Qt::CaseInsensitive) == 0) {
			after = uri.getQueryParameter(parameterKey);

		// Port of: else if(parameterKey.equalsIgnoreCase("limit")) (Java lines 120-125)
		} else if (parameterKey.compare(QStringLiteral("limit"), Qt::CaseInsensitive) == 0) {
			bool ok = false;
			int val = uri.getQueryParameter(parameterKey).toInt(&ok);
			if (ok) {
				limit = val;
			}
		}
	}

	// Port of: return new UserCommentListingURL(username, order, limit, after); (Java line 128)
	return new UserCommentListingURL(username, order, limit, after);
}

// Port of: @Override public Uri generateJsonUri() (Java lines 132-157)
PinkUri UserCommentListingURL::generateJsonUri() {
	PinkUri::Builder builder;

	// Port of: builder.scheme(Constants.Reddit.getScheme()).authority(Constants.Reddit.getDomain()); (Java lines 135-136)
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	// Port of: builder.appendEncodedPath("user"); (Java line 138)
	builder.appendEncodedPath(QStringLiteral("user"));

	// Port of: builder.appendPath(user); (Java line 139)
	builder.appendPath(user);

	// Port of: builder.appendEncodedPath("comments"); (Java line 140)
	builder.appendEncodedPath(QStringLiteral("comments"));

	// Port of: if(order != null) order.addToUserCommentListingUri(builder); (Java lines 142-144)
	if (order != nullptr) {
		order->addToUserCommentListingUri(builder);
	}

	// Port of: if(after != null) builder.appendQueryParameter("after", after); (Java lines 146-148)
	if (!after.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("after"), after);
	}

	// Port of: if(limit != null) builder.appendQueryParameter("limit", String.valueOf(limit)); (Java lines 150-152)
	if (limit.has_value()) {
		builder.appendQueryParameter(QStringLiteral("limit"), QString::number(limit.value()));
	}

	// Port of: builder.appendEncodedPath(".json"); (Java line 154)
	builder.appendEncodedPath(QStringLiteral(".json"));

	// Port of: return builder.build(); (Java line 156)
	return builder.build();
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 159-163)
int UserCommentListingURL::pathType() {
	return RedditURLParser::USER_COMMENT_LISTING_URL;
}

// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 165-175)
QString UserCommentListingURL::humanReadableName(bool shorter) {
	// Port of: final String name = context.getString(R.string.user_comments); (Java line 168)
	const QString name = QStringLiteral("User comments");

	// Port of: if(shorter) return name; (Java lines 170-171)
	if (shorter) {
		return name;

	// Port of: else return String.format("%s (%s)", name, user); (Java lines 172-173)
	} else {
		return QStringLiteral("%1 (%2)").arg(name, user);
	}
}

} // namespace PinkReader
