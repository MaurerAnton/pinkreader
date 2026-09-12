/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: intent_handler.h - Android intent routing
 */

#pragma once

#include <QObject>

namespace PinkReader {

/**
 * Routes Android share/view intents into the app. Owned by Application
 * (see Application::intentHandler()).
 */
class IntentHandler : public QObject {
    Q_OBJECT
public:
    explicit IntentHandler(QObject *parent = nullptr);
    ~IntentHandler() override;
};

} // namespace PinkReader
