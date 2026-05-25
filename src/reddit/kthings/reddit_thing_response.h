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
 *   org.quantumbadger.redreader.reddit.kthings.RedditThingResponse (Kotlin sealed class)
 *
 * Original annotations:
 *   @Serializable(with = RedditThingResponseSerializer::class)
 *
 * Every variant, field, method, serializer, and constant from the original Kotlin file.
 */

#pragma once

#include "reddit_thing.h"
#include <memory>
#include <QVector>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.RedditThingResponse
 *
 * Original: @Serializable(with = RedditThingResponseSerializer::class)
 * sealed class RedditThingResponse
 */
class RedditThingResponse {
public:
	virtual ~RedditThingResponse() = default;

	// Port of: @Serializable(with = RedditThingResponseSingleSerializer::class)
	// data class Single(val thing: RedditThing) : RedditThingResponse()
	class Single : public RedditThingResponse {
	public:
		// Port of: val thing: RedditThing
		std::shared_ptr<RedditThing> thing;

		// Port of: data class constructor
		Single() : thing(nullptr) {}
		explicit Single(std::shared_ptr<RedditThing> thing) : thing(std::move(thing)) {}
	};

	// Port of: @Serializable(with = RedditThingResponseMultipleSerializer::class)
	// data class Multiple(val things: List<RedditThing>) : RedditThingResponse()
	class Multiple : public RedditThingResponse {
	public:
		// Port of: val things: List<RedditThing>
		QVector<std::shared_ptr<RedditThing>> things;

		// Port of: data class constructor
		Multiple() = default;
		explicit Multiple(QVector<std::shared_ptr<RedditThing>> things)
			: things(std::move(things)) {}
	};

protected:
	// Port of: sealed class private constructor
	RedditThingResponse() = default;
};

} // namespace PinkReader

// Port of: object RedditThingResponseSerializer : JsonContentPolymorphicSerializer<RedditThingResponse>(RedditThingResponse::class)
// Port of: override fun selectDeserializer(element: JsonElement): KSerializer<out RedditThingResponse> {
//     return if (element is JsonArray) { RedditThingResponseMultipleSerializer }
//            else { RedditThingResponseSingleSerializer }
// }
//
// Port of: object RedditThingResponseSingleSerializer : KSerializer<RedditThingResponse.Single>
// Port of: override val descriptor: SerialDescriptor get() = RedditThing.serializer().descriptor
// Port of: override fun deserialize(decoder: Decoder) = RedditThingResponse.Single(decoder.decodeSerializableValue(RedditThing.serializer()))
// Port of: override fun serialize(encoder: Encoder, value: RedditThingResponse.Single) { encoder.encodeSerializableValue(RedditThing.serializer(), value.thing) }
//
// Port of: object RedditThingResponseMultipleSerializer : KSerializer<RedditThingResponse.Multiple>
// Port of: override val descriptor: SerialDescriptor get() = RedditThing.serializer().descriptor
// Port of: private val multipleThingSerializer = ListSerializer(RedditThing.serializer())
// Port of: override fun deserialize(decoder: Decoder) = RedditThingResponse.Multiple(decoder.decodeSerializableValue(multipleThingSerializer))
// Port of: override fun serialize(encoder: Encoder, value: RedditThingResponse.Multiple) { encoder.encodeSerializableValue(multipleThingSerializer, value.things) }
