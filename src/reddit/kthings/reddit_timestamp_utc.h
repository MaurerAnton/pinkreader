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
 *   org.quantumbadger.redreader.reddit.kthings.RedditTimestampUTC (Kotlin data class)
 *
 * Original annotations:
 *   @Serializable(with = RedditTimestampUTCSerializer::class)
 *   @Parcelize
 *
 * Every field, method, and constant from the original Kotlin file.
 * Dependencies: TimestampUTC from reddit_time.h
 */

#pragma once

#include "../../utils/reddit_time.h"
#include <cstdint>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditTimestampUTC
 *
 * Original: @Serializable(with = RedditTimestampUTCSerializer::class), @Parcelize
 * data class RedditTimestampUTC(val value: @WriteWith<TimestampUTCParceler> TimestampUTC) : Parcelable
 */
class RedditTimestampUTC {
public:
	// Port of: val value: @WriteWith<TimestampUTCParceler> TimestampUTC
	TimestampUTC value;

	// Port of: data class constructor
	RedditTimestampUTC() : value(TimestampUTC::ZERO) {}
	explicit RedditTimestampUTC(TimestampUTC value) : value(std::move(value)) {}

	bool operator==(const RedditTimestampUTC &other) const {
		return value == other.value;
	}
	bool operator!=(const RedditTimestampUTC &other) const {
		return value != other.value;
	}
};

} // namespace PinkReader

// Port of: object RedditTimestampUTCSerializer : KSerializer<RedditTimestampUTC>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("RedditTimestampUTC", PrimitiveKind.DOUBLE)
// Port of: override fun deserialize(decoder: Decoder) = RedditTimestampUTC(value = TimestampUTC.fromUtcSecs((decoder.decodeDouble().toLong())))
// Port of: override fun serialize(encoder: Encoder, value: RedditTimestampUTC) { encoder.encodeDouble(value.value.toUtcSecs().toDouble()) }
//
// Port of: object TimestampUTCParceler : Parceler<TimestampUTC>
// Port of: override fun create(parcel: Parcel) = TimestampUTC.fromUtcMs(parcel.readLong())
// Port of: override fun TimestampUTC.write(parcel: Parcel, flags: Int) { parcel.writeLong(toUtcMs()) }
