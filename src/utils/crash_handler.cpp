/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: crash_handler.cpp - graceful crash reporting
 */

#include "utils/crash_handler.h"
#include "utils/logging.h"

#include <cstdlib>
#include <exception>

namespace PinkReader {

namespace {

void onTerminate() {
    try {
        if (auto *e = std::current_exception()) {
            try {
                std::rethrow_exception(e);
            } catch (const std::exception &ex) {
                Logging::error("CrashHandler",
                    QString("Uncaught exception: %1").arg(ex.what()));
            } catch (...) {
                Logging::error("CrashHandler",
                    QString("Uncaught unknown exception"));
            }
        } else {
            Logging::error("CrashHandler",
                QString("Terminate called without an active exception"));
        }
    } catch (...) {
        // Never throw from the terminate handler.
    }
    abort();
}

} // anonymous namespace

void CrashHandler::install() {
    std::set_terminate(onTerminate);
    Logging::info("CrashHandler", "Crash handler installed");
}

} // namespace PinkReader
