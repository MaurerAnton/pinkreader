/*
 * PinkReader - GPLv3
 * File: json_double.h
 * Port of: org/quantumbadger/redreader/jsonwrap/JsonDouble.java
 * Every field, method matches exactly.
 * Origin: RedReader/src/main/java/org/quantumbadger/redreader/jsonwrap/JsonDouble.java
 */

#pragma once

#include "json_value.h"

namespace PinkReader {

// Port of: public class JsonDouble extends JsonValue (JsonDouble.java:22)
class JsonDouble : public JsonValue {
public:
    // Port of: protected JsonDouble(final double value) (JsonDouble.java:26-28)
    explicit JsonDouble(double value);

    // Port of: @Override protected void prettyPrint(final int indent, final StringBuilder sb) (JsonDouble.java:30-33)
    void prettyPrint(int indent, QString &sb) const override;

    // Port of: @NonNull @Override public String asString() (JsonDouble.java:35-39)
    QString *asString() override;
    std::optional<QString> asStringValue() const override;

    // Port of: @NonNull @Override public Double asDouble() (JsonDouble.java:41-45)
    double *asDouble() override;
    std::optional<double> asDoubleValue() const override;

    // Port of: @NonNull @Override public Long asLong() (JsonDouble.java:47-51)
    int64_t *asLong() override;
    std::optional<int64_t> asLongValue() const override;

private:
    // Port of: private final double mValue (JsonDouble.java:24)
    double m_value;
};

} // namespace PinkReader
