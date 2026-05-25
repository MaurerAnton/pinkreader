// Origin: org/quantumbadger/redreader/common/time/TimeFormatHelper.kt
#include "time_format_helper.h"

#include <QString>
#include <locale>

// TimePeriod is from redreader-common lib; forward-declared until ported
// For now, provide a minimal stub
namespace PinkReader {

QString TimeFormatHelper::format(
	const TimePeriod& period,
	const QString& formatString,
	const TimeStrings& strings,
	int unitsToDisplay) {
	// Original: String.format(Locale.US, res.getString(stringRes), period.format(TimeStringsImpl(res), unitsToDisplay))
	// Here, formatString is already the resolved resource string (e.g. "%s ago")
	// period.format(strings, unitsToDisplay) produces the time string
	QString timeString = period.format(strings, unitsToDisplay);
	return formatString.arg(timeString);
}

} // namespace PinkReader
