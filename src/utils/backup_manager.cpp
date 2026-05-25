/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: backup_manager.cpp - Backup manager implementation
 */


#include "utils/backup_manager.h"
#include "utils/logging.h"
#include "settings/preferences.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QFileInfo>

namespace PinkReader {

BackupManager::BackupManager(QObject *parent) : QObject(parent) {}
BackupManager::~BackupManager() = default;

void BackupManager::initialize()
{
    Logging::info("BackupManager", "Initializing backup manager...");
    m_autoBackup = Preferences::instance().getBool("backup_enabled", false);
    Logging::info("BackupManager", QString("Auto-backup: %1").arg(m_autoBackup ? "enabled" : "disabled"));
}

bool BackupManager::createBackup(const QString &path)
{
    QString backupPath = path.isEmpty() ? defaultBackupPath() : path;
    QDir().mkpath(QFileInfo(backupPath).absolutePath());

    QJsonObject backupData = Preferences::instance().exportAll();
    QJsonDocument doc(backupData);

    QFile file(backupPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        Logging::info("BackupManager", QString("Backup created: %1").arg(backupPath));
        emit backupCreated(backupPath);
        return true;
    }

    Logging::error("BackupManager", QString("Failed to create backup: %1").arg(backupPath));
    emit backupFailed("Failed to write backup file");
    return false;
}

bool BackupManager::restoreBackup(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        emit backupFailed("Cannot open backup file");
        return false;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        emit backupFailed(error.errorString());
        return false;
    }

    if (Preferences::instance().importAll(doc.object())) {
        emit backupRestored();
        return true;
    }

    emit backupFailed("Failed to import settings");
    return false;
}

QStringList BackupManager::listBackups() const
{
    QDir dir(QFileInfo(defaultBackupPath()).absolutePath());
    return dir.entryList({"*.json"}, QDir::Files, QDir::Time);
}

bool BackupManager::deleteBackup(const QString &filename)
{
    QString path = QFileInfo(defaultBackupPath()).absolutePath() + "/" + filename;
    return QFile::remove(path);
}

void BackupManager::setAutoBackupEnabled(bool enabled)
{
    m_autoBackup = enabled;
    Preferences::instance().setBool("backup_enabled", enabled);
}

bool BackupManager::isAutoBackupEnabled() const { return m_autoBackup; }

int BackupManager::backupCount() const { return listBackups().size(); }

BackupManager::BackupInfo BackupManager::backupInfo(const QString &filename) const
{
    BackupInfo info;
    info.filename = filename;
    QString path = QFileInfo(defaultBackupPath()).absolutePath() + "/" + filename;
    QFileInfo fi(path);
    info.timestamp = fi.lastModified();
    info.sizeBytes = fi.size();
    return info;
}

QString BackupManager::defaultBackupPath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/backups/pinkreader_backup_"
           + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")
           + ".json";
}

} // namespace PinkReader
