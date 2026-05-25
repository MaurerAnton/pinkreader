/*
 * PinkReader - GPLv3
 * File: json_boolean.cpp
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonBoolean.java
 * Every method and logic branch matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonBoolean.java
 */

#include "json_boolean.h"

namespace PinkReader {

// Port of: @NonNull public static final JsonBoolean TRUE = new JsonBoolean(true); (JsonBoolean.java:24)
const JsonBoolean JsonBoolean::TRUE(true);
// Port of: @NonNull public static final JsonBoolean FALSE = new JsonBoolean(false); (JsonBoolean.java:25)
const JsonBoolean JsonBoolean::FALSE(false);

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonBoolean.java:33-36)
//   sb.append(mValue ? "true" : "false");
void JsonBoolean::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append(mValue ? "true" : "false"); (JsonBoolean.java:35)
    sb += m_value ? QStringLiteral("true") : QStringLiteral("false");
}

// Port of: @Override @NonNull public Boolean asBoolean() (JsonBoolean.java:38-42)
//   return mValue;
bool *JsonBoolean::asBoolean()
{
    // Port of: return mValue; (JsonBoolean.java:41)
    static thread_local bool result;
    result = m_value;
    return &result;
}

} // namespace PinkReader
