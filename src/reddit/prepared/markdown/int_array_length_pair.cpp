/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: int_array_length_pair.cpp
 * Port of: IntArrayLengthPair.java
 *
 * Every method ported exactly, line-by-line.
 */

#include "int_array_length_pair.h"

#include <cstring>
#include <vector>

namespace PinkReader {

// ===== Constructor =====
// Port of:
//   public IntArrayLengthPair(final int capacity) {
//       this.data = new int[capacity];
//   }
IntArrayLengthPair::IntArrayLengthPair(int capacity)
	: data(new int[capacity]), pos(0) {}

// ===== Destructor =====
IntArrayLengthPair::~IntArrayLengthPair() {
	delete[] data;
}

// ===== clear =====
// Port of:
//   public void clear() {
//       pos = 0;
//   }
void IntArrayLengthPair::clear() {
	pos = 0;
}

// ===== append (int[]) =====
// Port of:
//   public void append(final int[] arr) {
//       System.arraycopy(arr, 0, data, pos, arr.length);
//       pos += arr.length;
//   }
void IntArrayLengthPair::append(const int* arr, int arrLen) {
	std::memcpy(data + pos, arr, static_cast<size_t>(arrLen) * sizeof(int));
	pos += arrLen;
}

// ===== append (char[]) =====
// Port of:
//   public void append(final char[] arr) {
//       for(int i = 0; i < arr.length; i++) {
//           data[pos + i] = arr[i];
//       }
//       pos += arr.length;
//   }
void IntArrayLengthPair::append(const char* arr, int arrLen) {
	for(int i = 0; i < arrLen; i++) {
		data[pos + i] = static_cast<int>(static_cast<unsigned char>(arr[i]));
	}
	pos += arrLen;
}

// ===== substringAsArray =====
// Port of:
//   public int[] substringAsArray(final int start) {
//       final int[] result = new int[pos - start];
//       System.arraycopy(data, start, result, 0, result.length);
//       return result;
//   }
std::vector<int> IntArrayLengthPair::substringAsArray(int start) const {
	std::vector<int> result(static_cast<size_t>(pos - start));
	std::memcpy(result.data(), data + start, static_cast<size_t>(pos - start) * sizeof(int));
	return result;
}

} // namespace PinkReader
