/*
 * PinkReader - GPLv3
 * File: timestamp_bound.h
 * Port of: org.quantumbadger.redreader.common.TimestampBound.java
 *
 * Every field, method, inner class, and logic branch from the original Java file.
 * Dependencies: TimestampUTC, TimeDuration from utils/reddit_time.h
 */

#pragma once

#include "../utils/reddit_time.h"
#include <memory>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.TimestampBound (abstract class)
 *
 * Original: public abstract class TimestampBound {
 *     public abstract boolean verifyTimestamp(TimestampUTC timestamp);
 *     ...
 * }
 */
class TimestampBound {
public:
	virtual ~TimestampBound() = default;

	// Port of: public abstract boolean verifyTimestamp(TimestampUTC timestamp);
	virtual bool verifyTimestamp(const TimestampUTC &timestamp) const = 0;

	// Port of: public static final TimestampBound ANY = new TimestampBound() {
	//     @Override public boolean verifyTimestamp(final TimestampUTC timestamp) { return true; }
	// };
	static const TimestampBound &ANY();

	// Port of: public static final TimestampBound NONE = new TimestampBound() {
	//     @Override public boolean verifyTimestamp(final TimestampUTC timestamp) { return false; }
	// };
	static const TimestampBound &NONE();

	// Port of: public static final class MoreRecentThanBound extends TimestampBound
	class MoreRecentThanBound final : public TimestampBound {
	public:
		// Port of: public MoreRecentThanBound(final TimestampUTC minTimestamp)
		explicit MoreRecentThanBound(TimestampUTC minTimestamp)
			: m_minTimestamp(std::move(minTimestamp)) {}

		// Port of: @Override public boolean verifyTimestamp(final TimestampUTC timestamp)
		//   return timestamp.isGreaterThan(minTimestamp);
		bool verifyTimestamp(const TimestampUTC &timestamp) const override {
			return timestamp.isGreaterThan(m_minTimestamp);
		}

	private:
		// Port of: private final TimestampUTC minTimestamp;
		TimestampUTC m_minTimestamp;
	};

	// Port of: public static MoreRecentThanBound notOlderThan(final TimeDuration age)
	//   return new MoreRecentThanBound(TimestampUTC.now().subtract(age));
	static MoreRecentThanBound notOlderThan(const TimeDuration &age) {
		return MoreRecentThanBound(TimestampUTC::now().subtract(age));
	}
};

// Inline definitions for the static singletons

namespace detail {
	// Port of: ANY anonymous subclass
	class TimestampBoundAny final : public TimestampBound {
	public:
		bool verifyTimestamp(const TimestampUTC & /*timestamp*/) const override {
			return true;
		}
	};

	// Port of: NONE anonymous subclass
	class TimestampBoundNone final : public TimestampBound {
	public:
		bool verifyTimestamp(const TimestampUTC & /*timestamp*/) const override {
			return false;
		}
	};
}

inline const TimestampBound &TimestampBound::ANY() {
	static const detail::TimestampBoundAny instance;
	return instance;
}

inline const TimestampBound &TimestampBound::NONE() {
	static const detail::TimestampBoundNone instance;
	return instance;
}

} // namespace PinkReader
