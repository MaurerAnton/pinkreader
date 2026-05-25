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
 *   org.quantumbadger.redreader.reddit.kthings.RedditMore (Kotlin data class)
 *
 * Original annotations:
 *   @Suppress("PropertyName")
 *   @Serializable
 *   @Parcelize
 *
 * Every field, method, and constant from the original Kotlin file.
 */

#pragma once

#include <QString>
#include <QVector>

namespace PinkReader {

// Forward declarations for dependencies
// (originally: import org.quantumbadger.redreader.reddit.url.PostCommentListingURL,
//             import org.quantumbadger.redreader.reddit.url.RedditURLParser,
//             import org.quantumbadger.redreader.reddit.url.RedditURLParser.RedditURL)

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditMore
 *
 * Original: @Suppress("PropertyName"), @Serializable, @Parcelize
 * data class RedditMore(
 *     var count: Int,
 *     var children: List<String>,
 *     var parent_id: String
 * ) : Parcelable
 */
class RedditMore {
public:
	// Port of: var count: Int
	int count = 0;

	// Port of: var children: List<String>
	QVector<QString> children;

	// Port of: var parent_id: String
	QString parent_id;

	// Port of: data class constructor
	RedditMore() = default;
	RedditMore(int count, QVector<QString> children, QString parent_id)
		: count(count), children(std::move(children)), parent_id(std::move(parent_id)) {}

	// Port of: fun getMoreUrls(commentListingURL: RedditURL): List<PostCommentListingURL>
	// NOTE: This method depends on RedditURLParser and PostCommentListingURL
	// which need to be ported separately. The method signature and body are
	// commented below until those types are available.
	//
	// Original:
	// fun getMoreUrls(commentListingURL: RedditURL): List<PostCommentListingURL> {
	//     val urls = ArrayList<PostCommentListingURL>(16)
	//     if (commentListingURL.pathType() == RedditURLParser.POST_COMMENT_LISTING_URL) {
	//         if (count > 0) {
	//             for (child in children) {
	//                 urls.add(commentListingURL.asPostCommentListURL().commentId(child))
	//             }
	//         } else {
	//             urls.add(commentListingURL.asPostCommentListURL().commentId(parent_id))
	//         }
	//     }
	//     return urls
	// }

	bool operator==(const RedditMore &other) const {
		return count == other.count
			&& children == other.children
			&& parent_id == other.parent_id;
	}
	bool operator!=(const RedditMore &other) const {
		return !(*this == other);
	}
};

} // namespace PinkReader
