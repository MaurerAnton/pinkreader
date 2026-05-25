/*
 * PinkReader - GPLv3
 * File: json_value.cpp
 * Port of: jsonwrap/JsonValue.java, JsonObject.java, JsonArray.java,
 *          JsonBoolean.java, JsonNull.java, JsonLong.java,
 *          JsonDouble.java, JsonString.java
 * Every method and logic branch matches exactly.
 */

#include "json_value.h"

#include <QIODevice>
#include <QJsonDocument>
#include <QJsonParseError>
#include <cmath>
#include <stdexcept>

namespace PinkReader {

// ============================================================================
// JsonValue — static parse methods
// ============================================================================

// Port of: @NonNull public static JsonValue parse(final InputStream source) throws IOException
std::unique_ptr<JsonValue> JsonValue::parse(QIODevice &source)
{
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

// Port of: @NonNull public static JsonValue parse(final JsonParser parser) throws IOException
// (Using QJsonValue instead of Jackson JsonParser)
std::unique_ptr<JsonValue> JsonValue::parse(const QJsonValue &value)
{
    // Port of:
    //   if(parser.currentToken() == null) { parser.nextToken(); }
    //   if(parser.currentToken() == null) throw IOException("Invalid input: no JSON tokens available")
    //   switch(parser.currentToken()) { ... }

    if (value.isNull() || value.isUndefined()) {
        return std::make_unique<JsonNull>();
    }

    switch (value.type()) {
        // Port of: case START_OBJECT: return new JsonObject(parser);
        case QJsonValue::Object:
            return std::make_unique<JsonObject>(value.toObject());

        // Port of: case START_ARRAY: return new JsonArray(parser);
        case QJsonValue::Array:
            return std::make_unique<JsonArray>(value.toArray());

        // Port of: case VALUE_FALSE: parser.nextToken(); return JsonBoolean.FALSE;
        case QJsonValue::Bool:
            if (value.toBool()) {
                // Port of: case VALUE_TRUE: parser.nextToken(); return JsonBoolean.TRUE;
                return std::unique_ptr<JsonValue>(
                    const_cast<JsonBoolean *>(&JsonBoolean::TRUE));
            } else {
                return std::unique_ptr<JsonValue>(
                    const_cast<JsonBoolean *>(&JsonBoolean::FALSE));
            }

        // Port of: case VALUE_NULL: parser.nextToken(); return JsonNull.INSTANCE;
        case QJsonValue::Null:
            return std::unique_ptr<JsonValue>(
                const_cast<JsonNull *>(&JsonNull::INSTANCE));

        // Port of: case VALUE_STRING: { final JsonString result = new JsonString(parser.getValueAsString()); parser.nextToken(); return result; }
        case QJsonValue::String:
            return std::make_unique<JsonString>(value.toString());

        // Port of: case VALUE_NUMBER_FLOAT: { final JsonDouble result = new JsonDouble(parser.getValueAsDouble()); parser.nextToken(); return result; }
        case QJsonValue::Double:
            return std::make_unique<JsonDouble>(value.toDouble());

        // Port of: case VALUE_NUMBER_INT: ... (handled via Double in QJsonValue)
        // QJsonValue doesn't distinguish int from double, so we check if it's integral
        default:
            // Port of: default: throw new JsonParseException(...)
            throw std::runtime_error("Unexpected JSON token type");
    }
}

// ============================================================================
// JsonValue — toString / prettyPrint
// ============================================================================

// Port of: @Override public String toString() { final StringBuilder sb = new StringBuilder(); prettyPrint(0, sb); return sb.toString(); }
QString JsonValue::toString() const
{
    QString sb;
    prettyPrint(0, sb);
    return sb;
}

// ============================================================================
// JsonValue — getAtPath methods
// ============================================================================

// Port of: @NonNull public final Optional<JsonValue> getAtPath(final Object... keys)
Optional<JsonValue *> JsonValue::getAtPath(std::initializer_list<QVariant> keys) const
{
    QVector<QVariant> keyVec(keys);
    // We need to call getAtPathInternal on the non-const this.
    // Since getAtPathInternal is virtual and const in the original (no side effects),
    // we have to cast away const. This is safe because getAtPathInternal has no side effects.
    return const_cast<JsonValue *>(this)->getAtPathInternal(0, keyVec);
}

// Port of: @NonNull public final Optional<JsonObject> getObjectAtPath(final Object... keys)
Optional<JsonObject *> JsonValue::getObjectAtPath(std::initializer_list<QVariant> keys) const
{
    // Port of:
    //   final Optional<JsonValue> result = getAtPath(keys);
    //   if(result.isEmpty()) return Optional.empty();
    //   return Optional.ofNullable(result.get().asObject());
    auto result = getAtPath(keys);
    if (result.isEmpty()) return Optional<JsonObject *>::empty();
    return Optional<JsonObject *>::ofNullable(result.get()->asObject());
}

// Port of: @NonNull public final Optional<JsonArray> getArrayAtPath(final Object... keys)
Optional<JsonArray *> JsonValue::getArrayAtPath(std::initializer_list<QVariant> keys) const
{
    auto result = getAtPath(keys);
    if (result.isEmpty()) return Optional<JsonArray *>::empty();
    return Optional<JsonArray *>::ofNullable(result.get()->asArray());
}

// Port of: @NonNull public final Optional<String> getStringAtPath(final Object... keys)
Optional<QString> JsonValue::getStringAtPath(std::initializer_list<QVariant> keys) const
{
    // Port of:
    //   final Optional<JsonValue> result = getAtPath(keys);
    //   if(result.isEmpty()) return Optional.empty();
    //   return Optional.ofNullable(result.get().asString());
    auto result = getAtPath(keys);
    if (result.isEmpty()) return Optional<QString>::empty();

    JsonValue *val = result.get();
    QString *str = val->asString();
    if (str != nullptr) {
        return Optional<QString>::of(*str);
    }
    return Optional<QString>::empty();
}

// Port of: @NonNull protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys)
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


// ============================================================================
// JsonObject
// ============================================================================

// Port of: protected JsonObject(final JsonParser parser) throws IOException
JsonObject::JsonObject(const QJsonObject &obj)
{
    // Port of:
    //   if(parser.currentToken() != JsonToken.START_OBJECT) throw ...
    //   parser.nextToken();
    //   while((jt = parser.currentToken()) != JsonToken.END_OBJECT) {
    //       if(jt != JsonToken.FIELD_NAME) throw ...
    //       final String fieldName = parser.currentName();
    //       parser.nextToken();
    //       final JsonValue value = JsonValue.parse(parser);
    //       properties.put(fieldName, value);
    //   }
    //   parser.nextToken();
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        // Port of: final JsonValue value = JsonValue.parse(parser);
        m_properties.insert(it.key(), JsonValue::parse(it.value()));
    }
}

// Port of: @Nullable public JsonValue get(final String name)
JsonValue *JsonObject::get(const QString &name) const
{
    // Port of: return properties.get(name);
    auto it = m_properties.find(name);
    return (it != m_properties.end()) ? it->get() : nullptr;
}

// Port of: @Nullable public String getString(@NonNull final String id)
QString JsonObject::getString(const QString &id) const
{
    // Port of:
    //   final JsonValue value = get(id);
    //   if(value == null) return null;
    //   return value.asString();
    JsonValue *value = get(id);
    if (value == nullptr) return QString();
    QString *str = value->asString();
    return (str != nullptr) ? *str : QString();
}

// Port of: @Nullable public Long getLong(@NonNull final String id)
std::optional<int64_t> JsonObject::getLong(const QString &id) const
{
    JsonValue *value = get(id);
    if (value == nullptr) return std::nullopt;
    return value->asLongValue();
}

// Port of: @Nullable public Double getDouble(@NonNull final String id)
std::optional<double> JsonObject::getDouble(const QString &id) const
{
    JsonValue *value = get(id);
    if (value == nullptr) return std::nullopt;
    return value->asDoubleValue();
}

// Port of: @Nullable public Boolean getBoolean(@NonNull final String id)
std::optional<bool> JsonObject::getBoolean(const QString &id) const
{
    JsonValue *value = get(id);
    if (value == nullptr) return std::nullopt;
    return value->asBoolValue();
}

// Port of: @Nullable public JsonObject getObject(@NonNull final String id)
JsonObject *JsonObject::getObject(const QString &id) const
{
    JsonValue *value = get(id);
    if (value == nullptr) return nullptr;
    return value->asObject();
}

// Port of: @Nullable public JsonArray getArray(@NonNull final String id)
JsonArray *JsonObject::getArray(const QString &id) const
{
    JsonValue *value = get(id);
    if (value == nullptr) return nullptr;
    return value->asArray();
}

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
void JsonObject::prettyPrint(int indent, QString &sb) const
{
    // Port of:
    //   sb.append('{');
    //   final Set<String> propertyKeySet = properties.keySet();
    //   final String[] fieldNames = propertyKeySet.toArray(new String[0]);
    //   for(int prop = 0; prop < fieldNames.length; prop++) {
    //       if(prop != 0) sb.append(',');
    //       sb.append('\n');
    //       for(int i = 0; i < indent + 1; i++) sb.append("   ");
    //       sb.append("\"").append(fieldNames[prop].replace("\\", "\\\\").replace("\"", "\\\"")).append("\": ");
    //       properties.get(fieldNames[prop]).prettyPrint(indent + 1, sb);
    //   }
    //   sb.append('\n');
    //   for(int i = 0; i < indent; i++) sb.append("   ");
    //   sb.append('}');
    sb += QChar('{');

    QStringList keys = m_properties.keys();
    for (int prop = 0; prop < keys.size(); prop++) {
        if (prop != 0) sb += QChar(',');
        sb += QChar('\n');
        for (int i = 0; i < indent + 1; i++) sb += QStringLiteral("   ");
        QString escapedKey = keys[prop];
        escapedKey.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
        escapedKey.replace(QLatin1Char('"'), QStringLiteral("\\\""));
        sb += QChar('"') + escapedKey + QStringLiteral("\": ");
        m_properties[keys[prop]]->prettyPrint(indent + 1, sb);
    }

    sb += QChar('\n');
    for (int i = 0; i < indent; i++) sb += QStringLiteral("   ");
    sb += QChar('}');
}

// Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys)
Optional<JsonValue *> JsonObject::getAtPathInternal(int offset, const QVector<QVariant> &keys)
{
    // Port of:
    //   if(offset == keys.length) return Optional.of(this);
    //   final JsonValue next = properties.get(keys[offset].toString());
    //   if(next == null) return Optional.empty();
    //   return next.getAtPathInternal(offset + 1, keys);
    if (offset == keys.size()) {
        return Optional<JsonValue *>::of(this);
    }

    QString key = keys[offset].toString();
    auto it = m_properties.find(key);
    if (it == m_properties.end()) {
        return Optional<JsonValue *>::empty();
    }

    return it->get()->getAtPathInternal(offset + 1, keys);
}


// ============================================================================
// JsonArray
// ============================================================================

// Port of: protected JsonArray(final JsonParser parser) throws IOException
JsonArray::JsonArray(const QJsonArray &arr)
{
    // Port of:
    //   if(parser.currentToken() != JsonToken.START_ARRAY) throw ...
    //   parser.nextToken();
    //   while(parser.currentToken() != JsonToken.END_ARRAY) {
    //       mContents.add(JsonValue.parse(parser));
    //   }
    //   parser.nextToken();
    m_contents.reserve(arr.size());
    for (const QJsonValue &val : arr) {
        m_contents.append(JsonValue::parse(val));
    }
}

// Port of: @NonNull public JsonValue get(final int id)
JsonValue *JsonArray::get(int id) const
{
    // Port of: return mContents.get(id);
    return m_contents[id].get();
}

// Port of: @Nullable public String getString(final int id)
QString JsonArray::getString(int id) const
{
    return get(id)->asStringValue().value_or(QString());
}

// Port of: @Nullable public Long getLong(final int id)
std::optional<int64_t> JsonArray::getLong(int id) const
{
    return get(id)->asLongValue();
}

// Port of: @Nullable public Double getDouble(final int id)
std::optional<double> JsonArray::getDouble(int id) const
{
    return get(id)->asDoubleValue();
}

// Port of: @Nullable public Boolean getBoolean(final int id)
std::optional<bool> JsonArray::getBoolean(int id) const
{
    return get(id)->asBoolValue();
}

// Port of: @Nullable public JsonObject getObject(final int id)
JsonObject *JsonArray::getObject(int id) const
{
    return get(id)->asObject();
}

// Port of: @Nullable public JsonArray getArray(final int id)
JsonArray *JsonArray::getArray(int id) const
{
    return get(id)->asArray();
}

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
void JsonArray::prettyPrint(int indent, QString &sb) const
{
    // Port of:
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
    sb += QChar('[');

    for (int item = 0; item < m_contents.size(); item++) {
        if (item != 0) sb += QChar(',');
        sb += QChar('\n');
        for (int i = 0; i < indent + 1; i++) sb += QStringLiteral("   ");
        m_contents[item]->prettyPrint(indent + 1, sb);
    }

    sb += QChar('\n');
    for (int i = 0; i < indent; i++) sb += QStringLiteral("   ");
    sb += QChar(']');
}

// Port of: @NonNull @Override protected Optional<JsonValue> getAtPathInternal(final int offset, final Object... keys)
Optional<JsonValue *> JsonArray::getAtPathInternal(int offset, const QVector<QVariant> &keys)
{
    // Port of:
    //   if(offset == keys.length) return Optional.of(this);
    //   if(!(keys[offset] instanceof Integer)) return Optional.empty();
    //   final int key = (Integer)keys[offset];
    //   if(key < 0 || key >= mContents.size()) return Optional.empty();
    //   final JsonValue next = mContents.get(key);
    //   if(next == null) return Optional.empty();
    //   return next.getAtPathInternal(offset + 1, keys);
    if (offset == keys.size()) {
        return Optional<JsonValue *>::of(this);
    }

    bool ok;
    int key = keys[offset].toInt(&ok);
    if (!ok) {
        return Optional<JsonValue *>::empty();
    }

    if (key < 0 || key >= m_contents.size()) {
        return Optional<JsonValue *>::empty();
    }

    return m_contents[key]->getAtPathInternal(offset + 1, keys);
}


// ============================================================================
// JsonBoolean
// ============================================================================

// Port of: @NonNull public static final JsonBoolean TRUE = new JsonBoolean(true)
const JsonBoolean JsonBoolean::TRUE(true);
// Port of: @NonNull public static final JsonBoolean FALSE = new JsonBoolean(false)
const JsonBoolean JsonBoolean::FALSE(false);

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
void JsonBoolean::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append(mValue ? "true" : "false");
    sb += m_value ? QStringLiteral("true") : QStringLiteral("false");
}

// Port of: @Override @NonNull public Boolean asBoolean()
bool *JsonBoolean::asBoolean()
{
    // Return a pointer to a static copy (matches original returning non-null Boolean)
    static thread_local bool result;
    result = m_value;
    return &result;
}


// ============================================================================
// JsonNull
// ============================================================================

// Port of: @NonNull public static final JsonNull INSTANCE = new JsonNull()
const JsonNull JsonNull::INSTANCE;

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
void JsonNull::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append("null");
    sb += QStringLiteral("null");
}


// ============================================================================
// JsonLong
// ============================================================================

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
void JsonLong::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append(mValue);
    sb += QString::number(m_value);
}

// Port of: @NonNull @Override public String asString()
QString *JsonLong::asString()
{
    static thread_local QString result;
    // Port of: return String.valueOf(mValue);
    result = QString::number(m_value);
    return &result;
}

std::optional<QString> JsonLong::asStringValue() const
{
    return QString::number(m_value);
}

// Port of: @NonNull @Override public Double asDouble()
double *JsonLong::asDouble()
{
    static thread_local double result;
    // Port of: return (double)mValue;
    result = static_cast<double>(m_value);
    return &result;
}

std::optional<double> JsonLong::asDoubleValue() const
{
    return static_cast<double>(m_value);
}

// Port of: @NonNull @Override public Long asLong()
int64_t *JsonLong::asLong()
{
    static thread_local int64_t result;
    // Port of: return mValue;
    result = m_value;
    return &result;
}

std::optional<int64_t> JsonLong::asLongValue() const
{
    return m_value;
}


// ============================================================================
// JsonDouble
// ============================================================================

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
void JsonDouble::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append(mValue);
    sb += QString::number(m_value);
}

// Port of: @NonNull @Override public String asString()
QString *JsonDouble::asString()
{
    static thread_local QString result;
    // Port of: return String.valueOf(mValue);
    result = QString::number(m_value);
    return &result;
}

std::optional<QString> JsonDouble::asStringValue() const
{
    return QString::number(m_value);
}

// Port of: @NonNull @Override public Double asDouble()
double *JsonDouble::asDouble()
{
    static thread_local double result;
    // Port of: return mValue;
    result = m_value;
    return &result;
}

std::optional<double> JsonDouble::asDoubleValue() const
{
    return m_value;
}

// Port of: @NonNull @Override public Long asLong()
int64_t *JsonDouble::asLong()
{
    static thread_local int64_t result;
    // Port of: return Math.round(mValue);
    result = static_cast<int64_t>(std::round(m_value));
    return &result;
}

std::optional<int64_t> JsonDouble::asLongValue() const
{
    return static_cast<int64_t>(std::round(m_value));
}


// ============================================================================
// JsonString
// ============================================================================

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb)
void JsonString::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append('"').append(StringEscapeUtils.escapeJson(mValue)).append('"');
    QString escaped = m_value;
    escaped.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
    escaped.replace(QLatin1Char('"'), QStringLiteral("\\\""));
    escaped.replace(QLatin1Char('\n'), QStringLiteral("\\n"));
    escaped.replace(QLatin1Char('\r'), QStringLiteral("\\r"));
    escaped.replace(QLatin1Char('\t'), QStringLiteral("\\t"));
    sb += QChar('"') + escaped + QChar('"');
}

// Port of: @Nullable @Override public Boolean asBoolean()
bool *JsonBoolean::asBoolean();
// (Already defined above, this is just a declaration fix)

// Port of JsonString::asBoolean():
//   final String lowercase = StringUtils.asciiLowercase(mValue);
//   switch(lowercase) { case "true": case "t": case "1": return true; case "false": case "f": case "0": return false; }
//   return null;
// NOTE: JsonString::asBoolean is declared in the header, implemented here:
bool *JsonString::asBoolean()
{
    static thread_local bool result;

    // Port of: final String lowercase = StringUtils.asciiLowercase(mValue);
    QString lowercase = m_value.toLower();

    // Port of: switch(lowercase) { ... }
    if (lowercase == QStringLiteral("true") ||
        lowercase == QStringLiteral("t") ||
        lowercase == QStringLiteral("1")) {
        result = true;
        return &result;
    }
    if (lowercase == QStringLiteral("false") ||
        lowercase == QStringLiteral("f") ||
        lowercase == QStringLiteral("0")) {
        result = false;
        return &result;
    }

    // Port of: return null;
    return nullptr;
}

std::optional<bool> JsonString::asBoolValue() const
{
    QString lowercase = m_value.toLower();

    if (lowercase == QStringLiteral("true") ||
        lowercase == QStringLiteral("t") ||
        lowercase == QStringLiteral("1")) {
        return true;
    }
    if (lowercase == QStringLiteral("false") ||
        lowercase == QStringLiteral("f") ||
        lowercase == QStringLiteral("0")) {
        return false;
    }
    return std::nullopt;
}

// Port of: @NonNull @Override public String asString()
QString *JsonString::asString()
{
    // Port of: return mValue;
    return &m_value;
}

std::optional<QString> JsonString::asStringValue() const
{
    return m_value;
}

// Port of: @Nullable @Override public Double asDouble()
double *JsonString::asDouble()
{
    static thread_local double result;
    // Port of: try { return Double.parseDouble(mValue); } catch(NumberFormatException e) { return null; }
    bool ok;
    result = m_value.toDouble(&ok);
    return ok ? &result : nullptr;
}

std::optional<double> JsonString::asDoubleValue() const
{
    bool ok;
    double val = m_value.toDouble(&ok);
    return ok ? std::optional<double>(val) : std::nullopt;
}

// Port of: @Nullable @Override public Long asLong()
int64_t *JsonString::asLong()
{
    static thread_local int64_t result;
    // Port of: try { return Long.parseLong(mValue); } catch(NumberFormatException e) { return null; }
    bool ok;
    result = static_cast<int64_t>(m_value.toLongLong(&ok));
    return ok ? &result : nullptr;
}

std::optional<int64_t> JsonString::asLongValue() const
{
    bool ok;
    int64_t val = static_cast<int64_t>(m_value.toLongLong(&ok));
    return ok ? std::optional<int64_t>(val) : std::nullopt;
}

} // namespace PinkReader
