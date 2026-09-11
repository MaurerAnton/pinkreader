/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: logging.h - leveled logging with optional file output
 */

#pragma once

#include <QString>

namespace PinkReader {

/**
 * Application-wide logger. Call sites pass a tag (usually the class or
 * module name) plus a message. Used across ~115 files, so the interface
 * is intentionally small and header-stable.
 */
class Logging {
public:
    enum class LogLevel {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
    };

    Logging() = delete;

    static void setLogLevel(LogLevel level);
    static void setLogFile(const QString &path);

    static void debug(const QString &tag, const QString &message);
    static void info(const QString &tag, const QString &message);
    static void warn(const QString &tag, const QString &message);
    static void error(const QString &tag, const QString &message);

private:
    static void log(LogLevel level, const QString &tag, const QString &message);
};

} // namespace PinkReader
