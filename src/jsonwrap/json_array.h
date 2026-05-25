/*
 * PinkReader - GPLv3
 * File: json_array.h
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonArray.java
 * Every field, method, inner class matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonArray.java
 */

#pragma once

#include "json_value.h"

#include <QVector>
#include <memory>

namespace PinkReader {

// Forward declaration needed for forEachObject
class JsonObject;

// Port of: public final class JsonArray extends JsonValue implements Iterable<JsonValue> (JsonArray.java:34)
class JsonArray : public JsonValue {
public:
    // Port of: protected JsonArray(final JsonParser parser) throws IOException (JsonArray.java:38-54)
    explicit JsonArray(const QJsonArray &arr);

    // Port of: @NonNull @Override public JsonArray asArray() (JsonArray.java:56-60)
    JsonArray *asArray() override { return this; }

    // Port of: @NonNull public JsonValue get(final int id) (JsonArray.java:62-65)
    JsonValue *get(int id) const;

    // Port of: @Nullable public String getString(final int id) (JsonArray.java:67-70)
    QString getString(int id) const;

    // Port of: @Nullable public Long getLong(final int id) (JsonArray.java:72-75)
    std::optional<int64_t> getLong(int id) const;

    // Port of: @Nullable public Double getDouble(final int id) (JsonArray.java:77-80)
    std::optional<double> getDouble(int id) const;

    // Port of: @Nullable public Boolean getBoolean(final int id) (JsonArray.java:82-85)
    std::optional<bool> getBoolean(int id) const;

    // Port of: @Nullable public JsonObject getObject(final int id) (JsonArray.java:87-90)
    JsonObject *getObject(int id) const;

    // Port of: @Nullable public <E extends JsonObject.JsonDeserializable> E getObject( (JsonArray.java:92-102)
    //           final int id, final Class<E> clazz) throws ...
    // NOTE: Reflection-based deserialization. In C++, deserializable types handle their own
    // population from JsonObject. See JsonObject::populateObject for the ported logic.

    // Port of: @Nullable public JsonArray getArray(final int id) (JsonArray.java:104-107)
    JsonArray *getArray(int id) const;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonArray.java:114-135)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: public int size() (JsonArray.java:137-139)
    int size() const;

    // Port of: @Override public Iterator<JsonValue> iterator() (JsonArray.java:109-112)
    // C++: range-based for loop support via begin()/end()
    using iterator_type = QVector<std::unique_ptr<JsonValue>>::const_iterator;
    iterator_type begin() const { return m_contents.begin(); }
    iterator_type end() const { return m_contents.end(); }

    // Port of: public void forEachObject(final Consumer<JsonObject> consumer) (JsonArray.java:141-146)
    //   for(final JsonValue value : mContents) { consumer.consume(value.asObject()); }
    template<typename Consumer>
    void forEachObject(Consumer consumer) {
        for (const auto &val : m_contents) {
            consumer(val->asObject());
        }
    }

    // Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(
    //           final int offset, final Object... keys) (JsonArray.java:148-173)
    Optional<JsonValue *> getAtPathInternal(int offset, const QVector<QVariant> &keys) override;

private:
    // Port of: private final ArrayList<JsonValue> mContents = new ArrayList<>(16); (JsonArray.java:36)
    QVector<std::unique_ptr<JsonValue>> m_contents;
};

} // namespace PinkReader
