/*
 * PinkReader - GPLv3
 * File: new_message_checker.h
 * Exact port of NewMessageChecker.java (318 lines)
 *
 * Ports every field, method, constant, and inner class exactly.
 * Android-specific BroadcastReceiver, NotificationManager, PendingIntent
 * are adapted to platform-independent interfaces.
 *
 * NOTE: The task requested "NewMessageReceiver.java" but that file does not
 * exist in the source tree. The closest match is NewMessageChecker.java,
 * which is ported here as new_message_checker.h/.cpp
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <atomic>
#include <functional>
#include <memory>

namespace PinkReader {

// Forward declarations — Android/RedReader types used in signatures
class Context;
class Intent;
class RedditAccount;
class CacheManager;
class CacheRequest;
class CacheRequestCallbacks;
class RRError;
class SharedPrefsWrapper;
class GenericFactory_SeekableInputStream;
class TimestampUTC;
class UriString;
class FailedRequestBody;
class RedditThing;
class RedditListing;
class RedditComment;
class RedditMessage;
class RedditIdAndType;
class UrlEncodedString;

/**
 * @brief Port of org.quantumbadger.redreader.receivers.NewMessageChecker
 *
 * Checks for new Reddit messages/notifications.
 * Extends BroadcastReceiver in Java. In C++, it provides the same
 * static checkForNewMessages method and notification creation logic.
 *
 * Every field, method, constant, and logic branch ported exactly.
 */
class NewMessageChecker : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Constants — Java lines 69-74
    // ========================================================================

    // Port of: private static final String TAG = "NewMessageChecker";
    static const QString TAG;

    // Port of: private static final String NOTIFICATION_CHANNEL_ID = "RRNewMessageChecker";
    static const QString NOTIFICATION_CHANNEL_ID;

    // Port of: public static final String PREFS_SAVED_MESSAGE_ID = "LastMessageId";
    static const QString PREFS_SAVED_MESSAGE_ID;

    // Port of: public static final String PREFS_SAVED_MESSAGE_TIMESTAMP = "LastMessageTimestamp";
    static const QString PREFS_SAVED_MESSAGE_TIMESTAMP;

    // ========================================================================
    // Constructor / Destructor
    // ========================================================================
    explicit NewMessageChecker(QObject *parent = nullptr);
    ~NewMessageChecker() override;

    // ========================================================================
    // onReceive — Java lines 77-81
    // Port of: @Override public void onReceive(final Context context,
    //                                            final Intent intent)
    // ========================================================================
    /**
     * @brief Handles the broadcast intent.
     *
     * Port of:
     *   checkForNewMessages(context);
     *   AnnouncementDownloader.performDownload(context);
     */
    void onReceive(Context *context, Intent *intent);

    // ========================================================================
    // checkForNewMessages — Java lines 83-253
    // Port of: public static void checkForNewMessages(final Context context)
    // ========================================================================
    /**
     * @brief Checks the Reddit API for unread messages.
     *
     * If new messages are found, saves the message ID/timestamp and
     * creates a notification. The full logic from Java lines 83-253
     * is ported exactly.
     */
    static void checkForNewMessages(Context *context);

    // ========================================================================
    // createNotification — Java lines 257-317
    // Port of: public static void createNotification(final String title,
    //                                                  final String text,
    //                                                  final Context context)
    // ========================================================================
    /**
     * @brief Creates and shows a system notification.
     *
     * Creates a notification channel on Android 8+ if not already created.
     * Port of Java lines 257-317 exactly.
     *
     * @param title The notification title
     * @param text The notification body text
     * @param context The context
     */
    static void createNotification(
        const QString &title,
        const QString &text,
        Context *context);

    // ========================================================================
    // Notification callback setup — C++ adaptation
    // ========================================================================
    /**
     * @brief Set a callback to be invoked when the notification is clicked.
     *
     * In Java, this is handled by PendingIntent.getActivity().
     * In C++, we use a callback function instead.
     */
    static void setNotificationClickCallback(std::function<void()> callback);

private:
    // ========================================================================
    // Private static fields
    // ========================================================================

    // Port of: private static final AtomicBoolean sChannelCreated
    //         = new AtomicBoolean(false); (Java line 255)
    static std::atomic<bool> s_channelCreated;

    // Port of: notification click callback (C++ adaptation of PendingIntent)
    static std::function<void()> s_notificationClickCallback;
};

} // namespace PinkReader
