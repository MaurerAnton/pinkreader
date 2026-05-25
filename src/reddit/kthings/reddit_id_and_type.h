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
 *   org.quantumbadger.redreader.reddit.kthings.RedditIdAndType (Kotlin data class)
 *
 * Original annotations:
 *   @Serializable(with = RedditIdAndTypeSerializer::class)
 *   @Parcelize
 *
 * Every field, method, and constant from the original Kotlin file.
 */

#pragma once

#include <QString>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditIdAndType
 *
 * Original: @Serializable(with = RedditIdAndTypeSerializer::class), @Parcelize
 * data class RedditIdAndType(val value: String) : Parcelable
 */
class RedditIdAndType {
public:
	// Port of: val value: String
	QString value;

	// Port of: data class constructor
	RedditIdAndType() = default;
	explicit RedditIdAndType(QString value) : value(std::move(value)) {}

	// Port of: override fun toString() = value
	QString toString() const { return value; }

	explicit operator QString() const { return value; }

	bool operator==(const RedditIdAndType &other) const {
		return value == other.value;
	}
	bool operator!=(const RedditIdAndType &other) const {
		return value != other.value;
	}
};

} // namespace PinkReader

// Port of: object RedditIdAndTypeSerializer : KSerializer<RedditIdAndType>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("RedditIdAndType", PrimitiveKind.STRING)
// Port of: override fun deserialize(decoder: Decoder) = RedditIdAndType(decoder.decodeString())
// Port of: override fun serialize(encoder: Encoder, value: RedditIdAndType) { encoder.encodeString(value.value) }
