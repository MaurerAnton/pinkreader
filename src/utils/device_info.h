/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: device_info.h - Port of DeviceInfo.java
 *
 * NOTE: DeviceInfo.java does not exist in the redreader source tree.
 * This file provides a common device information abstraction layer
 * that would typically extract system information (screen size,
 * OS version, device model, etc.) in an Android app.
 *
 * In the Qt/C++ port, this provides equivalent cross-platform
 * system information queries.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QSize>
#include <cstdint>

namespace PinkReader {

// ============================================================================
// DeviceType enum — port of common Android device classifications
// ============================================================================

// Port of: typical Android device type categorization
enum class DeviceType {
    PHONE,      // Screen width < 600dp
    TABLET_7,   // Screen width 600-719dp (7-inch tablet)
    TABLET_10,  // Screen width 720dp+ (10-inch tablet)
    UNKNOWN
};

// ============================================================================
// ScreenInfo — port of screen/display metrics
// ============================================================================

class ScreenInfo {
public:
    // Port of: DisplayMetrics.widthPixels
    int widthPixels = 0;

    // Port of: DisplayMetrics.heightPixels
    int heightPixels = 0;

    // Port of: DisplayMetrics.densityDpi
    int densityDpi = 160;  // mdpi default

    // Port of: DisplayMetrics.scaledDensity
    float scaledDensity = 1.0f;

    // Port of: DisplayMetrics.xdpi
    float xdpi = 160.0f;

    // Port of: DisplayMetrics.ydpi
    float ydpi = 160.0f;

    // Convenience: width in dp
    int widthDp() const {
        return static_cast<int>(widthPixels / (densityDpi / 160.0f));
    }

    // Convenience: height in dp
    int heightDp() const {
        return static_cast<int>(heightPixels / (densityDpi / 160.0f));
    }
};

// ============================================================================
// DeviceInfo — port of common.DeviceInfo
//
// Provides static methods to query device information.
// Every method ported from common Android device info patterns.
// ============================================================================

class DeviceInfo : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // Singleton access (port of typical Android singleton pattern)
    // ========================================================================

    static DeviceInfo *instance();

    // ========================================================================
    // Static utility methods
    // ========================================================================

    // Port of: getting screen/display metrics from Context.getResources().getDisplayMetrics()
    static ScreenInfo getScreenInfo();

    // Port of: device type detection (phone vs tablet)
    static DeviceType getDeviceType();

    // Port of: checks if the device is a tablet
    static bool isTablet();

    // Port of: checks if the device is a phone
    static bool isPhone();

    // ========================================================================
    // System version information
    // ========================================================================

    // Port of: Build.VERSION.SDK_INT
    static int getSdkVersion();

    // Port of: Build.VERSION.RELEASE
    static QString getOsVersion();

    // Port of: Build.MANUFACTURER
    static QString getManufacturer();

    // Port of: Build.MODEL
    static QString getModel();

    // ========================================================================
    // Hardware information
    // ========================================================================

    // Port of: detecting available memory
    static int64_t getTotalMemoryBytes();

    // Port of: detecting available storage
    static int64_t getAvailableStorageBytes(const QString &path = QStringLiteral("/"));

    // Port of: detecting screen size category
    static QString getScreenSizeCategory();

    // ========================================================================
    // Network information
    // ========================================================================

    // Port of: detecting if network is connected
    static bool isNetworkConnected();

    // Port of: detecting if connected via WiFi
    static bool isConnectedViaWifi();

    // Port of: detecting if connected via mobile data
    static bool isConnectedViaMobile();

    // ========================================================================
    // Application information
    // ========================================================================

    // Port of: getting app version name
    static QString getAppVersionName();

    // Port of: getting app version code
    static int getAppVersionCode();

    // Port of: getting package name
    static QString getPackageName();

    // Port of: checking if device supports feature (e.g., camera, NFC)
    static bool hasSystemFeature(const QString &feature);

signals:
    // Port of: screen size changed callback
    void screenSizeChanged(int width, int height);

    // Port of: network connectivity changed callback
    void networkConnectivityChanged(bool connected);

private:
    DeviceInfo(QObject *parent = nullptr);
    virtual ~DeviceInfo();

    static DeviceInfo *s_instance;

    ScreenInfo m_screenInfo;
    DeviceType m_deviceType = DeviceType::UNKNOWN;
};

} // namespace PinkReader
