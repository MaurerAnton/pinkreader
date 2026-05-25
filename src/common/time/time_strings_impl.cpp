// Origin: org/quantumbadger/redreader/common/time/TimeStringsImpl.kt
#include "time_strings_impl.h"

namespace PinkReader {

TimeStringsImpl::TimeStringsImpl(
	const QString& year,
	const QString& years,
	const QString& month,
	const QString& months,
	const QString& day,
	const QString& days,
	const QString& hour,
	const QString& hours,
	const QString& min,
	const QString& mins,
	const QString& sec,
	const QString& secs,
	const QString& ms)
	: m_year(year)
	, m_years(years)
	, m_month(month)
	, m_months(months)
	, m_day(day)
	, m_days(days)
	, m_hour(hour)
	, m_hours(hours)
	, m_min(min)
	, m_mins(mins)
	, m_sec(sec)
	, m_secs(secs)
	, m_ms(ms) {}

} // namespace PinkReader
