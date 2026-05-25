// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/MultiScaleTileManager.java
#pragma once

#include <mutex>
#include <vector>

namespace PinkReader {

// Forward declarations
class ImageTileSource;
class ImageViewTileLoaderThread;
class ImageViewTileLoader;
class Bitmap;

/**
 * Port of org.quantumbadger.redreader.views.imageview.MultiScaleTileManager
 */
class MultiScaleTileManager {
public:
	// Port of: public static final int MAX_SAMPLE_SIZE = 32;
	static constexpr int MAX_SAMPLE_SIZE = 32;

	// Port of: public static int scaleIndexToSampleSize(final int scaleIndex)
	static int scaleIndexToSampleSize(int scaleIndex);

	// Port of: public static int sampleSizeToScaleIndex(final int sampleSize)
	static int sampleSizeToScaleIndex(int sampleSize);

	// Port of: public MultiScaleTileManager(final ImageTileSource imageTileSource,
	//         final ImageViewTileLoaderThread thread, final int x, final int y,
	//         final ImageViewTileLoader.Listener listener)
	MultiScaleTileManager(
			ImageTileSource* imageTileSource,
			ImageViewTileLoaderThread* thread,
			int x,
			int y,
			ImageViewTileLoader::Listener* listener);

	~MultiScaleTileManager();

	// Port of: public Bitmap getAtDesiredScale()
	Bitmap* getAtDesiredScale();

	// Port of: public void markAsWanted(final int desiredScaleIndex)
	void markAsWanted(int desiredScaleIndex);

	// Port of: public void markAsUnwanted()
	void markAsUnwanted();

private:
	// Port of: private final ImageViewTileLoader[] mTileLoaders
	std::vector<ImageViewTileLoader*> mTileLoaders;

	// Port of: private int mDesiredScaleIndex = -1;
	int mDesiredScaleIndex = -1;

	// Port of: private final Object mLock = new Object();
	std::mutex mLock;
};

} // namespace PinkReader
