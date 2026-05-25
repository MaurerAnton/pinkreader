/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: gesture_settings.cpp - Implementation of Swipe and gesture configuration
 */

#include "settings/gesture_settings.h"
#include "utils/logging.h"

#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>

namespace PinkReader {

GestureSettings::GestureSettings(QObject *parent)
    : QObject(parent)
{
    m_settings = new QSettings(
        QStringLiteral("PinkReader"),
        QStringLiteral("PinkReader"),
        this);
}

GestureSettings::~GestureSettings()
{
    sync();
}

void GestureSettings::load()
{
    if (m_loaded) {
        Logging::debug("Settings::GestureSettings", "Already loaded");
        return;
    }

    Logging::info("Settings::GestureSettings", "Loading settings...");
    QMutexLocker locker(&m_mutex);

    registerDefaults();

    // Ensure defaults exist for all registered keys
    for (auto it = m_defaults.constBegin();
         it != m_defaults.constEnd(); ++it) {
        if (!m_settings->contains(it.key())) {
            m_settings->setValue(it.key(), it.value().value);
        }
    }

    m_loaded = true;
    emit settingsLoaded();

    Logging::info("Settings::GestureSettings",
        QString("Settings loaded (%1 keys)").arg(allKeys().size()));
}

void GestureSettings::sync()
{
    QMutexLocker locker(&m_mutex);
    m_settings->sync();
}

void GestureSettings::resetToDefaults()
{
    Logging::info("Settings::GestureSettings", "Resetting to defaults...");
    QMutexLocker locker(&m_mutex);

    m_settings->clear();

    for (auto it = m_defaults.constBegin();
         it != m_defaults.constEnd(); ++it) {
        m_settings->setValue(it.key(), it.value().value);
    }

    m_settings->sync();
    emit settingsReset();
}

QString GestureSettings::getString(const QString &key,
                                  const QString &defaultValue) const
{
    QMutexLocker locker(&m_mutex);
    return m_settings->value(key, defaultValue).toString();
}

bool GestureSettings::getBool(const QString &key, bool defaultValue) const
{
    QMutexLocker locker(&m_mutex);
    return m_settings->value(key, defaultValue).toBool();
}

int GestureSettings::getInt(const QString &key, int defaultValue) const
{
    QMutexLocker locker(&m_mutex);
    return m_settings->value(key, defaultValue).toInt();
}

double GestureSettings::getDouble(const QString &key,
                                 double defaultValue) const
{
    QMutexLocker locker(&m_mutex);
    return m_settings->value(key, defaultValue).toDouble();
}

QByteArray GestureSettings::getByteArray(const QString &key,
                                        const QByteArray &defaultValue) const
{
    QMutexLocker locker(&m_mutex);
    return m_settings->value(key, defaultValue).toByteArray();
}

void GestureSettings::setString(const QString &key, const QString &value)
{
    {
        QMutexLocker locker(&m_mutex);
        m_settings->setValue(key, value);
    }
    emit settingChanged(key, QVariant(value));
}

void GestureSettings::setBool(const QString &key, bool value)
{
    {
        QMutexLocker locker(&m_mutex);
        m_settings->setValue(key, value);
    }
    emit settingChanged(key, QVariant(value));
}

void GestureSettings::setInt(const QString &key, int value)
{
    {
        QMutexLocker locker(&m_mutex);
        m_settings->setValue(key, value);
    }
    emit settingChanged(key, QVariant(value));
}

void GestureSettings::setDouble(const QString &key, double value)
{
    {
        QMutexLocker locker(&m_mutex);
        m_settings->setValue(key, value);
    }
    emit settingChanged(key, QVariant(value));
}

void GestureSettings::setByteArray(const QString &key,
                                  const QByteArray &value)
{
    {
        QMutexLocker locker(&m_mutex);
        m_settings->setValue(key, value);
    }
    emit settingChanged(key, QVariant(value));
}

bool GestureSettings::contains(const QString &key) const
{
    QMutexLocker locker(&m_mutex);
    return m_settings->contains(key);
}

void GestureSettings::remove(const QString &key)
{
    QMutexLocker locker(&m_mutex);
    m_settings->remove(key);
}

QStringList GestureSettings::allKeys() const
{
    QMutexLocker locker(&m_mutex);
    return m_settings->allKeys();
}

QJsonObject GestureSettings::exportAll() const
{
    QMutexLocker locker(&m_mutex);
    QJsonObject obj;

    for (const QString &key : m_settings->allKeys()) {
        QVariant val = m_settings->value(key);
        QJsonValue jsonVal = QJsonValue::fromVariant(val);
        if (!jsonVal.isNull() || !val.isNull())
            obj[key] = jsonVal;
    }

    return obj;
}

bool GestureSettings::importAll(const QJsonObject &settings)
{
    Logging::info("Settings::GestureSettings",
        QString("Importing %1 settings...").arg(settings.size()));

    QMutexLocker locker(&m_mutex);

    for (auto it = settings.constBegin();
         it != settings.constEnd(); ++it) {
        m_settings->setValue(it.key(), it.value().toVariant());
    }

    m_settings->sync();
    return true;
}

void GestureSettings::registerDefaults()
{
    // Register default values for all settings
    m_defaults[QStringLiteral("theme")] =
        {QStringLiteral("dark"), QStringLiteral("UI theme")};
    m_defaults[QStringLiteral("font_size")] =
        {14, QStringLiteral("Base font size")};
    m_defaults[QStringLiteral("two_pane_mode")] =
        {true, QStringLiteral("Enable two-pane tablet layout")};
    m_defaults[QStringLiteral("left_handed_mode")] =
        {false, QStringLiteral("Enable left-handed mode")};
    m_defaults[QStringLiteral("nsfw_enabled")] =
        {false, QStringLiteral("Show NSFW content")};
    m_defaults[QStringLiteral("blur_nsfw")] =
        {true, QStringLiteral("Blur NSFW thumbnails")};
    m_defaults[QStringLiteral("auto_refresh")] =
        {false, QStringLiteral("Auto-refresh old listings")};
    m_defaults[QStringLiteral("precache_images")] =
        {QStringLiteral("wifi_only"), QStringLiteral("Image precaching strategy")};
    m_defaults[QStringLiteral("default_sort")] =
        {QStringLiteral("hot"), QStringLiteral("Default post sort order")};
    m_defaults[QStringLiteral("comment_sort")] =
        {QStringLiteral("confidence"), QStringLiteral("Default comment sort")};
    m_defaults[QStringLiteral("language_override")] =
        {QString(), QStringLiteral("Language override (empty = system)")};
    m_defaults[QStringLiteral("swipe_left_action")] =
        {QStringLiteral("downvote"), QStringLiteral("Post swipe left action")};
    m_defaults[QStringLiteral("swipe_right_action")] =
        {QStringLiteral("upvote"), QStringLiteral("Post swipe right action")};
    m_defaults[QStringLiteral("hide_read_posts")] =
        {false, QStringLiteral("Hide read posts")};
    m_defaults[QStringLiteral("keep_screen_on")] =
        {false, QStringLiteral("Keep screen on while reading")};
    m_defaults[QStringLiteral("internal_browser")] =
        {true, QStringLiteral("Use internal browser")};
    m_defaults[QStringLiteral("custom_tabs")] =
        {false, QStringLiteral("Use Android Custom Tabs")};
    m_defaults[QStringLiteral("show_link_buttons")] =
        {true, QStringLiteral("Show link buttons")};
    m_defaults[QStringLiteral("inbox_check_enabled")] =
        {true, QStringLiteral("Enable inbox checking")};
    m_defaults[QStringLiteral("inbox_check_interval")] =
        {15, QStringLiteral("Inbox check interval (minutes)")};
    m_defaults[QStringLiteral("cache_max_size_mb")] =
        {512, QStringLiteral("Maximum cache size (MB)")};
    m_defaults[QStringLiteral("cache_location")] =
        {QString(), QStringLiteral("Cache storage location")};
    m_defaults[QStringLiteral("thumbnail_size")] =
        {72, QStringLiteral("Thumbnail size (pixels)")};
    m_defaults[QStringLiteral("inline_image_previews")] =
        {true, QStringLiteral("Show inline image previews")};
    m_defaults[QStringLiteral("video_autoplay")] =
        {QStringLiteral("wifi_only"), QStringLiteral("Video autoplay setting")};
    m_defaults[QStringLiteral("notification_enabled")] =
        {true, QStringLiteral("Enable notifications")};
    m_defaults[QStringLiteral("navigation_bar_color")] =
        {QStringLiteral("primary"), QStringLiteral("Navigation bar color")};
    m_defaults[QStringLiteral("bottom_toolbar")] =
        {false, QStringLiteral("Use bottom toolbar")};
    m_defaults[QStringLiteral("hide_toolbar_on_scroll")] =
        {true, QStringLiteral("Hide toolbar on scroll")};
    m_defaults[QStringLiteral("screen_orientation")] =
        {QStringLiteral("auto"), QStringLiteral("Screen orientation preference")};
    m_defaults[QStringLiteral("backup_enabled")] =
        {false, QStringLiteral("Enable settings backup")};
    m_defaults[QStringLiteral("tor_enabled")] =
        {false, QStringLiteral("Enable Tor/Orbot proxy")};
    m_defaults[QStringLiteral("custom_client_id")] =
        {QString(), QStringLiteral("Custom Reddit OAuth client ID")};
    m_defaults[QStringLiteral("show_developer_announcements")] =
        {true, QStringLiteral("Show developer announcements")};
    m_defaults[QStringLiteral("accessibility_concise_mode")] =
        {false, QStringLiteral("Concise accessibility mode")};
    m_defaults[QStringLiteral("accessibility_min_comment_height")] =
        {48, QStringLiteral("Minimum comment height for accessibility")};
    m_defaults[QStringLiteral("accessibility_announce_indent")] =
        {false, QStringLiteral("Announce comment indent levels")};
}

} // namespace PinkReader
