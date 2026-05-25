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
 *   org.quantumbadger.redreader.reddit.kthings.MaybeParseError (Kotlin sealed class)
 *
 * Original annotations:
 *   @Serializable(with = MaybeParseErrorSerializer::class)
 *   @Parcelize // TODO test parcelize on sealed classes
 *
 * Every field, method, inner class, and constant from the original Kotlin file.
 */

#pragma once

#include <stdexcept>
#include <memory>
#include <type_traits>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.kthings.MaybeParseError<E>
 *
 * Original: @Serializable(with = MaybeParseErrorSerializer::class), @Parcelize
 * sealed class MaybeParseError<E: Parcelable> private constructor() : Parcelable
 *
 * In C++, this is modeled as a result-like type, storing either a value or an exception.
 *
 * @tparam E The inner value type (originally constrained to Parcelable in Kotlin)
 */
template<typename E>
class MaybeParseError {
public:
	// Port of: data class Ok<E: Parcelable>(val value: E) : MaybeParseError<E>()
	class Ok : public MaybeParseError<E> {
	public:
		// Port of: val value: E
		E value;

		// Port of: data class constructor
		Ok() : value() {}
		explicit Ok(E value) : value(std::move(value)) {}
	};

	// Port of: data class Err<E: Parcelable>(val error: Exception) : MaybeParseError<E>()
	class Err : public MaybeParseError<E> {
	public:
		// Port of: val error: Exception
		// In C++, we store the exception message as a string to avoid slicing
		std::string errorMessage;
		std::exception_ptr errorPtr;

		// Port of: data class constructor
		Err() : errorMessage(), errorPtr() {}
		explicit Err(const std::exception &e)
			: errorMessage(e.what()), errorPtr(std::make_exception_ptr(e)) {}
		explicit Err(std::string errorMessage)
			: errorMessage(std::move(errorMessage)), errorPtr() {}
	};

	virtual ~MaybeParseError() = default;

	// Port of: fun ok() = when (this) { is Ok -> value is Err -> throw error }
	E ok() const {
		if (auto *ok = dynamic_cast<const Ok *>(this)) {
			return ok->value;
		}
		if (auto *err = dynamic_cast<const Err *>(this)) {
			throw std::runtime_error(err->errorMessage);
		}
		throw std::runtime_error("MaybeParseError: neither Ok nor Err");
	}

protected:
	// Port of: private constructor()
	MaybeParseError() = default;
};

} // namespace PinkReader

// Port of: class MaybeParseErrorSerializer<E: Parcelable>(private val innerSerializer: KSerializer<E>) : KSerializer<MaybeParseError<E>>
// Port of: override val descriptor: SerialDescriptor get() = innerSerializer.descriptor
// Port of: override fun deserialize(decoder: Decoder): MaybeParseError<E> {
//     return try { MaybeParseError.Ok(decoder.decodeSerializableValue(innerSerializer)) }
//     catch (e: Exception) { MaybeParseError.Err(e) }
// }
// Port of: override fun serialize(encoder: Encoder, value: MaybeParseError<E>) {
//     if (!(value is MaybeParseError.Ok)) { throw RuntimeException("Cannot encode error type: $value") }
//     encoder.encodeSerializableValue(innerSerializer, value.value)
// }
