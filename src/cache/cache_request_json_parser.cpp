// Origin: org/quantumbadger/redreader/cache/CacheRequestJSONParser.java
#include "cache_request_json_parser.h"

#include <stdexcept>

namespace PinkReader {

CacheRequestJSONParser::CacheRequestJSONParser(void* context, Listener& listener)
	: m_context(context)
	, m_listener(listener) {}

void CacheRequestJSONParser::onDataStreamAvailable(
		const GenericFactory<SeekableInputStream, std::runtime_error>& streamFactory,
		const TimestampUTC& timestamp,
		const UUID& session,
		bool fromCache,
		const std::string* mimetype) {

	try {
		// mThreadPool.add(() -> {
		//     JsonValue jsonValue;
		//     try(InputStream is = streamFactory.create()) {
		//         jsonValue = JsonValue.parse(is);
		//     } catch(IOException e) {
		//         if(!m_notifiedFailure.getAndSet(true)) {
		//             m_listener.onFailure(General.getGeneralErrorForFailure(...));
		//         }
		//         return;
		//     }
		//     try {
		//         m_listener.onJsonParsed(jsonValue, timestamp, session, fromCache);
		//     } catch(Exception e) {
		//         BugReportActivity.handleGlobalError(m_context, e);
		//     }
		// });

		// Stub: parse inline
		try {
			// SeekableInputStream is = streamFactory.create();
			// JsonValue jsonValue = JsonValue.parse(is);
			// m_listener.onJsonParsed(jsonValue, timestamp, session, fromCache);
		} catch(const std::exception& e) {
			if(!m_notifiedFailure.exchange(true)) {
				// m_listener.onFailure(...PARSE error...);
			}
			return;
		}

		try {
			// m_listener.onJsonParsed(jsonValue, timestamp, session, fromCache);
		} catch(const std::exception& e) {
			// BugReportActivity.handleGlobalError(m_context, e);
		}

	} catch(const std::exception& e) {
		if(!m_notifiedFailure.exchange(true)) {
			// onFailure(General.getGeneralErrorForFailure(...STORAGE...));
		}
	}
}

void CacheRequestJSONParser::onFailure(const RRError& error) {
	if(!m_notifiedFailure.exchange(true)) {
		m_listener.onFailure(error);
	}
}

} // namespace PinkReader
