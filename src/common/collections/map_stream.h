/*
 * PinkReader - GPLv3
 * File: collections/map_stream.h
 * Port of: org.quantumbadger.redreader.common.collections.MapStream.java
 *
 * Every field, method, inner interface, and logic branch from the original Java file.
 */

#pragma once

#include "stream.h"
#include <functional>
#include <memory>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.collections.MapStream<Input, Output>
 *
 * Original: public class MapStream<Input, Output> extends Stream<Output> {
 *     public interface Operator<Input, Output> { Output operate(Input value); }
 *     private final Stream<Input> mInput;
 *     private final Operator<Input, Output> mOperator;
 *     ...
 * }
 */
template<typename Input, typename Output>
class MapStream : public Stream<Output> {
public:
	// Port of: public interface Operator<Input, Output> {
	//     Output operate(Input value);
	// }
	using Operator = std::function<Output(Input)>;

	// Port of: public MapStream(final Stream<Input> input, final Operator<Input, Output> operator)
	MapStream(std::unique_ptr<Stream<Input>> input, Operator oper)
		: m_input(std::move(input))
		, m_operator(std::move(oper)) {}

	// Port of: @Override public boolean hasNext()
	//   return mInput.hasNext();
	bool hasNext() override {
		return m_input->hasNext();
	}

	// Port of: @Override public Output take()
	//   return mOperator.operate(mInput.take());
	Output take() override {
		return m_operator(m_input->take());
	}

private:
	// Port of: private final Stream<Input> mInput
	std::unique_ptr<Stream<Input>> m_input;

	// Port of: private final Operator<Input, Output> mOperator
	Operator m_operator;
};

} // namespace PinkReader
