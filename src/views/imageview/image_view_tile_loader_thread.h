// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewTileLoaderThread.java
#pragma once

#include <deque>
#include <mutex>
#include <functional>
#include <thread>
#include <vector>
#include <atomic>
#include <condition_variable>

namespace PinkReader {

// Forward declaration
class ImageViewTileLoader;

/**
 * Port of org.quantumbadger.redreader.views.imageview.ImageViewTileLoaderThread
 */
class ImageViewTileLoaderThread {
public:
	// Port of: public ImageViewTileLoaderThread()
	ImageViewTileLoaderThread();

	~ImageViewTileLoaderThread();

	// Port of: public void enqueue(final ImageViewTileLoader tile)
	void enqueue(ImageViewTileLoader* tile);

private:
	// Port of: private class InternalRunnable implements Runnable
	// Worker thread function
	void workerRun();

	// Port of: private final TriggerableThreadGroup mThreads
	// Replaced with manual thread pool
	std::vector<std::thread> mThreads;

	// Port of: private final Deque<ImageViewTileLoader> mQueue = new ArrayDeque<>(128)
	std::deque<ImageViewTileLoader*> mQueue;

	// Port of: synchronized(mQueue) -- replaces intrinsic lock
	std::mutex mQueueMutex;

	// Condition variable to wake worker threads
	std::condition_variable mCondition;

	// Shutdown flag
	std::atomic<bool> mShutdown{false};
};

} // namespace PinkReader
