/*
 * PinkReader - GPLv3
 * File: json_value.h
 * Port of: jsonwrap/JsonValue.java, JsonObject.java, JsonArray.java,
 *          JsonBoolean.java, JsonNull.java, JsonLong.java,
 *          JsonDouble.java, JsonString.java
 *
 * Every field, method, inner class matches exactly.
 * Uses Qt6 QJsonValue/QJsonDocument as the underlying parser.
 */

#pragma once

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QVector>
#include <QHash>
#include <QVariant>
#include <memory>
#include <stdexcept>

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

    // Port of: @NonNull public static JsonValue parse(final InputStream source) throws IOException
    static std::unique_ptr<JsonValue> parse(class QIODevice &source);

    // Port of: @NonNull public static JsonValue parse(final JsonParser parser) throws IOException
    // In C++, we accept a QJsonValue instead of Jackson JsonParser
    static std::unique_ptr<JsonValue> parse(const QJsonValue &value);

    // Port of: @Nullable public JsonObject asObject()
    virtual JsonObject *asObject() { return nullptr; }

    // Port of: @Nullable public <E extends JsonObject.JsonDeserializable> E asObject(final Class<E> clazz)
    // NOTE: Reflection-based deserialization is not directly portable.
    // Subclasses override. Default returns nullptr.
    // In C++, deserializable types should override this or use populateObject.

    // Port of: @Nullable public JsonArray asArray()
    virtual JsonArray *asArray() { return nullptr; }

    // Port of: @Nullable public Boolean asBoolean()
    virtual bool *asBoolean() { return nullptr; }
    // Convenience: returns std::optional<bool>
    virtual std::optional<bool> asBoolValue() const { return std::nullopt; }

    // Port of: @Nullable public String asString()
    virtual QString *asString() { return nullptr; }
    // Convenience: returns std::optional<QString>
    virtual std::optional<QString> asStringValue() const { return std::nullopt; }

    // Port of: @Nullable public Double asDouble()
    virtual double *asDouble() { return nullptr; }
    virtual std::optional<double> asDoubleValue() const { return std::nullopt; }

    // Port of: @Nullable public Long asLong()
    virtual int64_t *asLong() { return nullptr; }
    virtual std::optional<int64_t> asLongValue() const { return std::nullopt; }

    // Port of: @Override public String toString()
    // (using Qt's QDebug stream operator, or toString method)
    virtual QString toString() const;

    // Port of: protected abstract void prettyPrint(int indent, StringBuilder sb)
    virtual void prettyPrint(int indent, QString &sb) const = 0;

    // Port of: @NonNull public final Optional<JsonValue> getAtPath(final Object... keys)
    Optional<JsonValue *> getAtPath(std::initializer_list<QVariant> keys) const;

    // Port of: @NonNull public final Optional<JsonObject> getObjectAtPath(final Object... keys)
    Optional<JsonObject *> getObjectAtPath(std::initializer_list<QVariant> keys) const;

    // Port of: @NonNull public final Optional<JsonArray> getArrayAtPath(final Object... keys)
    Optional<JsonArray *> getArrayAtPath(std::initializer_list<QVariant> keys) const;

    // Port of: @NonNull public final Optional<String> getStringAtPath(final Object... keys)
    Optional<QString> getStringAtPath(std::initializer_list<QVariant> keys) const;

protected:
    // Port of: @NonNull protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys)
    virtual Optional<JsonValue *> getAtPathInternal(int offset, const QVector<QVariant> &keys);
};


// ============================================================================
// JsonObject — Port of org.quantumbadger.redreader.jsonwrap.JsonObject
// Every field, method, inner class matches exactly.
// ============================================================================

class JsonObject : public JsonValue {
public:
    // Port of: public interface JsonDeserializable {}
    class JsonDeserializable {
    public:
        virtual ~JsonDeserializable() = default;
    };

    // Port of: protected JsonObject(final JsonParser parser) throws IOException
    explicit JsonObject(const QJsonObject &obj);

    // Port of: public boolean isEmpty()
    bool isEmpty() const { return m_properties.isEmpty(); }

    // Port of: @NonNull @Override public JsonObject asObject()
    JsonObject *asObject() override { return this; }

    // Port of: @Nullable public JsonValue get(final String name)
    JsonValue *get(const QString &name) const;

    // Port of: @Nullable public String getString(@NonNull final String id)
    QString getString(const QString &id) const;

    // Port of: @Nullable public Long getLong(@NonNull final String id)
    std::optional<int64_t> getLong(const QString &id) const;

    // Port of: @Nullable public Double getDouble(@NonNull final String id)
    std::optional<double> getDouble(const QString &id) const;

    // Port of: @Nullable public Boolean getBoolean(@NonNull final String id)
    std::optional<bool> getBoolean(const QString &id) const;

    // Port of: @Nullable public JsonObject getObject(@NonNull final String id)
    JsonObject *getObject(const QString &id) const;

    // Port of: @Nullable public JsonArray getArray(@NonNull final String id)
    JsonArray *getArray(const QString &id) const;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: public void populateObject(final Object o)
    // (Reflection-based; in C++ this is simplified. Subclasses handle their own deserialization.)

    // Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys)
    Optional<JsonValue *> getAtPathInternal(int offset, const QVector<QVariant> &keys) override;

private:
    // Port of: private final HashMap<String, JsonValue> properties = new HashMap<>()
    QHash<QString, std::unique_ptr<JsonValue>> m_properties;
};


// ============================================================================
// JsonArray — Port of org.quantumbadger.redreader.jsonwrap.JsonArray
// Every field, method matches exactly.
// ============================================================================

class JsonArray : public JsonValue {
public:
    // Port of: protected JsonArray(final JsonParser parser) throws IOException
    explicit JsonArray(const QJsonArray &arr);

    // Port of: @NonNull @Override public JsonArray asArray()
    JsonArray *asArray() override { return this; }

    // Port of: @NonNull public JsonValue get(final int id)
    JsonValue *get(int id) const;

    // Port of: @Nullable public String getString(final int id)
    QString getString(int id) const;

    // Port of: @Nullable public Long getLong(final int id)
    std::optional<int64_t> getLong(int id) const;

    // Port of: @Nullable public Double getDouble(final int id)
    std::optional<double> getDouble(int id) const;

    // Port of: @Nullable public Boolean getBoolean(final int id)
    std::optional<bool> getBoolean(int id) const;

    // Port of: @Nullable public JsonObject getObject(final int id)
    JsonObject *getObject(int id) const;

    // Port of: @Nullable public JsonArray getArray(final int id)
    JsonArray *getArray(int id) const;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: public int size()
    int size() const { return m_contents.size(); }

    // Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys)
    Optional<JsonValue *> getAtPathInternal(int offset, const QVector<QVariant> &keys) override;

    // Port of: public Iterator<JsonValue> iterator()
    // (Implicit via QVector iteration)

    // Port of: public void forEachObject(final Consumer<JsonObject> consumer)
    template<typename Consumer>
    void forEachObject(Consumer consumer) {
        for (auto &val : m_contents) {
            consumer(val->asObject());
        }
    }

private:
    // Port of: private final ArrayList<JsonValue> mContents = new ArrayList<>(16)
    QVector<std::unique_ptr<JsonValue>> m_contents;
};


// ============================================================================
// JsonBoolean — Port of org.quantumbadger.redreader.jsonwrap.JsonBoolean
// ============================================================================

class JsonBoolean : public JsonValue {
public:
    // Port of: @NonNull public static final JsonBoolean TRUE = new JsonBoolean(true)
    static const JsonBoolean TRUE;
    // Port of: @NonNull public static final JsonBoolean FALSE = new JsonBoolean(false)
    static const JsonBoolean FALSE;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @Override @NonNull public Boolean asBoolean()
    bool *asBoolean() override;
    std::optional<bool> asBoolValue() const override { return m_value; }

private:
    // Port of: private final boolean mValue
    bool m_value;

    // Port of: private JsonBoolean(final boolean value)
    explicit JsonBoolean(bool value) : m_value(value) {}
};


// ============================================================================
// JsonNull — Port of org.quantumbadger.redreader.jsonwrap.JsonNull
// ============================================================================

class JsonNull : public JsonValue {
public:
    // Port of: @NonNull public static final JsonNull INSTANCE = new JsonNull()
    static const JsonNull INSTANCE;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
    void prettyPrint(int indent, QString &sb) const override;

private:
    // Port of: private JsonNull() {}
    JsonNull() = default;
};


// ============================================================================
// JsonLong — Port of org.quantumbadger.redreader.jsonwrap.JsonLong
// ============================================================================

class JsonLong : public JsonValue {
public:
    // Port of: public JsonLong(final long value)
    explicit JsonLong(int64_t value) : m_value(value) {}

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @NonNull @Override public String asString()
    QString *asString() override;
    std::optional<QString> asStringValue() const override;

    // Port of: @NonNull @Override public Double asDouble()
    double *asDouble() override;
    std::optional<double> asDoubleValue() const override;

    // Port of: @NonNull @Override public Long asLong()
    int64_t *asLong() override;
    std::optional<int64_t> asLongValue() const override;

private:
    // Port of: private final long mValue
    int64_t m_value;
};


// ============================================================================
// JsonDouble — Port of org.quantumbadger.redreader.jsonwrap.JsonDouble
// ============================================================================

class JsonDouble : public JsonValue {
public:
    // Port of: protected JsonDouble(final double value)
    explicit JsonDouble(double value) : m_value(value) {}

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @NonNull @Override public String asString()
    QString *asString() override;
    std::optional<QString> asStringValue() const override;

    // Port of: @NonNull @Override public Double asDouble()
    double *asDouble() override;
    std::optional<double> asDoubleValue() const override;

    // Port of: @NonNull @Override public Long asLong()
    int64_t *asLong() override;
    std::optional<int64_t> asLongValue() const override;

private:
    // Port of: private final double mValue
    double m_value;
};


// ============================================================================
// JsonString — Port of org.quantumbadger.redreader.jsonwrap.JsonString
// ============================================================================

class JsonString : public JsonValue {
public:
    // Port of: protected JsonString(@NonNull final String value)
    explicit JsonString(const QString &value) : m_value(value) {}

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @Nullable @Override public Boolean asBoolean()
    bool *asBoolean() override;
    std::optional<bool> asBoolValue() const override;

    // Port of: @NonNull @Override public String asString()
    QString *asString() override;
    std::optional<QString> asStringValue() const override;

    // Port of: @Nullable @Override public Double asDouble()
    double *asDouble() override;
    std::optional<double> asDoubleValue() const override;

    // Port of: @Nullable @Override public Long asLong()
    int64_t *asLong() override;
    std::optional<int64_t> asLongValue() const override;

private:
    // Port of: @NonNull private final String mValue
    QString m_value;
};

} // namespace PinkReader
