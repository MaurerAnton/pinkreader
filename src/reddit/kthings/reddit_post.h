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
 *   org.quantumbadger.redreader.reddit.kthings.RedditPost (Kotlin data class)
 *
 * Original annotations:
 *   @Suppress("PropertyName")
 *   @Serializable
 *   @Parcelize
 *   : RedditThingWithIdAndType, Parcelable
 *
 * Every field, method, inner class, init block, and constant from the original Kotlin file.
 */

#pragma once

#include "url_encoded_string.h"
#include "reddit_id_and_type.h"
#include "reddit_field_edited.h"
#include "reddit_timestamp_utc.h"
#include "image_metadata.h"
#include "reddit_media_metadata.h"
#include "maybe_parse_error.h"
#include <QString>
#include <QVector>
#include <QHash>
#include <optional>
#include <memory>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditPost
 *
 * Original: @Suppress("PropertyName"), @Serializable, @Parcelize
 * data class RedditPost(...) : RedditThingWithIdAndType, Parcelable
 */
class RedditPost {
public:
	// ============================================================================
	// Inner class: GalleryData
	// Port of: @Serializable data class GalleryData(val items: List<MaybeParseError<GalleryItem>>)
	// ============================================================================
	struct GalleryData {
		// Port of: val items: List<MaybeParseError<GalleryItem>>
		QVector<MaybeParseError<GalleryItem>> items;

		// ========================================================================
		// Inner class: GalleryItem
		// Port of: @Serializable @Parcelize data class GalleryItem(
		//     val media_id: UrlEncodedString,
		//     val caption: UrlEncodedString? = null,
		//     val outbound_url: UrlEncodedString? = null,
		// ) : Parcelable
		// ========================================================================
		struct GalleryItem {
			// Port of: val media_id: UrlEncodedString
			UrlEncodedString media_id;

			// Port of: val caption: UrlEncodedString? = null
			std::optional<UrlEncodedString> caption;

			// Port of: val outbound_url: UrlEncodedString? = null
			std::optional<UrlEncodedString> outbound_url;

			bool operator==(const GalleryItem &other) const {
				return media_id == other.media_id
					&& caption == other.caption
					&& outbound_url == other.outbound_url;
			}
			bool operator!=(const GalleryItem &other) const { return !(*this == other); }
		};

		bool operator==(const GalleryData &other) const {
			return items == other.items;
		}
		bool operator!=(const GalleryData &other) const { return !(*this == other); }
	};

	// ============================================================================
	// Inner class: Media
	// Port of: @Serializable @Parcelize data class Media(val reddit_video: RedditVideo? = null) : Parcelable
	// ============================================================================
	struct Media {
		// Port of: val reddit_video: RedditVideo? = null
		std::optional<RedditVideo> reddit_video;

		// ========================================================================
		// Inner class: RedditVideo
		// Port of: @Serializable @Parcelize data class RedditVideo(val fallback_url: UrlEncodedString? = null) : Parcelable
		// ========================================================================
		struct RedditVideo {
			// Port of: val fallback_url: UrlEncodedString? = null
			std::optional<UrlEncodedString> fallback_url;

			bool operator==(const RedditVideo &other) const {
				return fallback_url == other.fallback_url;
			}
			bool operator!=(const RedditVideo &other) const { return !(*this == other); }
		};

		bool operator==(const Media &other) const {
			return reddit_video == other.reddit_video;
		}
		bool operator!=(const Media &other) const { return !(*this == other); }
	};

	// ============================================================================
	// Inner class: Preview
	// Port of: @Serializable @Parcelize data class Preview(
	//     val enabled: Boolean,
	//     val images: List<Image>? = null,
	//     val reddit_video_preview: RedditVideoPreview? = null
	// ) : Parcelable
	// ============================================================================
	struct Preview {
		// Port of: val enabled: Boolean
		bool enabled = false;

		// Port of: val images: List<Image>? = null
		std::optional<QVector<Image>> images;

		// Port of: val reddit_video_preview: RedditVideoPreview? = null
		std::optional<RedditVideoPreview> reddit_video_preview;

		// ========================================================================
		// Inner class: ImageDetails
		// Port of: @Serializable @Parcelize data class ImageDetails(
		//     val url: UrlEncodedString, val width: Int, val height: Int) : Parcelable
		// ========================================================================
		struct ImageDetails {
			// Port of: val url: UrlEncodedString
			UrlEncodedString url;

			// Port of: val width: Int
			int width = 0;

			// Port of: val height: Int
			int height = 0;

			bool operator==(const ImageDetails &other) const {
				return url == other.url && width == other.width && height == other.height;
			}
			bool operator!=(const ImageDetails &other) const { return !(*this == other); }
		};

		// ========================================================================
		// Inner class: ImageBase
		// Port of: @Serializable sealed class ImageBase {
		//     abstract val source: ImageDetails?
		//     abstract val resolutions: List<ImageDetails>?
		// }
		// ========================================================================
		class ImageBase {
		public:
			virtual ~ImageBase() = default;

			// Port of: abstract val source: ImageDetails?
			virtual std::optional<ImageDetails> getSource() const = 0;

			// Port of: abstract val resolutions: List<ImageDetails>?
			virtual std::optional<QVector<ImageDetails>> getResolutions() const = 0;
		};

		// ========================================================================
		// Inner class: Image
		// Port of: @Serializable @Parcelize data class Image(
		//     override val source: ImageDetails? = null,
		//     override val resolutions: List<ImageDetails>? = null,
		//     val variants: ImageVariants
		// ) : ImageBase(), Parcelable
		// ========================================================================
		struct Image : public ImageBase {
			// Port of: override val source: ImageDetails? = null
			std::optional<ImageDetails> source;

			// Port of: override val resolutions: List<ImageDetails>? = null
			std::optional<QVector<ImageDetails>> resolutions;

			// Port of: val variants: ImageVariants
			ImageVariants variants;

			// Port of: data class constructor
			Image() = default;
			Image(
				std::optional<ImageDetails> source,
				std::optional<QVector<ImageDetails>> resolutions,
				ImageVariants variants
			) : source(std::move(source)), resolutions(std::move(resolutions)), variants(std::move(variants)) {}

			std::optional<ImageDetails> getSource() const override { return source; }
			std::optional<QVector<ImageDetails>> getResolutions() const override { return resolutions; }

			bool operator==(const Image &other) const {
				return source == other.source
					&& resolutions == other.resolutions
					&& variants == other.variants;
			}
			bool operator!=(const Image &other) const { return !(*this == other); }
		};

		// ========================================================================
		// Inner class: ImageVariants
		// Port of: @Serializable @Parcelize data class ImageVariants(
		//     val mp4: ImageVariant? = null) : Parcelable
		// ========================================================================
		struct ImageVariants {
			// Port of: val mp4: ImageVariant? = null
			std::optional<ImageVariant> mp4;

			bool operator==(const ImageVariants &other) const {
				return mp4 == other.mp4;
			}
			bool operator!=(const ImageVariants &other) const { return !(*this == other); }
		};

		// ========================================================================
		// Inner class: ImageVariant
		// Port of: @Serializable @Parcelize data class ImageVariant(
		//     override val source: ImageDetails? = null,
		//     override val resolutions: List<ImageDetails>? = null
		// ) : ImageBase(), Parcelable
		// ========================================================================
		struct ImageVariant : public ImageBase {
			// Port of: override val source: ImageDetails? = null
			std::optional<ImageDetails> source;

			// Port of: override val resolutions: List<ImageDetails>? = null
			std::optional<QVector<ImageDetails>> resolutions;

			// Port of: data class constructor
			ImageVariant() = default;
			ImageVariant(
				std::optional<ImageDetails> source,
				std::optional<QVector<ImageDetails>> resolutions
			) : source(std::move(source)), resolutions(std::move(resolutions)) {}

			std::optional<ImageDetails> getSource() const override { return source; }
			std::optional<QVector<ImageDetails>> getResolutions() const override { return resolutions; }

			bool operator==(const ImageVariant &other) const {
				return source == other.source && resolutions == other.resolutions;
			}
			bool operator!=(const ImageVariant &other) const { return !(*this == other); }
		};

		// ========================================================================
		// Inner class: RedditVideoPreview
		// Port of: @Serializable @Parcelize data class RedditVideoPreview(
		//     val fallback_url: UrlEncodedString? = null) : Parcelable
		// ========================================================================
		struct RedditVideoPreview {
			// Port of: val fallback_url: UrlEncodedString? = null
			std::optional<UrlEncodedString> fallback_url;

			bool operator==(const RedditVideoPreview &other) const {
				return fallback_url == other.fallback_url;
			}
			bool operator!=(const RedditVideoPreview &other) const { return !(*this == other); }
		};

		bool operator==(const Preview &other) const {
			return enabled == other.enabled && images == other.images
				&& reddit_video_preview == other.reddit_video_preview;
		}
		bool operator!=(const Preview &other) const { return !(*this == other); }
	};

	// ============================================================================
	// Fields (ported exactly from Kotlin data class constructor)
	// ============================================================================

	// Port of: val id: String
	QString id;

	// Port of: val name: RedditIdAndType
	RedditIdAndType name;

	// Port of: val url: UrlEncodedString? = null
	std::optional<UrlEncodedString> url;

	// Port of: val url_overridden_by_dest: UrlEncodedString? = null
	std::optional<UrlEncodedString> url_overridden_by_dest;

	// Port of: val title: UrlEncodedString? = null
	std::optional<UrlEncodedString> title;

	// Port of: val author: UrlEncodedString? = null
	std::optional<UrlEncodedString> author;

	// Port of: val domain: UrlEncodedString? = null
	std::optional<UrlEncodedString> domain;

	// Port of: val subreddit: UrlEncodedString
	UrlEncodedString subreddit;

	// Port of: val num_comments: Int
	int num_comments = 0;

	// Port of: val score: Int
	int score = 0;

	// Port of: val gilded: Int = 0
	int gilded = 0;

	// Port of: val crosspost_parent: String? = null
	std::optional<QString> crosspost_parent;

	// Port of: val upvote_ratio: Double? = null
	std::optional<double> upvote_ratio;

	// Port of: val archived: Boolean = false
	bool archived = false;

	// Port of: val over_18: Boolean = false
	bool over_18 = false;

	// Port of: val hidden: Boolean = false
	bool hidden = false;

	// Port of: val saved: Boolean = false
	bool saved = false;

	// Port of: val is_self: Boolean = false
	bool is_self = false;

	// Port of: val clicked: Boolean = false
	bool clicked = false;

	// Port of: val stickied: Boolean = false
	bool stickied = false;

	// Port of: val can_mod_post: Boolean = false
	bool can_mod_post = false;

	// Port of: val edited: RedditFieldEdited? = null
	std::shared_ptr<RedditFieldEdited> edited;

	// Port of: val likes: Boolean? = null
	std::optional<bool> likes;

	// Port of: val spoiler: Boolean = false
	bool spoiler = false;

	// Port of: val locked: Boolean = false
	bool locked = false;

	// Port of: val created_utc: RedditTimestampUTC
	RedditTimestampUTC created_utc;

	// Port of: val selftext: UrlEncodedString? = null
	std::optional<UrlEncodedString> selftext;

	// Port of: val selftext_html: UrlEncodedString? = null
	std::optional<UrlEncodedString> selftext_html;

	// Port of: val permalink: UrlEncodedString
	UrlEncodedString permalink;

	// Port of: val link_flair_text: UrlEncodedString? = null
	std::optional<UrlEncodedString> link_flair_text;

	// Port of: val author_flair_text: UrlEncodedString? = null
	std::optional<UrlEncodedString> author_flair_text;

	// Port of: val thumbnail: UrlEncodedString? = null // an image URL
	std::optional<UrlEncodedString> thumbnail;

	// Port of: val media: Media? = null
	std::optional<Media> media;

	// Port of: val preview: Preview? = null
	std::optional<Preview> preview;

	// Port of: val is_video: Boolean = false
	bool is_video = false;

	// Port of: val distinguished: String? = null
	std::optional<QString> distinguished;

	// Port of: val suggested_sort: String? = null // TODO enum type
	std::optional<QString> suggested_sort;

	// Port of: @kotlinx.parcelize.IgnoredOnParcel
	// val media_metadata: Map<UrlEncodedString, MaybeParseError<RedditMediaMetadata>>? = null
	// @IgnoredOnParcel means this field is not serialized via Parcel (Android IPC).
	// In C++, this is just a regular field.
	std::optional<QHash<UrlEncodedString, MaybeParseError<RedditMediaMetadata>>> media_metadata;

	// Port of: @kotlinx.parcelize.IgnoredOnParcel
	// val gallery_data: GalleryData? = null
	// @IgnoredOnParcel means this field is not serialized via Parcel (Android IPC).
	// In C++, this is just a regular field.
	std::optional<GalleryData> gallery_data;

	// Port of: val removed_by_category: String? = null
	std::optional<QString> removed_by_category;

	// ============================================================================
	// Constructor
	// ============================================================================

	RedditPost() = default;

	// ============================================================================
	// Init block
	// Port of: init {
	//     try { RedditGalleryAPI.addToCache(this) }
	//     catch (t: Throwable) { Log.e("RedditPost", "Got error when adding post to cache", t) }
	// }
	// NOTE: The init block is ported as a static factory method that calls
	// RedditGalleryAPI::addToCache after construction. In C++, this is done
	// externally since constructors should not have complex side effects.
	// ============================================================================

	// ============================================================================
	// Methods (ported from RedditThingWithIdAndType interface)
	// ============================================================================

	// Port of: override fun getIdAlone(): String { return id }
	QString getIdAlone() const { return id; }

	// Port of: override fun getIdAndType(): RedditIdAndType { return name }
	RedditIdAndType getIdAndType() const { return name; }

	// Port of: fun findUrl(): UriString?
	// NOTE: Depends on UriString. Commented out until UriString is ported.
	// Original:
	// fun findUrl(): UriString? {
	//     media?.reddit_video?.fallback_url?.decoded?.apply { return UriString(this) }
	//     if (url?.decoded?.contains(".gif") == true) {
	//         preview?.images?.get(0)?.variants?.mp4?.source?.url?.decoded?.apply { return UriString(this) }
	//     }
	//     url_overridden_by_dest?.decoded?.apply { return UriString(this) }
	//     return url?.decoded?.let(::UriString)
	// }
};

} // namespace PinkReader
