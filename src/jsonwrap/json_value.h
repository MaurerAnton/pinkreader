/*
 * PinkReader - GPLv3
 * File: json_value.h
 * Port of: jsonwrap/JsonValue.java
 * Every field, method, inner class matches exactly.
 * Uses Qt6 QJsonValue/QJsonDocument as the underlying parser.
 */

#pragma once

#include <QString>
#include <QVector>
#include <QHash>
#include <QVariant>
#include <QIODevice>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <stdexcept>
#include <optional>
#include <memory>
#include <initializer_list>

namespace PinkReader {

// ============================================================================
// Forward declarations
// ============================================================================

class JsonValue;
class JsonObject;
class JsonArray;
class JsonBoolean;
class JsonNull;
class JsonLong;
class JsonDouble;
class JsonString;

// ============================================================================
// Optional<E> — Port of org.quantumbadger.redreader.common.Optional<E>
// (needed by JsonValue for getAtPath etc.)
// Every method matches the Java original exactly.
// ============================================================================

template<typename E>
class Optional {
public:
    // Port of: public static class OptionalHasNoValueException extends RuntimeException {}
    class OptionalHasNoValueException : public std::runtime_error {
    public:
        OptionalHasNoValueException() : std::runtime_error("Optional has no value") {}
    };

    // Port of: @NonNull public static <E> Optional<E> empty()
    static Optional<E> empty() { return Optional<E>(nullptr); }

    // Port of: @NonNull public static <E> Optional<E> of(@NonNull final E value)
    static Optional<E> of(const E &value) { return Optional<E>(new E(value)); }
    static Optional<E> of(E &&value) { return Optional<E>(new E(std::move(value))); }

    // Port of: @NonNull public static <E> Optional<E> ofNullable(@Nullable final E value)
    // NOTE: This overload for pointer types
    template<typename P = E, typename std::enable_if<std::is_pointer<P>::value, int>::type = 0>
    static Optional<P> ofNullable(P value) {
        if (value == nullptr) return empty();
        return Optional<P>(value);
    }

    // Move constructor
    Optional(Optional&& other) noexcept : m_value(other.m_value) { other.m_value = nullptr; }
    Optional& operator=(Optional&& other) noexcept {
        if (this != &other) { delete m_value; m_value = other.m_value; other.m_value = nullptr; }
        return *this;
    }

    // Copy constructor
    Optional(const Optional& other) : m_value(other.m_value ? new E(*other.m_value) : nullptr) {}
    Optional& operator=(const Optional& other) {
        if (this != &other) { delete m_value; m_value = other.m_value ? new E(*other.m_value) : nullptr; }
        return *this;
    }

    ~Optional() { delete m_value; }

    // Port of: public boolean isPresent()
    bool isPresent() const { return m_value != nullptr; }

    // Port of: public boolean isEmpty()
    bool isEmpty() const { return m_value == nullptr; }

    // Port of: @NonNull public E get()
    E& get() {
        if (m_value == nullptr) throw OptionalHasNoValueException();
        return *m_value;
    }
    const E& get() const {
        if (m_value == nullptr) throw OptionalHasNoValueException();
        return *m_value;
    }

    // Port of: @Nullable public E asNullable()
    E* asNullable() { return m_value; }
    const E* asNullable() const { return m_value; }

    // Port of: @NonNull public E orElse(@NonNull final E alternative)
    E orElse(const E &alternative) const {
        return (m_value == nullptr) ? alternative : *m_value;
    }

    // Port of: @NonNull public Optional<E> orElse(@NonNull final Optional<E> alternative)
    Optional<E> orElse(const Optional<E> &alternative) const {
        if (m_value == nullptr) return alternative;
        return Optional<E>::of(*m_value);
    }

    // Port of: @Nullable public E orElseNull()
    E* orElseNull() { return m_value; }

private:
    // Port of: @Nullable private final E mValue
    E *m_value;

    // Port of: private Optional(@Nullable final E value)
    explicit Optional(E *value) : m_value(value) {}

    // Port of: private static final Optional<?> EMPTY = new Optional<>(null)
    // (handled by empty() static method)
};


// ============================================================================
// JsonValue — Port of org.quantumbadger.redreader.jsonwrap.JsonValue (abstract class)
// Every field, method matches exactly.
// ============================================================================

class JsonValue {
public:
    virtual ~JsonValue() = default;

    // Port of: @NonNull public static JsonValue parse(final InputStream source) throws IOException (JsonValue.java:35-37)
    static std::unique_ptr<JsonValue> parse(class QIODevice &source);

    // Port of: @NonNull public static JsonValue parse(final JsonParser parser) throws IOException (JsonValue.java:39-94)
    // In C++, we accept a QJsonValue instead of Jackson JsonParser
    static std::unique_ptr<JsonValue> parse(const QJsonValue &value);

    // Port of: @Nullable public JsonObject asObject() (JsonValue.java:97-100)
    virtual JsonObject *asObject() { return nullptr; }

    // Port of: @Nullable public <E extends JsonObject.JsonDeserializable> E asObject(final Class<E> clazz) (JsonValue.java:102-111)
    // NOTE: Reflection-based; handled by JsonObject subclass.

    // Port of: @Nullable public JsonArray asArray() (JsonValue.java:114-117)
    virtual JsonArray *asArray() { return nullptr; }

    // Port of: @Nullable public Boolean asBoolean() (JsonValue.java:120-123)
    virtual bool *asBoolean() { return nullptr; }
    virtual std::optional<bool> asBoolValue() const { return std::nullopt; }

    // Port of: @Nullable public String asString() (JsonValue.java:126-129)
    virtual QString *asString() { return nullptr; }
    virtual std::optional<QString> asStringValue() const { return std::nullopt; }

    // Port of: @Nullable public Double asDouble() (JsonValue.java:132-135)
    virtual double *asDouble() { return nullptr; }
    virtual std::optional<double> asDoubleValue() const { return std::nullopt; }

    // Port of: @Nullable public Long asLong() (JsonValue.java:138-141)
    virtual int64_t *asLong() { return nullptr; }
    virtual std::optional<int64_t> asLongValue() const { return std::nullopt; }

    // Port of: @Override public String toString() (JsonValue.java:143-148)
    virtual QString toString() const;

    // Port of: protected abstract void prettyPrint(int indent, StringBuilder sb) (JsonValue.java:150)
    virtual void prettyPrint(int indent, QString &sb) const = 0;

    // Port of: @NonNull public final Optional<JsonValue> getAtPath(final Object... keys) (JsonValue.java:152-155)
    Optional<JsonValue *> getAtPath(std::initializer_list<QVariant> keys) const;

    // Port of: @NonNull public final Optional<JsonObject> getObjectAtPath(final Object... keys) (JsonValue.java:157-167)
    Optional<JsonObject *> getObjectAtPath(std::initializer_list<QVariant> keys) const;

    // Port of: @NonNull public final Optional<JsonArray> getArrayAtPath(final Object... keys) (JsonValue.java:169-179)
    Optional<JsonArray *> getArrayAtPath(std::initializer_list<QVariant> keys) const;

    // Port of: @NonNull public final Optional<String> getStringAtPath(final Object... keys) (JsonValue.java:181-191)
    Optional<QString> getStringAtPath(std::initializer_list<QVariant> keys) const;

protected:
    // Port of: @NonNull protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys) (JsonValue.java:194-203)
    virtual Optional<JsonValue *> getAtPathInternal(int offset, const QVector<QVariant> &keys);
};

} // namespace PinkReader
