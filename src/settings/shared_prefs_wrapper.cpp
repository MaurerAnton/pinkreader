/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: shared_prefs_wrapper.cpp
 * Port of RedReader's SharedPrefsWrapper.java
 */

#include "settings/shared_prefs_wrapper.h"
#include <QDebug>
#include <QReadLocker>
#include <QWriteLocker>

namespace PinkReader {

const QString SharedPrefsWrapper::TAG = QStringLiteral("SharedPrefsWrapper");

// ============================================================================
// Editor implementation
// ============================================================================

SharedPrefsWrapper::Editor::Editor(QSettings &prefs, QReadWriteLock &restoreLock)
    : m_prefs(prefs)
    , m_restoreLock(restoreLock) {}

SharedPrefsWrapper::Editor &SharedPrefsWrapper::Editor::putString(
        const QString &key,
        const QString &value) {
    m_prefs.setValue(key, value);
    return *this;
}

SharedPrefsWrapper::Editor &SharedPrefsWrapper::Editor::putFloat(
        const QString &key,
        float value) {
    m_prefs.setValue(key, QString::number(value));
    return *this;
}

SharedPrefsWrapper::Editor &SharedPrefsWrapper::Editor::putFloat(
        const QString &key,
        std::optional<float> value) {
    if (value.has_value()) {
        m_prefs.setValue(key, QString::number(value.value()));
    } else {
        m_prefs.setValue(key, QVariant());  // null
    }
    return *this;
}

SharedPrefsWrapper::Editor &SharedPrefsWrapper::Editor::putInt(
        const QString &key,
        int value) {
    m_prefs.setValue(key, value);
    return *this;
}

SharedPrefsWrapper::Editor &SharedPrefsWrapper::Editor::putLong(
        const QString &key,
        qint64 value) {
    m_prefs.setValue(key, value);
    return *this;
}

SharedPrefsWrapper::Editor &SharedPrefsWrapper::Editor::putBoolean(
        const QString &key,
        bool value) {
    m_prefs.setValue(key, value);
    return *this;
}

SharedPrefsWrapper::Editor &SharedPrefsWrapper::Editor::putStringSet(
        const QString &key,
        const QSet<QString> &value) {
    QStringList list(value.begin(), value.end());
    m_prefs.setValue(key, list);
    return *this;
}

void SharedPrefsWrapper::Editor::apply() {
    // Take read lock as we aren't doing an atomic restore
    // (Java: try(Locker ignored = new Locker(mRestoreLock.readLock())))
    QReadLocker locker(&m_restoreLock);
    m_prefs.sync();
}

// ============================================================================
// SharedPrefsWrapper implementation
// ============================================================================

SharedPrefsWrapper::SharedPrefsWrapper(QSettings &prefs)
    : m_prefs(prefs) {}

void SharedPrefsWrapper::registerOnSharedPreferenceChangeListener(
        OnSharedPreferenceChangeListener *listener) {
    QMutexLocker locker(&m_listenerMutex);
    m_activeListeners.insert(listener);
}

void SharedPrefsWrapper::unregisterOnSharedPreferenceChangeListener(
        OnSharedPreferenceChangeListener *listener) {
    QMutexLocker locker(&m_listenerMutex);
    m_activeListeners.remove(listener);
}

bool SharedPrefsWrapper::contains(const QString &key) {
    QReadLocker locker(&m_restoreLock);
    return m_prefs.contains(key);
}

QHash<QString, QVariant> SharedPrefsWrapper::getAllClone() {
    QReadLocker locker(&m_restoreLock);

    QHash<QString, QVariant> result;
    const auto keys = m_prefs.allKeys();
    for (const QString &key : keys) {
        result.insert(key, m_prefs.value(key));
    }
    return result;  // HashMap copy (new HashMap<>(mPrefs.getAll()))
}

QString SharedPrefsWrapper::getString(const QString &key,
                                       const QString &defValue) {
    QReadLocker locker(&m_restoreLock);
    return m_prefs.value(key, defValue).toString();
}

int SharedPrefsWrapper::getInt(const QString &key, int defValue) {
    try {
        QReadLocker locker(&m_restoreLock);
        return m_prefs.value(key, defValue).toInt();
    } catch (const std::exception &e) {
        // Log.e(TAG, "Failed to get pref", e)
        qWarning() << TAG << "Failed to get pref:" << key << e.what();
        return defValue;
    }
}

qint64 SharedPrefsWrapper::getLong(const QString &key, qint64 defValue) {
    try {
        QReadLocker locker(&m_restoreLock);
        return m_prefs.value(key, static_cast<qlonglong>(defValue)).toLongLong();
    } catch (const std::exception &e) {
        // Log.e(TAG, "Failed to get pref", e)
        qWarning() << TAG << "Failed to get pref:" << key << e.what();
        return defValue;
    }
}

QSet<QString> SharedPrefsWrapper::getStringSet(const QString &key,
                                                const QSet<QString> &defValues) {
    QReadLocker locker(&m_restoreLock);
    if (!m_prefs.contains(key)) {
        return defValues;
    }
    QStringList list = m_prefs.value(key).toStringList();
    return QSet<QString>(list.begin(), list.end());
}

bool SharedPrefsWrapper::getBoolean(const QString &key, bool defValue) {
    QReadLocker locker(&m_restoreLock);
    return m_prefs.value(key, defValue).toBool();
}

SharedPrefsWrapper::Editor SharedPrefsWrapper::edit() {
    return Editor(m_prefs, m_restoreLock);
}

} // namespace PinkReader
