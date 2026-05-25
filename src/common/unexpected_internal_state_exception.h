/*
 * PinkReader - GPLv3
 * File: unexpected_internal_state_exception.h
 * Port of: org.quantumbadger.redreader.common.UnexpectedInternalStateException.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include <QString>
#include <stdexcept>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.UnexpectedInternalStateException
 *
 * Original: public class UnexpectedInternalStateException extends RuntimeException
 */
class UnexpectedInternalStateException : public std::runtime_error {
public:
	// Port of: public UnexpectedInternalStateException()
	//   super("The application's internal state is invalid");
	UnexpectedInternalStateException()
		: std::runtime_error("The application's internal state is invalid") {}

	// Port of: public UnexpectedInternalStateException(final String message)
	//   super(message);
	explicit UnexpectedInternalStateException(const QString &message)
		: std::runtime_error(message.toStdString()) {}
};

} // namespace PinkReader
