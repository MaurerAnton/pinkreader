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
 *   org.quantumbadger.redreader.reddit.kthings.UrlEncodedString (Kotlin data class)
 *
 * Original annotations:
 *   @Serializable(with = UrlEncodedStringSerializer::class)
 *   @Parcelize
 *
 * Every field, method, and constant from the original Kotlin file.
 */

#pragma once

#include <QString>
#include <optional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.UrlEncodedString
 *
 * Original: @Serializable(with = UrlEncodedStringSerializer::class), @Parcelize
 * data class UrlEncodedString(val decoded: String) : Parcelable
 */
class UrlEncodedString {
public:
	// Port of: val decoded: String
	QString decoded;

	// Port of: data class constructor - val decoded: String
	UrlEncodedString() = default;
	explicit UrlEncodedString(QString decoded) : decoded(std::move(decoded)) {}

	bool operator==(const UrlEncodedString &other) const {
		return decoded == other.decoded;
	}
	bool operator!=(const UrlEncodedString &other) const {
		return decoded != other.decoded;
	}

	// Port of: (implicit) toString() -> decoded
	explicit operator QString() const { return decoded; }
};

} // namespace PinkReader

// Port of: object UrlEncodedStringSerializer : KSerializer<UrlEncodedString>
// Port of: override val descriptor: SerialDescriptor = PrimitiveSerialDescriptor("UrlEncodedString", PrimitiveKind.STRING)
// Port of: override fun deserialize(decoder: Decoder) = UrlEncodedString(decoded = StringEscapeUtils.unescapeHtml4(decoder.decodeString()))
// Port of: override fun serialize(encoder: Encoder, value: UrlEncodedString) { encoder.encodeString(StringEscapeUtils.escapeHtml4(value.decoded)) }
// NOTE: Serialization/deserialization uses Apache Commons Text StringEscapeUtils for HTML unescaping/escaping.
// In C++, equivalent HTML escape/unescape can be provided by a StringEscapeUtils port.
// The serializer is left as a comment since it depends on the kotlinx.serialization framework.
