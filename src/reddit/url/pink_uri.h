/*******************************************************************************
 * This file is part of RedReader.
 *
 * RedReader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RedReader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RedReader.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/*
 * PinkReader C++ port of android.net.Uri
 *
 * Simplified URI class providing the subset of android.net.Uri operations
 * used by the RedReader URL parser classes.
 *
 * Every field, method, inner class from android.net.Uri as used by
 * org.quantumbadger.redreader.reddit.url.*
 */

#pragma once

#include <QString>
#include <QStringList>
#include <QUrl>
#include <QUrlQuery>
#include <memory>
#include <vector>

namespace PinkReader {

/**
 * @brief Port of android.net.Uri, providing the subset of operations
 *        used by RedReader URL URL classes.
 */
class PinkUri {
public:
	// ---- Construction ----
	PinkUri() = default;
	explicit PinkUri(const QString &uriString);

	// ---- Accessors (port of android.net.Uri) ----
	QString getScheme() const;
	QString getHost() const;
	QString getPath() const;
	QStringList getPathSegments() const;
	QString getQueryParameter(const QString &key) const;
	QStringList getQueryParameterNames() const;

	// ---- Builder (port of android.net.Uri.Builder) ----
	class Builder {
	public:
		Builder();

		Builder &scheme(const QString &scheme);
		Builder &authority(const QString &authority);
		Builder &encodedPath(const QString &path);
		Builder &appendPath(const QString &newSegment);
		Builder &appendEncodedPath(const QString &newSegment);
		Builder &appendQueryParameter(const QString &key, const QString &value);

		PinkUri build() const;

	private:
		QString m_scheme;
		QString m_host;
		QString m_path;
		QUrlQuery m_query;
	};

	// ---- Utility ----
	PinkUri buildUpon() const;
	QString toString() const;

private:
	QUrl m_url;
};

} // namespace PinkReader
