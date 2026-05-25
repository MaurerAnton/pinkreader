/*
 * PinkReader - GPLv3
 * File: collections/stream.h
 * Port of: org.quantumbadger.redreader.common.collections.Stream.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include <vector>

namespace PinkReader {

// Forward declarations
template<typename Input, typename Output> class MapStream;
template<typename Input, typename Output> class MapStreamRethrowExceptions;

/**
 * @brief Port of org.quantumbadger.redreader.common.collections.Stream<Type>
 *
 * Original: public abstract class Stream<Type> {
 *     public abstract boolean hasNext();
 *     public abstract Type take();
 *     public final <Output> Stream<Output> map(final MapStream.Operator<Type, Output> operator);
 *     public final <Output> Stream<Output> mapRethrowExceptions(...);
 *     public final <Output extends Collection<? super Type>> Output collect(final Output output);
 * }
 */
template<typename Type>
class Stream {
public:
	virtual ~Stream() = default;

	// Port of: public abstract boolean hasNext();
	virtual bool hasNext() = 0;

	// Port of: public abstract Type take();
	virtual Type take() = 0;

	// Port of: public final <Output> Stream<Output> map(final MapStream.Operator<Type, Output> operator)
	//   return new MapStream<>(this, operator);
	template<typename Output>
	Stream<Output> *map(typename MapStream<Type, Output>::Operator oper);

	// Port of: public final <Output> Stream<Output> mapRethrowExceptions(
	//     final MapStreamRethrowExceptions.Operator<Type, Output> operator)
	//   return new MapStreamRethrowExceptions<>(this, operator);
	template<typename Output>
	Stream<Output> *mapRethrowExceptions(
		typename MapStreamRethrowExceptions<Type, Output>::Operator oper);

	// Port of: public final <Output extends Collection<? super Type>> Output collect(final Output output)
	//   while(hasNext()) { output.add(take()); }
	//   return output;
	template<typename Container>
	Container &collect(Container &output) {
		while (hasNext()) {
			output.push_back(take());
		}
		return output;
	}
};

} // namespace PinkReader
