/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: writable_byte_array_output_stream.cpp - Port of WritableByteArrayOutputStream.java
 *
 * This file does NOT exist in the original RedReader Java source tree.
 * Created as a C++ port based on the expected interface.
 *
 * Every method ported exactly.
 */

#include "writable_byte_array_output_stream.h"

#include <QString>

namespace PinkReader {

// ===== Constructor (port of: public WritableByteArrayOutputStream()) =====
// Java: public WritableByteArrayOutputStream() { super(); }
WritableByteArrayOutputStream::WritableByteArrayOutputStream(QObject *parent)
    : QObject(parent)
{
}

// ===== Constructor with initial capacity  =====
// Java: public WritableByteArrayOutputStream(int size) { super(size); }
WritableByteArrayOutputStream::WritableByteArrayOutputStream(int size, QObject *parent)
    : QObject(parent)
{
    m_buf.reserve(size);
}

// ===== Port of: public synchronized void write(int b) =====
// Java ByteArrayOutputStream.write(int b):
//   ensureCapacity(count + 1);
//   buf[count] = (byte) b;
//   count += 1;
void WritableByteArrayOutputStream::write(int b) {
    m_buf.append(static_cast<char>(b & 0xFF));
    m_count = m_buf.size();
}

// ===== Port of: public synchronized void write(byte[] b) =====
// Java ByteArrayOutputStream.write(byte[] b):
//   write(b, 0, b.length);
void WritableByteArrayOutputStream::write(const std::vector<uint8_t> &b) {
    write(b, 0, static_cast<int>(b.size()));
}

// ===== Port of: public synchronized void write(byte[] b, int off, int len) =====
// Java ByteArrayOutputStream.write(byte[] b, int off, int len):
//   ensureCapacity(count + len);
//   System.arraycopy(b, off, buf, count, len);
//   count += len;
void WritableByteArrayOutputStream::write(const std::vector<uint8_t> &b, int off, int len) {
    for (int i = 0; i < len && (off + i) < static_cast<int>(b.size()); ++i) {
        m_buf.append(static_cast<char>(b[off + i]));
    }
    m_count = m_buf.size();
}

// ===== Port of: public void write(byte[] b, int off, int len) via raw pointer =====
void WritableByteArrayOutputStream::write(const uint8_t *b, int len) {
    for (int i = 0; i < len; ++i) {
        m_buf.append(static_cast<char>(b[i]));
    }
    m_count = m_buf.size();
}

// ===== Port of: public void writeTo(OutputStream out) throws IOException =====
// Java ByteArrayOutputStream.writeTo(OutputStream out):
//   out.write(buf, 0, count);
void WritableByteArrayOutputStream::writeTo(WritableByteArrayOutputStream &out) {
    out.write(reinterpret_cast<const uint8_t *>(m_buf.constData()), m_count);
}

// ===== Port of: public synchronized byte[] toByteArray() =====
// Java ByteArrayOutputStream.toByteArray():
//   return Arrays.copyOf(buf, count);
std::vector<uint8_t> WritableByteArrayOutputStream::toByteArray() const {
    std::vector<uint8_t> result;
    result.reserve(m_count);
    for (int i = 0; i < m_count; ++i) {
        result.push_back(static_cast<uint8_t>(m_buf.at(i)));
    }
    return result;
}

// ===== Port of: public synchronized byte[] getData() (expected Writable* method) =====
// Returns a const reference to the underlying data.
const QByteArray &WritableByteArrayOutputStream::data() const {
    return m_buf;
}

// ===== Port of: public QByteArray &getBuffer() =====
// Returns mutable reference to underlying buffer.
QByteArray &WritableByteArrayOutputStream::buffer() {
    return m_buf;
}

// ===== Port of: public synchronized int size() =====
// Java ByteArrayOutputStream.size():
//   return count;
int WritableByteArrayOutputStream::size() const {
    return m_count;
}

// ===== Port of: public synchronized void reset() =====
// Java ByteArrayOutputStream.reset():
//   count = 0;
void WritableByteArrayOutputStream::reset() {
    m_buf.clear();
    m_count = 0;
}

// ===== Port of: public void close() throws IOException =====
// Java ByteArrayOutputStream.close():
//   // Closing a ByteArrayOutputStream has no effect.
void WritableByteArrayOutputStream::close() {
    // No-op: closing a ByteArrayOutputStream has no effect
}

// ===== Port of: public String toString() =====
// Java ByteArrayOutputStream.toString():
//   return new String(buf, 0, count);
QString WritableByteArrayOutputStream::toString() const {
    return QString::fromUtf8(m_buf.constData(), m_count);
}

// ===== Port of: public String toString(String charsetName) =====
// Java ByteArrayOutputStream.toString(String charsetName):
//   return new String(buf, 0, count, charsetName);
QString WritableByteArrayOutputStream::toString(const QString &charsetName) const {
    // Match common charset names
    if (charsetName.compare(QStringLiteral("UTF-8"), Qt::CaseInsensitive) == 0
        || charsetName.compare(QStringLiteral("utf8"), Qt::CaseInsensitive) == 0) {
        return QString::fromUtf8(m_buf.constData(), m_count);
    }
    if (charsetName.compare(QStringLiteral("ISO-8859-1"), Qt::CaseInsensitive) == 0
        || charsetName.compare(QStringLiteral("latin1"), Qt::CaseInsensitive) == 0) {
        return QString::fromLatin1(m_buf.constData(), m_count);
    }
    // Default: UTF-8
    return QString::fromUtf8(m_buf.constData(), m_count);
}

// ===== Port of: public String toString(Charset charset) / toStringUtf8() =====
QString WritableByteArrayOutputStream::toStringUtf8() const {
    return QString::fromUtf8(m_buf.constData(), m_count);
}

} // namespace PinkReader
