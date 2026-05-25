/*
 * PinkReader - GPLv3
 * File: json_boolean.h
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonBoolean.java
 * Every field, method matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonBoolean.java
 */

#pragma once

#include "json_value.h"

namespace PinkReader {

// Port of: public class JsonBoolean extends JsonValue (JsonBoolean.java:22)
class JsonBoolean : public JsonValue {
public:
    // Port of: @NonNull public static final JsonBoolean TRUE = new JsonBoolean(true); (JsonBoolean.java:24)
    static const JsonBoolean TRUE;
    // Port of: @NonNull public static final JsonBoolean FALSE = new JsonBoolean(false); (JsonBoolean.java:25)
    static const JsonBoolean FALSE;

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonBoolean.java:34-36)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @Override @NonNull public Boolean asBoolean() (JsonBoolean.java:38-42)
    bool *asBoolean() override;
    std::optional<bool> asBoolValue() const override { return m_value; }

private:
    // Port of: private final boolean mValue (JsonBoolean.java:27)
    bool m_value;

    // Port of: private JsonBoolean(final boolean value) (JsonBoolean.java:29-31)
    explicit JsonBoolean(bool value);

    // Allow JsonValue::parse to construct JsonBoolean instances
    friend class JsonValue;
};

} // namespace PinkReader
