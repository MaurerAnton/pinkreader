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
 *   org.quantumbadger.redreader.reddit.kthings.ImageMetadata (Kotlin data class)
 *
 * Original annotations:
 *   @Serializable
 *   @Parcelize
 *
 * Every field, method, and constant from the original Kotlin file.
 */

#pragma once

#include "url_encoded_string.h"
#include <cstdint>
#include <optional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.ImageMetadata
 *
 * Original: @Serializable, @Parcelize
 * data class ImageMetadata(
 *     val x: Long,
 *     val y: Long,
 *     val u: UrlEncodedString? = null,
 *     val mp4: UrlEncodedString? = null,
 *     val gif: UrlEncodedString? = null,
 * ) : Parcelable
 */
struct ImageMetadata {
	// Port of: val x: Long
	int64_t x = 0;

	// Port of: val y: Long
	int64_t y = 0;

	// Port of: val u: UrlEncodedString? = null
	std::optional<UrlEncodedString> u;

	// Port of: val mp4: UrlEncodedString? = null
	std::optional<UrlEncodedString> mp4;

	// Port of: val gif: UrlEncodedString? = null
	std::optional<UrlEncodedString> gif;

	bool operator==(const ImageMetadata &other) const {
		return x == other.x
			&& y == other.y
			&& u == other.u
			&& mp4 == other.mp4
			&& gif == other.gif;
	}
	bool operator!=(const ImageMetadata &other) const {
		return !(*this == other);
	}
};

} // namespace PinkReader
