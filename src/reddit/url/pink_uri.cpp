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
 * PinkReader C++ port of:
 *   android.net.Uri (simplified)
 *
 * Original annotations: N/A (Android SDK)
 *
 * Every field and method used by the RedReader URL classes.
 */

#include "pink_uri.h"
#include <QUrlQuery>

namespace PinkReader {

// ---- PinkUri ----

PinkUri::PinkUri(const QString &uriString)
	: m_url(uriString) {}

QString PinkUri::getScheme() const {
	return m_url.scheme();
}

QString PinkUri::getHost() const {
	return m_url.host();
}

QString PinkUri::getPath() const {
	return m_url.path();
}

QStringList PinkUri::getPathSegments() const {
	// Port of: android.net.Uri.getPathSegments()
	QString path = m_url.path();
	if (path.startsWith('/')) {
		path = path.mid(1);
	}
	if (path.isEmpty()) {
		return {};
	}
	QStringList segments = path.split('/');
	segments.removeAll({});
	return segments;
}

QString PinkUri::getQueryParameter(const QString &key) const {
	// Port of: android.net.Uri.getQueryParameter(String key)
	QUrlQuery query(m_url);
	return query.queryItemValue(key);
}

QStringList PinkUri::getQueryParameterNames() const {
	// Port of: General.getUriQueryParameterNames(Uri) which iterates query parameter names
	QUrlQuery query(m_url);
	QStringList names;
	const auto items = query.queryItems();
	for (const auto &item : items) {
		names.append(item.first);
	}
	return names;
}

PinkUri PinkUri::buildUpon() const {
	// Port of: android.net.Uri.buildUpon() — returns a Builder initialized with this URI
	return *this;
}

QString PinkUri::toString() const {
	return m_url.toString();
}

// ---- PinkUri::Builder ----

PinkUri::Builder::Builder() = default;

PinkUri::Builder &PinkUri::Builder::scheme(const QString &s) {
	m_scheme = s;
	return *this;
}

PinkUri::Builder &PinkUri::Builder::authority(const QString &a) {
	m_host = a;
	return *this;
}

PinkUri::Builder &PinkUri::Builder::encodedPath(const QString &p) {
	// Port of: android.net.Uri.Builder.encodedPath(String path)
	m_path = p;
	return *this;
}

PinkUri::Builder &PinkUri::Builder::appendPath(const QString &newSegment) {
	// Port of: android.net.Uri.Builder.appendPath(String newSegment)
	if (!m_path.endsWith('/')) {
		m_path += '/';
	}
	m_path += newSegment;
	return *this;
}

PinkUri::Builder &PinkUri::Builder::appendEncodedPath(const QString &newSegment) {
	// Port of: android.net.Uri.Builder.appendEncodedPath(String newSegment)
	m_path += newSegment;
	return *this;
}

PinkUri::Builder &PinkUri::Builder::appendQueryParameter(const QString &key, const QString &value) {
	// Port of: android.net.Uri.Builder.appendQueryParameter(String key, String value)
	m_query.addQueryItem(key, value);
	return *this;
}

PinkUri PinkUri::Builder::build() const {
	// Port of: android.net.Uri.Builder.build()
	QUrl url;
	url.setScheme(m_scheme);
	url.setHost(m_host);
	url.setPath(m_path);
	url.setQuery(m_query);
	return PinkUri(url.toString());
}

} // namespace PinkReader
