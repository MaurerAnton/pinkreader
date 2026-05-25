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

#include "json_utils.h"

namespace PinkReader {

// JsonUtils is a port of a Kotlin object that delegates JSON parsing
// to kotlinx.serialization. In C++, the actual deserialization will
// be implemented using a custom JSON-to-C++ mapper that reads from
// streams and populates the RedditThing polymorphism hierarchy.
//
// Port of the private JSON config:
//   json = Json { ignoreUnknownKeys = true; isLenient = true }
//
// Port of the API < 24 workaround (not applicable in C++):
//   Json.decodeFromStream is broken on API < 24
//   -> Use BufferedSource and Json.decodeFromBufferedSource instead.
//
// See https://github.com/Kotlin/kotlinx.serialization/issues/2457
// See https://issuetracker.google.com/issues/37054036

} // namespace PinkReader
