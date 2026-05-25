// Origin: org.quantumbadger.redreader.views.video.ExoPlayerSeekableInputStreamDataSourceFactory.java
// Ported from RedReader (GPLv3)

#include "views/video/exo_player_seekable_data_source_factory.h"
#include "views/video/exo_player_seekable_input_stream_data_source.h"

#include <memory>

namespace PinkReader {

ExoPlayerSeekableInputStreamDataSourceFactory::ExoPlayerSeekableInputStreamDataSourceFactory(
        bool isNetwork,
        SeekableInputStreamFactory streamFactory)
    : m_isNetwork(isNetwork)
    , m_streamFactory(std::move(streamFactory)) {}

std::unique_ptr<ExoPlayerSeekableInputStreamDataSource>
ExoPlayerSeekableInputStreamDataSourceFactory::createDataSource() {
    return std::make_unique<ExoPlayerSeekableInputStreamDataSource>(m_isNetwork, m_streamFactory);
}

} // namespace PinkReader
