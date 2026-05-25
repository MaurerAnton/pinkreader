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
 *   org.quantumbadger.redreader.reddit.url.ComposeMessageURL (lines 1-123)
 */

#include "compose_message_url.h"
#include "../../core/constants.h"

#include <QUrl>

namespace PinkReader {

// Port of: public ComposeMessageURL(String recipient, String subject, String message) (Java lines 34-38)
ComposeMessageURL::ComposeMessageURL(const QString &recipient, const QString &subject, const QString &message)
	: recipient(recipient)
	, subject(subject)
	, message(message) {}

// Port of: public static ComposeMessageURL parse(final Uri uri) (Java lines 40-90)
ComposeMessageURL *ComposeMessageURL::parse(const PinkUri &uri) {
	// Port of: final String[] pathSegments; (Java lines 42-62)
	QStringList pathSegmentsList = uri.getPathSegments();

	QStringList pathSegmentsFiltered;
	pathSegmentsFiltered.reserve(pathSegmentsList.size());
	for (QString segment : pathSegmentsList) {
		// Port of: while(StringUtils.asciiLowercase(segment).endsWith(".json") || ...endsWith(".xml")) (Java lines 50-53)
		while (segment.toLower().endsWith(QStringLiteral(".json"))
				|| segment.toLower().endsWith(QStringLiteral(".xml"))) {
			segment = segment.left(segment.lastIndexOf('.'));
		}

		// Port of: if(!segment.isEmpty()) pathSegmentsFiltered.add(segment); (Java lines 55-57)
		if (!segment.isEmpty()) {
			pathSegmentsFiltered.append(segment);
		}
	}

	// Port of: pathSegments = pathSegmentsFiltered.toArray(new String[0]); (Java lines 60-61)
	// pathSegmentsFiltered is used directly instead of array

	// Port of: if(pathSegments.length != 2) return null; (Java lines 64-66)
	if (pathSegmentsFiltered.size() != 2) {
		return nullptr;
	}

	// Port of: if(!pathSegments[0].equalsIgnoreCase("message") || !pathSegments[1].equalsIgnoreCase("compose")) (Java lines 68-71)
	if (!pathSegmentsFiltered[0].compare(QStringLiteral("message"), Qt::CaseInsensitive) == 0
			|| !pathSegmentsFiltered[1].compare(QStringLiteral("compose"), Qt::CaseInsensitive) == 0) {
		return nullptr;
	}

	QString recipient;
	QString subject;
	QString message;

	// Port of: for(final String parameterKey : General.getUriQueryParameterNames(uri)) (Java lines 76-87)
	QStringList paramNames = uri.getQueryParameterNames();
	for (const QString &parameterKey : paramNames) {
		// Port of: if(parameterKey.equalsIgnoreCase("to")) (Java lines 77-79)
		if (parameterKey.compare(QStringLiteral("to"), Qt::CaseInsensitive) == 0) {
			recipient = uri.getQueryParameter(parameterKey);

		// Port of: else if(parameterKey.equalsIgnoreCase("subject")) (Java lines 81-82)
		} else if (parameterKey.compare(QStringLiteral("subject"), Qt::CaseInsensitive) == 0) {
			subject = uri.getQueryParameter(parameterKey);

		// Port of: else if(parameterKey.equalsIgnoreCase("message")) (Java lines 84-85)
		} else if (parameterKey.compare(QStringLiteral("message"), Qt::CaseInsensitive) == 0) {
			message = uri.getQueryParameter(parameterKey);
		}
	}

	// Port of: return new ComposeMessageURL(recipient, subject, message); (Java line 89)
	return new ComposeMessageURL(recipient, subject, message);
}

// Port of: @Override public Uri generateJsonUri() (Java lines 93-116)
PinkUri ComposeMessageURL::generateJsonUri() {
	PinkUri::Builder builder;

	// Port of: builder.scheme(Constants.Reddit.getScheme()).authority(Constants.Reddit.getDomain()); (Java lines 95-96)
	builder.scheme(Reddit::getScheme());
	builder.authority(Reddit::getDomain());

	// Port of: builder.appendEncodedPath("message"); builder.appendEncodedPath("compose"); (Java lines 98-99)
	builder.appendEncodedPath(QStringLiteral("message"));
	builder.appendEncodedPath(QStringLiteral("compose"));

	// Port of: if(recipient != null) builder.appendQueryParameter("to", recipient); (Java lines 101-103)
	if (!recipient.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("to"), recipient);
	}

	// Port of: if(subject != null) builder.appendQueryParameter("subject", subject); (Java lines 105-107)
	if (!subject.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("subject"), subject);
	}

	// Port of: if(message != null) builder.appendQueryParameter("message", message); (Java lines 109-111)
	if (!message.isEmpty()) {
		builder.appendQueryParameter(QStringLiteral("message"), message);
	}

	// Port of: builder.appendEncodedPath(".json"); (Java line 113)
	builder.appendEncodedPath(QStringLiteral(".json"));

	// Port of: return builder.build(); (Java line 115)
	return builder.build();
}

// Port of: @Override public @RedditURLParser.PathType int pathType() (Java lines 118-122)
int ComposeMessageURL::pathType() {
	return RedditURLParser::COMPOSE_MESSAGE_URL;
}

} // namespace PinkReader
