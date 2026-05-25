// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/Optional.java
#pragma once

#include <stdexcept>
#include <functional>
#include <string>
#include <type_traits>

namespace PinkReader {

/**
 * Exception thrown when get() is called on an empty Optional.
 * Equivalent to: Optional.OptionalHasNoValueException
 */
class OptionalHasNoValueException : public std::runtime_error {
public:
	OptionalHasNoValueException() : std::runtime_error("Optional has no value") {}
};

/**
 * A container object which may or may not contain a non-null value.
 * Equivalent to: org.quantumbadger.redreader.common.Optional<E>
 *
 * @tparam E the type of value
 */
template<typename E>
class Optional {
private:
	static const Optional* EMPTY_INSTANCE;

	E* mValue; // nullable (nullptr = empty)

	explicit Optional(E* value) : mValue(value) {}

public:
	// -- Static factories --

	/** Returns an empty Optional instance. */
	static Optional<E> empty() {
		return Optional<E>(nullptr);
	}

	/** Returns an Optional with the specified present non-null value. */
	static Optional<E> of(const E& value) {
		return Optional<E>(new E(value));
	}

	/** Returns an Optional describing the specified value, if non-null,
	 *  otherwise returns an empty Optional. */
	static Optional<E> ofNullable(E* value) {
		if (value == nullptr) {
			return empty();
		}
		return Optional<E>(new E(*value));
	}

	// -- Copy / Move / Destructor --

	Optional(const Optional<E>& other) : mValue(other.mValue ? new E(*other.mValue) : nullptr) {}

	Optional(Optional<E>&& other) noexcept : mValue(other.mValue) {
		other.mValue = nullptr;
	}

	Optional& operator=(const Optional<E>& other) {
		if (this != &other) {
			delete mValue;
			mValue = other.mValue ? new E(*other.mValue) : nullptr;
		}
		return *this;
	}

	Optional& operator=(Optional<E>&& other) noexcept {
		if (this != &other) {
			delete mValue;
			mValue = other.mValue;
			other.mValue = nullptr;
		}
		return *this;
	}

	~Optional() {
		delete mValue;
	}

	// -- Query methods --

	/** If a value is present, returns true, otherwise false. */
	bool isPresent() const {
		return mValue != nullptr;
	}

	/** If a value is not present, returns true, otherwise false. */
	bool isEmpty() const {
		return mValue == nullptr;
	}

	// -- Value access --

	/** If a value is present, returns the value, otherwise throws OptionalHasNoValueException. */
	E& get() {
		if (mValue == nullptr) {
			throw OptionalHasNoValueException();
		}
		return *mValue;
	}

	/** If a value is present, returns the value, otherwise throws OptionalHasNoValueException. */
	const E& get() const {
		if (mValue == nullptr) {
			throw OptionalHasNoValueException();
		}
		return *mValue;
	}

	/** Returns the value if present, otherwise returns nullptr. */
	E* asNullable() {
		return mValue;
	}

	/** Returns the value if present, otherwise returns nullptr. */
	const E* asNullable() const {
		return mValue;
	}

	/** Return the value if present, otherwise return alternative. */
	E orElse(const E& alternative) const {
		if (mValue == nullptr) {
			return alternative;
		} else {
			return *mValue;
		}
	}

	/** Return this Optional if a value is present, otherwise return alternative. */
	Optional<E> orElse(const Optional<E>& alternative) const {
		if (mValue == nullptr) {
			return alternative;
		} else {
			return Optional<E>::of(*mValue);
		}
	}

	/** Returns the value if present, otherwise returns nullptr. */
	E* orElseNull() {
		return mValue;
	}

	/** Returns the value if present, otherwise returns nullptr. */
	const E* orElseNull() const {
		return mValue;
	}

	/** If a value is present, returns the value, otherwise throws an exception
	 *  created by the factory. */
	template<typename T>
	E& orThrow(const std::function<T()>& factory) {
		if (mValue == nullptr) {
			throw factory();
		}
		return *mValue;
	}

	// -- Transformations --

	/** If a value is present, applies the provided mapping function to it,
	 *  and returns an Optional describing the result. Otherwise returns empty. */
	template<typename R>
	Optional<R> map(const std::function<R(const E&)>& function) const {
		if (mValue == nullptr) {
			return Optional<R>::empty();
		} else {
			return Optional<R>::of(function(*mValue));
		}
	}

	/** If a value is present, applies the provided Optional-bearing mapping
	 *  function to it, returns that result, otherwise returns empty. */
	template<typename R>
	Optional<R> flatMap(const std::function<Optional<R>(const E&)>& function) const {
		if (mValue == nullptr) {
			return Optional<R>::empty();
		} else {
			return function(*mValue);
		}
	}

	/** If a value is present, performs the given action with the value,
	 *  otherwise does nothing. */
	void apply(const std::function<void(const E&)>& function) const {
		if (mValue != nullptr) {
			function(*mValue);
		}
	}

	/** If a value is present, and the value matches the given predicate,
	 *  returns an Optional describing the value, otherwise returns empty. */
	template<typename R>
	Optional<R> filter(const std::function<Optional<R>(const E&)>& function) const {
		if (mValue == nullptr) {
			return Optional<R>::empty();
		} else {
			return function(*mValue);
		}
	}

	/** If a value is present, performs the given action with the value,
	 *  otherwise does nothing. */
	void ifPresent(const std::function<void(const E&)>& consumer) const {
		if (mValue != nullptr) {
			consumer(*mValue);
		}
	}

	// -- Object overrides --

	int hashCode() const {
		if (mValue == nullptr) {
			return 0x28734823; // Random value, matching Java
		} else {
			// Use std::hash if available
			return std::hash<E>{}(*mValue);
		}
	}

	bool equals(const Optional<E>& obj) const {
		if (mValue == nullptr) {
			return obj.mValue == nullptr;
		}
		if (obj.mValue == nullptr) {
			return false;
		}
		return *mValue == *(obj.mValue);
	}

	std::string toString() const {
		if (mValue == nullptr) {
			return "<empty>";
		} else {
			// Requires operator<< or to_string for E
			return std::to_string(*mValue);
		}
	}
};

} // namespace PinkReader
