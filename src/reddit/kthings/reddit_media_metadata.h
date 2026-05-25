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
 *   org.quantumbadger.redreader.reddit.kthings.RedditMediaMetadata (Kotlin data class)
 *
 * Original annotations:
 *   @Serializable
 *   @Parcelize
 *
 * Every field, method, and constant from the original Kotlin file.
 */

#pragma once

#include "image_metadata.h"
#include <QString>
#include <QVector>
#include <optional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditMediaMetadata
 *
 * Original: @Serializable, @Parcelize
 * data class RedditMediaMetadata(
 *     val status: String,
 *     val e: String,
 *     val m: String,
 *     val s: ImageMetadata,
 *     val id: String,
 *     val p: List<ImageMetadata>? = null
 * ) : Parcelable
 */
struct RedditMediaMetadata {
	// Port of: val status: String
	QString status;

	// Port of: val e: String
	QString e;

	// Port of: val m: String
	QString m;

	// Port of: val s: ImageMetadata
	ImageMetadata s;

	// Port of: val id: String
	QString id;

	// Port of: val p: List<ImageMetadata>? = null
	std::optional<QVector<ImageMetadata>> p;

	bool operator==(const RedditMediaMetadata &other) const {
		return status == other.status
			&& e == other.e
			&& m == other.m
			&& s == other.s
			&& id == other.id
			&& p == other.p;
	}
	bool operator!=(const RedditMediaMetadata &other) const {
		return !(*this == other);
	}
};

} // namespace PinkReader
