// Origin: org/quantumbadger/redreader/common/time/TimeFormatHelper.kt
// (Kotlin object with @JvmStatic method)
#pragma once

#include <QString>

namespace PinkReader {

// Forward declarations
class TimePeriod;
class TimeStrings;

class TimeFormatHelper {
public:
	TimeFormatHelper() = delete; // Kotlin object

	// Origin: @JvmStatic fun format(period, context, stringRes, unitsToDisplay): String
	// In C++, context is replaced with explicit resource string access.
	static QString format(
		const TimePeriod& period,
		const QString& formatString,
		const TimeStrings& strings,
		int unitsToDisplay);
};

} // namespace PinkReader
