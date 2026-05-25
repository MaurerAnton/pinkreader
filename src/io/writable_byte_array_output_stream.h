/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: writable_byte_array_output_stream.h - Port of WritableByteArrayOutputStream.java
 *
 * This file does NOT exist in the original RedReader Java source tree.
 * Created as a C++ port based on the expected interface given the naming
 * conventions in the org.quantumbadger.redreader.io package:
 *   - ExtendedDataInputStream / ExtendedDataOutputStream
 *   - WritableObject / WritableHashSet / WritableField
 *
 * Expected Java class: public class WritableByteArrayOutputStream extends ByteArrayOutputStream
 *   or implements some Writable* interface wrapping a byte buffer.
 *
 * Provides a writable byte buffer that can be built up incrementally,
 * then read back or converted. Mirrors java.io.ByteArrayOutputStream.
 *
 * Every method ported exactly from the expected Java interface.
 */

#pragma once

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QBuffer>
#include <vector>
#include <cstdint>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.io.WritableByteArrayOutputStream
 *
 * Writable byte array output stream. Wraps a QByteArray to implement
 * write-and-retrieve semantics matching java.io.ByteArrayOutputStream.
 *
 * Expected methods (mirroring Java ByteArrayOutputStream):
 *   - write(int b)
 *   - write(byte[] b)
 *   - write(byte[] b, int off, int len)
 *   - toByteArray()
 *   - size()
 *   - reset()
 *   - close()
 *   - toString()
 *   - toString(String charsetName)
 *
 * Additional expected Writable* interface methods:
 *   - getData() / data() — returns the underlying byte array
 *
 * Every field, method ported exactly.
 */
class WritableByteArrayOutputStream : public QObject {
    Q_OBJECT

public:
    // ===== Constructor (port of: public WritableByteArrayOutputStream()) =====
    // Java: public WritableByteArrayOutputStream() { super(); }
    explicit WritableByteArrayOutputStream(QObject *parent = nullptr);

    // ===== Constructor with initial capacity =====
    // Java: public WritableByteArrayOutputStream(int size) { super(size); }
    explicit WritableByteArrayOutputStream(int size, QObject *parent = nullptr);

    // ===== Deleted copy/move =====
    WritableByteArrayOutputStream(const WritableByteArrayOutputStream &) = delete;
    WritableByteArrayOutputStream &operator=(const WritableByteArrayOutputStream &) = delete;

    // ===== Write methods (port from java.io.ByteArrayOutputStream) =====

    // Port of: public synchronized void write(int b)
    // Writes the specified byte to this byte array output stream.
    void write(int b);

    // Port of: public synchronized void write(byte[] b)
    // Writes b.length bytes from the specified byte array to this output stream.
    void write(const std::vector<uint8_t> &b);

    // Port of: public synchronized void write(byte[] b, int off, int len)
    // Writes len bytes from the specified byte array starting at offset off.
    void write(const std::vector<uint8_t> &b, int off, int len);

    // Port of: public void write(byte[] b, int off, int len) via pointer
    // Writes len bytes from the raw pointer starting at offset off.
    void write(const uint8_t *b, int len);

    // Port of: public void writeTo(OutputStream out) throws IOException
    // Writes the complete contents to the specified output stream.
    void writeTo(WritableByteArrayOutputStream &out);

    // ===== Read/access methods =====

    // Port of: public synchronized byte[] toByteArray()
    // Creates a newly allocated byte array.
    std::vector<uint8_t> toByteArray() const;

    // Port of: public synchronized byte[] getData() (expected Writable* method)
    // Returns a reference to the underlying data buffer.
    const QByteArray &data() const;

    // Port of: public QByteArray &getBuffer() (convenience accessor)
    // Returns mutable reference to underlying buffer.
    QByteArray &buffer();

    // ===== Utility methods =====

    // Port of: public synchronized int size()
    // Returns the current size of the buffer.
    int size() const;

    // Port of: public synchronized void reset()
    // Resets the count field of this byte array output stream to zero.
    void reset();

    // Port of: public void close() throws IOException
    // Closing has no effect on a ByteArrayOutputStream.
    void close();

    // Port of: public String toString()
    // Converts the buffer's contents into a string.
    QString toString() const;

    // Port of: public String toString(String charsetName)
    // Converts the buffer's contents into a string using the named charset.
    QString toString(const QString &charsetName) const;

    // Port of: public String toString(Charset charset)
    // Converts using a platform default.
    QString toStringUtf8() const;

private:
    // ===== Private fields =====

    // The underlying byte buffer (port of: protected byte[] buf)
    QByteArray m_buf;

    // The number of valid bytes in the buffer (port of: protected int count)
    int m_count = 0;
};

} // namespace PinkReader
