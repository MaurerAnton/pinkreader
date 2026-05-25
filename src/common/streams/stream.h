// Origin: org/quantumbadger/redreader/common/streams/Stream.java
// Origin: org/quantumbadger/redreader/common/streams/FilterStream.java
// Origin: org/quantumbadger/redreader/common/streams/IteratorStream.java
// Note: FilterStream and IteratorStream are colocated here (Java one-class-per-file
// artifacts; C++ template classes cohabitate cleanly in one header).
#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "predicate.h"
#include "../consumer.h"

namespace PinkReader {

// ---- Stream<E> (interface) ----
// Origin: org/quantumbadger/redreader/common/streams/Stream.java
template<typename E>
class Stream {
public:
	virtual ~Stream() = default;

	virtual bool hasNext() = 0;

	virtual E next() = 0;

	std::unique_ptr<Stream<E>> filter(Predicate<E>& predicate);

	void forEach(Consumer<E>& consumer) {
		while(hasNext()) {
			consumer(next());
		}
	}

	static std::unique_ptr<Stream<E>> from(std::vector<E>& iterable);
};

// ---- FilterStream<E> ----
// Origin: org/quantumbadger/redreader/common/streams/FilterStream.java
template<typename E>
class FilterStream : public Stream<E> {
private:
	Stream<E>& m_inner;
	Predicate<E>& m_predicate;

	bool m_hasNext;
	std::unique_ptr<E> m_next;

	void moveToNext() {
		while(m_inner.hasNext()) {
			m_next = std::make_unique<E>(m_inner.next());

			if(m_predicate.matches(*m_next)) {
				return;
			}
		}

		m_next.reset();
		m_hasNext = false;
	}

public:
	FilterStream(Stream<E>& inner, Predicate<E>& predicate)
		: m_inner(inner)
		, m_predicate(predicate)
		, m_hasNext(true) {
		moveToNext();
	}

	bool hasNext() override {
		return m_hasNext;
	}

	E next() override {
		E result = *m_next;
		moveToNext();
		return result;
	}
};

// ---- IteratorStream<E> ----
// Origin: org/quantumbadger/redreader/common/streams/IteratorStream.java
template<typename E>
class IteratorStream : public Stream<E> {
private:
	typename std::vector<E>::const_iterator m_begin;
	typename std::vector<E>::const_iterator m_end;

public:
	IteratorStream(
		typename std::vector<E>::const_iterator begin,
		typename std::vector<E>::const_iterator end)
		: m_begin(begin)
		, m_end(end) {}

	bool hasNext() override {
		return m_begin != m_end;
	}

	E next() override {
		return *(m_begin++);
	}
};

// ---- Out-of-line definitions for Stream methods ----
template<typename E>
std::unique_ptr<Stream<E>> Stream<E>::filter(Predicate<E>& predicate) {
	return std::make_unique<FilterStream<E>>(*this, predicate);
}

template<typename E>
std::unique_ptr<Stream<E>> Stream<E>::from(std::vector<E>& iterable) {
	return std::make_unique<IteratorStream<E>>(
		iterable.begin(), iterable.end());
}

} // namespace PinkReader
