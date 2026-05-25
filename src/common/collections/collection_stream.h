/*
 * PinkReader - GPLv3
 * File: collections/collection_stream.h
 * Port of: org.quantumbadger.redreader.common.collections.CollectionStream.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include "stream.h"
#include <vector>
#include <initializer_list>
#include <utility>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.collections.CollectionStream<Type>
 *
 * Original: public class CollectionStream<Type> extends Stream<Type> {
 *     private final Iterator<Type> mIterator;
 *     public CollectionStream(final Iterable<Type> iterable);
 *     public CollectionStream(final Type... array);
 *     @Override public boolean hasNext();
 *     @Override public Type take();
 * }
 */
template<typename Type>
class CollectionStream : public Stream<Type> {
public:
	// Port of: public CollectionStream(final Iterable<Type> iterable)
	//   mIterator = iterable.iterator();
	explicit CollectionStream(const std::vector<Type> &items)
		: m_items(items)
		, m_index(0) {}

	explicit CollectionStream(std::vector<Type> &&items)
		: m_items(std::move(items))
		, m_index(0) {}

	// Port of: @SafeVarargs public CollectionStream(final Type... array)
	//   this(Arrays.asList(array));
	CollectionStream(std::initializer_list<Type> list)
		: m_items(list)
		, m_index(0) {}

	// Port of: @Override public boolean hasNext()
	//   return mIterator.hasNext();
	bool hasNext() override {
		return m_index < m_items.size();
	}

	// Port of: @Override public Type take()
	//   return mIterator.next();
	Type take() override {
		return m_items[m_index++];
	}

private:
	// Port of: private final Iterator<Type> mIterator;
	// Replaced by vector + index
	std::vector<Type> m_items;
	size_t m_index;
};

} // namespace PinkReader
