/*
 * PinkReader - GPLv3
 * File: json_null.cpp
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonNull.java
 * Every method and logic branch matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonNull.java
 */

#include "json_null.h"

namespace PinkReader {

// Port of: @NonNull public static final JsonNull INSTANCE = new JsonNull(); (JsonNull.java:24)
const JsonNull JsonNull::INSTANCE;

// Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonNull.java:28-31)
//   sb.append("null");
void JsonNull::prettyPrint(int /*indent*/, QString &sb) const
{
    // Port of: sb.append("null"); (JsonNull.java:30)
    sb += QStringLiteral("null");
}

} // namespace PinkReader
