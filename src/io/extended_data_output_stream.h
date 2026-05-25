/*
 * PinkReader - GPLv3
 * File: extended_data_output_stream.h
 * Exact port of ExtendedDataOutputStream.java (42 lines)
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/ExtendedDataOutputStream.java
 *
 * Original: public class ExtendedDataOutputStream extends DataOutputStream
 *   public ExtendedDataOutputStream(final OutputStream out) { super(out); }
 *   public void writeNullableBoolean(@Nullable final Boolean value) throws IOException
 *
 * Every field, method, and logic branch ported exactly.
 */

#pragma once

#include <ostream>
#include <optional>
#include <cstdint>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.ExtendedDataOutputStream
 *
 * Extends DataOutputStream to provide writeNullableBoolean().
 *
 * In Java: public class ExtendedDataOutputStream extends DataOutputStream
 *
 * C++: Wraps std::ostream, providing binary write methods.
 */
class ExtendedDataOutputStream {
public:
    // ===== Constructor (port of: public ExtendedDataOutputStream(final OutputStream out)) =====
    // Java: public ExtendedDataOutputStream(final OutputStream out) { super(out); }
    explicit ExtendedDataOutputStream(std::ostream &out);

    // ===== Deleted copy/move =====
    ExtendedDataOutputStream(const ExtendedDataOutputStream &) = delete;
    ExtendedDataOutputStream &operator=(const ExtendedDataOutputStream &) = delete;

    // ===== Methods (port exactly from Java) =====

    // Port of: public void writeNullableBoolean(@Nullable final Boolean value) throws IOException
    // Java lines 32-41:
    //   if(value == null) { writeBoolean(false); }
    //   else { writeBoolean(true); writeBoolean(value); }
    void writeNullableBoolean(const std::optional<bool> &value);

    // Underlying write methods (port of super.writeBoolean, super.writeInt, etc.)
    void writeBoolean(bool value);
    void writeInt(int32_t value);

    // Flush (port of flush())
    void flush();

    // Port of: close()
    void close();

private:
    // ===== Private fields =====
    // Port of: the underlying OutputStream
    std::ostream *m_out;

    // Internal buffer for binary writes
    void writeRawByte(uint8_t byte);
};

} // namespace PinkReader
