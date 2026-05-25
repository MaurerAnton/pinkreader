/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: int_array_length_pair.h
 * Port of: IntArrayLengthPair.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/markdown/IntArrayLengthPair.java
 *
 * A reusable int array with a tracked write position. Every method ported exactly.
 */

#pragma once

#include <vector>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.markdown.IntArrayLengthPair
 *
 * Wraps an int array with a mutable position index. Provides append
 * and substring-as-array operations.
 */
class IntArrayLengthPair {
public:
	// Port of: public final int[] data;
	int* data;
	// Port of: public int pos = 0;
	int pos;

	// Port of: public IntArrayLengthPair(final int capacity)
	explicit IntArrayLengthPair(int capacity);

	// Destructor to free allocated data
	~IntArrayLengthPair();

	// Port of: public void clear()
	void clear();

	// Port of: public void append(final int[] arr)
	void append(const int* arr, int arrLen);

	// Port of: public void append(final char[] arr)
	void append(const char* arr, int arrLen);

	// Port of: public int[] substringAsArray(final int start)
	std::vector<int> substringAsArray(int start) const;

	// Prevent copy to avoid double-free
	IntArrayLengthPair(const IntArrayLengthPair&) = delete;
	IntArrayLengthPair& operator=(const IntArrayLengthPair&) = delete;

	// Allow move
	IntArrayLengthPair(IntArrayLengthPair&& other) noexcept
		: data(other.data), pos(other.pos) {
		other.data = nullptr;
		other.pos = 0;
	}
	IntArrayLengthPair& operator=(IntArrayLengthPair&& other) noexcept {
		if(this != &other) {
			delete[] data;
			data = other.data;
			pos = other.pos;
			other.data = nullptr;
			other.pos = 0;
		}
		return *this;
	}
};

} // namespace PinkReader
