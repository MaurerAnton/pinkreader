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
 * File: application.cpp
 * Description: Application implementation
 */


#include "core/application.h"
#include "core/constants.h"
#include "core/version.h"
#include "utils/logging.h"
#include "utils/deep_link_handler.h"
#include "utils/intent_handler.h"
#include "utils/crash_handler.h"
#include "settings/preferences.h"
#include "settings/theme_manager.h"

#include <QFileOpenEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QStyleHints>
#include <QTimer>
#include <QDir>
#include <QStandardPaths>
#include <QWindow>
#include <QDesktopWidget>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#endif

namespace PinkReader {

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_isForeground(true)
{
    Logging::info("Application", "Initializing PinkReader application...");

    // Set application metadata for identification
    setApplicationName(QStringLiteral("PinkReader"));
    setApplicationVersion(Version::fullVersionString());
    setOrganizationName(QStringLiteral("PinkReader"));
    setOrganizationDomain(QStringLiteral("org.pinkreader.app"));
    setApplicationDisplayName(QStringLiteral("PinkReader"));

    // Create utility handlers
    m_deepLinkHandler = new DeepLinkHandler(this);

#ifdef Q_OS_ANDROID
    m_intentHandler = new IntentHandler(this);
    initializeAndroid();
#else
    initializeDesktop();
#endif

    // Connect application-level signals
    connectScreenSignals();

    // Monitor application state changes (foreground/background)
    connect(this, &QApplication::applicationStateChanged,
            this, &Application::onApplicationStateChanged);

    // Monitor system theme changes (light/dark mode switching)
    connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
            this, [this]() {
        bool isDark = QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
        Logging::info("Application",
            QString("System theme changed to: %1")
                .arg(isDark ? QStringLiteral("Dark") : QStringLiteral("Light")));
        emit systemThemeChanged(isDark);
    });

    Logging::info("Application", "PinkReader application initialized");
}

Application::~Application()
{
    Logging::info("Application", "Shutting down PinkReader application...");

    // Save state before destruction
    Preferences::instance().sync();

    Logging::info("Application", "PinkReader application shutdown complete");
}

// ---------------------------------------------------------------------------
// Static Accessors
// ---------------------------------------------------------------------------

DeepLinkHandler &Application::deepLinkHandler()
{
    auto *app = qobject_cast<Application *>(QApplication::instance());
    Q_ASSERT(app != nullptr);
    Q_ASSERT(app->m_deepLinkHandler != nullptr);
    return *app->m_deepLinkHandler;
}

IntentHandler &Application::intentHandler()
{
    auto *app = qobject_cast<Application *>(QApplication::instance());
    Q_ASSERT(app != nullptr);
    Q_ASSERT(app->m_intentHandler != nullptr);
    return *app->m_intentHandler;
}

bool Application::isTabletDevice()
{
    // Check if the device has tablet-sized screen
    // Tablets typically have a smallest width of 600dp or more
    if (primaryScreen() == nullptr) {
        return false;
    }

    QSize screenSize = primaryScreen()->size();
    qreal dpi = primaryScreen()->logicalDotsPerInch();

    // Calculate screen size in inches
    qreal widthInches = screenSize.width() / dpi;
    qreal heightInches = screenSize.height() / dpi;
    qreal diagonalInches = std::sqrt(widthInches * widthInches +
                                      heightInches * heightInches);

    // Tablets typically have screens >= 7 inches diagonal
    bool isTablet = diagonalInches >= 7.0;

    Logging::debug("Application",
        QString("Screen: %1x%2, DPI: %3, Diagonal: %4\" -> Tablet: %5")
            .arg(screenSize.width())
            .arg(screenSize.height())
            .arg(dpi, 0, 'f', 1)
            .arg(diagonalInches, 0, 'f', 1)
            .arg(isTablet ? QStringLiteral("yes") : QStringLiteral("no")));

    return isTablet;
}

bool Application::isTwoPaneMode()
{
    // Two-pane mode is enabled if:
    // 1. The device is a tablet (or large phone)
    // 2. The user has not disabled it in settings
    bool userEnabled = Preferences::instance()
        .getBool("two_pane_mode", true);
    bool tabletDevice = isTabletDevice();

    // On very large phones (6.5"+), also allow two-pane
    bool largePhone = false;
    if (!tabletDevice && primaryScreen() != nullptr) {
        QSize size = primaryScreen()->size();
        qreal dpi = primaryScreen()->logicalDotsPerInch();
        qreal widthInches = size.width() / dpi;
        largePhone = widthInches >= 3.0; // 3 inches wide = big phone
    }

    return userEnabled && (tabletDevice || largePhone);
}

// ---------------------------------------------------------------------------
// Public Methods
// ---------------------------------------------------------------------------

Qt::ScreenOrientation Application::currentOrientation() const
{
    if (primaryScreen() == nullptr) {
        return Qt::PrimaryOrientation;
    }
    return primaryScreen()->orientation();
}

void Application::requestOrientation(Qt::ScreenOrientation orientation)
{
    Logging::debug("Application",
        QString("Requesting orientation: %1").arg(static_cast<int>(orientation)));

#ifdef Q_OS_ANDROID
    // On Android, we set the Activity's requested orientation
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        jint androidOrientation;
        switch (orientation) {
            case Qt::PortraitOrientation:
                androidOrientation = 1; // SCREEN_ORIENTATION_PORTRAIT
                break;
            case Qt::LandscapeOrientation:
                androidOrientation = 0; // SCREEN_ORIENTATION_LANDSCAPE
                break;
            case Qt::InvertedPortraitOrientation:
                androidOrientation = 9; // SCREEN_ORIENTATION_REVERSE_PORTRAIT
                break;
            case Qt::InvertedLandscapeOrientation:
                androidOrientation = 8; // SCREEN_ORIENTATION_REVERSE_LANDSCAPE
                break;
            default:
                androidOrientation = 4; // SCREEN_ORIENTATION_SENSOR (auto)
                break;
        }
        activity.callMethod<void>("setRequestedOrientation",
                                   "(I)V", androidOrientation);
    }
#else
    Q_UNUSED(orientation)
    // Desktop: orientation changes are handled by the window manager
#endif
}

void Application::handleDeepLink(const QUrl &url)
{
    Logging::info("Application",
        QString("Processing deep link: %1").arg(url.toString()));

    if (processRedditUrl(url)) {
        emit deepLinkReceived(url);
    }
}

// ---------------------------------------------------------------------------
// Protected - Event Handling
// ---------------------------------------------------------------------------

bool Application::event(QEvent *event)
{
    if (event->type() == QEvent::FileOpen) {
        auto *openEvent = static_cast<QFileOpenEvent *>(event);
        QString filePath = openEvent->file();

        Logging::info("Application",
            QString("File open event: %1").arg(filePath));

        emit fileOpenRequested(filePath);
        return true;
    }

    return QApplication::event(event);
}

// ---------------------------------------------------------------------------
// Private - Platform Initialization
// ---------------------------------------------------------------------------

void Application::initializeAndroid()
{
    Logging::info("Application", "Initializing Android platform...");

#ifdef Q_OS_ANDROID
    // Request necessary Android permissions
    // Note: Some permissions (like notifications on Android 13+)
    // are requested at runtime when first needed
    QStringList permissions;
    permissions << QStringLiteral("android.permission.INTERNET")
                << QStringLiteral("android.permission.ACCESS_NETWORK_STATE")
                << QStringLiteral("android.permission.WRITE_EXTERNAL_STORAGE")
                << QStringLiteral("android.permission.READ_EXTERNAL_STORAGE");

    // Check and request each permission
    for (const QString &permission : permissions) {
        auto result = QtAndroid::checkPermission(permission);
        if (result == QtAndroid::PermissionResult::Denied) {
            Logging::info("Application",
                QString("Requesting permission: %1").arg(permission));
            QtAndroid::requestPermissionsSync(QStringList() << permission);
        }
    }

    // Set up Android intent filter for deep links
    // This is typically handled in AndroidManifest.xml, but we can also
    // register programmatically for certain intent types
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        QAndroidJniObject intent = activity.callObjectMethod(
            "getIntent", "()Landroid/content/Intent;");
        if (intent.isValid()) {
            QAndroidJniObject action = intent.callObjectMethod(
                "getAction", "()Ljava/lang/String;");
            if (action.isValid()) {
                QString actionStr = action.toString();
                Logging::info("Application",
                    QString("Launch intent action: %1").arg(actionStr));

                // Handle VIEW intents (deep links from other apps)
                if (actionStr == QStringLiteral("android.intent.action.VIEW")) {
                    QAndroidJniObject data = intent.callObjectMethod(
                        "getData", "()Landroid/net/Uri;");
                    if (data.isValid()) {
                        QString urlStr = data.callObjectMethod(
                            "toString", "()Ljava/lang/String;").toString();
                        Logging::info("Application",
                            QString("Launch intent data: %1").arg(urlStr));

                        // Process the URL after the event loop starts
                        QTimer::singleShot(100, this, [this, urlStr]() {
                            handleDeepLink(QUrl(urlStr));
                        });
                    }
                }

                // Handle SEND intents (share from other apps)
                if (actionStr == QStringLiteral("android.intent.action.SEND")) {
                    QString type = intent.callObjectMethod(
                        "getType", "()Ljava/lang/String;").toString();

                    if (type == QStringLiteral("text/plain")) {
                        QString sharedText = intent.callObjectMethod(
                            "getStringExtra",
                            "(Ljava/lang/String;)Ljava/lang/String;",
                            QAndroidJniObject::fromString(
                                QStringLiteral("android.intent.extra.TEXT"))
                                .object<jstring>())
                            .toString();

                        if (!sharedText.isEmpty()) {
                            Logging::info("Application",
                                QString("Received shared text: %1")
                                    .arg(sharedText.left(100)));

                            QTimer::singleShot(200, this, [this, sharedText]() {
                                processShareIntent(sharedText);
                            });
                        }
                    }
                }
            }
        }
    }
#endif
}

void Application::initializeDesktop()
{
    Logging::info("Application", "Initializing desktop platform...");

    // Desktop-specific initialization:
    // - Set up system tray icon
    // - Register URL scheme handler (reddit://)
    // - Configure window management

    // Create required data directories
    QString dataPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);

    QString cachePath = QStandardPaths::writableLocation(
        QStandardPaths::CacheLocation);
    QDir().mkpath(cachePath);

    Logging::debug("Application",
        QString("Data path: %1").arg(dataPath));
    Logging::debug("Application",
        QString("Cache path: %1").arg(cachePath));
}

void Application::connectScreenSignals()
{
    // Monitor screen changes for orientation and DPI updates
    QScreen *screen = primaryScreen();
    if (screen == nullptr) {
        Logging::warn("Application", "No primary screen detected!");
        return;
    }

    // Orientation changes
    connect(screen, &QScreen::orientationChanged,
            this, &Application::onScreenOrientationChanged);

    // DPI changes (important for Android when moving between displays)
    connect(screen, &QScreen::logicalDotsPerInchChanged,
            this, &Application::onScreenLogicalDotsPerInchChanged);

    // Primary screen change (device connected to external display)
    connect(this, &QGuiApplication::primaryScreenChanged,
            this, &Application::onPrimaryScreenChanged);

    Logging::debug("Application",
        QString("Connected screen signals for: %1 (%2x%3)")
            .arg(screen->name())
            .arg(screen->size().width())
            .arg(screen->size().height()));
}

// ---------------------------------------------------------------------------
// Private - Signal Handlers
// ---------------------------------------------------------------------------

void Application::onPrimaryScreenChanged(QScreen *screen)
{
    if (screen) {
        Logging::info("Application",
            QString("Primary screen changed to: %1 (%2x%3)")
                .arg(screen->name())
                .arg(screen->size().width())
                .arg(screen->size().height()));

        // Re-apply theme and font scaling for the new screen
        ThemeManager::instance().applyCurrentTheme(this);
    }
}

void Application::onScreenOrientationChanged(Qt::ScreenOrientation orientation)
{
    Logging::debug("Application",
        QString("Screen orientation changed: %1")
            .arg(static_cast<int>(orientation)));

    // Update two-pane mode if needed
    emit orientationChanged(orientation);
}

void Application::onScreenLogicalDotsPerInchChanged(qreal dpi)
{
    Logging::debug("Application",
        QString("Screen DPI changed: %1").arg(dpi, 0, 'f', 1));

    // Font scaling may need re-application when DPI changes
}

void Application::onApplicationStateChanged(Qt::ApplicationState state)
{
    switch (state) {
        case Qt::ApplicationActive:
            if (!m_isForeground) {
                m_isForeground = true;
                Logging::info("Application", "App entered foreground");
                emit foregrounded();
            }
            break;

        case Qt::ApplicationInactive:
        case Qt::ApplicationSuspended:
        case Qt::ApplicationHidden:
            if (m_isForeground) {
                m_isForeground = false;
                Logging::info("Application", "App entered background");
                emit backgrounded();
            }
            break;

        default:
            break;
    }
}

// ---------------------------------------------------------------------------
// Private - URL/Intent Processing
// ---------------------------------------------------------------------------

bool Application::processRedditUrl(const QUrl &url)
{
    QString host = url.host().toLower();
    QString path = url.path();

    Logging::debug("Application",
        QString("Processing Reddit URL - Host: %1, Path: %2")
            .arg(host, path));

    // Handle reddit.com and www.reddit.com
    if (host.endsWith(QStringLiteral("reddit.com")) ||
        host.endsWith(QStringLiteral("redd.it"))) {

        // Check for /r/subreddit pattern
        QRegularExpression subredditRx("^/r/([^/]+)(/comments/([^/]+)/([^/]+))?/?(.*)?$");
        auto match = subredditRx.match(path);

        if (match.hasMatch()) {
            QString subreddit = match.captured(1);
            QString postId = match.captured(3);
            QString postSlug = match.captured(4);

            Logging::info("Application",
                QString("Deep link - Subreddit: %1, Post: %2")
                    .arg(subreddit, postId.isEmpty() ? QStringLiteral("(none)") : postId));

            if (!postId.isEmpty()) {
                // Navigate to specific post
                QTimer::singleShot(300, this, [this, subreddit, postId]() {
                    Q_UNUSED(subreddit)
                    Q_UNUSED(postId)
                    // Navigation handled by PinkReaderApp
                });
            } else {
                // Navigate to subreddit
                QTimer::singleShot(300, this, [this, subreddit]() {
                    Q_UNUSED(subreddit)
                    // Navigation handled by PinkReaderApp
                });
            }
            return true;
        }

        // Check for /u/username pattern
        QRegularExpression userRx("^/u(?:ser)?/([^/]+)/?(.*)?$");
        auto userMatch = userRx.match(path);

        if (userMatch.hasMatch()) {
            QString username = userMatch.captured(1);
            Logging::info("Application",
                QString("Deep link - User: %1").arg(username));
            // Navigate to user profile
            return true;
        }
    }

    // Handle v.redd.it (Reddit video links)
    if (host == QStringLiteral("v.redd.it")) {
        Logging::info("Application",
            QString("Deep link - Reddit video: %1").arg(url.toString()));
        return true;
    }

    // Handle reddit.com/s/ short links
    if (path.startsWith(QStringLiteral("/s/"))) {
        QString shortCode = path.mid(3);
        Logging::info("Application",
            QString("Deep link - Short link: %1").arg(shortCode));
        // Resolve short link via Reddit API
        return true;
    }

    // Handle reddit.com/r/subreddit/s/ short links
    QRegularExpression shortSubRx("^/r/([^/]+)/s/(.+)$");
    auto shortSubMatch = shortSubRx.match(path);
    if (shortSubMatch.hasMatch()) {
        QString subreddit = shortSubMatch.captured(1);
        QString shortCode = shortSubMatch.captured(2);
        Logging::info("Application",
            QString("Deep link - Subreddit short link: r/%1/s/%2")
                .arg(subreddit, shortCode));
        return true;
    }

    Logging::debug("Application",
        QString("URL not recognized as Reddit deep link: %1").arg(url.toString()));
    return false;
}

bool Application::processShareIntent(const QString &text)
{
    Logging::debug("Application",
        QString("Processing share intent text: %1").arg(text.left(100)));

    // Check if it's a URL
    QUrl url(text.trimmed());
    if (url.isValid() && !url.scheme().isEmpty()) {
        return processRedditUrl(url);
    }

    // For plain text, could use it as post text via submit dialog
    // This would be implemented to open PostSubmitView with the text pre-filled

    Logging::debug("Application", "Share intent text is not a URL");
    return false;
}

// ---------------------------------------------------------------------------
// Private Slots
// ---------------------------------------------------------------------------

void Application::onActivityResult(int requestCode, int resultCode,
                                     const QVariantMap &data)
{
    Logging::debug("Application",
        QString("Activity result - Request: %1, Result: %2")
            .arg(requestCode).arg(resultCode));

    // Handle OAuth login result
    if (requestCode == Constants::OAUTH_LOGIN_REQUEST_CODE) {
        if (resultCode == -1) { // RESULT_OK on Android
            Logging::info("Application", "OAuth login successful");
            // OAuth token extraction is handled by the OAuth manager
        } else {
            Logging::warn("Application", "OAuth login cancelled or failed");
        }
    }

    // Handle image picker result (for post/comment image upload)
    if (requestCode == Constants::IMAGE_PICKER_REQUEST_CODE) {
        if (resultCode == -1 && data.contains(QStringLiteral("uri"))) {
            QString imageUri = data.value(QStringLiteral("uri")).toString();
            Logging::info("Application",
                QString("Image selected: %1").arg(imageUri));
            // Image upload is handled by the active view
        }
    }

    // Handle file picker result
    if (requestCode == Constants::FILE_PICKER_REQUEST_CODE) {
        if (resultCode == -1 && data.contains(QStringLiteral("uri"))) {
            QString fileUri = data.value(QStringLiteral("uri")).toString();
            Logging::info("Application",
                QString("File selected: %1").arg(fileUri));
        }
    }
}

void Application::onPermissionResult(const QString &permission, bool granted)
{
    Logging::info("Application",
        QString("Permission result - %1: %2")
            .arg(permission,
                 granted ? QStringLiteral("GRANTED") : QStringLiteral("DENIED")));

    if (!granted) {
        Logging::warn("Application",
            QString("Permission denied: %1. Some features may not work.")
                .arg(permission));
    }
}

void Application::onNewIntent(const QVariantMap &data)
{
    Logging::debug("Application", "New intent received while app was running");

    // Handle URL from new intent
    if (data.contains(QStringLiteral("url"))) {
        QUrl url(data.value(QStringLiteral("url")).toString());
        handleDeepLink(url);
    }

    // Handle shared text from new intent
    if (data.contains(QStringLiteral("sharedText"))) {
        QString text = data.value(QStringLiteral("sharedText")).toString();
        processShareIntent(text);
    }
}

} // namespace PinkReader
