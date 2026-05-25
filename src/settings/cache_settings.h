/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: cache_settings.h - Cache behavior settings
 */

#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QSettings>
#include <QJsonObject>
#include <QMutex>
#include <QFont>

namespace PinkReader {

class CacheSettings : public QObject
{
    Q_OBJECT

public:
    explicit CacheSettings(QObject *parent = nullptr);
    ~CacheSettings() override;

    // Core settings access
    void load();
    void sync();
    void resetToDefaults();

    // Typed accessors
    QString getString(const QString &key, const QString &defaultValue = QString()) const;
    bool getBool(const QString &key, bool defaultValue = false) const;
    int getInt(const QString &key, int defaultValue = 0) const;
    double getDouble(const QString &key, double defaultValue = 0.0) const;
    QByteArray getByteArray(const QString &key, const QByteArray &defaultValue = QByteArray()) const;

    // Typed setters
    void setString(const QString &key, const QString &value);
    void setBool(const QString &key, bool value);
    void setInt(const QString &key, int value);
    void setDouble(const QString &key, double value);
    void setByteArray(const QString &key, const QByteArray &value);

    // Key management
    bool contains(const QString &key) const;
    void remove(const QString &key);
    QStringList allKeys() const;

    // Import/Export
    QJsonObject exportAll() const;
    bool importAll(const QJsonObject &settings);

signals:
    void settingChanged(const QString &key, const QVariant &value);
    void settingsLoaded();
    void settingsReset();

private:
    void registerDefaults();

    mutable QMutex m_mutex;
    QSettings *m_settings = nullptr;
    bool m_loaded = false;

    struct DefaultValue {
        QVariant value;
        QString description;
    };

    QMap<QString, DefaultValue> m_defaults;
};

} // namespace PinkReader
