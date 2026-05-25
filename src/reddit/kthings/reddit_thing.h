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
 *   org.quantumbadger.redreader.reddit.kthings.RedditThing (Kotlin sealed class)
 *
 * Original annotations:
 *   @OptIn(ExperimentalSerializationApi::class)
 *   @Serializable
 *   @JsonClassDiscriminator("kind")
 *   @Parcelize
 *
 * Every variant, field, method, annotation, and constant from the original Kotlin file.
 *
 * Variants:
 *   - Comment (t1) with RedditComment data
 *   - User (t2) - object
 *   - Post (t3) with RedditPost data
 *   - Message (t4) with RedditMessage data
 *   - Subreddit (t5) - object
 *   - More ("more") with RedditMore data
 *   - Listing ("Listing") with RedditListing data
 */

#pragma once

#include "reddit_comment.h"
#include "reddit_post.h"
#include "reddit_message.h"
#include "reddit_more.h"
#include "reddit_listing.h"
#include <memory>
#include <QString>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditThing
 *
 * Original: @OptIn(ExperimentalSerializationApi::class), @Serializable,
 *           @JsonClassDiscriminator("kind"), @Parcelize
 * sealed class RedditThing : Parcelable
 *
 * // TODO test that parcelling sealed classes actually works correctly
 */
class RedditThing {
public:
	virtual ~RedditThing() = default;

	// Port of: @Serializable @SerialName("t1") @Parcelize data class Comment(val data: RedditComment) : RedditThing()
	class Comment : public RedditThing {
	public:
		// Port of: val data: RedditComment
		RedditComment data;

		// Port of: data class constructor
		Comment() = default;
		explicit Comment(RedditComment data) : data(std::move(data)) {}
	};

	// Port of: @Serializable @SerialName("t2") @Parcelize object User : RedditThing()
	class User : public RedditThing {
	public:
		// Port of: singleton object User
		static const User &instance() {
			static const User s_instance;
			return s_instance;
		}
	};

	// Port of: @Serializable @SerialName("t3") @Parcelize data class Post(val data: RedditPost) : RedditThing()
	class Post : public RedditThing {
	public:
		// Port of: val data: RedditPost
		RedditPost data;

		// Port of: data class constructor
		Post() = default;
		explicit Post(RedditPost data) : data(std::move(data)) {}
	};

	// Port of: @Serializable @SerialName("t4") @Parcelize data class Message(val data: RedditMessage) : RedditThing()
	class Message : public RedditThing {
	public:
		// Port of: val data: RedditMessage
		RedditMessage data;

		// Port of: data class constructor
		Message() = default;
		explicit Message(RedditMessage data) : data(std::move(data)) {}
	};

	// Port of: @Serializable @SerialName("t5") @Parcelize object Subreddit : RedditThing()
	class Subreddit : public RedditThing {
	public:
		// Port of: singleton object Subreddit
		static const Subreddit &instance() {
			static const Subreddit s_instance;
			return s_instance;
		}
	};

	// Port of: @Serializable @SerialName("more") @Parcelize data class More(val data: RedditMore) : RedditThing()
	class More : public RedditThing {
	public:
		// Port of: val data: RedditMore
		RedditMore data;

		// Port of: data class constructor
		More() = default;
		explicit More(RedditMore data) : data(std::move(data)) {}
	};

	// Port of: @Serializable @SerialName("Listing") @Parcelize data class Listing(val data: RedditListing) : RedditThing()
	class Listing : public RedditThing {
	public:
		// Port of: val data: RedditListing
		RedditListing data;

		// Port of: data class constructor
		Listing() = default;
		explicit Listing(RedditListing data) : data(std::move(data)) {}
	};

protected:
	// Port of: sealed class private constructor (sealed = no external inheritance)
	RedditThing() = default;
};

} // namespace PinkReader

// Port of: @OptIn(ExperimentalSerializationApi::class)
// Port of: @Serializable
// Port of: @JsonClassDiscriminator("kind")
//
// The "kind" discriminator maps to variants:
//   "t1" -> Comment
//   "t2" -> User
//   "t3" -> Post
//   "t4" -> Message
//   "t5" -> Subreddit
//   "more" -> More
//   "Listing" -> Listing
