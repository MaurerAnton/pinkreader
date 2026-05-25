/*
 * PinkReader - GPLv3
 * File: json_string.h
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonString.java
 * Every field, method matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonString.java
 */

#pragma once

#include "json_value.h"

namespace PinkReader {

// Port of: public class JsonString extends JsonValue (JsonString.java:25)
class JsonString : public JsonValue {
public:
    // Port of: protected JsonString(@NonNull final String value) (JsonString.java:29-31)
    explicit JsonString(const QString &value);

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonString.java:33-36)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @Nullable @Override public Boolean asBoolean() (JsonString.java:38-56)
    bool *asBoolean() override;
    std::optional<bool> asBoolValue() const override;

    // Port of: @NonNull @Override public String asString() (JsonString.java:58-62)
    QString *asString() override;
    std::optional<QString> asStringValue() const override;

    // Port of: @Nullable @Override public Double asDouble() (JsonString.java:64-74)
    double *asDouble() override;
    std::optional<double> asDoubleValue() const override;

    // Port of: @Nullable @Override public Long asLong() (JsonString.java:76-86)
    int64_t *asLong() override;
    std::optional<int64_t> asLongValue() const override;

private:
    // Port of: @NonNull private final String mValue (JsonString.java:27)
    QString m_value;
};

} // namespace PinkReader
