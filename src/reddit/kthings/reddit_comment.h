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
 *   org.quantumbadger.redreader.reddit.kthings.RedditComment (Kotlin data class)
 *
 * Original annotations:
 *   @Suppress("PropertyName")
 *   @Serializable
 *   @Parcelize
 *   implements RedditThingWithIdAndType, Parcelable
 *
 * Every field, method, inner class, and constant from the original Kotlin file.
 *
 * TODO add copyright messages to all Kotlin files
 */

#pragma once

#include "url_encoded_string.h"
#include "reddit_field_replies.h"
#include "reddit_id_and_type.h"
#include "reddit_field_edited.h"
#include "reddit_timestamp_utc.h"
#include "image_metadata.h"
#include "reddit_media_metadata.h"
#include "maybe_parse_error.h"
#include <QString>
#include <QVector>
#include <QHash>
#include <QSet>
#include <optional>
#include <memory>

namespace PinkReader {

// Forward declaration for UriString (from common/)
// Port of: import org.quantumbadger.redreader.common.UriString
// Port of: import org.quantumbadger.redreader.common.LinkHandler
// Port of: import org.quantumbadger.redreader.reddit.url.PostCommentListingURL
// Port of: import org.quantumbadger.redreader.reddit.things.RedditThingWithIdAndType

class RedditThing; // forward declared for RedditFieldReplies

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditComment
 *
 * Original: @Suppress("PropertyName"), @Serializable, @Parcelize
 * data class RedditComment(...) : Parcelable, RedditThingWithIdAndType
 */
class RedditComment {
public:
	// ============================================================================
	// Inner classes (ported directly from Kotlin)
	// ============================================================================

	// Port of: @Serializable @Parcelize data class EmoteMetadata(val status: String, val e: String, val m: String, val s: ImageMetadata, val t: String, val id: String) : Parcelable
	struct EmoteMetadata {
		QString status;
		QString e;
		QString m;
		ImageMetadata s;
		QString t;
		QString id;

		bool operator==(const EmoteMetadata &other) const {
			return status == other.status && e == other.e && m == other.m
				&& s == other.s && t == other.t && id == other.id;
		}
		bool operator!=(const EmoteMetadata &other) const { return !(*this == other); }
	};

	// Port of: @Serializable @Parcelize data class ImageMetadata(val x: String, val y: String, val u: String? = null) : Parcelable
	// NOTE: This is a nested ImageMetadata distinct from the top-level ImageMetadata in image_metadata.h
	struct CommentImageMetadata {
		QString x;
		QString y;
		std::optional<QString> u;

		bool operator==(const CommentImageMetadata &other) const {
			return x == other.x && y == other.y && u == other.u;
		}
		bool operator!=(const CommentImageMetadata &other) const { return !(*this == other); }
	};

	// Port of: @Serializable @Parcelize data class FlairEmoteData(val e: String, val a: String, val u: String) : Parcelable
	struct FlairEmoteData {
		QString e;
		QString a;
		QString u;

		bool operator==(const FlairEmoteData &other) const {
			return e == other.e && a == other.a && u == other.u;
		}
		bool operator!=(const FlairEmoteData &other) const { return !(*this == other); }
	};

	// ============================================================================
	// Fields (ported exactly from Kotlin data class constructor)
	// ============================================================================

	// Port of: val body: UrlEncodedString? = null
	std::optional<UrlEncodedString> body;

	// Port of: val body_html: UrlEncodedString? = null
	std::optional<UrlEncodedString> body_html;

	// Port of: val COLLAPSED_REASON_BLOCKED_AUTHOR: String = "BLOCKED_AUTHOR"
	static const QString COLLAPSED_REASON_BLOCKED_AUTHOR;

	// Port of: val author: UrlEncodedString? = null
	std::optional<UrlEncodedString> author;

	// Port of: val subreddit: UrlEncodedString? = null
	std::optional<UrlEncodedString> subreddit;

	// Port of: val author_flair_text: UrlEncodedString? = null
	std::optional<UrlEncodedString> author_flair_text;

	// Port of: val author_flair_richtext: List<MaybeParseError<FlairEmoteData>>? = null
	std::optional<QVector<MaybeParseError<FlairEmoteData>>> author_flair_richtext;

	// Port of: val archived: Boolean = false
	bool archived = false;

	// Port of: val likes: Boolean? = null
	std::optional<bool> likes;

	// Port of: val score_hidden: Boolean = false
	bool score_hidden = false;

	// Port of: val locked: Boolean = false
	bool locked = false;

	// Port of: val can_mod_post: Boolean = false
	bool can_mod_post = false;

	// Port of: val media_metadata: Map<UrlEncodedString, MaybeParseError<RedditMediaMetadata>>? = null
	std::optional<QHash<UrlEncodedString, MaybeParseError<RedditMediaMetadata>>> media_metadata;

	// Port of: val replies: RedditFieldReplies = RedditFieldReplies.None
	// In C++, we store as shared_ptr to enable polymorphism with the sealed class.
	std::shared_ptr<RedditFieldReplies> replies;

	// Port of: val id: String
	QString id;

	// Port of: val subreddit_id: String? = null
	std::optional<QString> subreddit_id;

	// Port of: val link_id: String? = null
	std::optional<QString> link_id;

	// Port of: val parent_id: String? = null
	std::optional<QString> parent_id;

	// Port of: val name: RedditIdAndType
	RedditIdAndType name;

	// Port of: val context: UrlEncodedString? = null
	std::optional<UrlEncodedString> context;

	// Port of: val ups: Int = 0
	int ups = 0;

	// Port of: val downs: Int = 0
	int downs = 0;

	// Port of: val gilded: Int = 0
	int gilded = 0;

	// Port of: val controversiality: Int = 0
	int controversiality = 0;

	// Port of: val edited: RedditFieldEdited = RedditFieldEdited.Bool(false) // TODO do same in other one
	std::shared_ptr<RedditFieldEdited> edited;

	// Port of: val created_utc: RedditTimestampUTC
	RedditTimestampUTC created_utc;

	// Port of: val saved: Boolean = false
	bool saved = false;

	// Port of: val distinguished: String? = null // TODO enum? Test unknown values
	std::optional<QString> distinguished;

	// Port of: val stickied: Boolean = false
	bool stickied = false;

	// Port of: val collapsed_reason_code: String? = null
	std::optional<QString> collapsed_reason_code;

	// ============================================================================
	// Constructor (ported from data class)
	// ============================================================================

	RedditComment()
		: replies(std::make_shared<RedditFieldReplies::None>(
			RedditFieldReplies::None::instance()))
		, edited(std::make_shared<RedditFieldEdited::Bool>(false))
	{}

	// ============================================================================
	// Methods (ported exactly from Kotlin)
	// ============================================================================

	// Port of: fun copyWithNewBodyHtml(value: String) = copy(body_html = UrlEncodedString(value))
	// TODO do this in the HTML parser instead
	RedditComment copyWithNewBodyHtml(const QString &value) const {
		RedditComment result = *this;
		result.body_html = UrlEncodedString(value);
		return result;
	}

	// Port of: override fun getIdAlone() = id
	QString getIdAlone() const { return id; }

	// Port of: override fun getIdAndType() = name
	RedditIdAndType getIdAndType() const { return name; }

	// Port of: fun getContextUrl(): PostCommentListingURL
	// NOTE: Depends on PostCommentListingURL which needs to be ported.
	// Original logic:
	// fun getContextUrl(): PostCommentListingURL {
	//     return context?.run {
	//         var result = decoded
	//         if (result.startsWith("r/")) { result = "/$result" }
	//         if (result.startsWith("/")) { result = "https://reddit.com$result" }
	//         PostCommentListingURL.parse(result.toUri())
	//     } ?: PostCommentListingURL(null, link_id, idAlone, 3, null, null, false)
	// }
	// This method is commented out until PostCommentListingURL is ported.

	// Port of: fun computeAllLinks(): Set<UriString>
	// Port of: return body_html?.decoded?.run { LinkHandler.computeAllLinks(this) } ?: emptySet()
	// NOTE: Depends on UriString and LinkHandler. Commented out until those types are ported.
	// Original: QSet<QString> computeAllLinks() const { ... }

	// Port of: fun computeAllLinksString(): List<String>
	// Port of: return computeAllLinks().map { it.value }
	// NOTE: Depends on computeAllLinks(). Commented out until that method is ported.

	// Port of: fun wasEdited(): Boolean = edited != RedditFieldEdited.Bool(false)
	bool wasEdited() const {
		if (auto *b = dynamic_cast<const RedditFieldEdited::Bool *>(edited.get())) {
			return b->value != false;
		}
		return true; // Timestamp is always edited
	}

	// Port of: fun isControversial(): Boolean = controversiality == 1
	bool isControversial() const { return controversiality == 1; }

	// Port of: fun isBlockedByUser(): Boolean
	// Port of: return COLLAPSED_REASON_BLOCKED_AUTHOR == collapsed_reason_code
	bool isBlockedByUser() const {
		return collapsed_reason_code.has_value()
			&& collapsed_reason_code.value() == COLLAPSED_REASON_BLOCKED_AUTHOR;
	}
};

} // namespace PinkReader
