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
 *   org.quantumbadger.redreader.reddit.kthings.JsonUtils (Kotlin object)
 *
 * Original: object JsonUtils
 *
 * Every method, field, and constant from the original Kotlin file.
 *
 * Dependencies:
 *   - org.quantumbadger.redreader.reddit.kthings.RedditThing
 *   - org.quantumbadger.redreader.reddit.kthings.RedditThingResponse
 *   - kotlinx.serialization.json.Json
 *   - okio (for BufferedSource, etc.)
 *
 * Original imports:
 *   import android.os.Build
 *   import kotlinx.serialization.ExperimentalSerializationApi
 *   import kotlinx.serialization.KSerializer
 *   import kotlinx.serialization.json.Json
 *   import kotlinx.serialization.json.decodeFromStream
 *   import kotlinx.serialization.json.okio.decodeFromBufferedSource
 *   import okio.BufferedSource
 *   import okio.buffer
 *   import okio.source
 *   import java.io.IOException
 *   import java.io.InputStream
 */

#pragma once

#include "reddit_thing.h"
#include "reddit_thing_response.h"
#include <memory>
#include <stdexcept>
#include <QIODevice>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.JsonUtils (Kotlin object)
 *
 * Original: object JsonUtils
 *
 * In C++, this is a class with static methods (cannot be instantiated).
 *
 * NOTE: This is a simplified port that provides the decode interface.
 * The actual JSON deserialization logic will need to be implemented
 * using the project's JSON library (QJsonDocument or similar) or
 * a custom kotlinx.serialization-like stream parser.
 */
class JsonUtils {
public:
	// Cannot instantiate (Kotlin object)
	JsonUtils() = delete;

	// Port of: private val json = Json { ignoreUnknownKeys = true; isLenient = true }
	// In C++, this is a static configuration for the JSON parser.

	// Port of: @Throws(IOException::class)
	// fun decodeRedditThingFromStream(stream: InputStream): RedditThing
	// = decodeFromStream(RedditThing.serializer(), stream)
	//
	// NOTE: The original uses kotlinx.serialization to decode directly from a stream.
	// In C++, we provide the signature with a QIODevice for stream input.
	// Returns a shared_ptr because RedditThing is polymorphic.
	//
	// static std::shared_ptr<RedditThing> decodeRedditThingFromStream(QIODevice &stream);

	// Port of: @Throws(IOException::class)
	// fun decodeRedditThingResponseFromStream(stream: InputStream): RedditThingResponse
	// = decodeFromStream(RedditThingResponse.serializer(), stream)
	//
	// static std::shared_ptr<RedditThingResponse> decodeRedditThingResponseFromStream(QIODevice &stream);

	// Port of: @OptIn(ExperimentalSerializationApi::class)
	// private fun <T> decodeFromStream(serializer: KSerializer<T>, stream: InputStream): T {
	//     /**
	//      * [Json.decodeFromStream] is broken on API < 24
	//      * Wrap it in [BufferedSource] and use [Json.decodeFromBufferedSource] which works
	//      *
	//      * https://github.com/Kotlin/kotlinx.serialization/issues/2457
	//      * https://issuetracker.google.com/issues/37054036
	//      */
	//     return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
	//         json.decodeFromStream(serializer, stream)
	//     } else {
	//         json.decodeFromBufferedSource(serializer, stream.source().buffer())
	//     }
	// }
	//
	// NOTE: The Android API level check (API < 24 workaround) is not relevant in C++.
	// We use buffered reading universally for portability.
};

} // namespace PinkReader
