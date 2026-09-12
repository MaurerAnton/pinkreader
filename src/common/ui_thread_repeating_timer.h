/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: ui_thread_repeating_timer.h - QTimer-based repeating callback
 *
 * Port of RedReader's UIThreadRepeatingTimer: fires
 * Listener::onUIThreadRepeatingTimer at a fixed interval until stopped.
 */

#pragma once

#include <QObject>
#include <QTimer>

namespace PinkReader {

/**
 * Repeating timer that notifies a Listener on the UI thread.
 * Used for double-tap detection gaps in the image viewers.
 */
class UIThreadRepeatingTimer : public QObject {
    Q_OBJECT
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void onUIThreadRepeatingTimer(UIThreadRepeatingTimer *timer) = 0;
    };

    explicit UIThreadRepeatingTimer(int intervalMs, Listener *listener,
        QObject *parent = nullptr);
    ~UIThreadRepeatingTimer() override;

    void startTimer();
    void stopTimer();
    bool isRunning() const;

private:
    QTimer m_timer;
    Listener *m_listener;
};

} // namespace PinkReader
