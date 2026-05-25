/*
 * PinkReader - GPLv3
 * File: json_object.cpp
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonObject.java
 * Every method and logic branch matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonObject.java
 */

#include "json_object.h"

namespace PinkReader {

// Port of: protected JsonObject(final JsonParser parser) throws IOException (JsonObject.java:44-73)
//   if(parser.currentToken() != JsonToken.START_OBJECT) { throw new JsonParseException(...); }
//   parser.nextToken();
//   JsonToken jt;
//   while((jt = parser.currentToken()) != JsonToken.END_OBJECT) {
//       if(jt != JsonToken.FIELD_NAME) { throw new JsonParseException(...); }
//       final String fieldName = parser.currentName();
//       parser.nextToken();
//       final JsonValue value = JsonValue.parse(parser);
//       properties.put(fieldName, value);
//   }
//   parser.nextToken();
JsonObject::JsonObject(const QJsonObject &obj)
{
    // Port of: get("file_does_not_exist") will return null
    // Port of: while((jt = parser.currentToken()) != JsonToken.END_OBJECT) {           // (JsonObject.java:57-70)
    //              if(jt != JsonToken.FIELD_NAME) { throw ...; }
    //              final String fieldName = parser.currentName();
    //              parser.nextToken();
    //              final JsonValue value = JsonValue.parse(parser);
    //              properties.put(fieldName, value);
    //          }
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        // Port of: final String fieldName = parser.currentName();                      // (JsonObject.java:64)
        // Port of: parser.nextToken();                                                  // (JsonObject.java:66)
        // Port of: final JsonValue value = JsonValue.parse(parser);                    // (JsonObject.java:67)
        // Port of: properties.put(fieldName, value);                                   // (JsonObject.java:69)
        m_properties.insert(it.key(), JsonValue::parse(it.value()));
    }
    // Port of: parser.nextToken();                                                     // (JsonObject.java:72)
    // (QJsonObject iteration consumes all, no explicit advance needed)
}

// Port of: public boolean isEmpty() (JsonObject.java:75-77)
//   return properties.isEmpty();
bool JsonObject::isEmpty() const
{
    // Port of: return properties.isEmpty(); (JsonObject.java:76)
    return m_properties.isEmpty();
}

// Port of: @Nullable public JsonValue get(final String name) (JsonObject.java:98-101)
//   return properties.get(name);
JsonValue *JsonObject::get(const QString &name) const
{
    // Port of: return properties.get(name); (JsonObject.java:100)
    auto it = m_properties.find(name);
    return (it != m_properties.end()) ? it->get() : nullptr;
}

// Port of: @Nullable public String getString(@NonNull final String id) (JsonObject.java:103-113)
//   final JsonValue value = get(id);
//   if(value == null) { return null; }
//   return value.asString();
QString JsonObject::getString(const QString &id) const
{
    // Port of: final JsonValue value = get(id); (JsonObject.java:106)
    JsonValue *value = get(id);

    // Port of: if(value == null) { return null; } (JsonObject.java:108-110)
    if (value == nullptr) {
        return QString();
    }

    // Port of: return value.asString(); (JsonObject.java:112)
    QString *str = value->asString();
    return (str != nullptr) ? *str : QString();
}

// Port of: @Nullable public Long getLong(@NonNull final String id) (JsonObject.java:115-125)
//   final JsonValue value = get(id);
//   if(value == null) { return null; }
//   return value.asLong();
std::optional<int64_t> JsonObject::getLong(const QString &id) const
{
    // Port of: final JsonValue value = get(id); (JsonObject.java:118)
    JsonValue *value = get(id);

    // Port of: if(value == null) { return null; } (JsonObject.java:120-122)
    if (value == nullptr) {
        return std::nullopt;
    }

    // Port of: return value.asLong(); (JsonObject.java:124)
    return value->asLongValue();
}

// Port of: @Nullable public Double getDouble(@NonNull final String id) (JsonObject.java:127-137)
//   final JsonValue value = get(id);
//   if(value == null) { return null; }
//   return value.asDouble();
std::optional<double> JsonObject::getDouble(const QString &id) const
{
    // Port of: final JsonValue value = get(id); (JsonObject.java:130)
    JsonValue *value = get(id);

    // Port of: if(value == null) { return null; } (JsonObject.java:132-134)
    if (value == nullptr) {
        return std::nullopt;
    }

    // Port of: return value.asDouble(); (JsonObject.java:136)
    return value->asDoubleValue();
}

// Port of: @Nullable public Boolean getBoolean(@NonNull final String id) (JsonObject.java:139-149)
//   final JsonValue value = get(id);
//   if(value == null) { return null; }
//   return value.asBoolean();
std::optional<bool> JsonObject::getBoolean(const QString &id) const
{
    // Port of: final JsonValue value = get(id); (JsonObject.java:142)
    JsonValue *value = get(id);

    // Port of: if(value == null) { return null; } (JsonObject.java:144-146)
    if (value == nullptr) {
        return std::nullopt;
    }

    // Port of: return value.asBoolean(); (JsonObject.java:148)
    return value->asBoolValue();
}

// Port of: @Nullable public JsonObject getObject(@NonNull final String id) (JsonObject.java:151-161)
//   final JsonValue value = get(id);
//   if(value == null) { return null; }
//   return value.asObject();
JsonObject *JsonObject::getObject(const QString &id) const
{
    // Port of: final JsonValue value = get(id); (JsonObject.java:154)
    JsonValue *value = get(id);

    // Port of: if(value == null) { return null; } (JsonObject.java:156-158)
    if (value == nullptr) {
        return nullptr;
    }

    // Port of: return value.asObject(); (JsonObject.java:160)
    return value->asObject();
}

// Port of: @Nullable public JsonArray getArray(@NonNull final String id) (JsonObject.java:181-191)
//   final JsonValue value = get(id);
//   if(value == null) { return null; }
//   return value.asArray();
JsonArray *JsonObject::getArray(const QString &id) const
{
    // Port of: final JsonValue value = get(id); (JsonObject.java:184)
    JsonValue *value = get(id);

    // Port of: if(value == null) { return null; } (JsonObject.java:186-188)
    if (value == nullptr) {
        return nullptr;
    }

    // Port of: return value.asArray(); (JsonObject.java:190)
    return value->asArray();
}

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonObject.java:193-220)
//   sb.append('{');
//   final Set<String> propertyKeySet = properties.keySet();
//   final String[] fieldNames = propertyKeySet.toArray(new String[0]);
//   for(int prop = 0; prop < fieldNames.length; prop++) {
//       if(prop != 0) { sb.append(','); }
//       sb.append('\n');
//       for(int i = 0; i < indent + 1; i++) { sb.append("   "); }
//       sb.append('\"').append(fieldNames[prop].replace("\\", "\\\\").replace("\"", "\\\"")).append("\": ");
//       properties.get(fieldNames[prop]).prettyPrint(indent + 1, sb);
//   }
//   sb.append('\n');
//   for(int i = 0; i < indent; i++) { sb.append("   "); }
//   sb.append('}');
void JsonObject::prettyPrint(int indent, QString &sb) const
{
    // Port of: sb.append('{'); (JsonObject.java:196)
    sb += QChar('{');

    // Port of: final Set<String> propertyKeySet = properties.keySet();            // (JsonObject.java:198)
    // Port of: final String[] fieldNames = propertyKeySet.toArray(new String[0]); // (JsonObject.java:199)
    QStringList keys = m_properties.keys();

    // Port of: for(int prop = 0; prop < fieldNames.length; prop++) {              // (JsonObject.java:201-213)
    for (int prop = 0; prop < keys.size(); prop++) {
        // Port of: if(prop != 0) { sb.append(','); }                             // (JsonObject.java:202-204)
        if (prop != 0) {
            sb += QChar(',');
        }
        // Port of: sb.append('\n');                                               // (JsonObject.java:205)
        sb += QChar('\n');
        // Port of: for(int i = 0; i < indent + 1; i++) { sb.append("   "); }     // (JsonObject.java:206-208)
        for (int i = 0; i < indent + 1; i++) {
            sb += QStringLiteral("   ");
        }
        // Port of: sb.append("\"")                                                // (JsonObject.java:209-211)
        //           .append(fieldNames[prop].replace("\\", "\\\\").replace("\"", "\\\""))
        //           .append("\": ");
        QString escapedKey = keys[prop];
        escapedKey.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
        escapedKey.replace(QLatin1Char('"'), QStringLiteral("\\\""));
        sb += QChar('"') + escapedKey + QStringLiteral("\": ");

        // Port of: properties.get(fieldNames[prop]).prettyPrint(indent + 1, sb);  // (JsonObject.java:212)
        m_properties[keys[prop]]->prettyPrint(indent + 1, sb);
    }

    // Port of: sb.append('\n');                                                   // (JsonObject.java:215)
    sb += QChar('\n');
    // Port of: for(int i = 0; i < indent; i++) { sb.append("   "); }             // (JsonObject.java:216-218)
    for (int i = 0; i < indent; i++) {
        sb += QStringLiteral("   ");
    }
    // Port of: sb.append('}');                                                    // (JsonObject.java:219)
    sb += QChar('}');
}

// Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(
//           final int offset, final Object... keys) (JsonObject.java:306-321)
//   if(offset == keys.length) { return Optional.of(this); }
//   final JsonValue next = properties.get(keys[offset].toString());
//   if(next == null) { return Optional.empty(); }
//   return next.getAtPathInternal(offset + 1, keys);
Optional<JsonValue *> JsonObject::getAtPathInternal(int offset, const QVector<QVariant> &keys)
{
    // Port of: if(offset == keys.length) { return Optional.of(this); } (JsonObject.java:310-312)
    if (offset == keys.size()) {
        return Optional<JsonValue *>::of(this);
    }

    // Port of: final JsonValue next = properties.get(keys[offset].toString()); (JsonObject.java:314)
    QString key = keys[offset].toString();
    auto it = m_properties.find(key);

    // Port of: if(next == null) { return Optional.empty(); } (JsonObject.java:316-318)
    if (it == m_properties.end()) {
        return Optional<JsonValue *>::empty();
    }

    // Port of: return next.getAtPathInternal(offset + 1, keys); (JsonObject.java:320)
    return it->get()->getAtPathInternal(offset + 1, keys);
}

} // namespace PinkReader
