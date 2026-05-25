/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: backup_manager.h - Settings backup and restore manager
 */


#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QJsonObject>

namespace PinkReader {

class BackupManager : public QObject
{
    Q_OBJECT

public:
    explicit BackupManager(QObject *parent = nullptr);
    ~BackupManager() override;

    void initialize();

    bool createBackup(const QString &path = QString());
    bool restoreBackup(const QString &path);
    QStringList listBackups() const;

    bool deleteBackup(const QString &filename);
    void setAutoBackupEnabled(bool enabled);
    bool isAutoBackupEnabled() const;
    int backupCount() const;

    struct BackupInfo {
        QString filename;
        QDateTime timestamp;
        QString version;
        qint64 sizeBytes = 0;
    };

    BackupInfo backupInfo(const QString &filename) const;

signals:
    void backupCreated(const QString &path);
    void backupRestored();
    void backupFailed(const QString &error);

private:
    bool m_autoBackup = false;
    QString defaultBackupPath() const;
};

} // namespace PinkReader
