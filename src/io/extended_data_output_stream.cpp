/*
 * PinkReader - GPLv3
 * File: extended_data_output_stream.cpp
 * Exact port of ExtendedDataOutputStream.java method bodies
 *
 * Port of: redreader/src/main/java/org/quantumbadger/redreader/io/ExtendedDataOutputStream.java
 */

#include "extended_data_output_stream.h"

namespace PinkReader {

// ===== Constructor =====
// Java: public ExtendedDataOutputStream(final OutputStream out) { super(out); }
ExtendedDataOutputStream::ExtendedDataOutputStream(std::ostream &out)
    : m_out(&out) {}

// ===== writeNullableBoolean =====
// Port of: public void writeNullableBoolean(@Nullable final Boolean value) throws IOException
// Java lines 32-41
void ExtendedDataOutputStream::writeNullableBoolean(const std::optional<bool> &value) {
    if (!value.has_value()) {
        // Port of: if(value == null) { writeBoolean(false); }
        writeBoolean(false);
    } else {
        // Port of: else { writeBoolean(true); writeBoolean(value); }
        writeBoolean(true);
        writeBoolean(value.value());
    }
}

// ===== writeBoolean =====
void ExtendedDataOutputStream::writeBoolean(bool value) {
    writeRawByte(value ? 1 : 0);
}

// ===== writeInt =====
void ExtendedDataOutputStream::writeInt(int32_t value) {
    writeRawByte(static_cast<uint8_t>((value >> 24) & 0xFF));
    writeRawByte(static_cast<uint8_t>((value >> 16) & 0xFF));
    writeRawByte(static_cast<uint8_t>((value >> 8) & 0xFF));
    writeRawByte(static_cast<uint8_t>(value & 0xFF));
}

// ===== flush =====
void ExtendedDataOutputStream::flush() {
    if (m_out) {
        m_out->flush();
    }
}

// ===== close =====
void ExtendedDataOutputStream::close() {
    if (m_out) {
        m_out->flush();
        m_out = nullptr;
    }
}

void ExtendedDataOutputStream::writeRawByte(uint8_t byte) {
    if (m_out) {
        m_out->put(static_cast<char>(byte));
    }
}

} // namespace PinkReader
