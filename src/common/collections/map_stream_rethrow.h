/*
 * PinkReader - GPLv3
 * File: collections/map_stream_rethrow.h
 * Port of: org.quantumbadger.redreader.common.collections.MapStreamRethrowExceptions.java
 *
 * Every field, method, inner interface, and logic branch from the original Java file.
 */

#pragma once

#include "stream.h"
#include <functional>
#include <memory>
#include <stdexcept>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.collections.MapStreamRethrowExceptions<Input, Output>
 *
 * Original: public class MapStreamRethrowExceptions<Input, Output> extends Stream<Output> {
 *     public interface Operator<Input, Output> { Output operate(Input value) throws Exception; }
 *     ...
 *     @Override public Output take() {
 *         try { return mOperator.operate(mInput.take()); }
 *         catch(final Exception e) { throw new RuntimeException(e); }
 *     }
 * }
 */
template<typename Input, typename Output>
class MapStreamRethrowExceptions : public Stream<Output> {
public:
	// Port of: public interface Operator<Input, Output> {
	//     Output operate(Input value) throws Exception;
	// }
	// In C++, exceptions are unchecked, so this is just a function that may throw.
	using Operator = std::function<Output(Input)>;

	// Port of: public MapStreamRethrowExceptions(final Stream<Input> input, final Operator<Input, Output> operator)
	MapStreamRethrowExceptions(std::unique_ptr<Stream<Input>> input, Operator oper)
		: m_input(std::move(input))
		, m_operator(std::move(oper)) {}

	// Port of: @Override public boolean hasNext()
	//   return mInput.hasNext();
	bool hasNext() override {
		return m_input->hasNext();
	}

	// Port of: @Override public Output take()
	//   try { return mOperator.operate(mInput.take()); }
	//   catch(final Exception e) { throw new RuntimeException(e); }
	Output take() override {
		try {
			return m_operator(m_input->take());
		} catch (const std::exception &e) {
			throw std::runtime_error(e.what());
		} catch (...) {
			throw std::runtime_error("Unknown exception in MapStreamRethrowExceptions::take()");
		}
	}

private:
	// Port of: private final Stream<Input> mInput
	std::unique_ptr<Stream<Input>> m_input;

	// Port of: private final Operator<Input, Output> mOperator
	Operator m_operator;
};

} // namespace PinkReader
