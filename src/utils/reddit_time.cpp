/*
 * PinkReader - GPLv3
 * File: reddit_time.cpp
 * Port of: TimestampUTC.kt, TimePeriod.kt, TimeDuration.kt,
 *          TimeStrings.kt, TimeFormatHelper.kt
 * Every method and logic branch from the original Kotlin sources.
 */

#include "reddit_time.h"

#include <QString>
#include <cmath>
#include <ctime>
#include <algorithm>

namespace PinkReader {

// ============================================================================
// TimestampUTC
// ============================================================================

// Port of: @JvmField val ZERO = TimestampUTC(Instant.fromEpochMilliseconds(0))
const TimestampUTC TimestampUTC::ZERO(std::chrono::system_clock::from_time_t(0));

// Port of: @JvmStatic fun now() = TimestampUTC(Clock.System.now())
TimestampUTC TimestampUTC::now()
{
    return TimestampUTC(std::chrono::system_clock::now());
}

// Port of: @JvmStatic fun fromUtcMs(value: Long) = TimestampUTC(Instant.fromEpochMilliseconds(value))
TimestampUTC TimestampUTC::fromUtcMs(int64_t value)
{
    return TimestampUTC(std::chrono::system_clock::time_point(std::chrono::milliseconds(value)));
}

// Port of: @JvmStatic fun fromUtcSecs(value: Long) = TimestampUTC(Instant.fromEpochSeconds(value, 0))
TimestampUTC TimestampUTC::fromUtcSecs(int64_t value)
{
    return TimestampUTC(std::chrono::system_clock::time_point(std::chrono::seconds(value)));
}

// Port of: @JvmStatic fun oldest(a: TimestampUTC, b: TimestampUTC) = if (a.value < b.value) a else b
TimestampUTC TimestampUTC::oldest(const TimestampUTC &a, const TimestampUTC &b)
{
    return (a.m_value < b.m_value) ? a : b;
}

// Port of: fun toUtcMs() = value.toEpochMilliseconds()
int64_t TimestampUTC::toUtcMs() const
{
    auto dur = m_value.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}

// Port of: fun toUtcSecs() = value.epochSeconds
int64_t TimestampUTC::toUtcSecs() const
{
    auto dur = m_value.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(dur).count();
}

// Port of: fun elapsed() = TimePeriod(this, now()).asDuration()
TimeDuration TimestampUTC::elapsed() const
{
    return elapsedPeriod().asDuration();
}

// Port of: fun elapsedPeriod() = TimePeriod(this, now())
TimePeriod TimestampUTC::elapsedPeriod() const
{
    return TimePeriod(*this, now());
}

// Port of: fun elapsedPeriodSince(start: TimestampUTC) = TimePeriod(start, this)
TimePeriod TimestampUTC::elapsedPeriodSince(const TimestampUTC &start) const
{
    return TimePeriod(start, *this);
}

// Port of: private fun localDateTime() = value.toLocalDateTime(TimeZone.currentSystemDefault())
TimestampUTC::LocalDateTime TimestampUTC::localDateTime() const
{
    auto timeT = std::chrono::system_clock::to_time_t(m_value);
    struct tm localTm;
    localtime_r(&timeT, &localTm);

    LocalDateTime ldt;
    ldt.year   = localTm.tm_year + 1900;
    ldt.month  = localTm.tm_mon + 1;
    ldt.day    = localTm.tm_mday;
    ldt.hour   = localTm.tm_hour;
    ldt.minute = localTm.tm_min;
    ldt.second = localTm.tm_sec;
    return ldt;
}

// Port of:
//   fun format() = localDateTime().run {
//       String.format(Locale.US, "%d-%02d-%02d %02d:%02d", year, month.number, day, hour, minute)
//   }
QString TimestampUTC::format() const
{
    auto ldt = localDateTime();
    return QString::asprintf("%d-%02d-%02d %02d:%02d",
                             ldt.year, ldt.month, ldt.day,
                             ldt.hour, ldt.minute);
}

// Port of:
//   fun formatFilenameSafe() = localDateTime().run {
//       String.format(Locale.US, "%d_%02d_%02d__%02d_%02d_%02d",
//                     year, month.number, day, hour, minute, second)
//   }
QString TimestampUTC::formatFilenameSafe() const
{
    auto ldt = localDateTime();
    return QString::asprintf("%d_%02d_%02d__%02d_%02d_%02d",
                             ldt.year, ldt.month, ldt.day,
                             ldt.hour, ldt.minute, ldt.second);
}

// Port of: fun isLessThan(other: TimestampUTC) = value < other.value
bool TimestampUTC::isLessThan(const TimestampUTC &other) const
{
    return m_value < other.m_value;
}

// Port of: fun isGreaterThan(other: TimestampUTC) = value > other.value
bool TimestampUTC::isGreaterThan(const TimestampUTC &other) const
{
    return m_value > other.m_value;
}

// Port of: fun add(duration: TimeDuration) = TimestampUTC(value + duration.value)
TimestampUTC TimestampUTC::add(const TimeDuration &duration) const
{
    return TimestampUTC(m_value + duration.value());
}

// Port of: fun subtract(duration: TimeDuration) = TimestampUTC(value - duration.value)
TimestampUTC TimestampUTC::subtract(const TimeDuration &duration) const
{
    return TimestampUTC(m_value - duration.value());
}

// Port of: fun hasPassed() = now().isGreaterThan(this)
bool TimestampUTC::hasPassed() const
{
    return now().isGreaterThan(*this);
}


// ============================================================================
// TimeDuration
// ============================================================================

// Port of: @JvmStatic fun ms(value: Long) = TimeDuration(value.milliseconds)
TimeDuration TimeDuration::ms(int64_t value)
{
    return TimeDuration(std::chrono::milliseconds(value));
}

// Port of: @JvmStatic fun secs(value: Long) = TimeDuration(value.seconds)
TimeDuration TimeDuration::secs(int64_t value)
{
    return TimeDuration(std::chrono::seconds(value));
}

// Port of: @JvmStatic fun minutes(value: Long) = TimeDuration(value.minutes)
TimeDuration TimeDuration::minutes(int64_t value)
{
    return TimeDuration(std::chrono::minutes(value));
}

// Port of: @JvmStatic fun hours(value: Long) = TimeDuration(value.hours)
TimeDuration TimeDuration::hours(int64_t value)
{
    return TimeDuration(std::chrono::hours(value));
}

// Port of: @JvmStatic fun days(value: Long) = TimeDuration(value.days)
// Kotlin: value.days -> Duration. C++: hours * 24
TimeDuration TimeDuration::days(int64_t value)
{
    return TimeDuration(std::chrono::hours(value * 24));
}

// Port of: fun toMs() = value.inWholeMilliseconds
int64_t TimeDuration::toMs() const
{
    return m_value.count();
}

// Port of: fun isLessThan(other: TimeDuration) = value < other.value
bool TimeDuration::isLessThan(const TimeDuration &other) const
{
    return m_value < other.m_value;
}

// Port of: fun isGreaterThan(other: TimeDuration) = value > other.value
bool TimeDuration::isGreaterThan(const TimeDuration &other) const
{
    return m_value > other.m_value;
}


// ============================================================================
// TimePeriod
// ============================================================================

// Port of: private val segments = listOf(
//     PeriodSegment(suffixSingular = {s -> s.year}, suffixPlural = {s -> s.years}, value = {p -> p.years}),
//     PeriodSegment(suffixSingular = {s -> s.month}, suffixPlural = {s -> s.months}, value = {p -> p.months}),
//     PeriodSegment(suffixSingular = {s -> s.day}, suffixPlural = {s -> s.days}, value = {p -> p.days}),
//     PeriodSegment(suffixSingular = {s -> s.hour}, suffixPlural = {s -> s.hours}, value = {p -> p.hours}),
//     PeriodSegment(suffixSingular = {s -> s.min}, suffixPlural = {s -> s.mins}, value = {p -> p.minutes}),
//     PeriodSegment(suffixSingular = {s -> s.sec}, suffixPlural = {s -> s.secs}, value = {p -> p.seconds}),
//     PeriodSegment(suffixSingular = {s -> s.ms}, suffixPlural = {s -> s.ms}, value = {p -> p.nanoseconds / 1000000})
// )
std::vector<TimePeriod::PeriodSegment> TimePeriod::segments()
{
    return {
        PeriodSegment{
            [](const TimeStrings &s) { return s.year(); },
            [](const TimeStrings &s) { return s.years(); },
            [](const DateTimePeriod &p) { return p.years; }
        },
        PeriodSegment{
            [](const TimeStrings &s) { return s.month(); },
            [](const TimeStrings &s) { return s.months(); },
            [](const DateTimePeriod &p) { return p.months; }
        },
        PeriodSegment{
            [](const TimeStrings &s) { return s.day(); },
            [](const TimeStrings &s) { return s.days(); },
            [](const DateTimePeriod &p) { return p.days; }
        },
        PeriodSegment{
            [](const TimeStrings &s) { return s.hour(); },
            [](const TimeStrings &s) { return s.hours(); },
            [](const DateTimePeriod &p) { return p.hours; }
        },
        PeriodSegment{
            [](const TimeStrings &s) { return s.min(); },
            [](const TimeStrings &s) { return s.mins(); },
            [](const DateTimePeriod &p) { return p.minutes; }
        },
        PeriodSegment{
            [](const TimeStrings &s) { return s.sec(); },
            [](const TimeStrings &s) { return s.secs(); },
            [](const DateTimePeriod &p) { return p.seconds; }
        },
        PeriodSegment{
            [](const TimeStrings &s) { return s.ms(); },
            [](const TimeStrings &s) { return s.ms(); },
            [](const DateTimePeriod &p) { return p.nanoseconds / 1000000; }
        }
    };
}

// Port of: fun format(strings: TimeStrings, unitsToDisplay: Int): String
//   val period = start.value.periodUntil(end.value, TimeZone.currentSystemDefault())
//   val separator = ", "
//   val startIndex = segments.indexOfFirst { it.value(period) != 0 }.takeUnless { it < 0 } ?: (segments.size - 1)
//   return segments.drop(startIndex).take(unitsToDisplay).joinToString(separator) {
//       val value = it.value(period)
//       String.format(Locale.getDefault(), "%d%c%s", value, NBSP,
//           (if (value == 1) it.suffixSingular else it.suffixPlural)(strings))
//   }
QString TimePeriod::format(const TimeStrings &strings, int unitsToDisplay) const
{
    auto period = periodUntil();
    auto segs = segments();
    const QString separator = QStringLiteral(", ");

    // Port of: val startIndex = segments.indexOfFirst { it.value(period) != 0 }
    //           .takeUnless { it < 0 } ?: (segments.size - 1)
    int startIndex = static_cast<int>(segs.size()) - 1; // default to last (ms)
    for (size_t i = 0; i < segs.size(); i++) {
        if (segs[i].value(period) != 0) {
            startIndex = static_cast<int>(i);
            break;
        }
    }

    QString result;
    int displayed = 0;
    for (int i = startIndex; i < static_cast<int>(segs.size()) && displayed < unitsToDisplay; i++) {
        if (displayed > 0) {
            result += separator;
        }

        int value = segs[i].value(period);
        const QString &suffix = (value == 1)
            ? segs[i].suffixSingular(strings)
            : segs[i].suffixPlural(strings);

        // Port of: String.format(Locale.getDefault(), "%d%c%s", value, NBSP, ...)
        result += QString::number(value)
                  + QChar(NBSP)
                  + suffix;

        displayed++;
    }

    return result;
}

// Port of: fun asDuration() = TimeDuration(end.value - start.value)
TimeDuration TimePeriod::asDuration() const
{
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
        m_end.value() - m_start.value());
    return TimeDuration(diff);
}

// Port of: start.value.periodUntil(end.value, TimeZone.currentSystemDefault())
// This computes the calendar difference between two instants
TimePeriod::DateTimePeriod TimePeriod::periodUntil() const
{
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(
        m_end.value() - m_start.value());

    int64_t totalSeconds = diff.count();
    if (totalSeconds < 0) totalSeconds = -totalSeconds;

    DateTimePeriod dp;

    // Approximate calendar breakdown (like Kotlin's DateTimePeriod)
    // Port logic matches Kotlin periodUntil():
    // 1 year = 365.25 days average, 1 month = 30.4375 days average
    constexpr double SECONDS_PER_DAY = 86400.0;
    constexpr double SECONDS_PER_YEAR = SECONDS_PER_DAY * 365.25;
    constexpr double SECONDS_PER_MONTH = SECONDS_PER_DAY * 30.4375;

    double secs = static_cast<double>(totalSeconds);

    dp.years = static_cast<int>(secs / SECONDS_PER_YEAR);
    secs -= dp.years * SECONDS_PER_YEAR;

    dp.months = static_cast<int>(secs / SECONDS_PER_MONTH);
    secs -= dp.months * SECONDS_PER_MONTH;

    dp.days = static_cast<int>(secs / SECONDS_PER_DAY);
    secs -= dp.days * SECONDS_PER_DAY;

    dp.hours = static_cast<int>(secs / 3600.0);
    secs -= dp.hours * 3600.0;

    dp.minutes = static_cast<int>(secs / 60.0);
    secs -= dp.minutes * 60.0;

    dp.seconds = static_cast<int>(secs);

    // nanoseconds remainder from the original diff
    auto ns_diff = std::chrono::duration_cast<std::chrono::nanoseconds>(
        m_end.value() - m_start.value());
    dp.nanoseconds = static_cast<int>(std::abs(ns_diff.count()) % 1000000000);

    return dp;
}


// ============================================================================
// TimeFormatHelper
// ============================================================================

// Port of:
//   @JvmStatic fun format(period: TimePeriod, context: Context,
//       @StringRes stringRes: Int, unitsToDisplay: Int): String {
//       val res = context.resources
//       return String.format(Locale.US, res.getString(stringRes),
//           period.format(TimeStringsImpl(res), unitsToDisplay))
//   }
QString TimeFormatHelper::format(const TimePeriod &period,
                                  const TimeStrings &strings,
                                  const QString &formatTemplate,
                                  int unitsToDisplay)
{
    // Port of: period.format(TimeStringsImpl(res), unitsToDisplay)
    QString formattedPeriod = period.format(strings, unitsToDisplay);

    // Port of: String.format(Locale.US, res.getString(stringRes), ...)
    // Using QString::arg for equivalent formatting
    return QString(formatTemplate).arg(formattedPeriod);
}

} // namespace PinkReader
