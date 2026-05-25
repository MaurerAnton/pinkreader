// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewTileLoader.java
#include "views/imageview/image_view_tile_loader.h"
#include "views/imageview/image_tile_source.h"
#include "views/imageview/image_view_tile_loader_thread.h"

#include <stdexcept>

namespace PinkReader {

// Port of: public ImageViewTileLoader(final ImageTileSource source, final ImageViewTileLoaderThread thread,
//         final int x, final int y, final int sampleSize, final Listener listener, final Object lock)
ImageViewTileLoader::ImageViewTileLoader(
		ImageTileSource* source,
		ImageViewTileLoaderThread* thread,
		int x,
		int y,
		int sampleSize,
		Listener* listener,
		std::mutex& lock)
	// Port of: mSource = source;
	: mSource(source)
	// Port of: mThread = thread;
	, mThread(thread)
	// Port of: mX = x;
	, mX(x)
	// Port of: mY = y;
	, mY(y)
	// Port of: mSampleSize = sampleSize;
	, mSampleSize(sampleSize)
	// Port of: mListener = listener;
	, mListener(listener)
	// Port of: mLock = lock;
	, mLock(lock)
{
	// Port of: mNotifyRunnable = () -> mListener.onTileLoaded(mX, mY, mSampleSize);
	// (no Runnable needed in C++; we call directly in doPrepare)
}

// Port of: // Caller must synchronize on mLock
//          public void markAsWanted()
void ImageViewTileLoader::markAsWanted() {
	// Port of: if(mWanted) { return; }
	if (mWanted) {
		return;
	}

	// Port of: if(mResult != null) {
	//             throw new RuntimeException("Not wanted, but the image is loaded anyway!");
	//         }
	if (mResult != nullptr) {
		throw std::runtime_error("Not wanted, but the image is loaded anyway!");
	}

	// Port of: mThread.enqueue(this);
	mThread->enqueue(this);

	// Port of: mWanted = true;
	mWanted = true;
}

// Port of: public void doPrepare()
void ImageViewTileLoader::doPrepare() {
	// Port of: synchronized(mLock) {
	{
		std::lock_guard<std::mutex> lockGuard(mLock);

		// Port of: if(!mWanted) { return; }
		if (!mWanted) {
			return;
		}

		// Port of: if(mResult != null) { return; }
		if (mResult != nullptr) {
			return;
		}
	}

	// Port of: final Bitmap tile;
	Bitmap* tile = nullptr;

	// Port of: try {
	//             tile = mSource.getTile(mSampleSize, mX, mY);
	//          } catch(final OutOfMemoryError e) {
	//             AndroidCommon.UI_THREAD_HANDLER.post(new NotifyOOMRunnable());
	//             return;
	//          } catch(final Throwable t) {
	//             Log.e("ImageViewTileLoader", "Exception in getTile()", t);
	//             AndroidCommon.UI_THREAD_HANDLER.post(new NotifyErrorRunnable(t));
	//             return;
	//          }
	try {
		tile = mSource->getTile(mSampleSize, mX, mY);
	} catch (const std::bad_alloc&) {
		// Port of: AndroidCommon.UI_THREAD_HANDLER.post(new NotifyOOMRunnable());
		if (mListener) {
			mListener->onTileLoaderOutOfMemory();
		}
		return;
	} catch (const std::exception& e) {
		// Port of: Log.e("ImageViewTileLoader", "Exception in getTile()", t);
		//          AndroidCommon.UI_THREAD_HANDLER.post(new NotifyErrorRunnable(t));
		if (mListener) {
			mListener->onTileLoaderException(e.what());
		}
		return;
	} catch (...) {
		if (mListener) {
			mListener->onTileLoaderException("Unknown exception in getTile()");
		}
		return;
	}

	// Port of: synchronized(mLock) {
	{
		std::lock_guard<std::mutex> lockGuard(mLock);

		// Port of: if(mWanted) {
		//             mResult = tile;
		//         } else if(tile != null) {
		//             tile.recycle();
		//         }
		if (mWanted) {
			mResult = tile;
		} else if (tile != nullptr) {
			// tile.recycle(); -- placeholder for bitmap recycling
			delete tile;
			tile = nullptr;
		}
	}

	// Port of: AndroidCommon.UI_THREAD_HANDLER.post(mNotifyRunnable);
	// Notify immediately (assuming caller runs on appropriate thread or
	// the listener handles thread safety)
	if (mListener) {
		mListener->onTileLoaded(mX, mY, mSampleSize);
	}
}

// Port of: public Bitmap get()
Bitmap* ImageViewTileLoader::get() {
	// Port of: synchronized(mLock) {
	std::lock_guard<std::mutex> lockGuard(mLock);

	// Port of: if(!mWanted) {
	//             throw new RuntimeException("Attempted to get unwanted image!");
	//         }
	if (!mWanted) {
		throw std::runtime_error("Attempted to get unwanted image!");
	}

	// Port of: return mResult;
	return mResult;
}

// Port of: // Caller must synchronize on mLock
//          public void markAsUnwanted()
void ImageViewTileLoader::markAsUnwanted() {
	// Port of: mWanted = false;
	mWanted = false;

	// Port of: if(mResult != null) {
	//             mResult.recycle();
	//             mResult = null;
	//         }
	if (mResult != nullptr) {
		// mResult.recycle(); -- placeholder for bitmap recycling
		delete mResult;
		mResult = nullptr;
	}
}

} // namespace PinkReader
