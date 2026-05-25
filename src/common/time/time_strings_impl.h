// Origin: org/quantumbadger/redreader/common/time/TimeStringsImpl.kt
// (Kotlin class implementing TimeStrings via Android Resources)
#pragma once

#include <QString>

namespace PinkReader {

// Forward declaration of TimeStrings interface
class TimeStrings {
public:
	virtual ~TimeStrings() = default;
	virtual QString year() const = 0;
	virtual QString years() const = 0;
	virtual QString month() const = 0;
	virtual QString months() const = 0;
	virtual QString day() const = 0;
	virtual QString days() const = 0;
	virtual QString hour() const = 0;
	virtual QString hours() const = 0;
	virtual QString min() const = 0;
	virtual QString mins() const = 0;
	virtual QString sec() const = 0;
	virtual QString secs() const = 0;
	virtual QString ms() const = 0;
};

// TimeStringsImpl - wraps string resources
// Origin: class TimeStringsImpl constructor(private val resources: Resources) : TimeStrings
class TimeStringsImpl : public TimeStrings {
private:
	QString m_year;
	QString m_years;
	QString m_month;
	QString m_months;
	QString m_day;
	QString m_days;
	QString m_hour;
	QString m_hours;
	QString m_min;
	QString m_mins;
	QString m_sec;
	QString m_secs;
	QString m_ms;

public:
	TimeStringsImpl(
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
		const QString& ms);

	QString year() const override { return m_year; }
	QString years() const override { return m_years; }
	QString month() const override { return m_month; }
	QString months() const override { return m_months; }
	QString day() const override { return m_day; }
	QString days() const override { return m_days; }
	QString hour() const override { return m_hour; }
	QString hours() const override { return m_hours; }
	QString min() const override { return m_min; }
	QString mins() const override { return m_mins; }
	QString sec() const override { return m_sec; }
	QString secs() const override { return m_secs; }
	QString ms() const override { return m_ms; }
};

} // namespace PinkReader
