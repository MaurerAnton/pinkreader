/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: subreddit_sidebar.cpp - Implementation of Subreddit info sidebar
 */

#include "ui/subreddit_sidebar.h"
#include "utils/logging.h"
#include "core/constants.h"
#include "settings/preferences.h"
#include "settings/theme_manager.h"

#include <QApplication>
#include <QStyle>
#include <QMessageBox>
#include <QTimer>

namespace PinkReader {

SubredditSidebar::SubredditSidebar(QWidget *parent)
    : QWidget(parent)
{
    Logging::debug("UI::SubredditSidebar", "Creating view...");

    setObjectName(QStringLiteral("SubredditSidebar"));
    setAttribute(Qt::WA_DeleteOnClose, false);

    setupUi();
    connectSignals();

    Logging::debug("UI::SubredditSidebar", "View created");
}

SubredditSidebar::~SubredditSidebar()
{
    Logging::debug("UI::SubredditSidebar", "Destroying view...");
}

void SubredditSidebar::activate()
{
    if (m_isActive) {
        Logging::debug("UI::SubredditSidebar", "View already active");
        return;
    }

    Logging::info("UI::SubredditSidebar", "Activating view...");
    m_isActive = true;

    if (m_isEmpty && !m_hasError) {
        loadContent();
    }

    Logging::info("UI::SubredditSidebar", "View activated");
}

void SubredditSidebar::deactivate()
{
    if (!m_isActive) {
        return;
    }

    Logging::debug("UI::SubredditSidebar", "Deactivating view...");
    m_isActive = false;
}

void SubredditSidebar::refresh()
{
    Logging::debug("UI::SubredditSidebar", "Refreshing view...");
    m_isEmpty = true;
    m_hasError = false;
    loadContent();
}

QString SubredditSidebar::title() const
{
    return QStringLiteral("subreddit_sidebar");
}

QIcon SubredditSidebar::icon() const
{
    return QIcon();
}

void SubredditSidebar::showLoading()
{
    Logging::debug("UI::SubredditSidebar", "Showing loading indicator");
    m_isLoading = true;

    if (m_loadingIndicator) {
        m_loadingIndicator->show();
        m_loadingIndicator->setRange(0, 0);  // Indeterminate mode
    }

    if (m_errorLabel) m_errorLabel->hide();
    if (m_emptyLabel) m_emptyLabel->hide();
}

void SubredditSidebar::hideLoading()
{
    m_isLoading = false;

    if (m_loadingIndicator) {
        m_loadingIndicator->hide();
    }
}

void SubredditSidebar::showError(const QString &message)
{
    Logging::error("UI::SubredditSidebar",
        QString("Showing error: %1").arg(message));

    m_hasError = true;
    m_errorMessage = message;
    hideLoading();

    if (m_errorLabel) {
        m_errorLabel->setText(message);
        m_errorLabel->show();
    }

    emit errorOccurred(message);
}

void SubredditSidebar::showEmptyState(const QString &message)
{
    Logging::debug("UI::SubredditSidebar",
        QString("Showing empty state: %1").arg(message));

    m_isEmpty = true;
    hideLoading();

    if (m_emptyLabel) {
        m_emptyLabel->setText(message);
        m_emptyLabel->show();
    }
}

bool SubredditSidebar::isLoading() const
{
    return m_isLoading;
}

bool SubredditSidebar::hasError() const
{
    return m_hasError;
}

bool SubredditSidebar::isEmpty() const
{
    return m_isEmpty;
}

void SubredditSidebar::closeEvent(QCloseEvent *event)
{
    Logging::debug("UI::SubredditSidebar", "Close event received");
    deactivate();
    QWidget::closeEvent(event);
}

void SubredditSidebar::setupUi()
{
    Logging::debug("UI::SubredditSidebar", "Setting up UI...");

    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // Toolbar
    m_toolbar = new QToolBar(this);
    m_toolbar->setObjectName(QStringLiteral("SubredditSidebarToolbar"));
    m_toolbar->setMovable(false);
    m_toolbar->setIconSize(QSize(24, 24));

    // Back button
    QAction *backAction = m_toolbar->addAction(
        style()->standardIcon(QStyle::SP_ArrowBack),
        tr("Back"));
    connect(backAction, &QAction::triggered,
            this, &SubredditSidebar::backRequested);

    // Title label
    QLabel *titleLabel = new QLabel(title(), this);
    titleLabel->setObjectName(QStringLiteral("viewTitle"));
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(16);
    titleLabel->setFont(titleFont);
    m_toolbar->addWidget(titleLabel);

    m_toolbar->addSeparator();

    // Refresh action
    QAction *refreshAction = m_toolbar->addAction(
        style()->standardIcon(QStyle::SP_BrowserReload),
        tr("Refresh"));
    connect(refreshAction, &QAction::triggered,
            this, &SubredditSidebar::refresh);

    // Spacer
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_toolbar->addWidget(spacer);

    // Menu button
    QAction *menuAction = m_toolbar->addAction(
        style()->standardIcon(QStyle::SP_TitleBarMenuButton),
        tr("Menu"));

    m_mainLayout->addWidget(m_toolbar);

    // Loading indicator
    m_loadingIndicator = new QProgressBar(this);
    m_loadingIndicator->setObjectName(QStringLiteral("loadingBar"));
    m_loadingIndicator->setTextVisible(false);
    m_loadingIndicator->setFixedHeight(3);
    m_loadingIndicator->setRange(0, 0);  // Indeterminate
    m_loadingIndicator->hide();
    m_mainLayout->addWidget(m_loadingIndicator);

    // Error label
    m_errorLabel = new QLabel(this);
    m_errorLabel->setObjectName(QStringLiteral("errorLabel"));
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setStyleSheet(
        QStringLiteral("color: #ff4444; padding: 16px;"));
    m_errorLabel->hide();
    m_mainLayout->addWidget(m_errorLabel);

    // Empty state label
    m_emptyLabel = new QLabel(this);
    m_emptyLabel->setObjectName(QStringLiteral("emptyLabel"));
    m_emptyLabel->setWordWrap(true);
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet(
        QStringLiteral("color: #888888; padding: 32px;"));
    m_emptyLabel->hide();
    m_mainLayout->addWidget(m_emptyLabel);

    // Content area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setObjectName(QStringLiteral("contentScrollArea"));
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_contentWidget = new QWidget(this);
    m_contentWidget->setObjectName(QStringLiteral("contentWidget"));
    m_scrollArea->setWidget(m_contentWidget);

    m_mainLayout->addWidget(m_scrollArea, 1);  // Stretch factor 1

    // Apply theme styling
    ThemeManager::instance().applyToWidget(this);

    setLayout(m_mainLayout);
    Logging::debug("UI::SubredditSidebar", "UI setup complete");
}

void SubredditSidebar::connectSignals()
{
    Logging::debug("UI::SubredditSidebar", "Connecting signals...");

    // Connect preferences changes
    connect(&Preferences::instance(), &Preferences::settingChanged,
            this, [this](const QString &key, const QVariant &value) {
        Q_UNUSED(value)
        if (key == QStringLiteral("theme")) {
            Logging::debug("UI::SubredditSidebar", "Theme changed, updating...");
            ThemeManager::instance().applyToWidget(this);
        }
    });

    // Connect theme changes
    connect(&ThemeManager::instance(), &ThemeManager::themeApplied,
            this, [this](const QString &theme) {
        Q_UNUSED(theme)
        update();  // Request repaint with new theme
    });

    Logging::debug("UI::SubredditSidebar", "Signals connected");
}

void SubredditSidebar::loadContent()
{
    Logging::debug("UI::SubredditSidebar", "Loading content...");
    showLoading();

    // Content loading logic would go here
    // This is where API calls would be made to fetch data

    // Simulate content loaded
    QTimer::singleShot(100, this, [this]() {
        hideLoading();
        m_isEmpty = false;
        emit contentLoaded();
        Logging::debug("UI::SubredditSidebar", "Content loaded successfully");
    });
}

} // namespace PinkReader
