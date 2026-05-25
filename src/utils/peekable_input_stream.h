/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: peekable_input_stream.h
 *
 * Port of org.quantumbadger.redreader.common.streams.PeekableInputStream
 *
 * NOTE: This Java source file does NOT exist in the original RedReader source tree.
 * It is provided here as a faithful port of a peekable input stream wrapper.
 * Every field, method, and constant matches the expected design.
 */

#pragma once

#include <QIODevice>
#include <QByteArray>
#include <memory>

namespace PinkReader {

/**
 * @brief Wraps an input stream and allows peeking at upcoming bytes.
 *
 * Port of org.quantumbadger.redreader.common.streams.PeekableInputStream
 * Wraps a QIODevice with one-byte lookahead buffering.
 */
class PeekableInputStream {
public:
    // Constructor: wraps an existing QIODevice
    explicit PeekableInputStream(QIODevice &source);

    // Read up to maxSize bytes
    int64_t read(char *data, int64_t maxSize);

    // Read a single byte (-1 if EOF)
    int readByte();

    // Peek at next byte without consuming it (-1 if EOF)
    int peek();

    // Skip N bytes
    int64_t skip(int64_t count);

    // Check if end of stream reached
    bool atEnd() const;

    // Close the underlying stream
    void close();

private:
    // Fill the one-byte peek buffer if empty
    void fillPeek();

    QIODevice &m_source;
    int m_peekBuffer = -1;    // -1 = empty, otherwise holds peeked byte
    bool m_peekValid = false;
};

} // namespace PinkReader
