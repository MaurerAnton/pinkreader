// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/MultiScaleTileManager.java
#include "views/imageview/multi_scale_tile_manager.h"
#include "views/imageview/image_tile_source.h"
#include "views/imageview/image_view_tile_loader_thread.h"
#include "views/imageview/image_view_tile_loader.h"

#include <algorithm>

namespace PinkReader {

// Port of: public static int scaleIndexToSampleSize(final int scaleIndex)
//   return 1 << scaleIndex;
int MultiScaleTileManager::scaleIndexToSampleSize(int scaleIndex) {
	return 1 << scaleIndex;
}

// Port of: public static int sampleSizeToScaleIndex(final int sampleSize)
//   return Integer.numberOfTrailingZeros(sampleSize);
int MultiScaleTileManager::sampleSizeToScaleIndex(int sampleSize) {
	// Count trailing zeros (like Integer.numberOfTrailingZeros)
	if (sampleSize == 0) return 32;
	int count = 0;
	while ((sampleSize & 1) == 0) {
		count++;
		sampleSize >>= 1;
	}
	return count;
}

// Port of: public MultiScaleTileManager(final ImageTileSource imageTileSource,
//         final ImageViewTileLoaderThread thread, final int x, final int y,
//         final ImageViewTileLoader.Listener listener)
MultiScaleTileManager::MultiScaleTileManager(
		ImageTileSource* imageTileSource,
		ImageViewTileLoaderThread* thread,
		int x,
		int y,
		ImageViewTileLoader::Listener* listener)
{
	// Port of: mTileLoaders = new ImageViewTileLoader[sampleSizeToScaleIndex(MAX_SAMPLE_SIZE) + 1];
	int loaderCount = sampleSizeToScaleIndex(MAX_SAMPLE_SIZE) + 1;
	mTileLoaders.resize(static_cast<size_t>(loaderCount));

	// Port of: for(int s = 0; s < mTileLoaders.length; s++) {
	//             mTileLoaders[s] = new ImageViewTileLoader(
	//                 imageTileSource, thread, x, y, scaleIndexToSampleSize(s), listener, mLock);
	//         }
	for (int s = 0; s < loaderCount; s++) {
		mTileLoaders[static_cast<size_t>(s)] = new ImageViewTileLoader(
				imageTileSource,
				thread,
				x,
				y,
				scaleIndexToSampleSize(s),
				listener,
				mLock);
	}
}

MultiScaleTileManager::~MultiScaleTileManager() {
	for (auto* loader : mTileLoaders) {
		delete loader;
	}
	mTileLoaders.clear();
}

// Port of: public Bitmap getAtDesiredScale()
//   return mTileLoaders[mDesiredScaleIndex].get();
Bitmap* MultiScaleTileManager::getAtDesiredScale() {
	return mTileLoaders[static_cast<size_t>(mDesiredScaleIndex)]->get();
}

// Port of: public void markAsWanted(final int desiredScaleIndex)
void MultiScaleTileManager::markAsWanted(int desiredScaleIndex) {
	// Port of: if(desiredScaleIndex == mDesiredScaleIndex) { return; }
	if (desiredScaleIndex == mDesiredScaleIndex) {
		return;
	}

	// Port of: mDesiredScaleIndex = desiredScaleIndex;
	mDesiredScaleIndex = desiredScaleIndex;

	// Port of: synchronized(mLock) {
	{
		std::lock_guard<std::mutex> lockGuard(mLock);

		// Port of: mTileLoaders[desiredScaleIndex].markAsWanted();
		mTileLoaders[static_cast<size_t>(desiredScaleIndex)]->markAsWanted();

		// Port of: for(int s = 0; s < mTileLoaders.length; s++) {
		//             if(s != desiredScaleIndex) {
		//                 mTileLoaders[s].markAsUnwanted();
		//             }
		//         }
		for (size_t s = 0; s < mTileLoaders.size(); s++) {
			if (static_cast<int>(s) != desiredScaleIndex) {
				mTileLoaders[s]->markAsUnwanted();
			}
		}
	}
}

// Port of: public void markAsUnwanted()
void MultiScaleTileManager::markAsUnwanted() {
	// Port of: if(mDesiredScaleIndex == -1) { return; }
	if (mDesiredScaleIndex == -1) {
		return;
	}

	// Port of: mDesiredScaleIndex = -1;
	mDesiredScaleIndex = -1;

	// Port of: synchronized(mLock) {
	{
		std::lock_guard<std::mutex> lockGuard(mLock);

		// Port of: for(int s = 0; s < mTileLoaders.length; s++) {
		//             mTileLoaders[s].markAsUnwanted();
		//         }
		for (size_t s = 0; s < mTileLoaders.size(); s++) {
			mTileLoaders[s]->markAsUnwanted();
		}
	}
}

} // namespace PinkReader
