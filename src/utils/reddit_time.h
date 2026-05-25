/*
 * PinkReader - GPLv3
 * File: reddit_time.h
 * Port of: libs/redreader-common/.../time/TimestampUTC.kt,
 *          TimePeriod.kt, TimeDuration.kt, TimeStrings.kt,
 *          and common/time/TimeFormatHelper.kt
 *
 * Every field, method, and logic branch from the original Kotlin sources.
 */

#pragma once

#include <QString>
#include <cstdint>
#include <chrono>
#include <compare>
#include <functional>
#include <vector>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.time.TimeStrings (interface)
 *
 * Every field from the original interface.
 */
class TimeStrings {
public:
    virtual ~TimeStrings() = default;

    // Port of: val year: String
    virtual QString year() const = 0;
    // Port of: val years: String
    virtual QString years() const = 0;
    // Port of: val month: String
    virtual QString month() const = 0;
    // Port of: val months: String
    virtual QString months() const = 0;
    // Port of: val day: String
    virtual QString day() const = 0;
    // Port of: val days: String
    virtual QString days() const = 0;
    // Port of: val hour: String
    virtual QString hour() const = 0;
    // Port of: val hours: String
    virtual QString hours() const = 0;
    // Port of: val min: String
    virtual QString min() const = 0;
    // Port of: val mins: String
    virtual QString mins() const = 0;
    // Port of: val sec: String
    virtual QString sec() const = 0;
    // Port of: val secs: String
    virtual QString secs() const = 0;
    // Port of: val ms: String
    virtual QString ms() const = 0;
};

// Forward declaration for TimeDuration
class TimeDuration;

/**
 * @brief Port of org.quantumbadger.redreader.common.time.TimestampUTC
 *        (Kotlin data class with value: Instant, implements Comparable<TimestampUTC>)
 *
 * Every constructor, companion object method, and instance method matches exactly.
 */
class TimestampUTC {
public:
    // Port of: companion object

    // Port of: @JvmStatic fun now() = TimestampUTC(Clock.System.now())
    static TimestampUTC now();

    // Port of: @JvmStatic fun fromUtcMs(value: Long) = TimestampUTC(Instant.fromEpochMilliseconds(value))
    static TimestampUTC fromUtcMs(int64_t value);

    // Port of: @JvmStatic fun fromUtcSecs(value: Long) = TimestampUTC(Instant.fromEpochSeconds(value, 0))
    static TimestampUTC fromUtcSecs(int64_t value);

    // Port of: @JvmField val ZERO = TimestampUTC(Instant.fromEpochMilliseconds(0))
    static const TimestampUTC ZERO;

    // Port of: @JvmStatic fun oldest(a: TimestampUTC, b: TimestampUTC) = if (a.value < b.value) a else b
    static TimestampUTC oldest(const TimestampUTC &a, const TimestampUTC &b);

    // Port of: val value: Instant (constructor parameter)
    // Using std::chrono::system_clock::time_point as C++ equivalent of kotlinx.datetime.Instant
    TimestampUTC() : m_value(std::chrono::system_clock::from_time_t(0)) {}
    explicit TimestampUTC(std::chrono::system_clock::time_point value) : m_value(value) {}

    // Port of: fun toUtcMs() = value.toEpochMilliseconds()
    int64_t toUtcMs() const;

    // Port of: fun toUtcSecs() = value.epochSeconds
    int64_t toUtcSecs() const;

    // Port of: fun elapsed() = TimePeriod(this, now()).asDuration()
    TimeDuration elapsed() const;

    // Port of: fun elapsedPeriod() = TimePeriod(this, now())
    class TimePeriod elapsedPeriod() const;

    // Port of: fun elapsedPeriodSince(start: TimestampUTC) = TimePeriod(start, this)
    class TimePeriod elapsedPeriodSince(const TimestampUTC &start) const;

    // Port of: fun format() = localDateTime().run { String.format(Locale.US, "%d-%02d-%02d %02d:%02d", ...) }
    QString format() const;

    // Port of: fun formatFilenameSafe() = localDateTime().run { String.format(Locale.US, "%d_%02d_%02d__%02d_%02d_%02d", ...) }
    QString formatFilenameSafe() const;

    // Port of: fun isLessThan(other: TimestampUTC) = value < other.value
    bool isLessThan(const TimestampUTC &other) const;

    // Port of: fun isGreaterThan(other: TimestampUTC) = value > other.value
    bool isGreaterThan(const TimestampUTC &other) const;

    // Port of: fun add(duration: TimeDuration) = TimestampUTC(value + duration.value)
    TimestampUTC add(const TimeDuration &duration) const;

    // Port of: fun subtract(duration: TimeDuration) = TimestampUTC(value - duration.value)
    TimestampUTC subtract(const TimeDuration &duration) const;

    // Port of: override fun compareTo(other: TimestampUTC) = value.compareTo(other.value)
    auto operator<=>(const TimestampUTC &other) const { return m_value <=> other.m_value; }
    bool operator==(const TimestampUTC &other) const = default;

    // Port of: fun hasPassed() = now().isGreaterThan(this)
    bool hasPassed() const;

    // Accessor for internal value (needed by TimePeriod)
    std::chrono::system_clock::time_point value() const { return m_value; }

private:
    // Port of: val value: Instant
    std::chrono::system_clock::time_point m_value;

    // Port of: private fun localDateTime() = value.toLocalDateTime(TimeZone.currentSystemDefault())
    struct LocalDateTime {
        int year;
        int month;      // 1-12
        int day;
        int hour;
        int minute;
        int second;
    };
    LocalDateTime localDateTime() const;
};


/**
 * @brief Port of org.quantumbadger.redreader.common.time.TimeDuration
 *        (Kotlin data class with value: Duration)
 */
class TimeDuration {
public:
    // Port of: companion object

    // Port of: @JvmStatic fun ms(value: Long) = TimeDuration(value.milliseconds)
    static TimeDuration ms(int64_t value);

    // Port of: @JvmStatic fun secs(value: Long) = TimeDuration(value.seconds)
    static TimeDuration secs(int64_t value);

    // Port of: @JvmStatic fun minutes(value: Long) = TimeDuration(value.minutes)
    static TimeDuration minutes(int64_t value);

    // Port of: @JvmStatic fun hours(value: Long) = TimeDuration(value.hours)
    static TimeDuration hours(int64_t value);

    // Port of: @JvmStatic fun days(value: Long) = TimeDuration(value.days)
    static TimeDuration days(int64_t value);

    // Port of: data class TimeDuration(val value: Duration)
    TimeDuration() : m_value(std::chrono::milliseconds(0)) {}
    explicit TimeDuration(std::chrono::milliseconds value) : m_value(value) {}

    // Port of: fun toMs() = value.inWholeMilliseconds
    int64_t toMs() const;

    // Port of: fun isLessThan(other: TimeDuration) = value < other.value
    bool isLessThan(const TimeDuration &other) const;

    // Port of: fun isGreaterThan(other: TimeDuration) = value > other.value
    bool isGreaterThan(const TimeDuration &other) const;

    // Accessor for internal value (needed by TimestampUTC)
    std::chrono::milliseconds value() const { return m_value; }

private:
    // Port of: val value: Duration
    std::chrono::milliseconds m_value;
};


/**
 * @brief Port of org.quantumbadger.redreader.common.time.TimePeriod
 *        (Kotlin data class with start/end: TimestampUTC)
 */
class TimePeriod {
public:
    // Port of: data class TimePeriod(val start: TimestampUTC, val end: TimestampUTC)
    TimePeriod(const TimestampUTC &start, const TimestampUTC &end)
        : m_start(start), m_end(end) {}

    // Port of: fun format(strings: TimeStrings, unitsToDisplay: Int): String
    QString format(const TimeStrings &strings, int unitsToDisplay) const;

    // Port of: fun asDuration() = TimeDuration(end.value - start.value)
    TimeDuration asDuration() const;

    // Port of: companion object { private const val NBSP = '\u00A0' }
    static constexpr char16_t NBSP = u'\u00A0';

private:
    // Port of: private data class PeriodSegment(val suffixSingular, val suffixPlural, val value)
    struct PeriodSegment {
        std::function<QString(const TimeStrings &)> suffixSingular;
        std::function<QString(const TimeStrings &)> suffixPlural;
        std::function<int(const struct DateTimePeriod &)> value;
    };

    // Port of: private val segments = listOf(...)
    static std::vector<PeriodSegment> segments();

    // Port of: Kotlin DateTimePeriod equivalent for time difference
    struct DateTimePeriod {
        int years = 0;
        int months = 0;
        int days = 0;
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        int nanoseconds = 0;
    };

    DateTimePeriod periodUntil() const;

    // Port of: val start: TimestampUTC, val end: TimestampUTC
    TimestampUTC m_start;
    TimestampUTC m_end;
};


/**
 * @brief Port of org.quantumbadger.redreader.common.time.TimeFormatHelper (Kotlin object)
 *
 * Every static method matches exactly.
 */
class TimeFormatHelper {
public:
    TimeFormatHelper() = delete; // Cannot instantiate (Kotlin object)

    // Port of:
    // @JvmStatic fun format(period: TimePeriod, context: Context,
    //     @StringRes stringRes: Int, unitsToDisplay: Int): String
    //
    // In C++, the context and string resource are replaced with
    // a format string template and the TimeStrings directly.
    // Port of logic:
    //   val res = context.resources
    //   return String.format(Locale.US, res.getString(stringRes),
    //       period.format(TimeStringsImpl(res), unitsToDisplay))
    static QString format(const TimePeriod &period,
                          const TimeStrings &strings,
                          const QString &formatTemplate,
                          int unitsToDisplay);
};

} // namespace PinkReader
