/*
 * PinkReader - GPLv3
 * File: rr_void.h
 * Port of: org.quantumbadger.redreader.common.Void.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.Void
 *
 * Original: public final class Void
 *   @NonNull public static final Void INSTANCE = new Void();
 *   private Void() {}
 */
class RrVoid {
public:
	// Port of: @NonNull public static final Void INSTANCE = new Void();
	static const RrVoid INSTANCE;

	// Copy/move disabled (private constructor in original)
	RrVoid(const RrVoid &) = delete;
	RrVoid &operator=(const RrVoid &) = delete;

private:
	// Port of: private Void() {}
	RrVoid() = default;
};

// Port of: @NonNull public static final Void INSTANCE = new Void();
inline const RrVoid RrVoid::INSTANCE{};

} // namespace PinkReader
