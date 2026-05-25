/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: counting_input_stream.cpp
 *
 * Port of CountingInputStream (see header).
 */

#include "counting_input_stream.h"

namespace PinkReader {

CountingInputStream::CountingInputStream(QIODevice &source)
    : m_source(source)
{
}

int64_t CountingInputStream::read(char *data, int64_t maxSize) {
    int64_t bytesRead = m_source.read(data, maxSize);
    if (bytesRead > 0) {
        m_count += bytesRead;
    }
    return bytesRead;
}

int CountingInputStream::readByte() {
    char c;
    int64_t n = read(&c, 1);
    if (n <= 0) {
        return -1;
    }
    return static_cast<unsigned char>(c);
}

int64_t CountingInputStream::skip(int64_t count) {
    int64_t skipped = m_source.skip(count);
    if (skipped > 0) {
        m_count += skipped;
    }
    return skipped;
}

int64_t CountingInputStream::getCount() const {
    return m_count;
}

bool CountingInputStream::atEnd() const {
    return m_source.atEnd();
}

void CountingInputStream::close() {
    m_source.close();
}

void CountingInputStream::resetCount() {
    m_count = 0;
}

} // namespace PinkReader
