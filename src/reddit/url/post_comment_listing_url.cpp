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
 *   org.quantumbadger.redreader.reddit.url.PostCommentListingURL (lines 1-269)
 */

#include "post_comment_listing_url.h"
#include "../../core/constants.h"

// Placeholder for PostCommentSort
namespace PinkReader {

class PostCommentSort {
public:
	QString key;
	explicit PostCommentSort(const QString &k) : key(k) {}

	static PostCommentSort *lookup(const QString &key) {
		if (key.isEmpty()) return nullptr;
		return new PostCommentSort(key.toLower());
	}
};

} // namespace PinkReader

namespace PinkReader {

// Port of: public static PostCommentListingURL forPostId(final String postId) (Java lines 45-47)
PostCommentListingURL *PostCommentListingURL::forPostId(const QString &postId) {
	return new PostCommentListingURL(QString(), postId, QString(), std::nullopt, std::nullopt, nullptr, false);
}

// Port of: public PostCommentListingURL(String, String, String, Integer, Integer, PostCommentSort, boolean) (Java lines 49-73)
PostCommentListingURL::PostCommentListingURL(const QString &after, QString postId, QString commentId,
		std::optional<int> context, std::optional<int> limit,
		PostCommentSort *order, bool video)
	: after_str(after)
	// Port of: if(postId != null && postId.startsWith("t3_")) postId = postId.substring(3); (Java lines 58-60)
	, postId(postId.startsWith(QStringLiteral("t3_")) ? postId.mid(3) : postId)
	// Port of: if(commentId != null && commentId.startsWith("t1_")) commentId = commentId.substring(3); (Java lines 62-64)
	, commentId(commentId.startsWith(QStringLiteral("t1_")) ? commentId.mid(3) : commentId)
	, context(context)
	, limit(limit)
	, order(order)
	, video(video) {}

// Port of: @Override public PostCommentListingURL after(final String after) (Java lines 76-78)
CommentListingURL *PostCommentListingURL::after(const QString &newAfter) {
	return new PostCommentListingURL(newAfter, postId, commentId, context, limit, order, video);
}

// Port of: @Override public PostCommentListingURL limit(final Integer limit) (Java lines 80-83)
CommentListingURL *PostCommentListingURL::limit(std::optional<int> newLimit) {
	return new PostCommentListingURL(after_str, postId, commentId, context, newLimit, order, video);
}

// Port of: public PostCommentListingURL context(final Integer context) (Java lines 85-87)
PostCommentListingURL *PostCommentListingURL::withContext(std::optional<int> newContext) {
	return new PostCommentListingURL(after_str, postId, commentId, newContext, limit, order, video);
}

// Port of: public PostCommentListingURL order(final PostCommentSort order) (Java lines 89-91)
PostCommentListingURL *PostCommentListingURL::withOrder(PostCommentSort *newOrder) {
	return new PostCommentListingURL(after_str, postId, commentId, context, limit, newOrder, video);
}

// Port of: public PostCommentListingURL commentId(String commentId) (Java lines 93-100)
PostCommentListingURL *PostCommentListingURL::withCommentId(QString newCommentId) {
	// Port of: if(commentId != null && commentId.startsWith("t1_")) commentId = commentId.substring(3); (Java lines 95-97)
	if (newCommentId.startsWith(QStringLiteral("t1_"))) {
		newCommentId = newCommentId.mid(3);
	}
	return new PostCommentListingURL(after_str, postId, newCommentId, context, limit, order, video);
}

// Port of: @Override public Uri generateJsonUri() (Java lines 103-114)
PinkUri PostCommentListingURL::generateJsonUri() {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	internalGenerateCommon(builder);

	builder.appendEncodedPath(QStringLiteral(".json"));

	return builder.build();
}

// Port of: public Uri generateNonJsonUri() (Java lines 116-123)
PinkUri PostCommentListingURL::generateNonJsonUri() {
	PinkUri::Builder builder;
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getHumanReadableDomain());

	internalGenerateCommon(builder);

	return builder.build();
}

// Port of: private void internalGenerateCommon(@NonNull final Uri.Builder builder) (Java lines 125-155)
void PostCommentListingURL::internalGenerateCommon(PinkUri::Builder &builder) {
	// Port of: if(video) builder.encodedPath("/video"); else builder.encodedPath("/comments"); (Java lines 127-131)
	if (video) {
		builder.encodedPath(QStringLiteral("/video"));
	} else {
		builder.encodedPath(QStringLiteral("/comments"));
	}

	// Port of: builder.appendPath(postId); (Java line 132)
	builder.appendPath(postId);

	// Port of: if(commentId != null) (Java lines 134-142)
	if (!commentId.isEmpty()) {
		builder.appendEncodedPath(QStringLiteral("comment"));
		builder.appendPath(commentId);

		// Port of: if(context != null) builder.appendQueryParameter("context", context.toString()); (Java lines 139-141)
		if (context.has_value()) {
			builder.appendQueryParameter(QStringLiteral("context"), QString::number(context.value()));
		}
	}

	// Port of: if(after != null) builder.appendQueryParameter("after", after); (Java lines 144-146)
	if (!after_str.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("after"), after_str);
	}

	// Port of: if(limit != null) builder.appendQueryParameter("limit", limit.toString()); (Java lines 148-150)
	if (limit.has_value()) {
		builder.appendQueryParameter(QStringLiteral("limit"), QString::number(limit.value()));
	}

	// Port of: if(order != null) builder.appendQueryParameter("sort", order.key); (Java lines 152-154)
	if (order != nullptr) {
		builder.appendQueryParameter(QStringLiteral("sort"), order->key);
	}
}

// Port of: public static PostCommentListingURL parse(final Uri uri) (Java lines 157-256)
PostCommentListingURL *PostCommentListingURL::parse(const PinkUri &uri) {
	// Port of: final String[] pathSegments; (Java lines 159-177)
	QStringList pathSegmentsList = uri.getPathSegments();

	QStringList pathSegmentsFiltered;
	pathSegmentsFiltered.reserve(pathSegmentsList.size());
	for (QString segment : pathSegmentsList) {
		while (segment.toLower().endsWith(QStringLiteral(".json"))
				|| segment.toLower().endsWith(QStringLiteral(".xml"))) {
			segment = segment.left(segment.lastIndexOf('.'));
		}
		pathSegmentsFiltered.append(segment);
	}

	// Port of: if(pathSegments.length == 1) (Java lines 179-193)
	if (pathSegmentsFiltered.size() == 1) {
		// Port of: if(uri.getHost().equals("redd.it")) (Java line 180)
		if (uri.getHost() == QStringLiteral("redd.it")) {
			return forPostId(pathSegmentsFiltered[0]);
		}
		// Port of: if(uri.getHost().equals("v.redd.it")) (Java lines 183-192)
		if (uri.getHost() == QStringLiteral("v.redd.it")) {
			return new PostCommentListingURL(QString(), pathSegmentsFiltered[0], QString(),
					std::nullopt, std::nullopt, nullptr, true);
		}
	}

	// Port of: if(pathSegments.length < 2) return null; (Java lines 195-197)
	if (pathSegmentsFiltered.size() < 2) {
		return nullptr;
	}

	// Port of: int offset = 0; (Java line 199)
	int offset = 0;

	// Port of: if(pathSegments[0].equalsIgnoreCase("r")) (Java lines 201-207)
	if (pathSegmentsFiltered[0].compare(QStringLiteral("r"), Qt::CaseInsensitive) == 0) {
		offset = 2;
		if (pathSegmentsFiltered.size() - offset < 2) {
			return nullptr;
		}
	}

	// Port of: boolean video = false; (Java line 209)
	bool video = false;

	// Port of: if(pathSegments[offset].equalsIgnoreCase("video")) { video = true; }
	//        else if(!pathSegments[offset].equalsIgnoreCase("comments") && !pathSegments[offset].equalsIgnoreCase("gallery")) (Java lines 211-216)
	if (pathSegmentsFiltered[offset].compare(QStringLiteral("video"), Qt::CaseInsensitive) == 0) {
		video = true;
	} else if (!(pathSegmentsFiltered[offset].compare(QStringLiteral("comments"), Qt::CaseInsensitive) == 0)
			&& !(pathSegmentsFiltered[offset].compare(QStringLiteral("gallery"), Qt::CaseInsensitive) == 0)) {
		return nullptr;
	}

	// Port of: final String postId; String commentId = null; (Java lines 218-219)
	QString postId;
	QString commentId;

	// Port of: postId = pathSegments[offset + 1]; offset += 2; (Java lines 221-222)
	postId = pathSegmentsFiltered[offset + 1];
	offset += 2;

	// Port of: if(pathSegments.length - offset >= 2) commentId = pathSegments[offset + 1]; (Java lines 224-226)
	if (pathSegmentsFiltered.size() - offset >= 2) {
		commentId = pathSegmentsFiltered[offset + 1];
	}

	QString after;
	std::optional<int> limit;
	std::optional<int> context;
	PostCommentSort *order = nullptr;

	// Port of: for(final String parameterKey : General.getUriQueryParameterNames(uri)) (Java lines 233-253)
	QStringList paramNames = uri.getQueryParameterNames();
	for (const QString &parameterKey : paramNames) {
		if (parameterKey.compare(QStringLiteral("after"), Qt::CaseInsensitive) == 0) {
			after = uri.getQueryParameter(parameterKey);
		} else if (parameterKey.compare(QStringLiteral("limit"), Qt::CaseInsensitive) == 0) {
			bool ok = false;
			int val = uri.getQueryParameter(parameterKey).toInt(&ok);
			if (ok) limit = val;
		} else if (parameterKey.compare(QStringLiteral("context"), Qt::CaseInsensitive) == 0) {
			bool ok = false;
			int val = uri.getQueryParameter(parameterKey).toInt(&ok);
			if (ok) context = val;
		} else if (parameterKey.compare(QStringLiteral("sort"), Qt::CaseInsensitive) == 0) {
			order = PostCommentSort::lookup(uri.getQueryParameter(parameterKey));
		}
	}

	// Port of: return new PostCommentListingURL(after, postId, commentId, context, limit, order, video); (Java line 255)
	return new PostCommentListingURL(after, postId, commentId, context, limit, order, video);
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 258-262)
int PostCommentListingURL::pathType() {
	return RedditURLParser::POST_COMMENT_LISTING_URL;
}

// Port of: @Override public String humanReadableName(Context, boolean) (Java lines 264-267)
QString PostCommentListingURL::humanReadableName(bool shorter) {
	return RedditURLParser::RedditURL::humanReadableName(shorter);
}

} // namespace PinkReader
