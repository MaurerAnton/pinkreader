/*
 * PinkReader - GPLv3
 * File: alarms.h
 * Exact port of Alarms.java (134 lines)
 *
 * Ports every field, method, inner enum Alarm exactly.
 * Android-specific AlarmManager/PendingIntent/BroadcastReceiver are adapted
 * to a platform-independent interface.
 */

#pragma once

#include <QString>
#include <QMap>
#include <functional>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.Alarms (Java public class)
 *
 * Manages repeating alarms for background tasks like message checking
 * and cache pruning. The Android AlarmManager, PendingIntent, and
 * BroadcastReceiver are replaced with platform-independent callbacks.
 *
 * Every field, method, inner class matches the Java original.
 */
class Alarms {
public:
    Alarms() = delete; // Static utility class

    // ========================================================================
    // Alarm enum — Java lines 43-71
    // Port of: public enum Alarm
    // ========================================================================
    /**
     * @brief Port of: public enum Alarm (Java lines 43-71)
     *
     * An enum to represent an alarm that may be created.
     * If you wish to add an alarm, just add it at the top of the enum
     * with the 3 arguments, and then call startAlarm() on it.
     */
    enum class AlarmType {
        /** Port of: MESSAGE_CHECKER(AlarmManager.INTERVAL_HALF_HOUR, NewMessageChecker.class, true) */
        MESSAGE_CHECKER,
        /** Port of: CACHE_PRUNER(AlarmManager.INTERVAL_HOUR, RegularCachePruner.class, true) */
        CACHE_PRUNER
    };

    /**
     * @brief Port of: the Alarm enum's instance fields and methods
     *
     * In Java, each enum constant has: interval (long), alarmClass (Class<?>),
     * and startOnBoot (boolean). In C++ we provide these via static methods
     * since enum classes cannot hold instance data.
     */
    struct AlarmInfo {
        /** Port of: private final long interval; (Java line 47) */
        qint64 interval;

        /** Port of: private final boolean startOnBoot; (Java line 49) */
        bool startOnBoot;

        /** Port of: private final Class<? extends BroadcastReceiver> alarmClass; (Java line 48)
         *  In C++ we use a string identifier for the alarm class type. */
        QString alarmClassId;
    };

    // ========================================================================
    // Port of: private final long interval() (Java lines 60-62)
    // ========================================================================
    /**
     * @brief Returns the interval for the specified alarm type.
     *
     * Port of MESSAGE_CHECKER: AlarmManager.INTERVAL_HALF_HOUR = 30 * 60 * 1000 ms
     * Port of CACHE_PRUNER:    AlarmManager.INTERVAL_HOUR = 60 * 60 * 1000 ms
     */
    static qint64 interval(AlarmType alarm);

    // ========================================================================
    // Port of: private Class<? extends BroadcastReceiver> alarmClass() (Java lines 64-66)
    // ========================================================================
    /**
     * @brief Returns the alarm class identifier for the specified alarm.
     *
     * Port of:
     *   MESSAGE_CHECKER -> NewMessageChecker.class
     *   CACHE_PRUNER -> RegularCachePruner.class
     */
    static QString alarmClassId(AlarmType alarm);

    // ========================================================================
    // Port of: private boolean startOnBoot() (Java lines 68-70)
    // ========================================================================
    /**
     * @brief Returns whether this alarm should start on device boot.
     *
     * Both alarms return true in the original.
     */
    static bool startOnBoot(AlarmType alarm);

    // ========================================================================
    // Port of: public static void startAlarm(final Alarm alarm, final Context context)
    // Java lines 77-105
    // ========================================================================
    /**
     * @brief Starts the specified alarm.
     *
     * In the Java original, this creates a PendingIntent and registers
     * the alarm with AlarmManager.setInexactRepeating(). In C++, we use
     * a callback-based timer system.
     *
     * Port of the exact logic:
     *   if(!alarmMap.containsKey(alarm)) {
     *       final Intent alarmIntent = new Intent(context, alarm.alarmClass());
     *       ... create PendingIntent with FLAG_IMMUTABLE ...
     *       alarmManager.setInexactRepeating(AlarmManager.RTC, now, interval, pendingIntent);
     *       alarmMap.put(alarm, alarmManager);
     *       intentMap.put(alarm, pendingIntent);
     *   }
     *
     * @param alarm The alarm to start
     * @param callback The function to call when the alarm fires
     */
    static void startAlarm(AlarmType alarm, std::function<void()> callback);

    // ========================================================================
    // Port of: public static void stopAlarm(final Alarm alarm) (Java lines 113-119)
    // ========================================================================
    /**
     * @brief Stops the specified alarm.
     *
     * Port of:
     *   if(alarmMap.containsKey(alarm)) {
     *       alarmMap.get(alarm).cancel(intentMap.get(alarm));
     *       alarmMap.remove(alarm);
     *       intentMap.remove(alarm);
     *   }
     *
     * @param alarm The alarm to stop
     */
    static void stopAlarm(AlarmType alarm);

    // ========================================================================
    // Port of: public static void onBoot(final Context context) (Java lines 127-133)
    // ========================================================================
    /**
     * @brief Starts all alarms that are supposed to start at device boot.
     *
     * Port of:
     *   for(final Alarm alarm : Alarm.values()) {
     *       if(alarm.startOnBoot()) {
     *           startAlarm(alarm, context);
     *       }
     *   }
     *
     * @param callback The function to call when any alarm fires
     */
    static void onBoot(std::function<void(AlarmType)> callback);

    // ========================================================================
    // AlarmManager constants — port of Android constants used
    // ========================================================================
    /** Port of: AlarmManager.INTERVAL_HALF_HOUR = 30 * 60 * 1000 */
    static constexpr qint64 INTERVAL_HALF_HOUR = 30LL * 60LL * 1000LL;

    /** Port of: AlarmManager.INTERVAL_HOUR = 60 * 60 * 1000 */
    static constexpr qint64 INTERVAL_HOUR = 60LL * 60LL * 1000LL;

private:
    // ========================================================================
    // Port of: private static final Map<Alarm, AlarmManager> alarmMap (Java line 34)
    // ========================================================================
    /** @brief Maps alarm type -> timer ID (for cancellation) */
    static QMap<AlarmType, int> s_alarmMap;

    // ========================================================================
    // Port of: private static final Map<Alarm, PendingIntent> intentMap (Java line 35)
    // ========================================================================
    /** @brief Maps alarm type -> callback function */
    static QMap<AlarmType, std::function<void()>> s_intentMap;
};

} // namespace PinkReader
