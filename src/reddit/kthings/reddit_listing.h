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
 *   org.quantumbadger.redreader.reddit.kthings.RedditListing (Kotlin data class)
 *
 * Original annotations:
 *   @Serializable
 *   @Parcelize
 *
 * Every field, method, and constant from the original Kotlin file.
 */

#pragma once

#include "maybe_parse_error.h"
#include <QString>
#include <QVector>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declaration (RedditThing is defined in reddit_thing.h)
class RedditThing;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditListing
 *
 * Original: @Serializable, @Parcelize
 * data class RedditListing(
 *     val after: String? = null,
 *     val children: ArrayList<MaybeParseError<RedditThing>>
 * ) : Parcelable
 *
 * NOTE: Uses shared_ptr<RedditThing> because RedditThing is a polymorphic sealed class.
 */
class RedditListing {
public:
	// Port of: val after: String? = null
	std::optional<QString> after;

	// Port of: val children: ArrayList<MaybeParseError<RedditThing>>
	// In C++, RedditThing is polymorphic, so we use shared_ptr.
	QVector<MaybeParseError<std::shared_ptr<RedditThing>>> children;

	// Port of: data class constructor
	RedditListing() = default;
	RedditListing(
		std::optional<QString> after,
		QVector<MaybeParseError<std::shared_ptr<RedditThing>>> children
	) : after(std::move(after)), children(std::move(children)) {}

	// Equality comparison (partial - children comparison requires deep compare)
	bool operator==(const RedditListing &other) const {
		return after == other.after;
		// children comparison omitted - would require deep comparison of shared_ptr contents
	}
	bool operator!=(const RedditListing &other) const {
		return !(*this == other);
	}
};

} // namespace PinkReader
