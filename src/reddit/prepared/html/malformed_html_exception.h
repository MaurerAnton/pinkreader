/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: malformed_html_exception.h
 * Port of: MalformedHtmlException.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/prepared/html/MalformedHtmlException.java
 *
 * Every field, method, inner class, and constant ported exactly.
 */

#pragma once

#include <QString>
#include <optional>
#include <stdexcept>
#include <string>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.reddit.prepared.html.MalformedHtmlException
 *
 * Exception thrown when HTML parsing encounters malformed input.
 * Every field, constructor, and logic branch ported exactly.
 *
 * Origin: MalformedHtmlException.java lines 23-48
 */
class MalformedHtmlException : public std::runtime_error {
public:
    // Port of: @NonNull public final String html; (line 25)
    // Origin: MalformedHtmlException.java line 25
    QString html;

    // Port of: @Nullable public final Integer charPosition; (line 26)
    // Origin: MalformedHtmlException.java line 26
    std::optional<int> charPosition;

    // Port of: public MalformedHtmlException(
    //     @NonNull final String message,
    //     @NonNull final String html,
    //     @Nullable final Integer charPosition,
    //     @NonNull final Exception e) (lines 28-37)
    // Origin: MalformedHtmlException.java lines 28-37
    MalformedHtmlException(
            const QString &message,
            const QString &html,
            const std::optional<int> &charPosition,
            const std::exception &cause)
        : std::runtime_error(message.toStdString() + ": " + cause.what())
        , html(html)
        , charPosition(charPosition) {}

    // Port of: public MalformedHtmlException(
    //     @NonNull final String message,
    //     @NonNull final String html,
    //     @Nullable final Integer charPosition) (lines 39-47)
    // Origin: MalformedHtmlException.java lines 39-47
    MalformedHtmlException(
            const QString &message,
            const QString &html,
            const std::optional<int> &charPosition)
        : std::runtime_error(message.toStdString())
        , html(html)
        , charPosition(charPosition) {}
};

} // namespace PinkReader
