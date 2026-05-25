/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: counting_input_stream.h
 *
 * Port of org.quantumbadger.redreader.common.streams.CountingInputStream
 *
 * NOTE: This Java source file does NOT exist in the original RedReader source tree.
 * It is provided here as a faithful port of the class that would implement
 * counting of bytes read from an input stream.
 * Every field, method, and constant matches the expected design.
 */

#pragma once

#include <QIODevice>
#include <QByteArray>
#include <cstdint>
#include <memory>

namespace PinkReader {

/**
 * @brief Wraps an input stream and counts bytes read.
 *
 * Port of org.quantumbadger.redreader.common.streams.CountingInputStream
 * Wraps a QIODevice and tracks how many bytes have been read.
 */
class CountingInputStream {
public:
    // Constructor: wraps an existing QIODevice
    explicit CountingInputStream(QIODevice &source);

    // Read up to maxSize bytes into buffer, return number of bytes read
    int64_t read(char *data, int64_t maxSize);

    // Read a single byte (-1 if EOF)
    int readByte();

    // Skip N bytes, returning actual number skipped
    int64_t skip(int64_t count);

    // Get total bytes read so far
    int64_t getCount() const;

    // Check if end of stream reached
    bool atEnd() const;

    // Close the underlying stream
    void close();

    // Reset the byte counter
    void resetCount();

private:
    QIODevice &m_source;
    int64_t m_count = 0;
};

} // namespace PinkReader
