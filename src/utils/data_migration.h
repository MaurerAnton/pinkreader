/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: data_migration.h - app-update data migrations
 */

#pragma once

namespace PinkReader {

/**
 * Runs one-time data migrations on app start (called from main()).
 * The migration registry is append-only: add a new step with a higher
 * version instead of modifying completed ones.
 */
class DataMigration {
public:
    DataMigration() = delete;

    static void runMigrations();

    // Current schema version. Bump when adding a migration step.
    static constexpr int kCurrentVersion = 1;
};

} // namespace PinkReader
