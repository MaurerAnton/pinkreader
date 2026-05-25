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
 *   org.quantumbadger.redreader.reddit.kthings.RedditFieldReplies (Kotlin sealed class)
 *
 * Original annotations:
 *   @Serializable(with = RedditFieldRepliesSerializer::class)
 *   @Parcelize
 *
 * Every field, method, inner class, singleton, and constant from the original Kotlin file.
 */

#pragma once

#include <memory>
#include <stdexcept>

namespace PinkReader {

// Forward declaration (RedditThing is defined in reddit_thing.h)
class RedditThing;

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditFieldReplies
 *
 * Original: @Serializable(with = RedditFieldRepliesSerializer::class), @Parcelize
 * sealed class RedditFieldReplies : Parcelable
 */
class RedditFieldReplies {
public:
	virtual ~RedditFieldReplies() = default;

	// Port of: object None : RedditFieldReplies()
	// Original: @Serializable(with = RedditFieldRepliesNoneStringSerializer::class), @Parcelize
	class None : public RedditFieldReplies {
	public:
		// Port of: singleton object None
		static const None &instance() {
			static const None s_instance;
			return s_instance;
		}

		bool operator==(const None &) const { return true; }
		bool operator!=(const None &) const { return false; }
	};

	// Port of: data class Some(val value: RedditThing) : RedditFieldReplies()
	// Original: @Serializable(with = RedditFieldRepliesSomeSerializer::class), @Parcelize
	class Some : public RedditFieldReplies {
	public:
		// Port of: val value: RedditThing
		// In C++, RedditThing is polymorphic, so we use shared_ptr.
		std::shared_ptr<RedditThing> value;

		// Port of: data class constructor
		Some() : value(nullptr) {}
		explicit Some(std::shared_ptr<RedditThing> value) : value(std::move(value)) {}
	};

protected:
	// Port of: sealed class private constructor
	RedditFieldReplies() = default;
};

} // namespace PinkReader

// Port of: object RedditFieldRepliesSerializer : JsonContentPolymorphicSerializer<RedditFieldReplies>(RedditFieldReplies::class)
// Port of: override fun selectDeserializer(element: JsonElement): KSerializer<out RedditFieldReplies> {
//     return if (element is JsonObject) { RedditFieldRepliesSomeSerializer }
//            else if (element is JsonPrimitive) { RedditFieldRepliesNoneStringSerializer }
//            else if (element is JsonNull) { RedditFieldRepliesNoneNullSerializer }
//            else { throw IOException("Unexpected replies type $element") }
// }
//
// Port of: object RedditFieldRepliesNoneStringSerializer : KSerializer<RedditFieldReplies.None>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("RedditFieldReplies.None", PrimitiveKind.BOOLEAN)
// Port of: override fun deserialize(decoder: Decoder): RedditFieldReplies.None { decoder.decodeString(); return RedditFieldReplies.None }
// Port of: override fun serialize(encoder: Encoder, value: RedditFieldReplies.None) { encoder.encodeString("") }
//
// Port of: object RedditFieldRepliesNoneNullSerializer : KSerializer<RedditFieldReplies.None>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("RedditFieldReplies.None", PrimitiveKind.BOOLEAN)
// Port of: @OptIn(ExperimentalSerializationApi::class)
// Port of: override fun deserialize(decoder: Decoder): RedditFieldReplies.None { decoder.decodeNull(); return RedditFieldReplies.None }
// Port of: override fun serialize(encoder: Encoder, value: RedditFieldReplies.None) { encoder.encodeString("") }
//
// Port of: object RedditFieldRepliesSomeSerializer : KSerializer<RedditFieldReplies.Some>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("RedditFieldReplies.Some", PrimitiveKind.BOOLEAN)
// Port of: override fun deserialize(decoder: Decoder) = RedditFieldReplies.Some(decoder.decodeSerializableValue(RedditThing.serializer()))
// Port of: override fun serialize(encoder: Encoder, value: RedditFieldReplies.Some) { encoder.encodeSerializableValue(RedditThing.serializer(), value.value) }
