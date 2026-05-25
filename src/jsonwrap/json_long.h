/*
 * PinkReader - GPLv3
 * File: json_long.h
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonLong.java
 * Every field, method matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonLong.java
 */

#pragma once

#include "json_value.h"

namespace PinkReader {

// Port of: public class JsonLong extends JsonValue (JsonLong.java:22)
class JsonLong : public JsonValue {
public:
    // Port of: public JsonLong(final long value) (JsonLong.java:26-28)
    explicit JsonLong(int64_t value);

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonLong.java:30-33)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @NonNull @Override public String asString() (JsonLong.java:35-39)
    QString *asString() override;
    std::optional<QString> asStringValue() const override;

    // Port of: @NonNull @Override public Double asDouble() (JsonLong.java:41-45)
    double *asDouble() override;
    std::optional<double> asDoubleValue() const override;

    // Port of: @NonNull @Override public Long asLong() (JsonLong.java:47-51)
    int64_t *asLong() override;
    std::optional<int64_t> asLongValue() const override;

private:
    // Port of: private final long mValue (JsonLong.java:24)
    int64_t m_value;
};

} // namespace PinkReader
