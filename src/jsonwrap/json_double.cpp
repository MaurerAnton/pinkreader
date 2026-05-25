/*
 * PinkReader - GPLv3
 * File: json_double.cpp
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonDouble.java
 * Every method and logic branch matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonDouble.java
 */

#include "json_double.h"
#include <cmath>

namespace PinkReader {

// Port of: protected JsonDouble(final double value) (JsonDouble.java:26-28)
//   mValue = value;
JsonDouble::JsonDouble(double value)
    : m_value(value)
{
}

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonDouble.java:30-33)
//   sb.append(mValue);
void JsonDouble::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append(mValue); (JsonDouble.java:32)
    sb += QString::number(m_value);
}

// Port of: @NonNull @Override public String asString() (JsonDouble.java:35-39)
//   return String.valueOf(mValue);
QString *JsonDouble::asString()
{
    // Port of: return String.valueOf(mValue); (JsonDouble.java:38)
    static thread_local QString result;
    result = QString::number(m_value);
    return &result;
}

std::optional<QString> JsonDouble::asStringValue() const
{
    // Port of: return String.valueOf(mValue); (JsonDouble.java:38)
    return QString::number(m_value);
}

// Port of: @NonNull @Override public Double asDouble() (JsonDouble.java:41-45)
//   return mValue;
double *JsonDouble::asDouble()
{
    // Port of: return mValue; (JsonDouble.java:44)
    static thread_local double result;
    result = m_value;
    return &result;
}

std::optional<double> JsonDouble::asDoubleValue() const
{
    // Port of: return mValue; (JsonDouble.java:44)
    return m_value;
}

// Port of: @NonNull @Override public Long asLong() (JsonDouble.java:47-51)
//   return Math.round(mValue);
int64_t *JsonDouble::asLong()
{
    // Port of: return Math.round(mValue); (JsonDouble.java:50)
    static thread_local int64_t result;
    result = static_cast<int64_t>(std::round(m_value));
    return &result;
}

std::optional<int64_t> JsonDouble::asLongValue() const
{
    // Port of: return Math.round(mValue); (JsonDouble.java:50)
    return static_cast<int64_t>(std::round(m_value));
}

} // namespace PinkReader
