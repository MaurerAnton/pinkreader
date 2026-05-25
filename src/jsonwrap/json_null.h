/*
 * PinkReader - GPLv3
 * File: json_null.h
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonNull.java
 * Every field, method matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonNull.java
 */

#pragma once

#include "json_value.h"

namespace PinkReader {

// Port of: public class JsonNull extends JsonValue (JsonNull.java:22)
class JsonNull : public JsonValue {
public:
    // Port of: @NonNull public static final JsonNull INSTANCE = new JsonNull(); (JsonNull.java:24)
    static const JsonNull INSTANCE;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonNull.java:28-31)
    void prettyPrint(int indent, QString &sb) const override;

private:
    // Port of: private JsonNull() {} (JsonNull.java:26)
    JsonNull() = default;

    // Allow JsonValue::parse to construct/reference JsonNull instances
    friend class JsonValue;
};

} // namespace PinkReader
