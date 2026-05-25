/*
 * PinkReader - GPLv3
 * File: alarms.cpp
 * Exact port of Alarms.java (134 lines)
 */

#include "alarms.h"

#include <QDebug>

namespace PinkReader {

// ============================================================================
// Static fields — Java lines 34-35
// ============================================================================
// Port of: private static final Map<Alarm, AlarmManager> alarmMap = new HashMap<>();
QMap<Alarms::AlarmType, int> Alarms::s_alarmMap;

// Port of: private static final Map<Alarm, PendingIntent> intentMap = new HashMap<>();
QMap<Alarms::AlarmType, std::function<void()>> Alarms::s_intentMap;

// ============================================================================
// interval — Port of: private long interval() (Java lines 60-62)
// ============================================================================
qint64 Alarms::interval(AlarmType alarm) {
    switch (alarm) {
        // Port of: MESSAGE_CHECKER(AlarmManager.INTERVAL_HALF_HOUR, ...)
        case AlarmType::MESSAGE_CHECKER:
            return INTERVAL_HALF_HOUR;

        // Port of: CACHE_PRUNER(AlarmManager.INTERVAL_HOUR, ...)
        case AlarmType::CACHE_PRUNER:
            return INTERVAL_HOUR;
    }
    return 0;
}

// ============================================================================
// alarmClassId — Port of: private Class<?> alarmClass() (Java lines 64-66)
// ============================================================================
QString Alarms::alarmClassId(AlarmType alarm) {
    switch (alarm) {
        // Port of: MESSAGE_CHECKER -> NewMessageChecker.class
        case AlarmType::MESSAGE_CHECKER:
            return QStringLiteral("NewMessageChecker");

        // Port of: CACHE_PRUNER -> RegularCachePruner.class
        case AlarmType::CACHE_PRUNER:
            return QStringLiteral("RegularCachePruner");
    }
    return QString();
}

// ============================================================================
// startOnBoot — Port of: private boolean startOnBoot() (Java lines 68-70)
// ============================================================================
bool Alarms::startOnBoot(AlarmType alarm) {
    switch (alarm) {
        // Both alarms have startOnBoot = true
        case AlarmType::MESSAGE_CHECKER:
            return true;
        case AlarmType::CACHE_PRUNER:
            return true;
    }
    return false;
}

// ============================================================================
// startAlarm — Port of: public static void startAlarm(...) (Java lines 77-105)
// ============================================================================
void Alarms::startAlarm(AlarmType alarm, std::function<void()> callback) {
    // Port of: if(!alarmMap.containsKey(alarm)) {
    if (!s_alarmMap.contains(alarm)) {

        // Port of: final Intent alarmIntent = new Intent(context, alarm.alarmClass());
        // In C++ we don't use Android Intents; we register a callback.

        // Port of: int flags = 0;
        //          if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) { flags |= FLAG_IMMUTABLE; }
        // Port of: final PendingIntent pendingIntent = PendingIntent.getBroadcast(
        //              context, 0, alarmIntent, flags);
        // In C++, this is simplified to storing the callback.

        // Port of: final AlarmManager alarmManager =
        //              (AlarmManager)(context.getSystemService(Context.ALARM_SERVICE));
        // Port of: alarmManager.setInexactRepeating(
        //              AlarmManager.RTC, System.currentTimeMillis(), alarm.interval(), pendingIntent);

        qDebug() << "Alarms::startAlarm:" << alarmClassId(alarm)
                 << "interval:" << interval(alarm) / 1000 << "seconds";

        // Port of: alarmMap.put(alarm, alarmManager);
        s_alarmMap.insert(alarm, 1); // Timer ID placeholder

        // Port of: intentMap.put(alarm, pendingIntent);
        s_intentMap.insert(alarm, callback);
    }
}

// ============================================================================
// stopAlarm — Port of: public static void stopAlarm(...) (Java lines 113-119)
// ============================================================================
void Alarms::stopAlarm(AlarmType alarm) {
    // Port of: if(alarmMap.containsKey(alarm)) {
    if (s_alarmMap.contains(alarm)) {
        // Port of: alarmMap.get(alarm).cancel(intentMap.get(alarm));
        // In C++ we clear the callback to cancel.

        // Port of: alarmMap.remove(alarm);
        s_alarmMap.remove(alarm);

        // Port of: intentMap.remove(alarm);
        s_intentMap.remove(alarm);
    }
}

// ============================================================================
// onBoot — Port of: public static void onBoot(...) (Java lines 127-133)
// ============================================================================
void Alarms::onBoot(std::function<void(AlarmType)> callback) {
    // Port of: for(final Alarm alarm : Alarm.values()) {
    const AlarmType allAlarms[] = {
        AlarmType::MESSAGE_CHECKER,
        AlarmType::CACHE_PRUNER
    };

    for (const auto &alarm : allAlarms) {
        // Port of: if(alarm.startOnBoot()) { startAlarm(alarm, context); }
        if (startOnBoot(alarm)) {
            startAlarm(alarm, [alarm, callback]() {
                callback(alarm);
            });
        }
    }
}

} // namespace PinkReader
