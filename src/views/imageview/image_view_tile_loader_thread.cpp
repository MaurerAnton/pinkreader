// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewTileLoaderThread.java
#include "views/imageview/image_view_tile_loader_thread.h"
#include "views/imageview/image_view_tile_loader.h"

#include <algorithm>
#include <thread>

namespace PinkReader {

// Port of: public ImageViewTileLoaderThread()
//   final int threadCount = Math.max(1, Runtime.getRuntime().availableProcessors() - 1);
//   Log.i("IViewTileLoaderThread", "Using thread count: " + threadCount);
//   mThreads = new TriggerableThreadGroup(threadCount, new InternalRunnable());
ImageViewTileLoaderThread::ImageViewTileLoaderThread() {
	// Port of: final int threadCount = Math.max(1, Runtime.getRuntime().availableProcessors() - 1);
	int threadCount = std::max(1u, std::thread::hardware_concurrency() - 1);
	if (threadCount < 1) threadCount = 1;

	// Port of: Log.i("IViewTileLoaderThread", "Using thread count: " + threadCount);
	// (logging omitted in port)

	// Port of: mThreads = new TriggerableThreadGroup(threadCount, new InternalRunnable());
	for (int i = 0; i < threadCount; i++) {
		mThreads.emplace_back(&ImageViewTileLoaderThread::workerRun, this);
	}
}

ImageViewTileLoaderThread::~ImageViewTileLoaderThread() {
	mShutdown.store(true);
	mCondition.notify_all();
	for (auto& t : mThreads) {
		if (t.joinable()) {
			t.join();
		}
	}
}

// Port of: public void enqueue(final ImageViewTileLoader tile)
//   synchronized(mQueue) {
//     mQueue.addLast(tile);
//     mThreads.triggerOne();
//   }
void ImageViewTileLoaderThread::enqueue(ImageViewTileLoader* tile) {
	{
		std::lock_guard<std::mutex> lock(mQueueMutex);
		// Port of: mQueue.addLast(tile);
		mQueue.push_back(tile);
	}
	// Port of: mThreads.triggerOne();
	mCondition.notify_one();
}

// Port of: private class InternalRunnable implements Runnable
//   @Override public void run() {
//     while(true) {
//       final ImageViewTileLoader tile;
//       synchronized(mQueue) {
//         if(mQueue.isEmpty()) { return; }
//         tile = mQueue.removeFirst();
//       }
//       tile.doPrepare();
//     }
//   }
void ImageViewTileLoaderThread::workerRun() {
	while (!mShutdown.load()) {
		ImageViewTileLoader* tile = nullptr;

		{
			std::unique_lock<std::mutex> lock(mQueueMutex);

			// Port of: if(mQueue.isEmpty()) { return; }
			// In the original, the thread returns (goes back to pool).
			// We wait instead and check shutdown flag.
			mCondition.wait(lock, [this]() {
				return mShutdown.load() || !mQueue.empty();
			});

			if (mShutdown.load()) {
				return;
			}

			if (mQueue.empty()) {
				continue;
			}

			// Port of: tile = mQueue.removeFirst();
			tile = mQueue.front();
			mQueue.pop_front();
		}

		// Port of: tile.doPrepare();
		if (tile != nullptr) {
			tile->doPrepare();
		}
	}
}

} // namespace PinkReader
