/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: data_migration.cpp - app-update data migrations
 */

#include "utils/data_migration.h"
#include "utils/logging.h"

#include <QSettings>

namespace PinkReader {

namespace {

constexpr char kVersionKey[] = "migration/version";

int storedVersion() {
    return QSettings().value(QLatin1String(kVersionKey), 0).toInt();
}

void storeVersion(int version) {
    QSettings settings;
    settings.setValue(QLatin1String(kVersionKey), version);
    settings.sync();
}

} // anonymous namespace

void DataMigration::runMigrations() {
    const int from = storedVersion();
    if (from >= kCurrentVersion) {
        return;
    }
    // v1: baseline — no data changes, just stamp the version so future
    // migrations know where this install started.
    Logging::info("DataMigration",
        QString("Migrating data from v%1 to v%2").arg(from).arg(kCurrentVersion));
    storeVersion(kCurrentVersion);
}

} // namespace PinkReader
