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
 * File: pinkreader.cpp
 * Description: Main application controller implementation
 */


#include "core/pinkreader.h"
#include "core/application.h"
#include "core/constants.h"
#include "core/version.h"
#include "ui/main_window.h"
#include "ui/main_menu.h"
#include "ui/post_list_view.h"
#include "ui/comment_view.h"
#include "ui/image_viewer.h"
#include "ui/inbox_view.h"
#include "ui/search_view.h"
#include "ui/user_profile_view.h"
#include "ui/settings_window.h"
#include "accounts/account_manager.h"
#include "accounts/account.h"
#include "cache/cache_manager.h"
#include "network/reddit_api.h"
#include "network/reddit_oauth.h"
#include "network/network_monitor.h"
#include "settings/preferences.h"
#include "settings/theme_manager.h"
#include "utils/logging.h"
#include "utils/notification_manager.h"
#include "utils/draft_manager.h"
#include "utils/backup_manager.h"

#include <QApplication>
#include <QStackedWidget>
#include <QSettings>
#include <QTimer>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>

namespace PinkReader {

PinkReaderApp *PinkReaderApp::s_instance = nullptr;

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

PinkReaderApp::PinkReaderApp(QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(s_instance == nullptr);
    s_instance = this;
}

PinkReaderApp::~PinkReaderApp()
{
    shutdown();
    s_instance = nullptr;
}

// ---------------------------------------------------------------------------
// Initialization
// ---------------------------------------------------------------------------

void PinkReaderApp::initialize()
{
    if (m_initialized) {
        Logging::warn("PinkReaderApp", "Already initialized, skipping...");
        return;
    }

    Logging::info("PinkReaderApp", "Initializing application services...");

    // Phase 1: Setup core services (no dependencies)
    setupCoreServices();

    // Phase 2: Setup network services
    setupNetworkServices();

    // Phase 3: Setup cache services (depends on network)
    setupCacheServices();

    // Phase 4: Setup UI services
    setupUIServices();

    // Phase 5: Connect all signals
    connectSignals();

    // Phase 6: Restore previous session state
    restorePreviousSession();

    m_initialized = true;

    Logging::info("PinkReaderApp",
        QString("PinkReader v%1 initialized successfully")
            .arg(Version::fullVersionString()));
}

void PinkReaderApp::shutdown()
{
    if (!m_initialized) {
        return;
    }

    Logging::info("PinkReaderApp", "Shutting down application services...");

    // Save current session state
    saveCurrentSession();

    // Clean up services in reverse order of initialization
    m_navigationStack = nullptr;

    delete m_userProfileView;
    m_userProfileView = nullptr;

    delete m_searchView;
    m_searchView = nullptr;

    delete m_inboxView;
    m_inboxView = nullptr;

    delete m_imageViewer;
    m_imageViewer = nullptr;

    delete m_commentView;
    m_commentView = nullptr;

    delete m_postListView;
    m_postListView = nullptr;

    delete m_mainMenu;
    m_mainMenu = nullptr;

    delete m_notificationManager;
    m_notificationManager = nullptr;

    delete m_draftManager;
    m_draftManager = nullptr;

    delete m_backupManager;
    m_backupManager = nullptr;

    delete m_networkMonitor;
    m_networkMonitor = nullptr;

    delete m_oauth;
    m_oauth = nullptr;

    delete m_redditAPI;
    m_redditAPI = nullptr;

    delete m_cacheManager;
    m_cacheManager = nullptr;

    delete m_accountManager;
    m_accountManager = nullptr;

    delete m_themeManager;
    m_themeManager = nullptr;

    // Preferences::instance() is a singleton, not deleted here

    m_initialized = false;

    Logging::info("PinkReaderApp", "Shutdown complete");
}

// ---------------------------------------------------------------------------
// Navigation
// ---------------------------------------------------------------------------

void PinkReaderApp::navigateToSubreddit(const QString &subreddit)
{
    Logging::info("PinkReaderApp",
        QString("Navigating to subreddit: r/%1").arg(subreddit));
}

void PinkReaderApp::navigateToPost(const QString &postId, const QString &subreddit)
{
    Logging::info("PinkReaderApp",
        QString("Navigating to post: %1").arg(postId));
}

void PinkReaderApp::navigateToUser(const QString &username)
{
    Logging::info("PinkReaderApp",
        QString("Navigating to user: u/%1").arg(username));
}

void PinkReaderApp::navigateToInbox()
{
    Logging::info("PinkReaderApp", "Navigating to inbox");
}

void PinkReaderApp::navigateToSettings()
{
    Logging::info("PinkReaderApp", "Navigating to settings");
    auto *settings = new SettingsWindow(m_mainWindow);
    if (settings) {
        settings->exec();
        settings->deleteLater();
    }
}

void PinkReaderApp::navigateToSearch()
{
    Logging::info("PinkReaderApp", "Navigating to search");
}

void PinkReaderApp::navigateToSubmitPost(const QString &subreddit)
{
    Logging::info("PinkReaderApp",
        QString("Navigating to submit post in r/%1").arg(subreddit));
}

void PinkReaderApp::navigateToMultireddit(const QString &user, const QString &multi)
{
    Logging::info("PinkReaderApp",
        QString("Navigating to multireddit: u/%1/m/%2").arg(user, multi));
}

void PinkReaderApp::goBack()
{
    if (canGoBack()) {
        m_currentNavIndex--;
        Logging::debug("PinkReaderApp",
            QString("Navigation back to index: %1").arg(m_currentNavIndex));
    }
}

bool PinkReaderApp::canGoBack() const
{
    return m_currentNavIndex > 0;
}

// ---------------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------------

QMainWindow *PinkReaderApp::mainWindow() const
{
    return m_mainWindow;
}

MainMenu *PinkReaderApp::mainMenu() const
{
    return m_mainMenu;
}

AccountManager *PinkReaderApp::accountManager() const
{
    return m_accountManager;
}

CacheManager *PinkReaderApp::cacheManager() const
{
    return m_cacheManager;
}

RedditAPI *PinkReaderApp::redditAPI() const
{
    return m_redditAPI;
}

Preferences *PinkReaderApp::preferences() const
{
    return &Preferences::instance();
}

NotificationManager *PinkReaderApp::notificationManager() const
{
    return m_notificationManager;
}

DraftManager *PinkReaderApp::draftManager() const
{
    return m_draftManager;
}

PinkReaderApp *PinkReaderApp::instance()
{
    return s_instance;
}

// ---------------------------------------------------------------------------
// Private - Service Setup
// ---------------------------------------------------------------------------

void PinkReaderApp::setupCoreServices()
{
    Logging::debug("PinkReaderApp", "Setting up core services...");

    // Theme manager - handles application theming (light, dark, night, etc.)
    m_themeManager = new ThemeManager(this);

    // Account manager - handles multiple Reddit accounts and OAuth tokens
    m_accountManager = new AccountManager(this);

    // Notification manager - handles local notifications for new messages
    m_notificationManager = new NotificationManager(this);

    // Draft manager - saves/loads post and comment drafts
    m_draftManager = new DraftManager(this);

    // Backup manager - handles settings backup and restore
    m_backupManager = new BackupManager(this);

    // Load saved accounts
    m_accountManager->loadAccounts();

    Logging::debug("PinkReaderApp",
        QString("Loaded %1 accounts").arg(m_accountManager->accountCount()));
}

void PinkReaderApp::setupNetworkServices()
{
    Logging::debug("PinkReaderApp", "Setting up network services...");

    // Network monitor - tracks connectivity state (WiFi, mobile, offline)
    m_networkMonitor = new NetworkMonitor(this);

    // OAuth handler - manages Reddit OAuth2 authentication flow
    m_oauth = new RedditOAuth(this);

    // Reddit API - main interface to the Reddit REST API
    m_redditAPI = new RedditAPI(m_oauth, this);

    // Configure API with current account
    if (m_accountManager->currentAccount()) {
        m_redditAPI->setAccount(m_accountManager->currentAccount());
    }

    Logging::debug("PinkReaderApp", "Network services initialized");
}

void PinkReaderApp::setupCacheServices()
{
    Logging::debug("PinkReaderApp", "Setting up cache services...");

    // Cache manager - manages SQLite database and disk cache
    // for posts, comments, images, and thumbnails
    m_cacheManager = new CacheManager(this);

    // Initialize the cache system
    QString cachePath = QStandardPaths::writableLocation(
        QStandardPaths::CacheLocation);
    if (!m_cacheManager->initialize(cachePath)) {
        Logging::error("PinkReaderApp", "Failed to initialize cache system!");
    }

    Logging::debug("PinkReaderApp", "Cache services initialized");
}

void PinkReaderApp::setupUIServices()
{
    Logging::debug("PinkReaderApp", "Setting up UI services...");

    // Create the main window
    m_mainWindow = new MainWindow();
    m_mainWindow->setWindowTitle(
        QStringLiteral("PinkReader for Reddit"));
    m_mainWindow->resize(400, 700); // Phone-sized default

    // Create the main menu (sidebar navigation)
    m_mainMenu = new MainMenu(m_mainWindow);
    m_mainWindow->setMainMenu(m_mainMenu);

    Logging::debug("PinkReaderApp", "UI services initialized");
}

void PinkReaderApp::connectSignals()
{
    Logging::debug("PinkReaderApp", "Connecting signals...");

    // Network state changes - update cache behavior
    connect(m_networkMonitor, &NetworkMonitor::connectivityChanged,
            this, [this](NetworkMonitor::Connectivity state) {
        Logging::info("PinkReaderApp",
            QString("Network state changed: %1").arg(static_cast<int>(state)));
        // Update cache download strategy based on connectivity
        if (m_cacheManager) {
            m_cacheManager->onConnectivityChanged(state);
        }
    });

    // Account changes - update API
    connect(m_accountManager, &AccountManager::currentAccountChanged,
            this, [this](Account *account) {
        Logging::info("PinkReaderApp",
            QString("Account changed: %1")
                .arg(account ? account->username() : QStringLiteral("(none)")));
        if (m_redditAPI) {
            m_redditAPI->setAccount(account);
        }
    });

    // Application lifecycle - foreground/background
    auto *app = qobject_cast<Application *>(QApplication::instance());
    if (app) {
        connect(app, &Application::foregrounded, this, [this]() {
            Logging::debug("PinkReaderApp", "App foregrounded - refreshing...");
            if (m_redditAPI && m_accountManager->currentAccount()) {
                // Check for new messages
                // m_redditAPI->checkInbox(); -- implemented in full version
            }
        });

        connect(app, &Application::backgrounded, this, [this]() {
            Logging::debug("PinkReaderApp", "App backgrounded - saving state...");
            saveCurrentSession();
        });
    }

    Logging::debug("PinkReaderApp", "Signals connected");
}

void PinkReaderApp::restorePreviousSession()
{
    Logging::debug("PinkReaderApp", "Restoring previous session...");

    QSettings settings;
    QString lastSubreddit = settings.value(
        QStringLiteral("session/last_subreddit"), QString()).toString();

    if (!lastSubreddit.isEmpty()) {
        Logging::info("PinkReaderApp",
            QString("Restoring previous session: r/%1").arg(lastSubreddit));
        QTimer::singleShot(100, this, [this, lastSubreddit]() {
            navigateToSubreddit(lastSubreddit);
        });
    }
}

void PinkReaderApp::saveCurrentSession()
{
    Logging::debug("PinkReaderApp", "Saving current session...");

    QSettings settings;
    // Save current navigation state
    settings.setValue(QStringLiteral("session/nav_index"), m_currentNavIndex);
    settings.setValue(QStringLiteral("session/nav_history"), m_navigationHistory);
    settings.sync();
}

} // namespace PinkReader
