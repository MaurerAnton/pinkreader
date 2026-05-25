// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/MediaUtils.java
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <cstdint>

namespace PinkReader {

/**
 * Media utility class for muxing multiple media files into one output.
 * Equivalent to: org.quantumbadger.redreader.common.MediaUtils
 *
 * NOTE: This is a platform-specific implementation that uses Android's
 * MediaExtractor and MediaMuxer APIs. In a portable C++ port, this is
 * provided as a stub that delegates to platform-specific code.
 */
class MediaUtils {
public:
	MediaUtils() = delete;

	/**
	 * Muxes multiple input media files into a single output file.
	 * Equivalent to: MediaUtils.muxFiles(File, File[], Runnable, FunctionOneArgNoReturn<Exception>)
	 *
	 * This method runs on a background thread and calls success/failure callbacks.
	 *
	 * @param outputFilePath path for the output file
	 * @param inputFilePaths paths of input files to mux
	 * @param successCallback invoked on successful mux
	 * @param failureCallback invoked on error with exception message
	 */
	static void muxFiles(
		const std::string& outputFilePath,
		const std::vector<std::string>& inputFilePaths,
		std::function<void()> successCallback,
		std::function<void(const std::string&)> failureCallback);
};

} // namespace PinkReader
