/*
 * PinkReader - GPLv3
 * File: old_reddit_api_nonce.h
 *
 * Port of OldRedditAPINonce.java
 *
 * NOTE: The source file OldRedditAPINonce.java does not currently exist
 * in the RedReader codebase at:
 *   redreader/src/main/java/org/quantumbadger/redreader/common/
 *
 * This file is created as a port placeholder per the task specification.
 * It will be populated when the original Java source is available.
 *
 * Typical usage in Old Reddit API: a nonce (number used once) is used
 * to prevent CSRF attacks. The nonce is obtained from the old Reddit
 * login page and submitted with the login form.
 */

#pragma once

#include <QString>

namespace PinkReader {

/**
 * @brief Port placeholder for OldRedditAPINonce
 *
 * Represents a nonce token obtained from the old Reddit login page.
 * Used to authenticate API requests via the old Reddit interface.
 *
 * Original Java package: org.quantumbadger.redreader.common
 */
class OldRedditAPINonce {
public:
    OldRedditAPINonce() = default;
    explicit OldRedditAPINonce(const QString &nonce) : m_nonce(nonce) {}

    /** @return The nonce string value */
    QString getNonce() const { return m_nonce; }

    /** @brief Set the nonce value */
    void setNonce(const QString &nonce) { m_nonce = nonce; }

    /** @return true if the nonce is valid (non-empty) */
    bool isValid() const { return !m_nonce.isEmpty(); }

    /** @brief Clear the nonce */
    void clear() { m_nonce.clear(); }

private:
    QString m_nonce;
};

} // namespace PinkReader
