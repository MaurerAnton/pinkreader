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
 * File: main.cpp
 * Description: Application entry point
 */


#include "core/application.h"
#include "core/pinkreader.h"
#include "core/constants.h"
#include "core/version.h"
#include "utils/logging.h"
#include "utils/crash_handler.h"
#include "utils/data_migration.h"
#include "settings/preferences.h"
#include "settings/theme_manager.h"
#include "settings/font_manager.h"
#include "settings/language_manager.h"
#include "settings/layout_manager.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QStandardPaths>
#include <QSurfaceFormat>
#include <QStyle>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QSysInfo>
#include <QScreen>

/**
 * @brief Parse command line arguments for PinkReader
 *
 * Supported arguments:
 *   --debug         Enable debug logging
 *   --theme NAME    Override the default theme
 *   --reset-settings Reset all settings to defaults
 *   --version       Display version information
 *   --help          Display help message
 *   --log-file PATH Redirect logs to a specific file
 *   --no-sandbox    Disable security sandboxing (debug builds only)
 *   --locale LANG   Override the system locale
 *   --test-mode     Run in test mode with simulated data
 *
 * @param app Reference to the QApplication instance
 * @return true if the application should continue, false to exit
 */
static bool parseCommandLine(QApplication &app)
{
    // Configure command line parser with detailed descriptions
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral(
        "PinkReader - An open source Reddit client for Android\n"
        "Version: %1\n"
        "Built with Qt %2\n\n"
        "PinkReader is a feature-complete Reddit client that provides:\n"
        "  - Multi-account support with OAuth authentication\n"
        "  - Browse subreddits, multireddits, and user profiles\n"
        "  - View posts, comments, images, videos, and galleries\n"
        "  - Vote, save, hide, and report content\n"
        "  - Inbox management with private messaging\n"
        "  - Extensive customization with themes and layout options\n"
        "  - Offline caching and image precaching\n"
        "  - Accessibility features and screen reader support\n"
        "  - Material Design 3 interface\n"
    ).arg(PinkReader::Version::fullVersionString(),
          QString::fromLatin1(qVersion())));

    parser.addHelpOption();
    parser.addVersionOption();

    // Debug mode flag - enables verbose logging and developer tools
    QCommandLineOption debugOption(
        QStringList() << "d" << "debug",
        QStringLiteral("Enable debug logging and developer tools."));
    parser.addOption(debugOption);

    // Theme override - allows specifying theme from command line
    QCommandLineOption themeOption(
        QStringList() << "t" << "theme",
        QStringLiteral("Override the default theme. Options: light, dark, night, "
                       "ultra_black, blue, low_contrast_night."),
        QStringLiteral("theme"),
        QStringLiteral("light"));
    parser.addOption(themeOption);

    // Reset settings to factory defaults
    QCommandLineOption resetOption(
        QStringList() << "reset-settings",
        QStringLiteral("Reset all settings to factory defaults."));
    parser.addOption(resetOption);

    // Custom log file path
    QCommandLineOption logOption(
        QStringList() << "l" << "log-file",
        QStringLiteral("Redirect log output to a specific file."),
        QStringLiteral("path"));
    parser.addOption(logOption);

    // No sandbox mode (debug builds)
    QCommandLineOption noSandboxOption(
        QStringLiteral("no-sandbox"),
        QStringLiteral("Disable security sandboxing. For debug builds only."));
    parser.addOption(noSandboxOption);

    // Locale override
    QCommandLineOption localeOption(
        QStringLiteral("locale"),
        QStringLiteral("Override the system locale (e.g., en_US, de_DE, fr_FR)."),
        QStringLiteral("locale"));
    parser.addOption(localeOption);

    // Test mode for automated testing
    QCommandLineOption testModeOption(
        QStringLiteral("test-mode"),
        QStringLiteral("Run in test mode with simulated data."));
    parser.addOption(testModeOption);

    // Parse the arguments
    parser.process(app);

    // Process debug mode
    if (parser.isSet(debugOption)) {
        PinkReader::Logging::setLogLevel(PinkReader::Logging::LogLevel::Debug);
        PinkReader::Logging::info("main", "Debug logging enabled");
    }

    // Process theme override
    if (parser.isSet(themeOption)) {
        QString theme = parser.value(themeOption).toLower();
        PinkReader::Logging::info("main",
            QString("Command line theme override: %1").arg(theme));
        // Theme will be applied during Application initialization
    }

    // Process settings reset
    if (parser.isSet(resetOption)) {
        PinkReader::Logging::warn("main",
            "Resetting all settings to factory defaults...");
        PinkReader::Preferences::instance().resetToDefaults();
        PinkReader::Preferences::instance().sync();
    }

    // Process log file redirect
    if (parser.isSet(logOption)) {
        QString logPath = parser.value(logOption);
        PinkReader::Logging::setLogFile(logPath);
        PinkReader::Logging::info("main",
            QString("Log output redirected to: %1").arg(logPath));
    }

    // Process locale override
    if (parser.isSet(localeOption)) {
        QString locale = parser.value(localeOption);
        QLocale::setDefault(QLocale(locale));
        PinkReader::Logging::info("main",
            QString("Locale overridden to: %1").arg(locale));
    }

    // Process test mode
    if (parser.isSet(testModeOption)) {
        PinkReader::Logging::info("main", "Running in test mode");
        // Test mode configuration will be handled by Application
    }

    return true;
}

/**
 * @brief Initialize the Qt application environment
 *
 * Sets up the application attributes, surface format for OpenGL,
 * default font configuration, and platform-specific settings
 * for optimal rendering on Android devices.
 *
 * @param app The QApplication instance to configure
 */
static void initializeApplicationEnvironment(QApplication &app)
{
    // Set application metadata for QSettings and platform identification
    app.setApplicationName(QStringLiteral("PinkReader"));
    app.setApplicationVersion(PinkReader::Version::fullVersionString());
    app.setOrganizationName(QStringLiteral("PinkReader"));
    app.setOrganizationDomain(QStringLiteral("org.pinkreader.app"));
    app.setApplicationDisplayName(QStringLiteral("PinkReader for Reddit"));

    // Enable high-DPI scaling for modern displays
    // Qt automatically handles pixel ratio on Android, but we set it explicitly
    // for desktop testing compatibility
    app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    // Configure OpenGL surface format for image viewer
    // Request OpenGL ES 3.0 for better mobile GPU compatibility
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 0);
    glFormat.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
    glFormat.setRenderableType(QSurfaceFormat::RenderableType::OpenGLES);
    glFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
    glFormat.setSwapInterval(1);  // Enable vsync for smooth animations
    glFormat.setDepthBufferSize(24);
    glFormat.setStencilBufferSize(8);
    glFormat.setSamples(4);  // 4x MSAA for antialiased rendering
    QSurfaceFormat::setDefaultFormat(glFormat);

    // Set application style to Fusion for consistent cross-platform appearance
    // Fusion style provides better performance on mobile devices than native styles
    app.setStyle(QStyleFactory::create(QStringLiteral("Fusion")));

    // Configure default font
    QFont defaultFont = app.font();
    defaultFont.setPixelSize(14);
    defaultFont.setHintingPreference(QFont::PreferFullHinting);
    app.setFont(defaultFont);

    // Platform-specific initialization
#ifdef Q_OS_ANDROID
    // Android-specific: keep screen on while reading
    // This is handled in the main window for user preference override
    PinkReader::Logging::info("main",
        QString("Running on Android %1 (SDK %2)")
            .arg(QSysInfo::productVersion())
            .arg(QSysInfo::kernelVersion()));

    // On Android, the data directory is in the app's private storage
    QString dataPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    PinkReader::Logging::info("main",
        QString("Data directory: %1").arg(dataPath));
#endif

    // Install crash handler for graceful error reporting
    PinkReader::CrashHandler::install();

    // Run data migration if needed (for app updates)
    PinkReader::DataMigration::runMigrations();
}

/**
 * @brief Load and apply system translations
 *
 * Loads Qt base translations and PinkReader-specific translations
 * based on the current locale and user language preferences.
 *
 * @param app The QApplication instance
 * @param translator Pointer to the app translator (modified in-place)
 * @param qtTranslator Pointer to the Qt base translator (modified in-place)
 */
static void loadTranslations(QApplication &app,
                              QTranslator &translator,
                              QTranslator &qtTranslator)
{
    // Determine the target locale
    QLocale locale = QLocale::system();
    QString langPref = PinkReader::Preferences::instance()
        .getString("language_override", "");

    if (!langPref.isEmpty()) {
        locale = QLocale(langPref);
        PinkReader::Logging::info("main",
            QString("Language override active: %1").arg(langPref));
    }

    // Load Qt base translations (standard dialogs, message boxes, etc.)
    QString qtTranslationsPath =
        QLibraryInfo::path(QLibraryInfo::TranslationsPath);

    if (qtTranslator.load(locale, QStringLiteral("qtbase"),
                          QStringLiteral("_"), qtTranslationsPath)) {
        app.installTranslator(&qtTranslator);
        PinkReader::Logging::debug("main",
            QString("Loaded Qt base translation for: %1")
                .arg(locale.name()));
    } else {
        PinkReader::Logging::debug("main",
            QString("No Qt base translation available for: %1")
                .arg(locale.name()));
    }

    // Load PinkReader translations
    QString appTranslationsPath = QStringLiteral(":/translations");

    if (translator.load(locale, QStringLiteral("pinkreader"),
                        QStringLiteral("_"), appTranslationsPath)) {
        app.installTranslator(&translator);
        PinkReader::Logging::info("main",
            QString("Loaded PinkReader translation for: %1")
                .arg(locale.name()));
    } else {
        PinkReader::Logging::info("main",
            QString("No PinkReader translation for: %1, using English")
                .arg(locale.name()));
    }
}

/**
 * @brief Main entry point for the PinkReader application
 *
 * Initializes the Qt application framework, loads settings,
 * applies the user's theme, sets up translations, and launches
 * the main application window. Handles both Android and desktop
 * platforms through Qt's cross-platform abstractions.
 *
 * The initialization sequence is:
 * 1. Create QApplication
 * 2. Load and apply persisted preferences
 * 3. Parse command line arguments
 * 4. Configure application environment and surface format
 * 5. Apply theme from user preferences
 * 6. Configure font scaling
 * 7. Load translations
 * 8. Create and show the main window
 * 9. Enter the event loop
 *
 * @param argc Argument count from the platform
 * @param argv Argument vector from the platform
 * @return Application exit code (0 for success, non-zero for error)
 *
 * @note On Android, the application lifecycle is managed by the Android
 *       Activity, not by main(). Qt handles the Activity lifecycle events
 *       internally through its Android platform plugin.
 */
int main(int argc, char *argv[])
{
    // Create the application instance
    // Use default constructor - high DPI is enabled in initializeApplicationEnvironment
    PinkReader::Application app(argc, argv);

    // Load persisted user preferences from storage
    // This must happen before any UI is created so that theme,
    // font, and layout settings are applied during initialization
    PinkReader::Preferences::instance().load();

    // Parse command line arguments - may override some preferences
    if (!parseCommandLine(app)) {
        return 0;  // User requested --help or --version
    }

    // Initialize the application environment:
    // - Set application metadata
    // - Configure OpenGL surface format for image/video rendering
    // - Apply Fusion style for cross-platform consistency
    // - Set up default font configuration
    // - Install crash handler
    // - Run data migrations if updating from older version
    initializeApplicationEnvironment(app);

    // Apply the user's selected theme via stylesheet
    // The theme system supports 6 built-in themes plus custom CSS
    PinkReader::ThemeManager::instance().applyCurrentTheme(&app);

    // Configure font scaling based on user preferences
    // Users can independently scale different text categories:
    // body, comment headers, link buttons, posts, subtitles, post headers
    PinkReader::FontManager::instance().applyFontScaling(&app);

    // Apply layout settings (left-handed mode, tablet mode, etc.)
    PinkReader::LayoutManager::instance().applyLayoutSettings();

    // Load and install translations for the user's locale
    QTranslator appTranslator;
    QTranslator qtTranslator;
    loadTranslations(app, appTranslator, qtTranslator);

    // Log successful initialization
    PinkReader::Logging::info("main",
        QString("PinkReader v%1 initialized successfully")
            .arg(PinkReader::Version::fullVersionString()));
    PinkReader::Logging::info("main",
        QString("Qt version: %1, Platform: %2")
            .arg(QString::fromLatin1(qVersion()),
                 QSysInfo::prettyProductName()));
    PinkReader::Logging::debug("main",
        QString("Screen DPI: %1, Device Pixel Ratio: %2")
            .arg(app.primaryScreen()->logicalDotsPerInch())
            .arg(app.primaryScreen()->devicePixelRatio()));

    // Create the main PinkReader application controller
    // This manages the main window, navigation stack, and global state
    PinkReader::PinkReaderApp pinkReaderApp;

    // Initialize the main application (load saved session, restore state)
    pinkReaderApp.initialize();

    // Get and show the main window
    QMainWindow *mainWindow = pinkReaderApp.mainWindow();
    if (mainWindow) {
        // Restore window geometry from saved preferences
        QByteArray savedGeometry = PinkReader::Preferences::instance()
            .getByteArray("window_geometry");
        if (!savedGeometry.isEmpty()) {
            mainWindow->restoreGeometry(savedGeometry);
        }

        // Restore window state (toolbar positions, dock widgets, etc.)
        QByteArray savedState = PinkReader::Preferences::instance()
            .getByteArray("window_state");
        if (!savedState.isEmpty()) {
            mainWindow->restoreState(savedState);
        }

        mainWindow->show();
        PinkReader::Logging::info("main", "Main window displayed");
    } else {
        PinkReader::Logging::error("main",
            "Failed to create main window!");
        return 1;
    }

    // Enter the Qt event loop
    // On Android, this loop is managed by the Android Activity lifecycle
    // On desktop, this runs until the last window is closed
    int result = app.exec();

    // Save window geometry and state before exit
    if (mainWindow) {
        PinkReader::Preferences::instance().setByteArray(
            "window_geometry", mainWindow->saveGeometry());
        PinkReader::Preferences::instance().setByteArray(
            "window_state", mainWindow->saveState());
    }

    // Save preferences to persistent storage
    PinkReader::Preferences::instance().sync();

    // Cleanup application resources
    pinkReaderApp.shutdown();

    PinkReader::Logging::info("main",
        QString("PinkReader exiting with code %1").arg(result));

    return result;
}
