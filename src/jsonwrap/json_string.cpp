/*
 * PinkReader - GPLv3
 * File: json_string.cpp
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonString.java
 * Every method and logic branch matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonString.java
 */

#include "json_string.h"

namespace PinkReader {

// Port of: protected JsonString(@NonNull final String value) (JsonString.java:29-31)
//   mValue = value;
JsonString::JsonString(const QString &value)
    : m_value(value)
{
}

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonString.java:33-36)
//   sb.append('"').append(StringEscapeUtils.escapeJson(mValue)).append('"');
void JsonString::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append('"').append(StringEscapeUtils.escapeJson(mValue)).append('"'); (JsonString.java:35)
    QString escaped = m_value;
    escaped.replace(QLatin1Char('\\'), QStringLiteral("\\\\"));
    escaped.replace(QLatin1Char('"'), QStringLiteral("\\\""));
    escaped.replace(QLatin1Char('\n'), QStringLiteral("\\n"));
    escaped.replace(QLatin1Char('\r'), QStringLiteral("\\r"));
    escaped.replace(QLatin1Char('\t'), QStringLiteral("\\t"));
    sb += QChar('"') + escaped + QChar('"');
}

// Port of: @Nullable @Override public Boolean asBoolean() (JsonString.java:38-56)
//   final String lowercase = StringUtils.asciiLowercase(mValue);
//   switch(lowercase) {
//       case "true": case "t": case "1": return true;
//       case "false": case "f": case "0": return false;
//   }
//   return null;
bool *JsonString::asBoolean()
{
    // Port of: final String lowercase = StringUtils.asciiLowercase(mValue); (JsonString.java:42)
    QString lowercase = m_value.toLower();

    // Port of: switch(lowercase) { case "true": case "t": case "1": return true; ... } (JsonString.java:44-53)
    if (lowercase == QStringLiteral("true") ||
        lowercase == QStringLiteral("t") ||
        lowercase == QStringLiteral("1")) {
        static thread_local bool result;
        result = true;
        return &result;
    }
    if (lowercase == QStringLiteral("false") ||
        lowercase == QStringLiteral("f") ||
        lowercase == QStringLiteral("0")) {
        static thread_local bool result;
        result = false;
        return &result;
    }

    // Port of: return null; (JsonString.java:55)
    return nullptr;
}

std::optional<bool> JsonString::asBoolValue() const
{
    // Port of: final String lowercase = StringUtils.asciiLowercase(mValue); (JsonString.java:42)
    QString lowercase = m_value.toLower();

    // Port of: switch(lowercase) { ... } (JsonString.java:44-55)
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

// Port of: @NonNull @Override public String asString() (JsonString.java:58-62)
//   return mValue;
QString *JsonString::asString()
{
    // Port of: return mValue; (JsonString.java:61)
    return &m_value;
}

std::optional<QString> JsonString::asStringValue() const
{
    // Port of: return mValue; (JsonString.java:61)
    return m_value;
}

// Port of: @Nullable @Override public Double asDouble() (JsonString.java:64-74)
//   try { return Double.parseDouble(mValue); } catch(NumberFormatException e) { return null; }
double *JsonString::asDouble()
{
    // Port of: try { return Double.parseDouble(mValue); } catch(NumberFormatException e) { return null; } (JsonString.java:68-73)
    static thread_local double result;
    bool ok;
    result = m_value.toDouble(&ok);
    return ok ? &result : nullptr;
}

std::optional<double> JsonString::asDoubleValue() const
{
    // Port of: try { return Double.parseDouble(mValue); } catch(NumberFormatException e) { return null; } (JsonString.java:68-73)
    bool ok;
    double val = m_value.toDouble(&ok);
    return ok ? std::optional<double>(val) : std::nullopt;
}

// Port of: @Nullable @Override public Long asLong() (JsonString.java:76-86)
//   try { return Long.parseLong(mValue); } catch(NumberFormatException e) { return null; }
int64_t *JsonString::asLong()
{
    // Port of: try { return Long.parseLong(mValue); } catch(NumberFormatException e) { return null; } (JsonString.java:80-85)
    static thread_local int64_t result;
    bool ok;
    result = static_cast<int64_t>(m_value.toLongLong(&ok));
    return ok ? &result : nullptr;
}

std::optional<int64_t> JsonString::asLongValue() const
{
    // Port of: try { return Long.parseLong(mValue); } catch(NumberFormatException e) { return null; } (JsonString.java:80-85)
    bool ok;
    int64_t val = static_cast<int64_t>(m_value.toLongLong(&ok));
    return ok ? std::optional<int64_t>(val) : std::nullopt;
}

} // namespace PinkReader
