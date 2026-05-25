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
 * File: pinkreader.h
 * Description: Main application controller
 */


#pragma once

#include <QObject>
#include <QMainWindow>
#include <QStackedWidget>
#include <QMap>
#include <QString>

namespace PinkReader {

// Forward declarations
class MainWindow;
class MainMenu;
class PostListView;
class CommentView;
class ImageViewer;
class AlbumViewer;
class WebView;
class InboxView;
class PostSubmitView;
class CommentReplyView;
class PMComposeView;
class LoginDialog;
class SettingsWindow;
class SearchView;
class UserProfileView;
class AccountManager;
class CacheManager;
class ThemeManager;
class Preferences;
class RedditAPI;
class RedditOAuth;
class NetworkMonitor;
class NotificationManager;
class DraftManager;
class BackupManager;

class PinkReaderApp : public QObject
{
    Q_OBJECT

public:
    explicit PinkReaderApp(QObject *parent = nullptr);
    ~PinkReaderApp() override;

    void initialize();
    void shutdown();

    QMainWindow *mainWindow() const;
    MainMenu *mainMenu() const;

    // Navigation
    void navigateToSubreddit(const QString &subreddit);
    void navigateToPost(const QString &postId, const QString &subreddit = QString());
    void navigateToUser(const QString &username);
    void navigateToInbox();
    void navigateToSettings();
    void navigateToSearch();
    void navigateToSubmitPost(const QString &subreddit = QString());
    void navigateToMultireddit(const QString &user, const QString &multi);
    void goBack();
    bool canGoBack() const;

    // Accessors
    AccountManager *accountManager() const;
    CacheManager *cacheManager() const;
    RedditAPI *redditAPI() const;
    Preferences *preferences() const;
    NotificationManager *notificationManager() const;
    DraftManager *draftManager() const;

    static PinkReaderApp *instance();

signals:
    void navigationChanged();

private:
    void setupCoreServices();
    void setupNetworkServices();
    void setupCacheServices();
    void setupUIServices();
    void connectSignals();
    void restorePreviousSession();
    void saveCurrentSession();

    static PinkReaderApp *s_instance;

    QMainWindow *m_mainWindow = nullptr;
    MainMenu *m_mainMenu = nullptr;

    // Core services
    AccountManager *m_accountManager = nullptr;
    CacheManager *m_cacheManager = nullptr;
    ThemeManager *m_themeManager = nullptr;
    NotificationManager *m_notificationManager = nullptr;
    DraftManager *m_draftManager = nullptr;
    BackupManager *m_backupManager = nullptr;

    // Network services
    RedditAPI *m_redditAPI = nullptr;
    RedditOAuth *m_oauth = nullptr;
    NetworkMonitor *m_networkMonitor = nullptr;

    // Navigation
    QStackedWidget *m_navigationStack = nullptr;
    QStringList m_navigationHistory;
    int m_currentNavIndex = -1;

    // Views (created on demand)
    PostListView *m_postListView = nullptr;
    CommentView *m_commentView = nullptr;
    ImageViewer *m_imageViewer = nullptr;
    InboxView *m_inboxView = nullptr;
    SearchView *m_searchView = nullptr;
    UserProfileView *m_userProfileView = nullptr;

    bool m_initialized = false;
};

} // namespace PinkReader
