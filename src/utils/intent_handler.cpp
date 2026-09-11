/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: intent_handler.cpp - Android intent routing
 */

#include "utils/intent_handler.h"

namespace PinkReader {

IntentHandler::IntentHandler(QObject *parent)
    : QObject(parent) {
}

IntentHandler::~IntentHandler() = default;

} // namespace PinkReader
