/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * File: application.h
 * Description: Custom QApplication subclass for PinkReader
 */


#pragma once

#include <QApplication>
#include <QEvent>
#include <QFileOpenEvent>
#include <QUrl>
#include <QString>
#include <QObject>
#include <QTimer>
#include <QScreen>
#include <QPalette>

/**
 * @namespace PinkReader
 * @brief Root namespace for all PinkReader application code
 *
 * All PinkReader classes, functions, and types are contained
 * within this namespace to avoid naming collisions with Qt
 * and third-party libraries.
 */
namespace PinkReader {

// Forward declarations
class DeepLinkHandler;
class IntentHandler;

/**
 * @brief Custom QApplication subclass for PinkReader
 *
 * Provides centralized event handling for the entire application,
 * including deep link processing, file open events, screen change
 * notifications, and application state transitions (suspend/resume).
 *
 * This class extends QApplication to add PinkReader-specific
 * functionality while maintaining full Qt compatibility.
 *
 * Key responsibilities:
 * - Deep link handling (reddit.com URLs, /r/subreddit links)
 * - Application lifecycle management (Android Activity events)
 * - Screen orientation and density change handling
 * - Global palette and styling management
 * - File open event interception for sharing
 *
 * @note On Android, this class interfaces with the Qt Android
 *       platform plugin which translates Android Activity lifecycle
 *       events into Qt application events.
 */
class Application : public QApplication
{
    Q_OBJECT

public:
    /**
     * @brief Construct the PinkReader Application
     *
     * Initializes the application with standard Qt arguments,
     * sets up deep link handling, and configures Android-specific
     * behavior if running on the Android platform.
     *
     * @param argc Reference to argument count (from main)
     * @param argv Argument vector (from main)
     */
    explicit Application(int &argc, char **argv);

    /**
     * @brief Destructor - cleans up application resources
     *
     * Saves application state, flushes logs, and releases
     * platform-specific resources.
     */
    ~Application() override;

    /**
     * @brief Get the deep link handler instance
     * @return Reference to the DeepLinkHandler singleton
     */
    static DeepLinkHandler &deepLinkHandler();

    /**
     * @brief Get the intent handler instance (Android-specific)
     * @return Reference to the IntentHandler singleton
     */
    static IntentHandler &intentHandler();

    /**
     * @brief Check if the application is running on a tablet
     * @return true if the device screen size suggests tablet usage
     */
    static bool isTabletDevice();

    /**
     * @brief Check if the application is in two-pane tablet mode
     * @return true if two-pane layout is active
     */
    static bool isTwoPaneMode();

    /**
     * @brief Check if the app is currently in the foreground
     * @return true if the app is active/foregrounded
     */
    bool isForeground() const { return m_isForeground; }

    /**
     * @brief Get the current screen orientation
     * @return Qt::ScreenOrientation value
     */
    Qt::ScreenOrientation currentOrientation() const;

    /**
     * @brief Request a specific screen orientation
     * @param orientation The desired orientation
     *
     * On Android, this sets the Activity's requested orientation.
     * On desktop, this has no effect.
     */
    void requestOrientation(Qt::ScreenOrientation orientation);

    /**
     * @brief Handle a deep link URL (e.g., from another app)
     * @param url The URL to handle
     *
     * Processes reddit.com, redd.it, v.redd.it, and /s/ URLs
     * and navigates to the appropriate content within PinkReader.
     */
    void handleDeepLink(const QUrl &url);

signals:
    /**
     * @brief Emitted when the application enters the foreground
     *
     * On Android, this corresponds to Activity.onResume().
     * Used to refresh content, update notifications, etc.
     */
    void foregrounded();

    /**
     * @brief Emitted when the application enters the background
     *
     * On Android, this corresponds to Activity.onPause().
     * Used to pause video playback, save state, etc.
     */
    void backgrounded();

    /**
     * @brief Emitted when the screen orientation changes
     * @param orientation The new orientation
     */
    void orientationChanged(Qt::ScreenOrientation orientation);

    /**
     * @brief Emitted when a deep link is received
     * @param url The received URL
     */
    void deepLinkReceived(const QUrl &url);

    /**
     * @brief Emitted when a file open request is received
     * @param filePath Path to the file to open
     */
    void fileOpenRequested(const QString &filePath);

    /**
     * @brief Emitted when the system theme changes (light/dark)
     * @param isDark true if the system switched to dark mode
     */
    void systemThemeChanged(bool isDark);

protected:
    /**
     * @brief Override event() to intercept platform-specific events
     *
     * Handles QFileOpenEvent for deep linking on macOS/iOS,
     * and other platform-specific events that need application-level
     * processing.
     *
     * @param event The event to process
     * @return true if the event was handled, false otherwise
     */
    bool event(QEvent *event) override;

private:
    bool m_isForeground = true;
    DeepLinkHandler *m_deepLinkHandler = nullptr;
    IntentHandler *m_intentHandler = nullptr;

    // Platform-specific initialization
    void initializeAndroid();
    void initializeDesktop();

    // Screen management
    void connectScreenSignals();
    void onPrimaryScreenChanged(QScreen *screen);
    void onScreenOrientationChanged(Qt::ScreenOrientation orientation);
    void onScreenLogicalDotsPerInchChanged(qreal dpi);

    // Application state
    void onApplicationStateChanged(Qt::ApplicationState state);

    // Deep link processing
    bool processRedditUrl(const QUrl &url);
    bool processShareIntent(const QString &text);

private slots:
    /**
     * @brief Handle Android Activity result (for OAuth, sharing, etc.)
     * @param requestCode The request code from startActivityForResult
     * @param resultCode The result code from the activity
     * @param data Intent data returned by the activity
     */
    void onActivityResult(int requestCode, int resultCode, const QVariantMap &data);

    /**
     * @brief Handle Android permission result
     * @param permission The requested permission
     * @param granted Whether permission was granted
     */
    void onPermissionResult(const QString &permission, bool granted);

    /**
     * @brief Handle Android new intent (when app is already running)
     * @param data Intent data
     */
    void onNewIntent(const QVariantMap &data);
};

} // namespace PinkReader
