/*
 * PinkReader - GPLv3
 * File: json_value.cpp
 * Port of: jsonwrap/JsonValue.java
 * Every method and logic branch matches exactly.
 */

#include "json_value.h"
#include "json_object.h"
#include "json_array.h"
#include "json_boolean.h"
#include "json_null.h"
#include "json_long.h"
#include "json_double.h"
#include "json_string.h"

#include <QIODevice>
#include <QJsonDocument>
#include <QJsonParseError>
#include <cmath>
#include <stdexcept>

namespace PinkReader {

// ============================================================================
// JsonValue — static parse methods
// ============================================================================

// Port of: @NonNull public static JsonValue parse(final InputStream source) throws IOException (JsonValue.java:35-37)
std::unique_ptr<JsonValue> JsonValue::parse(QIODevice &source)
{
    // Port of: return parse(new JsonFactory().createParser(source));
    QByteArray data = source.readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        throw std::runtime_error(
            QString("JSON parse error: %1").arg(error.errorString()).toStdString());
    }

    if (doc.isObject()) {
        return std::make_unique<JsonObject>(doc.object());
    } else if (doc.isArray()) {
        return std::make_unique<JsonArray>(doc.array());
    } else {
        // Handle single values
        return parse(QJsonValue(doc.object().isEmpty()
            ? (doc.isArray() ? QJsonValue(doc.array()) : QJsonValue())
            : QJsonValue(doc.object())));
    }
}

// Port of: @NonNull public static JsonValue parse(final JsonParser parser) throws IOException (JsonValue.java:39-94)
// (Using QJsonValue instead of Jackson JsonParser)
std::unique_ptr<JsonValue> JsonValue::parse(const QJsonValue &value)
{
    // Port of:
    //   if(parser.currentToken() == null) { parser.nextToken(); }                           // (JsonValue.java:42-44)
    //   if(parser.currentToken() == null) throw IOException("Invalid input...");            // (JsonValue.java:46-48)
    //   switch(parser.currentToken()) { ... }                                               // (JsonValue.java:50-93)

    if (value.isNull() || value.isUndefined()) {
        // Port of: case VALUE_NULL: parser.nextToken(); return JsonNull.INSTANCE;           // (JsonValue.java:66-68)
        return std::unique_ptr<JsonValue>(
            const_cast<JsonNull *>(&JsonNull::INSTANCE));
    }

    switch (value.type()) {
        // Port of: case START_OBJECT: return new JsonObject(parser);                        // (JsonValue.java:52-53)
        case QJsonValue::Object:
            return std::make_unique<JsonObject>(value.toObject());

        // Port of: case START_ARRAY: return new JsonArray(parser);                          // (JsonValue.java:55-56)
        case QJsonValue::Array:
            return std::make_unique<JsonArray>(value.toArray());

        // Port of: case VALUE_FALSE: parser.nextToken(); return JsonBoolean.FALSE;          // (JsonValue.java:58-60)
        // Port of: case VALUE_TRUE:  parser.nextToken(); return JsonBoolean.TRUE;           // (JsonValue.java:62-64)
        case QJsonValue::Bool:
            if (value.toBool()) {
                return std::unique_ptr<JsonValue>(
                    const_cast<JsonBoolean *>(&JsonBoolean::TRUE));
            } else {
                return std::unique_ptr<JsonValue>(
                    const_cast<JsonBoolean *>(&JsonBoolean::FALSE));
            }

        // Port of: case VALUE_NULL: parser.nextToken(); return JsonNull.INSTANCE;           // (JsonValue.java:66-68)
        case QJsonValue::Null:
            return std::unique_ptr<JsonValue>(
                const_cast<JsonNull *>(&JsonNull::INSTANCE));

        // Port of: case VALUE_STRING: { final JsonString result = new JsonString(           // (JsonValue.java:70-74)
        //           parser.getValueAsString()); parser.nextToken(); return result; }
        case QJsonValue::String:
            return std::make_unique<JsonString>(value.toString());

        // Port of: case VALUE_NUMBER_FLOAT: { final JsonDouble result = new JsonDouble(     // (JsonValue.java:76-80)
        //           parser.getValueAsDouble()); parser.nextToken(); return result; }
        case QJsonValue::Double:
            return std::make_unique<JsonDouble>(value.toDouble());

        // Port of: default: throw new JsonParseException(parser,                            // (JsonValue.java:88-92)
        //           "Expecting an object, literal, or array, got: " + parser.currentToken(),
        //           parser.currentLocation());
        default:
            throw std::runtime_error("Unexpected JSON token type");
    }
}

// ============================================================================
// JsonValue — toString / prettyPrint
// ============================================================================

// Port of: @Override public String toString() (JsonValue.java:143-148)
//   final StringBuilder sb = new StringBuilder();
//   prettyPrint(0, sb);
//   return sb.toString();
QString JsonValue::toString() const
{
    QString sb;
    prettyPrint(0, sb);
    return sb;
}

// ============================================================================
// JsonValue — getAtPath methods
// ============================================================================

// Port of: @NonNull public final Optional<JsonValue> getAtPath(final Object... keys) (JsonValue.java:152-155)
//   return getAtPathInternal(0, keys);
Optional<JsonValue *> JsonValue::getAtPath(std::initializer_list<QVariant> keys) const
{
    QVector<QVariant> keyVec(keys);
    // getAtPathInternal is virtual. Safe to const_cast since it has no side effects.
    return const_cast<JsonValue *>(this)->getAtPathInternal(0, keyVec);
}

// Port of: @NonNull public final Optional<JsonObject> getObjectAtPath(final Object... keys) (JsonValue.java:157-167)
//   final Optional<JsonValue> result = getAtPath(keys);
//   if(result.isEmpty()) return Optional.empty();
//   return Optional.ofNullable(result.get().asObject());
Optional<JsonObject *> JsonValue::getObjectAtPath(std::initializer_list<QVariant> keys) const
{
    auto result = getAtPath(keys);
    if (result.isEmpty()) return Optional<JsonObject *>::empty();
    return Optional<JsonObject *>::ofNullable(result.get()->asObject());
}

// Port of: @NonNull public final Optional<JsonArray> getArrayAtPath(final Object... keys) (JsonValue.java:169-179)
//   final Optional<JsonValue> result = getAtPath(keys);
//   if(result.isEmpty()) return Optional.empty();
//   return Optional.ofNullable(result.get().asArray());
Optional<JsonArray *> JsonValue::getArrayAtPath(std::initializer_list<QVariant> keys) const
{
    auto result = getAtPath(keys);
    if (result.isEmpty()) return Optional<JsonArray *>::empty();
    return Optional<JsonArray *>::ofNullable(result.get()->asArray());
}

// Port of: @NonNull public final Optional<String> getStringAtPath(final Object... keys) (JsonValue.java:181-191)
//   final Optional<JsonValue> result = getAtPath(keys);
//   if(result.isEmpty()) return Optional.empty();
//   return Optional.ofNullable(result.get().asString());
Optional<QString> JsonValue::getStringAtPath(std::initializer_list<QVariant> keys) const
{
    auto result = getAtPath(keys);
    if (result.isEmpty()) return Optional<QString>::empty();

    JsonValue *val = result.get();
    QString *str = val->asString();
    if (str != nullptr) {
        return Optional<QString>::of(*str);
    }
    return Optional<QString>::empty();
}

// Port of: @NonNull protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys) (JsonValue.java:194-203)
// Default implementation:
//   if(offset == keys.length) return Optional.of(this);
//   return Optional.empty();
Optional<JsonValue *> JsonValue::getAtPathInternal(int offset, const QVector<QVariant> &keys)
{
    if (offset == keys.size()) {
        return Optional<JsonValue *>::of(this);
    }
    return Optional<JsonValue *>::empty();
}

} // namespace PinkReader
