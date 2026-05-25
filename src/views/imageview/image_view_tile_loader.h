// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewTileLoader.java
#pragma once

#include <functional>
#include <mutex>

namespace PinkReader {

// Forward declarations
class ImageTileSource;
class ImageViewTileLoaderThread;
class Bitmap;

/**
 * Port of org.quantumbadger.redreader.views.imageview.ImageViewTileLoader
 */
class ImageViewTileLoader {
public:
	// Port of: @UiThread public interface Listener
	class Listener {
	public:
		virtual ~Listener() = default;

		// Port of: void onTileLoaded(int x, int y, int sampleSize)
		virtual void onTileLoaded(int x, int y, int sampleSize) = 0;

		// Port of: void onTileLoaderOutOfMemory()
		virtual void onTileLoaderOutOfMemory() = 0;

		// Port of: void onTileLoaderException(Throwable t)
		virtual void onTileLoaderException(const char* error) = 0;
	};

	// Port of: public ImageViewTileLoader(final ImageTileSource source, final ImageViewTileLoaderThread thread,
	//         final int x, final int y, final int sampleSize, final Listener listener, final Object lock)
	ImageViewTileLoader(
			ImageTileSource* source,
			ImageViewTileLoaderThread* thread,
			int x,
			int y,
			int sampleSize,
			Listener* listener,
			std::mutex& lock);

	// Port of: // Caller must synchronize on mLock
	//          public void markAsWanted()
	void markAsWanted();

	// Port of: public void doPrepare()
	void doPrepare();

	// Port of: public Bitmap get()
	Bitmap* get();

	// Port of: // Caller must synchronize on mLock
	//          public void markAsUnwanted()
	void markAsUnwanted();

private:
	// Port of: private final ImageTileSource mSource
	ImageTileSource* mSource;

	// Port of: private final ImageViewTileLoaderThread mThread
	ImageViewTileLoaderThread* mThread;

	// Port of: private final int mX
	int mX;

	// Port of: private final int mY
	int mY;

	// Port of: private final int mSampleSize
	int mSampleSize;

	// Port of: private boolean mWanted
	bool mWanted = false;

	// Port of: private Bitmap mResult
	Bitmap* mResult = nullptr;

	// Port of: private final Listener mListener
	Listener* mListener;

	// Port of: private final Runnable mNotifyRunnable
	// In C++: just use a member function

	// Port of: private final Object mLock
	std::mutex& mLock;
};

} // namespace PinkReader
