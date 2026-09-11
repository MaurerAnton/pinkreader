/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: crash_handler.h - graceful crash reporting
 */

#pragma once

namespace PinkReader {

/**
 * Installs process-wide crash handling (terminate handler that logs the
 * failure before aborting). Called once from main().
 */
class CrashHandler {
public:
    CrashHandler() = delete;

    static void install();
};

} // namespace PinkReader
