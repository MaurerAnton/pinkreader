/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: notification_manager.h - Local notification manager
 */


#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include <QVector>

namespace PinkReader {

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    explicit NotificationManager(QObject *parent = nullptr);
    ~NotificationManager() override;

    void initialize();

    void showNotification(const QString &title, const QString &message);
    void showInboxNotification(int unreadCount);
    void showReplyNotification(const QString &author, const QString &subreddit);
    void cancelAll();

    void setEnabled(bool enabled);
    bool isEnabled() const;

    void setCheckInterval(int minutes);
    int checkInterval() const;

    void checkNow();

signals:
    void notificationPosted(const QString &title, const QString &message);
    void unreadCountChanged(int unread);
    void checkCompleted(bool hasNewMessages);

private:
    void startTimer();
    void stopTimer();
    void performCheck();

    bool m_enabled = true;
    int m_checkInterval = 15; // minutes
    int m_lastUnreadCount = 0;
    QTimer *m_checkTimer = nullptr;
    QVector<QString> m_notificationHistory;
    static constexpr int MAX_HISTORY = 50;
};

} // namespace PinkReader
