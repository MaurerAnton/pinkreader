/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: peekable_input_stream.cpp
 *
 * Port of PeekableInputStream (see header).
 */

#include "peekable_input_stream.h"

namespace PinkReader {

PeekableInputStream::PeekableInputStream(QIODevice &source)
    : m_source(source)
{
}

void PeekableInputStream::fillPeek() {
    if (!m_peekValid) {
        char c;
        int64_t n = m_source.read(&c, 1);
        if (n > 0) {
            m_peekBuffer = static_cast<unsigned char>(c);
            m_peekValid = true;
        } else {
            m_peekBuffer = -1;
            m_peekValid = true;
        }
    }
}

int64_t PeekableInputStream::read(char *data, int64_t maxSize) {
    if (maxSize <= 0) return 0;

    int64_t totalRead = 0;

    // First, consume any peeked byte
    if (m_peekValid && m_peekBuffer >= 0) {
        data[0] = static_cast<char>(m_peekBuffer);
        m_peekValid = false;
        m_peekBuffer = -1;
        totalRead = 1;

        if (maxSize == 1) {
            return 1;
        }
    }

    // Read remaining bytes from source
    int64_t extra = m_source.read(data + totalRead, maxSize - totalRead);
    if (extra > 0) {
        totalRead += extra;
    }

    return (totalRead > 0) ? totalRead : (m_peekBuffer >= 0 ? 0 : -1);
}

int PeekableInputStream::readByte() {
    fillPeek();
    if (m_peekBuffer >= 0) {
        int b = m_peekBuffer;
        m_peekValid = false;
        m_peekBuffer = -1;
        return b;
    }
    return -1;
}

int PeekableInputStream::peek() {
    fillPeek();
    return m_peekBuffer;
}

int64_t PeekableInputStream::skip(int64_t count) {
    int64_t skipped = 0;

    // Skip any peeked byte first
    if (m_peekValid && m_peekBuffer >= 0 && count > 0) {
        m_peekValid = false;
        m_peekBuffer = -1;
        skipped = 1;
    }

    if (count > skipped) {
        skipped += m_source.skip(count - skipped);
    }
    return skipped;
}

bool PeekableInputStream::atEnd() const {
    if (m_peekValid && m_peekBuffer >= 0) {
        return false; // still has data in peek buffer
    }
    return m_source.atEnd();
}

void PeekableInputStream::close() {
    m_source.close();
    m_peekValid = false;
    m_peekBuffer = -1;
}

} // namespace PinkReader
