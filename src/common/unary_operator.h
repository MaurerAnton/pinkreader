/*
 * PinkReader - GPLv3
 * File: unary_operator.h
 * Port of: org.quantumbadger.redreader.common.UnaryOperator.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.UnaryOperator<E, R>
 *
 * Original: public interface UnaryOperator<E, R>
 *   R operate(E value);
 */
template<typename E, typename R>
class UnaryOperator {
public:
	virtual ~UnaryOperator() = default;

	// Port of: R operate(E value);
	virtual R operate(E value) = 0;
};

} // namespace PinkReader
