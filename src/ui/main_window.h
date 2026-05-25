/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: main_window.h - Application main window
 */

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QProgressBar>
#include <QStatusBar>
#include <QDialog>
#include <QMainWindow>
#include <QCloseEvent>

namespace PinkReader {

/**
 * @brief Application main window
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    // View lifecycle
    virtual void activate();
    virtual void deactivate();
    virtual void refresh();

    // Navigation
    virtual QString title() const;
    virtual QIcon icon() const;

    // Content loading
    void showLoading();
    void hideLoading();
    void showError(const QString &message);
    void showEmptyState(const QString &message);

    bool isLoading() const;
    bool hasError() const;
    bool isEmpty() const;

signals:
    void titleChanged(const QString &title);
    void backRequested();
    void refreshRequested();
    void contentLoaded();
    void errorOccurred(const QString &error);

protected:
    void closeEvent(QCloseEvent *event) override;

    virtual void setupUi();
    virtual void connectSignals();
    virtual void loadContent();

    // UI components
    QVBoxLayout *m_mainLayout = nullptr;
    QToolBar *m_toolbar = nullptr;
    QScrollArea *m_scrollArea = nullptr;
    QWidget *m_contentWidget = nullptr;
    QProgressBar *m_loadingIndicator = nullptr;
    QLabel *m_errorLabel = nullptr;
    QLabel *m_emptyLabel = nullptr;

    // State
    bool m_isLoading = false;
    bool m_hasError = false;
    bool m_isEmpty = true;
    QString m_errorMessage;
    bool m_isActive = false;
};

} // namespace PinkReader
