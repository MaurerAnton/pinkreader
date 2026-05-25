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
 *   org.quantumbadger.redreader.reddit.kthings.RedditFieldEdited (Kotlin sealed class)
 *
 * Original annotations:
 *   @Serializable(with = RedditBoolOrTimestampUTCSerializer::class)
 *   @Parcelize
 *
 * Every field, method, inner class, and constant from the original Kotlin file.
 */

#pragma once

#include "reddit_timestamp_utc.h"
#include <memory>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditFieldEdited
 *
 * Original: @Serializable(with = RedditBoolOrTimestampUTCSerializer::class), @Parcelize
 * sealed class RedditFieldEdited : Parcelable
 */
class RedditFieldEdited {
public:
	virtual ~RedditFieldEdited() = default;

	// Port of: data class Bool(val value: Boolean) : RedditFieldEdited()
	// Original: @Serializable(with = RedditFieldEditedBoolSerializer::class), @Parcelize
	class Bool : public RedditFieldEdited {
	public:
		// Port of: val value: Boolean
		bool value;

		// Port of: data class constructor
		Bool() : value(false) {}
		explicit Bool(bool value) : value(value) {}

		bool operator==(const Bool &other) const { return value == other.value; }
		bool operator!=(const Bool &other) const { return value != other.value; }
	};

	// Port of: data class Timestamp(val value: RedditTimestampUTC) : RedditFieldEdited()
	// Original: @Serializable(with = RedditFieldEditedTimestampSerializer::class), @Parcelize
	class Timestamp : public RedditFieldEdited {
	public:
		// Port of: val value: RedditTimestampUTC
		RedditTimestampUTC value;

		// Port of: data class constructor
		Timestamp() : value() {}
		explicit Timestamp(RedditTimestampUTC value) : value(std::move(value)) {}

		bool operator==(const Timestamp &other) const { return value == other.value; }
		bool operator!=(const Timestamp &other) const { return value != other.value; }
	};

protected:
	// Port of: sealed class private constructor
	RedditFieldEdited() = default;
};

// Helper function: Port of == comparison for RedditFieldEdited
inline bool operator==(const RedditFieldEdited &a, const RedditFieldEdited &b) {
	if (auto *ab = dynamic_cast<const RedditFieldEdited::Bool *>(&a)) {
		if (auto *bb = dynamic_cast<const RedditFieldEdited::Bool *>(&b)) {
			return *ab == *bb;
		}
	}
	if (auto *at = dynamic_cast<const RedditFieldEdited::Timestamp *>(&a)) {
		if (auto *bt = dynamic_cast<const RedditFieldEdited::Timestamp *>(&b)) {
			return *at == *bt;
		}
	}
	return false;
}
inline bool operator!=(const RedditFieldEdited &a, const RedditFieldEdited &b) {
	return !(a == b);
}

} // namespace PinkReader

// Port of: object RedditBoolOrTimestampUTCSerializer : JsonContentPolymorphicSerializer<RedditFieldEdited>(RedditFieldEdited::class)
// Port of: override fun selectDeserializer(element: JsonElement): KSerializer<out RedditFieldEdited> {
//     if (!(element is JsonPrimitive)) { throw SerializationException("Expecting JSON primitive for BoolOrTimestamp") }
//     return if (element.booleanOrNull == null) { RedditFieldEdited.Timestamp.serializer() }
//            else { RedditFieldEdited.Bool.serializer() }
// }
//
// Port of: object RedditFieldEditedBoolSerializer : KSerializer<RedditFieldEdited.Bool>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("RedditFieldEdited.Bool", PrimitiveKind.BOOLEAN)
// Port of: override fun deserialize(decoder: Decoder) = RedditFieldEdited.Bool(decoder.decodeBoolean())
// Port of: override fun serialize(encoder: Encoder, value: RedditFieldEdited.Bool) { encoder.encodeBoolean(value.value) }
//
// Port of: object RedditFieldEditedTimestampSerializer : KSerializer<RedditFieldEdited.Timestamp>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("RedditFieldEdited.Timestamp", PrimitiveKind.BOOLEAN)
// Port of: override fun deserialize(decoder: Decoder) = RedditFieldEdited.Timestamp(decoder.decodeSerializableValue(RedditTimestampUTC.serializer()))
// Port of: override fun serialize(encoder: Encoder, value: RedditFieldEdited.Timestamp) { encoder.encodeSerializableValue(RedditTimestampUTC.serializer(), value.value) }
