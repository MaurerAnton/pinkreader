/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: deep_link_handler.h - reddit:// deep link routing
 */

#pragma once

#include <QObject>

namespace PinkReader {

/**
 * Routes reddit:// deep links to the right screen. Owned by Application
 * (see Application::deepLinkHandler()).
 */
class DeepLinkHandler : public QObject {
    Q_OBJECT
public:
    explicit DeepLinkHandler(QObject *parent = nullptr);
    ~DeepLinkHandler() override;
};

} // namespace PinkReader
