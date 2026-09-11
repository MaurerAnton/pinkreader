/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: logging.cpp - leveled logging with optional file output
 */

#include "utils/logging.h"

#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>

namespace PinkReader {

namespace {

Logging::LogLevel g_level = Logging::LogLevel::Info;
QFile g_logFile;
QMutex g_mutex;

const char *levelName(Logging::LogLevel level) {
    switch (level) {
    case Logging::LogLevel::Debug:   return "D";
    case Logging::LogLevel::Info:    return "I";
    case Logging::LogLevel::Warning: return "W";
    case Logging::LogLevel::Error:   return "E";
    }
    return "?";
}

} // anonymous namespace

void Logging::setLogLevel(LogLevel level) {
    QMutexLocker locker(&g_mutex);
    g_level = level;
}

void Logging::setLogFile(const QString &path) {
    QMutexLocker locker(&g_mutex);
    if (g_logFile.isOpen()) {
        g_logFile.close();
    }
    g_logFile.setFileName(path);
    g_logFile.open(QIODevice::Append | QIODevice::Text);
}

void Logging::debug(const QString &tag, const QString &message) {
    log(LogLevel::Debug, tag, message);
}

void Logging::info(const QString &tag, const QString &message) {
    log(LogLevel::Info, tag, message);
}

void Logging::warn(const QString &tag, const QString &message) {
    log(LogLevel::Warning, tag, message);
}

void Logging::error(const QString &tag, const QString &message) {
    log(LogLevel::Error, tag, message);
}

void Logging::log(LogLevel level, const QString &tag, const QString &message) {
    QMutexLocker locker(&g_mutex);
    if (level < g_level) {
        return;
    }
    const QString line = QString("[%1] %2: %3")
        .arg(QLatin1String(levelName(level)), tag, message);
    switch (level) {
    case LogLevel::Debug:
        qDebug().noquote() << line;
        break;
    case LogLevel::Info:
        qInfo().noquote() << line;
        break;
    case LogLevel::Warning:
        qWarning().noquote() << line;
        break;
    case LogLevel::Error:
        qCritical().noquote() << line;
        break;
    }
    if (g_logFile.isOpen()) {
        g_logFile.write(line.toUtf8() + '\n');
        g_logFile.flush();
    }
}

} // namespace PinkReader
