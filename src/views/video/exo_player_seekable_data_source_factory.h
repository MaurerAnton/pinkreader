// Origin: org.quantumbadger.redreader.views.video.ExoPlayerSeekableInputStreamDataSourceFactory.java
// Ported from RedReader (GPLv3)
#pragma once

#include <functional>
#include <memory>

namespace PinkReader {

// Forward declaration
class ExoPlayerSeekableInputStreamDataSource;
using SeekableInputStreamFactory = std::function<std::unique_ptr<class SeekableInputStream>()>;

class ExoPlayerSeekableInputStreamDataSourceFactory {
public:
    ExoPlayerSeekableInputStreamDataSourceFactory(
            bool isNetwork,
            SeekableInputStreamFactory streamFactory);

    std::unique_ptr<ExoPlayerSeekableInputStreamDataSource> createDataSource();

private:
    bool m_isNetwork;
    SeekableInputStreamFactory m_streamFactory;
};

} // namespace PinkReader
