/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: notification_manager.cpp - Notification manager implementation
 */


#include "utils/notification_manager.h"
#include "utils/logging.h"

namespace PinkReader {

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent)
    , m_checkTimer(new QTimer(this))
{
    connect(m_checkTimer, &QTimer::timeout, this, &NotificationManager::performCheck);
}

NotificationManager::~NotificationManager()
{
    stopTimer();
}

void NotificationManager::initialize()
{
    Logging::info("NotificationManager", "Initializing notification manager...");
    if (m_enabled && m_checkInterval > 0) {
        startTimer();
    }
    Logging::info("NotificationManager", "Notification manager initialized");
}

void NotificationManager::showNotification(const QString &title, const QString &message)
{
    if (!m_enabled) return;
    Logging::info("NotificationManager", QString("Notification: %1 - %2").arg(title, message));
    m_notificationHistory.prepend(QString("%1: %2").arg(title, message));
    if (m_notificationHistory.size() > MAX_HISTORY)
        m_notificationHistory.resize(MAX_HISTORY);
    emit notificationPosted(title, message);
}

void NotificationManager::showInboxNotification(int unreadCount)
{
    if (!m_enabled || unreadCount <= m_lastUnreadCount) return;
    m_lastUnreadCount = unreadCount;
    showNotification(tr("New Messages"),
                     tr("You have %1 unread message(s)").arg(unreadCount));
    emit unreadCountChanged(unreadCount);
}

void NotificationManager::showReplyNotification(const QString &author, const QString &subreddit)
{
    showNotification(tr("New Reply"),
                     tr("%1 replied in r/%2").arg(author, subreddit));
}

void NotificationManager::cancelAll()
{
    m_notificationHistory.clear();
}

void NotificationManager::setEnabled(bool enabled)
{
    m_enabled = enabled;
    if (enabled) startTimer(); else stopTimer();
}

bool NotificationManager::isEnabled() const { return m_enabled; }

void NotificationManager::setCheckInterval(int minutes)
{
    m_checkInterval = qMax(1, qMin(minutes, 60));
    if (m_checkTimer->isActive()) {
        stopTimer();
        startTimer();
    }
}

int NotificationManager::checkInterval() const { return m_checkInterval; }

void NotificationManager::checkNow() { performCheck(); }

void NotificationManager::startTimer()
{
    if (!m_checkTimer->isActive())
        m_checkTimer->start(m_checkInterval * 60 * 1000);
}

void NotificationManager::stopTimer()
{
    if (m_checkTimer->isActive())
        m_checkTimer->stop();
}

void NotificationManager::performCheck()
{
    Logging::debug("NotificationManager", "Performing inbox check...");
    emit checkCompleted(false);
}

} // namespace PinkReader
