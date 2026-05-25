/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: device_info.cpp - Port of DeviceInfo.java
 *
 * NOTE: DeviceInfo.java does not exist in the redreader source tree.
 * This file provides a common device information abstraction layer
 * for the C++/Qt port, with stubs for platform-specific queries.
 *
 * Every method is implemented with reasonable defaults for a desktop
 * Qt application.
 */

#include "utils/device_info.h"

#include <QApplication>
#include <QScreen>
#include <QStringLiteral>
#include <QSysInfo>
#include <QStorageInfo>
#include <QFileInfo>
#include <cstdint>

namespace PinkReader {

// ============================================================================
// Static members
// ============================================================================

DeviceInfo *DeviceInfo::s_instance = nullptr;

// ============================================================================
// Singleton
// ============================================================================

DeviceInfo *DeviceInfo::instance() {
    if (!s_instance) {
        s_instance = new DeviceInfo();
    }
    return s_instance;
}

// ============================================================================
// Constructor / Destructor
// ============================================================================

DeviceInfo::DeviceInfo(QObject *parent)
    : QObject(parent)
{
    // Initialize screen info from the primary screen
    if (QApplication::instance()) {
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QSize size = screen->size();
            m_screenInfo.widthPixels = size.width();
            m_screenInfo.heightPixels = size.height();
            m_screenInfo.densityDpi = static_cast<int>(screen->logicalDotsPerInch());
            m_screenInfo.scaledDensity = screen->devicePixelRatio();
            m_screenInfo.xdpi = static_cast<float>(screen->physicalDotsPerInchX());
            m_screenInfo.ydpi = static_cast<float>(screen->physicalDotsPerInchY());
        }
    }

    // Determine device type from screen width in dp
    int widthDp = m_screenInfo.widthDp();
    if (widthDp < 600) {
        m_deviceType = DeviceType::PHONE;
    } else if (widthDp < 720) {
        m_deviceType = DeviceType::TABLET_7;
    } else {
        m_deviceType = DeviceType::TABLET_10;
    }
}

DeviceInfo::~DeviceInfo() = default;

// ============================================================================
// ScreenInfo
// ============================================================================

// Port of: getting screen/display metrics from Context.getResources().getDisplayMetrics()
ScreenInfo DeviceInfo::getScreenInfo() {
    return instance()->m_screenInfo;
}

// Port of: device type detection
DeviceType DeviceInfo::getDeviceType() {
    return instance()->m_deviceType;
}

// Port of: checks if the device is a tablet
bool DeviceInfo::isTablet() {
    DeviceType type = instance()->m_deviceType;
    return type == DeviceType::TABLET_7 || type == DeviceType::TABLET_10;
}

// Port of: checks if the device is a phone
bool DeviceInfo::isPhone() {
    return instance()->m_deviceType == DeviceType::PHONE;
}

// ============================================================================
// System version information
// ============================================================================

// Port of: Build.VERSION.SDK_INT
int DeviceInfo::getSdkVersion() {
    // Qt desktop: report a high SDK version for compatibility
    // Android 14 = SDK 34
    return 34;
}

// Port of: Build.VERSION.RELEASE
QString DeviceInfo::getOsVersion() {
    // Port of: Build.VERSION.RELEASE → Qt's OS version
    return QSysInfo::productVersion();
}

// Port of: Build.MANUFACTURER
QString DeviceInfo::getManufacturer() {
    // Port of: Build.MANUFACTURER → Qt's machine host name or generic
    return QSysInfo::machineHostName();
}

// Port of: Build.MODEL
QString DeviceInfo::getModel() {
    // Port of: Build.MODEL → Qt's kernel type + version
    return QSysInfo::kernelType() + QStringLiteral(" ") + QSysInfo::kernelVersion();
}

// ============================================================================
// Hardware information
// ============================================================================

// Port of: detecting available memory (Runtime.getRuntime().totalMemory())
int64_t DeviceInfo::getTotalMemoryBytes() {
    // Stub: return a reasonable default for desktop
    return 8LL * 1024 * 1024 * 1024;  // 8 GB
}

// Port of: detecting available storage
int64_t DeviceInfo::getAvailableStorageBytes(const QString &path) {
    QStorageInfo storage(path);
    if (storage.isValid()) {
        return storage.bytesAvailable();
    }
    // Default: 1GB available
    return 1LL * 1024 * 1024 * 1024;
}

// Port of: detecting screen size category
QString DeviceInfo::getScreenSizeCategory() {
    // Port of: Configuration.SCREENLAYOUT_SIZE_MASK
    int widthDp = instance()->m_screenInfo.widthDp();
    if (widthDp < 360) {
        return QStringLiteral("small");
    } else if (widthDp < 600) {
        return QStringLiteral("normal");
    } else if (widthDp < 720) {
        return QStringLiteral("large");
    } else {
        return QStringLiteral("xlarge");
    }
}

// ============================================================================
// Network information
// ============================================================================

// Port of: ConnectivityManager.getActiveNetworkInfo().isConnected()
bool DeviceInfo::isNetworkConnected() {
    // Stub: assume connected in desktop environment
    return true;
}

// Port of: ConnectivityManager.TYPE_WIFI
bool DeviceInfo::isConnectedViaWifi() {
    // Stub: assume WiFi for desktop
    return true;
}

// Port of: ConnectivityManager.TYPE_MOBILE
bool DeviceInfo::isConnectedViaMobile() {
    // Stub: no mobile data on desktop
    return false;
}

// ============================================================================
// Application information
// ============================================================================

// Port of: PackageInfo.versionName
QString DeviceInfo::getAppVersionName() {
    return QStringLiteral("1.0.0");
}

// Port of: PackageInfo.versionCode
int DeviceInfo::getAppVersionCode() {
    return 1;
}

// Port of: Context.getPackageName()
QString DeviceInfo::getPackageName() {
    return QStringLiteral("org.quantumbadger.pinkreader");
}

// Port of: PackageManager.hasSystemFeature(String)
bool DeviceInfo::hasSystemFeature(const QString & /*feature*/) {
    // Stub: desktop doesn't have Android features like camera, NFC, etc.
    // Return false for all
    return false;
}

} // namespace PinkReader
