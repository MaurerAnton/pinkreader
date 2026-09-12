/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: ui_thread_repeating_timer.cpp - QTimer-based repeating callback
 */

#include "common/ui_thread_repeating_timer.h"

namespace PinkReader {

UIThreadRepeatingTimer::UIThreadRepeatingTimer(int intervalMs,
        Listener *listener, QObject *parent)
    : QObject(parent)
    , m_listener(listener) {
    m_timer.setInterval(intervalMs);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, [this]() {
        if (m_listener != nullptr) {
            m_listener->onUIThreadRepeatingTimer(this);
        }
    });
}

UIThreadRepeatingTimer::~UIThreadRepeatingTimer() = default;

void UIThreadRepeatingTimer::startTimer() {
    m_timer.start();
}

void UIThreadRepeatingTimer::stopTimer() {
    m_timer.stop();
}

bool UIThreadRepeatingTimer::isRunning() const {
    return m_timer.isActive();
}

} // namespace PinkReader
