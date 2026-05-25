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
 *   org.quantumbadger.redreader.reddit.kthings.RedditMessage (Kotlin data class)
 *
 * Original annotations:
 *   @Suppress("PropertyName")
 *   @Serializable
 *   @Parcelize
 *   implements RedditThingWithIdAndType, Parcelable
 *
 * Every field, method, and constant from the original Kotlin file.
 */

#pragma once

#include "url_encoded_string.h"
#include "reddit_field_replies.h"
#include "reddit_id_and_type.h"
#include "reddit_timestamp_utc.h"
#include <QString>
#include <optional>
#include <memory>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditMessage
 *
 * Original: @Suppress("PropertyName"), @Serializable, @Parcelize
 * data class RedditMessage(
 *     val id: String,
 *     val name: RedditIdAndType,
 *     val author: UrlEncodedString? = null,
 *     val dest: UrlEncodedString? = null,
 *     val body: UrlEncodedString? = null,
 *     val body_html: UrlEncodedString? = null,
 *     val context: UrlEncodedString? = null,
 *     val subject: UrlEncodedString? = null,
 *     val subreddit_name_prefixed: UrlEncodedString? = null,
 *     val replies: RedditFieldReplies = RedditFieldReplies.None,
 *     val created_utc: RedditTimestampUTC
 * ) : Parcelable, RedditThingWithIdAndType
 */
class RedditMessage {
public:
	// Port of: val id: String
	QString id;

	// Port of: val name: RedditIdAndType
	RedditIdAndType name;

	// Port of: val author: UrlEncodedString? = null
	std::optional<UrlEncodedString> author;

	// Port of: val dest: UrlEncodedString? = null
	std::optional<UrlEncodedString> dest;

	// Port of: val body: UrlEncodedString? = null
	std::optional<UrlEncodedString> body;

	// Port of: val body_html: UrlEncodedString? = null
	std::optional<UrlEncodedString> body_html;

	// Port of: val context: UrlEncodedString? = null
	std::optional<UrlEncodedString> context;

	// Port of: val subject: UrlEncodedString? = null
	std::optional<UrlEncodedString> subject;

	// Port of: val subreddit_name_prefixed: UrlEncodedString? = null
	std::optional<UrlEncodedString> subreddit_name_prefixed;

	// Port of: val replies: RedditFieldReplies = RedditFieldReplies.None
	std::shared_ptr<RedditFieldReplies> replies;

	// Port of: val created_utc: RedditTimestampUTC
	RedditTimestampUTC created_utc;

	// ============================================================================
	// Constructor
	// ============================================================================

	RedditMessage()
		: replies(std::make_shared<RedditFieldReplies::None>(
			RedditFieldReplies::None::instance()))
	{}

	// ============================================================================
	// Methods (ported from RedditThingWithIdAndType interface)
	// ============================================================================

	// Port of: override fun getIdAlone() = id
	QString getIdAlone() const { return id; }

	// Port of: override fun getIdAndType() = name
	RedditIdAndType getIdAndType() const { return name; }

	bool operator==(const RedditMessage &other) const {
		return id == other.id
			&& name == other.name
			&& author == other.author
			&& dest == other.dest
			&& body == other.body
			&& body_html == other.body_html
			&& context == other.context
			&& subject == other.subject
			&& subreddit_name_prefixed == other.subreddit_name_prefixed
			&& created_utc == other.created_utc;
		// replies comparison omitted (polymorphic shared_ptr)
	}
	bool operator!=(const RedditMessage &other) const {
		return !(*this == other);
	}
};

} // namespace PinkReader
