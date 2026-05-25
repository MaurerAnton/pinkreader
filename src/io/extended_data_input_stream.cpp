/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: extended_data_input_stream.cpp - Port of ExtendedDataInputStream.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/io/ExtendedDataInputStream.java
 *
 * Every field, method, and constant ported exactly.
 */

#include "extended_data_input_stream.h"

namespace PinkReader {

// ===== Constructor (port of: public ExtendedDataInputStream(final InputStream in)) =====
// Java: public ExtendedDataInputStream(final InputStream in) { super(in); }
ExtendedDataInputStream::ExtendedDataInputStream(QIODevice *in, QObject *parent)
    : QObject(parent)
    , m_in(in)
    , m_stream(in)
{
    // super(in) equivalent: DataInputStream wraps the InputStream
    // In C++, QDataStream wraps QIODevice
}

// ===== Port of: public boolean readBoolean() (from DataInputStream superclass) =====
// Java DataInputStream.readBoolean(): reads one byte; 0 = false, non-zero = true
bool ExtendedDataInputStream::readBoolean() {
    quint8 byte;
    m_stream >> byte;
    return byte != 0;
}

// ===== Port of: @Nullable public Boolean readNullableBoolean() throws IOException =====
// Java lines 33-40:
//   if(!readBoolean()) {
//       return null;
//   }
//   return readBoolean();
std::optional<bool> ExtendedDataInputStream::readNullableBoolean() {
    if (!readBoolean()) {
        return std::nullopt;
    }
    return readBoolean();
}

} // namespace PinkReader
