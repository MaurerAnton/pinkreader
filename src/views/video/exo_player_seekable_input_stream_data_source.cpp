// Origin: org.quantumbadger.redreader.views.video.ExoPlayerSeekableInputStreamDataSource.java
// Ported from RedReader (GPLv3)

#include "views/video/exo_player_seekable_input_stream_data_source.h"

#include <cstdint>
#include <stdexcept>
#include <string>

namespace PinkReader {

const std::string ExoPlayerSeekableInputStreamDataSource::URI = "redreader://video";

ExoPlayerSeekableInputStreamDataSource::ExoPlayerSeekableInputStreamDataSource(
        bool isNetwork,
        SeekableInputStreamFactory streamFactory)
    : m_streamFactory(std::move(streamFactory)) {
    // super(isNetwork) - BaseDataSource constructor
    (void)isNetwork;
}

long ExoPlayerSeekableInputStreamDataSource::open(void* dataSpec) {
    // dataSpec has a .position field (long)
    if (m_currentStream != nullptr) {
        throw std::runtime_error("Already open!");
    }

    // transferInitializing(dataSpec);

    m_currentStream = m_streamFactory();
    // m_currentStream.seek(dataSpec.position);

    // transferStarted(dataSpec);

    return -1; // C.LENGTH_UNSET equivalent
}

int ExoPlayerSeekableInputStreamDataSource::read(uint8_t* buffer, int offset, int readLength) {
    if (readLength == 0) {
        return 0;
    }

    if (m_currentStream == nullptr) {
        throw std::runtime_error("Stream is null");
    }

    int result = m_currentStream->read(buffer, offset, readLength);

    if (result < 0) {
        return -1; // C.RESULT_END_OF_INPUT
    }

    // bytesTransferred(result);
    return result;
}

const std::string& ExoPlayerSeekableInputStreamDataSource::getUri() const {
    return URI;
}

void ExoPlayerSeekableInputStreamDataSource::close() {
    if (m_currentStream != nullptr) {
        m_currentStream->close();
        m_currentStream.reset();
        // transferEnded();
    }
}

} // namespace PinkReader
