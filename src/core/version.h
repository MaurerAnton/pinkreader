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
 * File: version.h
 * Description: Version information
 */


#pragma once

#include <QString>
#include <QVersionNumber>
#include <QDateTime>

namespace PinkReader {

/**
 * @brief Version information for PinkReader
 *
 * Contains the current version number, build metadata,
 * and methods to format version strings for display.
 *
 * Version numbering follows Semantic Versioning 2.0.0:
 *   MAJOR.MINOR.PATCH[-PRERELEASE][+BUILD]
 *
 * The version is updated by the CI pipeline during release builds.
 */
class Version
{
public:
    /** @brief Major version number (breaking changes) */
    static constexpr int MAJOR = 1;

    /** @brief Minor version number (new features, backward compatible) */
    static constexpr int MINOR = 0;

    /** @brief Patch version number (bug fixes, backward compatible) */
    static constexpr int PATCH = 0;

    /** @brief Pre-release identifier (empty for stable releases) */
    static constexpr const char *PRERELEASE = "alpha";

    /** @brief Build metadata (e.g., git commit hash) */
    static constexpr const char *BUILD_META = "";

    /** @brief Build timestamp (set at compile time) */
    static constexpr const char *BUILD_DATE = __DATE__;

    /** @brief Build time (set at compile time) */
    static constexpr const char *BUILD_TIME = __TIME__;

    /** @brief Git commit hash (set by CI, empty for local builds) */
    static constexpr const char *GIT_COMMIT = "";

    /** @brief Git branch name (set by CI, empty for local builds) */
    static constexpr const char *GIT_BRANCH = "";

    /** @brief CI build number (set by CI, 0 for local builds) */
    static constexpr int BUILD_NUMBER = 0;

    /**
     * @brief Get the simple version string (e.g., "1.0.0")
     * @return Version string in MAJOR.MINOR.PATCH format
     */
    static QString versionString()
    {
        return QStringLiteral("%1.%2.%3")
            .arg(MAJOR)
            .arg(MINOR)
            .arg(PATCH);
    }

    /**
     * @brief Get the full version string including pre-release info
     * @return Full version string (e.g., "1.0.0-alpha")
     */
    static QString fullVersionString()
    {
        QString ver = versionString();
        if (QString::fromLatin1(PRERELEASE).length() > 0) {
            ver += QStringLiteral("-") + QString::fromLatin1(PRERELEASE);
        }
        if (QString::fromLatin1(BUILD_META).length() > 0) {
            ver += QStringLiteral("+") + QString::fromLatin1(BUILD_META);
        }
        return ver;
    }

    /**
     * @brief Get the version as a QVersionNumber
     * @return QVersionNumber representing the current version
     */
    static QVersionNumber versionNumber()
    {
        return QVersionNumber(MAJOR, MINOR, PATCH);
    }

    /**
     * @brief Get a formatted version string for display in UI
     * @return User-friendly version string
     */
    static QString displayString()
    {
        QString result = QStringLiteral("PinkReader v%1").arg(fullVersionString());

        if (BUILD_NUMBER > 0) {
            result += QStringLiteral(" (Build #%1)").arg(BUILD_NUMBER);
        }

        QString gitCommit = QString::fromLatin1(GIT_COMMIT);
        if (gitCommit.length() >= 7) {
            result += QStringLiteral("\nGit: %1").arg(gitCommit.left(7));
        }

        QString branch = QString::fromLatin1(GIT_BRANCH);
        if (!branch.isEmpty()) {
            result += QStringLiteral(" (%1 branch)").arg(branch);
        }

        return result;
    }

    /**
     * @brief Get the build date string
     * @return Build date in "Month Day Year" format
     */
    static QString buildDateString()
    {
        return QStringLiteral("%1 %2")
            .arg(QString::fromLatin1(BUILD_DATE),
                 QString::fromLatin1(BUILD_TIME));
    }

    /**
     * @brief Get user agent string for HTTP requests
     * @return Formatted user agent string
     */
    static QString userAgent()
    {
        return QStringLiteral("PinkReader/%1 (Android; Qt 6)")
            .arg(fullVersionString());
    }

    /**
     * @brief Check if this is a stable release (no pre-release tag)
     * @return true if this is a stable release version
     */
    static bool isStable()
    {
        return QString::fromLatin1(PRERELEASE).isEmpty();
    }

    /**
     * @brief Check if this is a development build
     * @return true if this is a development/nightly build
     */
    static bool isDevelopment()
    {
        return !isStable();
    }

    /**
     * @brief Get the version code for Android (integer)
     * @return Android version code
     *
     * Format: MAJOR * 1000000 + MINOR * 1000 + PATCH
     * This scheme supports up to 999 minor versions and 999 patches
     * per major version.
     */
    static int androidVersionCode()
    {
        return MAJOR * 1000000 + MINOR * 1000 + PATCH;
    }

    /**
     * @brief Get the changelog URL for this version
     * @return URL to the changelog on GitHub
     */
    static QString changelogUrl()
    {
        if (isStable()) {
            return QStringLiteral(
                "https://github.com/pinkreader/pinkreader/releases/tag/v%1")
                .arg(versionString());
        }
        return QStringLiteral(
            "https://github.com/pinkreader/pinkreader/blob/main/CHANGELOG.md");
    }

    /**
     * @brief Compare this version with another
     * @param other The version string to compare against
     * @return -1 if this < other, 0 if equal, 1 if this > other
     */
    static int compare(const QString &other)
    {
        return versionNumber().compare(QVersionNumber::fromString(other));
    }

private:
    Version() = delete;
    ~Version() = delete;
    Version(const Version &) = delete;
    Version &operator=(const Version &) = delete;
};

} // namespace PinkReader
