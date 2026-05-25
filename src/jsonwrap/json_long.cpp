/*
 * PinkReader - GPLv3
 * File: json_long.cpp
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonLong.java
 * Every method and logic branch matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonLong.java
 */

#include "json_long.h"

namespace PinkReader {

// Port of: public JsonLong(final long value) (JsonLong.java:26-28)
//   mValue = value;
JsonLong::JsonLong(int64_t value)
    : m_value(value)
{
}

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonLong.java:30-33)
//   sb.append(mValue);
void JsonLong::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append(mValue); (JsonLong.java:32)
    sb += QString::number(m_value);
}

// Port of: @NonNull @Override public String asString() (JsonLong.java:35-39)
//   return String.valueOf(mValue);
QString *JsonLong::asString()
{
    // Port of: return String.valueOf(mValue); (JsonLong.java:38)
    static thread_local QString result;
    result = QString::number(m_value);
    return &result;
}

std::optional<QString> JsonLong::asStringValue() const
{
    // Port of: return String.valueOf(mValue); (JsonLong.java:38)
    return QString::number(m_value);
}

// Port of: @NonNull @Override public Double asDouble() (JsonLong.java:41-45)
//   return (double)mValue;
double *JsonLong::asDouble()
{
    // Port of: return (double)mValue; (JsonLong.java:44)
    static thread_local double result;
    result = static_cast<double>(m_value);
    return &result;
}

std::optional<double> JsonLong::asDoubleValue() const
{
    // Port of: return (double)mValue; (JsonLong.java:44)
    return static_cast<double>(m_value);
}

// Port of: @NonNull @Override public Long asLong() (JsonLong.java:47-51)
//   return mValue;
int64_t *JsonLong::asLong()
{
    // Port of: return mValue; (JsonLong.java:50)
    static thread_local int64_t result;
    result = m_value;
    return &result;
}

std::optional<int64_t> JsonLong::asLongValue() const
{
    // Port of: return mValue; (JsonLong.java:50)
    return m_value;
}

} // namespace PinkReader
