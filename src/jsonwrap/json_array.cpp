/*
 * PinkReader - GPLv3
 * File: json_array.cpp
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonArray.java
 * Every method and logic branch matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonArray.java
 */

#include "json_array.h"
#include "json_object.h"

namespace PinkReader {

// Port of: protected JsonArray(final JsonParser parser) throws IOException (JsonArray.java:38-54)
//   if(parser.currentToken() != JsonToken.START_ARRAY) { throw new JsonParseException(...); }
//   parser.nextToken();
//   while(parser.currentToken() != JsonToken.END_ARRAY) { mContents.add(JsonValue.parse(parser)); }
//   parser.nextToken();
JsonArray::JsonArray(const QJsonArray &arr)
{
    // Port of: private final ArrayList<JsonValue> mContents = new ArrayList<>(16);    // (JsonArray.java:36)
    // Port of: while(parser.currentToken() != JsonToken.END_ARRAY) {                  // (JsonArray.java:49-51)
    //              mContents.add(JsonValue.parse(parser));
    //          }
    m_contents.reserve(arr.size());
    for (const QJsonValue &val : arr) {
        m_contents.append(JsonValue::parse(val));
    }
    // Port of: parser.nextToken();                                                    // (JsonArray.java:53)
    // (QJsonArray iteration consumes all, no explicit advance needed)
}

// Port of: @NonNull public JsonValue get(final int id) (JsonArray.java:62-65)
//   return mContents.get(id);
JsonValue *JsonArray::get(int id) const
{
    // Port of: return mContents.get(id); (JsonArray.java:64)
    return m_contents[id].get();
}

// Port of: @Nullable public String getString(final int id) (JsonArray.java:67-70)
//   return get(id).asString();
QString JsonArray::getString(int id) const
{
    // Port of: return get(id).asString(); (JsonArray.java:69)
    JsonValue *val = get(id);
    QString *str = val->asString();
    return (str != nullptr) ? *str : QString();
}

// Port of: @Nullable public Long getLong(final int id) (JsonArray.java:72-75)
//   return get(id).asLong();
std::optional<int64_t> JsonArray::getLong(int id) const
{
    // Port of: return get(id).asLong(); (JsonArray.java:74)
    return get(id)->asLongValue();
}

// Port of: @Nullable public Double getDouble(final int id) (JsonArray.java:77-80)
//   return get(id).asDouble();
std::optional<double> JsonArray::getDouble(int id) const
{
    // Port of: return get(id).asDouble(); (JsonArray.java:79)
    return get(id)->asDoubleValue();
}

// Port of: @Nullable public Boolean getBoolean(final int id) (JsonArray.java:82-85)
//   return get(id).asBoolean();
std::optional<bool> JsonArray::getBoolean(int id) const
{
    // Port of: return get(id).asBoolean(); (JsonArray.java:84)
    return get(id)->asBoolValue();
}

// Port of: @Nullable public JsonObject getObject(final int id) (JsonArray.java:87-90)
//   return get(id).asObject();
JsonObject *JsonArray::getObject(int id) const
{
    // Port of: return get(id).asObject(); (JsonArray.java:89)
    return get(id)->asObject();
}

// Port of: @Nullable public JsonArray getArray(final int id) (JsonArray.java:104-107)
//   return get(id).asArray();
JsonArray *JsonArray::getArray(int id) const
{
    // Port of: return get(id).asArray(); (JsonArray.java:106)
    return get(id)->asArray();
}

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonArray.java:114-135)
//   sb.append('[');
//   for(int item = 0; item < mContents.size(); item++) {
//       if(item != 0) sb.append(',');
//       sb.append('\n');
//       for(int i = 0; i < indent + 1; i++) sb.append("   ");
//       mContents.get(item).prettyPrint(indent + 1, sb);
//   }
//   sb.append('\n');
//   for(int i = 0; i < indent; i++) sb.append("   ");
//   sb.append(']');
void JsonArray::prettyPrint(int indent, QString &sb) const
{
    // Port of: sb.append('['); (JsonArray.java:117)
    sb += QChar('[');

    // Port of: for(int item = 0; item < mContents.size(); item++) { (JsonArray.java:119-128)
    for (int item = 0; item < m_contents.size(); item++) {
        // Port of: if(item != 0) { sb.append(','); } (JsonArray.java:120-122)
        if (item != 0) {
            sb += QChar(',');
        }
        // Port of: sb.append('\n'); (JsonArray.java:123)
        sb += QChar('\n');
        // Port of: for(int i = 0; i < indent + 1; i++) { sb.append("   "); } (JsonArray.java:124-126)
        for (int i = 0; i < indent + 1; i++) {
            sb += QStringLiteral("   ");
        }
        // Port of: mContents.get(item).prettyPrint(indent + 1, sb); (JsonArray.java:127)
        m_contents[item]->prettyPrint(indent + 1, sb);
    }

    // Port of: sb.append('\n'); (JsonArray.java:130)
    sb += QChar('\n');
    // Port of: for(int i = 0; i < indent; i++) { sb.append("   "); } (JsonArray.java:131-133)
    for (int i = 0; i < indent; i++) {
        sb += QStringLiteral("   ");
    }
    // Port of: sb.append(']'); (JsonArray.java:134)
    sb += QChar(']');
}

// Port of: public int size() (JsonArray.java:137-139)
//   return mContents.size();
int JsonArray::size() const
{
    // Port of: return mContents.size(); (JsonArray.java:138)
    return m_contents.size();
}

// Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(
//           final int offset, final Object... keys) (JsonArray.java:148-173)
//   if(offset == keys.length) { return Optional.of(this); }
//   if(!(keys[offset] instanceof Integer)) { return Optional.empty(); }
//   final int key = (Integer)keys[offset];
//   if(key < 0 || key >= mContents.size()) { return Optional.empty(); }
//   final JsonValue next = mContents.get(key);
//   if(next == null) { return Optional.empty(); }
//   return next.getAtPathInternal(offset + 1, keys);
Optional<JsonValue *> JsonArray::getAtPathInternal(int offset, const QVector<QVariant> &keys)
{
    // Port of: if(offset == keys.length) { return Optional.of(this); } (JsonArray.java:152-154)
    if (offset == keys.size()) {
        return Optional<JsonValue *>::of(this);
    }

    // Port of: if(!(keys[offset] instanceof Integer)) { return Optional.empty(); } (JsonArray.java:156-158)
    bool ok;
    int key = keys[offset].toInt(&ok);
    if (!ok) {
        return Optional<JsonValue *>::empty();
    }

    // Port of: final int key = (Integer)keys[offset]; (JsonArray.java:160)
    // (already got it above)

    // Port of: if(key < 0 || key >= mContents.size()) { return Optional.empty(); } (JsonArray.java:162-164)
    if (key < 0 || key >= m_contents.size()) {
        return Optional<JsonValue *>::empty();
    }

    // Port of: final JsonValue next = mContents.get(key); (JsonArray.java:166)
    JsonValue *next = m_contents[key].get();

    // Port of: if(next == null) { return Optional.empty(); } (JsonArray.java:168-170)
    if (next == nullptr) {
        return Optional<JsonValue *>::empty();
    }

    // Port of: return next.getAtPathInternal(offset + 1, keys); (JsonArray.java:172)
    return next->getAtPathInternal(offset + 1, keys);
}

} // namespace PinkReader
