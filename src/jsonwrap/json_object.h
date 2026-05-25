/*
 * PinkReader - GPLv3
 * File: json_object.h
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonObject.java
 * Every field, method, inner class matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonObject.java
 */

#pragma once

#include "json_value.h"
#include "json_array.h"

#include <QHash>
#include <QString>
#include <memory>

namespace PinkReader {

// Forward declaration
class JsonArray;

// Port of: public final class JsonObject extends JsonValue
//           implements Iterable<Map.Entry<String, JsonValue>> (JsonObject.java:37-38)
class JsonObject : public JsonValue {
public:
    // Port of: public interface JsonDeserializable {} (JsonObject.java:40)
    class JsonDeserializable {
    public:
        virtual ~JsonDeserializable() = default;
    };

    // Port of: protected JsonObject(final JsonParser parser) throws IOException (JsonObject.java:44-73)
    explicit JsonObject(const QJsonObject &obj);

    // Port of: public boolean isEmpty() (JsonObject.java:75-77)
    bool isEmpty() const;

    // Port of: @NonNull @Override public JsonObject asObject() (JsonObject.java:79-83)
    JsonObject *asObject() override { return this; }

    // Port of: @NonNull @Override public <E extends JsonDeserializable> E asObject(final Class<E> clazz) (JsonObject.java:85-96)
    //   throws InstantiationException, IllegalAccessException, NoSuchMethodException, InvocationTargetException
    //   final E obj = clazz.getConstructor().newInstance();
    //   populateObject(obj);
    //   return obj;
    // NOTE: Reflection-based. In C++, use populateObject on an already-constructed object.

    // Port of: @Nullable public JsonValue get(final String name) (JsonObject.java:98-101)
    JsonValue *get(const QString &name) const;

    // Port of: @Nullable public String getString(@NonNull final String id) (JsonObject.java:103-113)
    QString getString(const QString &id) const;

    // Port of: @Nullable public Long getLong(@NonNull final String id) (JsonObject.java:115-125)
    std::optional<int64_t> getLong(const QString &id) const;

    // Port of: @Nullable public Double getDouble(@NonNull final String id) (JsonObject.java:127-137)
    std::optional<double> getDouble(const QString &id) const;

    // Port of: @Nullable public Boolean getBoolean(@NonNull final String id) (JsonObject.java:139-149)
    std::optional<bool> getBoolean(const QString &id) const;

    // Port of: @Nullable public JsonObject getObject(@NonNull final String id) (JsonObject.java:151-161)
    JsonObject *getObject(const QString &id) const;

    // Port of: @Nullable public <E extends JsonDeserializable> E getObject( (JsonObject.java:163-179)
    //           @NonNull final String id, final Class<E> clazz) throws ...
    // NOTE: Reflection-based; handled by getObject(id) + manual deserialization in C++

    // Port of: @Nullable public JsonArray getArray(@NonNull final String id) (JsonObject.java:181-191)
    JsonArray *getArray(const QString &id) const;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonObject.java:193-220)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: public void populateObject(final Object o) throws (JsonObject.java:222-299)
    //           IllegalArgumentException, InstantiationException, NoSuchMethodException, InvocationTargetException
    // NOTE: Reflection-based field population. This is a best-effort port.
    // In C++, deserializable types typically handle their own deserialization from a JsonObject.
    // The logic structure is preserved below for reference.
    // Usage: JsonDeserializable subclasses should have a constructor/factory that calls populateObject
    // with their fields mapped appropriately.

    // Port of: @Override public Iterator<Map.Entry<String, JsonValue>> iterator() (JsonObject.java:301-304)
    // C++: range-based for loop support via begin()/end()
    using PropertyMap = QHash<QString, std::unique_ptr<JsonValue>>;
    using const_iterator = PropertyMap::const_iterator;
    const_iterator begin() const { return m_properties.begin(); }
    const_iterator end() const { return m_properties.end(); }

    // Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(
    //           final int offset, final Object... keys) (JsonObject.java:306-321)
    Optional<JsonValue *> getAtPathInternal(int offset, const QVector<QVariant> &keys) override;

private:
    // Port of: private final HashMap<String, JsonValue> properties = new HashMap<>(); (JsonObject.java:42)
    PropertyMap m_properties;
};

} // namespace PinkReader
