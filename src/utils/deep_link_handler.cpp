/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: deep_link_handler.cpp - reddit:// deep link routing
 */

#include "utils/deep_link_handler.h"

namespace PinkReader {

DeepLinkHandler::DeepLinkHandler(QObject *parent)
    : QObject(parent) {
}

DeepLinkHandler::~DeepLinkHandler() = default;

} // namespace PinkReader
