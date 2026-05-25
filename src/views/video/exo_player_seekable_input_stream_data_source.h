// Origin: org.quantumbadger.redreader.views.video.ExoPlayerSeekableInputStreamDataSource.java
// Ported from RedReader (GPLv3)
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace PinkReader {

// Forward declarations
class SeekableInputStream;

// GenericFactory<SeekableInputStream, IOException> equivalent
using SeekableInputStreamFactory = std::function<std::unique_ptr<SeekableInputStream>()>;

class ExoPlayerSeekableInputStreamDataSource {
public:
    static const std::string URI; // "redreader://video"

    explicit ExoPlayerSeekableInputStreamDataSource(
            bool isNetwork,
            SeekableInputStreamFactory streamFactory);

    long open(void* dataSpec); // DataSpec* with position field
    int read(uint8_t* buffer, int offset, int readLength);
    const std::string& getUri() const;
    void close();

private:
    SeekableInputStreamFactory m_streamFactory;
    std::unique_ptr<SeekableInputStream> m_currentStream;
};

} // namespace PinkReader
