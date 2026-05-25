/*
 * PinkReader - GPLv3
 * File: collections/stack.h
 * Port of: org.quantumbadger.redreader.common.collections.Stack.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include <vector>
#include <optional>
#include <algorithm>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.collections.Stack<E>
 *
 * Original: public class Stack<E> {
 *     private final ArrayList<E> mData;
 *     public Stack(final int initialCapacity);
 *     public void push(final E obj);
 *     public E pop();
 *     public boolean isEmpty();
 *     public boolean remove(final E obj);
 *     @Nullable public E peek();
 * }
 */
template<typename E>
class Stack {
public:
	// Port of: public Stack(final int initialCapacity)
	//   mData = new ArrayList<>(initialCapacity);
	explicit Stack(int initialCapacity = 0) {
		if (initialCapacity > 0) {
			m_data.reserve(static_cast<size_t>(initialCapacity));
		}
	}

	// Port of: public void push(final E obj)
	//   mData.add(obj);
	void push(const E &obj) {
		m_data.push_back(obj);
	}

	void push(E &&obj) {
		m_data.push_back(std::move(obj));
	}

	// Port of: public E pop()
	//   return mData.remove(mData.size() - 1);
	E pop() {
		E result = std::move(m_data.back());
		m_data.pop_back();
		return result;
	}

	// Port of: public boolean isEmpty()
	//   return mData.isEmpty();
	bool isEmpty() const {
		return m_data.empty();
	}

	// Port of: public boolean remove(final E obj)
	//   return mData.remove(obj);
	bool remove(const E &obj) {
		auto iter = std::find(m_data.begin(), m_data.end(), obj);
		if (iter != m_data.end()) {
			m_data.erase(iter);
			return true;
		}
		return false;
	}

	// Port of: @Nullable public E peek()
	//   if(isEmpty()) { return null; }
	//   return mData.get(mData.size() - 1);
	std::optional<E> peek() const {
		if (isEmpty()) {
			return std::nullopt;
		}
		return m_data.back();
	}

private:
	// Port of: private final ArrayList<E> mData
	std::vector<E> m_data;
};

} // namespace PinkReader
