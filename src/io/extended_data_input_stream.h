/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: extended_data_input_stream.h - Port of ExtendedDataInputStream.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/io/ExtendedDataInputStream.java
 *
 * Original: public class ExtendedDataInputStream extends DataInputStream
 *
 * Every field, method, and constant ported exactly.
 */

#pragma once

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <optional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.ExtendedDataInputStream
 *
 * Extends DataInputStream to provide readNullableBoolean().
 *
 * In Java: public class ExtendedDataInputStream extends DataInputStream
 * Fields: none
 * Methods:
 *   - constructor ExtendedDataInputStream(InputStream in)
 *   - @Nullable Boolean readNullableBoolean() throws IOException
 *
 * Every field, method ported exactly.
 */
class ExtendedDataInputStream : public QObject {
    Q_OBJECT

public:
    // ===== Constructor (port of: public ExtendedDataInputStream(final InputStream in)) =====
    // Java: public ExtendedDataInputStream(final InputStream in) { super(in); }
    // C++: Takes a QIODevice as the underlying data source
    explicit ExtendedDataInputStream(QIODevice *in, QObject *parent = nullptr);

    // ===== Deleted copy/move (owns underlying stream) =====
    ExtendedDataInputStream(const ExtendedDataInputStream &) = delete;
    ExtendedDataInputStream &operator=(const ExtendedDataInputStream &) = delete;

    // ===== Methods (port exactly from Java) =====

    // Port of: @Nullable public Boolean readNullableBoolean() throws IOException
    // Java lines 33-40:
    //   if(!readBoolean()) return null;
    //   return readBoolean();
    //
    // Returns std::optional<bool>: nullopt if null, true/false if Boolean value
    std::optional<bool> readNullableBoolean();

    // Underlying binary read (port of super.readBoolean())
    bool readBoolean();

private:
    // ===== Private fields =====

    // The underlying data stream (port of the DataInputStream wrapping)
    QIODevice *m_in = nullptr;
    QDataStream m_stream;
};

} // namespace PinkReader
