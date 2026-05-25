// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageTileSourceWholeBitmap.java
#include "views/imageview/image_tile_source_whole_bitmap.h"

#include <algorithm>
#include <cstring>

namespace PinkReader {

// Forward declare Bitmap (placeholder - actual implementation will be provided by the rendering system)
struct Bitmap {
	int width;
	int height;
	uint8_t* pixels; // RGBA 8888
	int stride;

	Bitmap(int w, int h) : width(w), height(h), stride(w * 4) {
		pixels = new uint8_t[width * height * 4]();
	}

	~Bitmap() {
		delete[] pixels;
	}

	// Prevent copying
	Bitmap(const Bitmap&) = delete;
	Bitmap& operator=(const Bitmap&) = delete;

	// Port of: Bitmap.createBitmap(source, x, y, w, h, matrix, filter)
	static Bitmap* createSubBitmap(const Bitmap* source, int x, int y, int w, int h, float scaleX = 1.0f, float scaleY = 1.0f) {
		int outW = static_cast<int>(w * scaleX);
		int outH = static_cast<int>(h * scaleY);
		Bitmap* result = new Bitmap(outW, outH);

		// Simple nearest-neighbor scaling for now (real implementation would use bilinear)
		for (int dy = 0; dy < outH; dy++) {
			for (int dx = 0; dx < outW; dx++) {
				int sx = x + static_cast<int>(dx / scaleX);
				int sy = y + static_cast<int>(dy / scaleY);
				if (sx >= 0 && sx < source->width && sy >= 0 && sy < source->height) {
					int srcIdx = (sy * source->stride) + (sx * 4);
					int dstIdx = (dy * result->stride) + (dx * 4);
					result->pixels[dstIdx + 0] = source->pixels[srcIdx + 0];
					result->pixels[dstIdx + 1] = source->pixels[srcIdx + 1];
					result->pixels[dstIdx + 2] = source->pixels[srcIdx + 2];
					result->pixels[dstIdx + 3] = source->pixels[srcIdx + 3];
				}
			}
		}

		return result;
	}

	// Port of: bitmap.recycle()
	void recycle() {
		// In C++, we just delete
	}
};

// Port of: public ImageTileSourceWholeBitmap(final Bitmap bitmap)
//   mBitmap = bitmap;
//   mWidth = bitmap.getWidth();
//   mHeight = bitmap.getHeight();
ImageTileSourceWholeBitmap::ImageTileSourceWholeBitmap(Bitmap* bitmap)
	: mBitmap(bitmap)
	, mWidth(bitmap->width)
	, mHeight(bitmap->height)
{
}

// Port of: @Override public int getWidth()
//   return mWidth;
int ImageTileSourceWholeBitmap::getWidth() {
	return mWidth;
}

// Port of: @Override public int getHeight()
//   return mHeight;
int ImageTileSourceWholeBitmap::getHeight() {
	return mHeight;
}

// Port of: @Override public int getTileSize()
//   return TILE_SIZE;
int ImageTileSourceWholeBitmap::getTileSize() {
	return TILE_SIZE;
}

// Port of: @Override public int getHTileCount()
//   return General.divideCeil(getWidth(), TILE_SIZE);
int ImageTileSourceWholeBitmap::getHTileCount() {
	// Port of: General.divideCeil(getWidth(), TILE_SIZE)
	// divideCeil: (a + b - 1) / b
	return (getWidth() + TILE_SIZE - 1) / TILE_SIZE;
}

// Port of: @Override public int getVTileCount()
//   return General.divideCeil(getHeight(), TILE_SIZE);
int ImageTileSourceWholeBitmap::getVTileCount() {
	// Port of: General.divideCeil(getHeight(), TILE_SIZE)
	return (getHeight() + TILE_SIZE - 1) / TILE_SIZE;
}

// Port of: @Override public Bitmap getTile(final int sampleSize, final int tileX, final int tileY)
Bitmap* ImageTileSourceWholeBitmap::getTile(int sampleSize, int tileX, int tileY) {
	// Port of: if(sampleSize == 1 && TILE_SIZE >= mWidth && TILE_SIZE >= mHeight) {
	//             return mBitmap;
	//         }
	if (sampleSize == 1 && TILE_SIZE >= mWidth && TILE_SIZE >= mHeight) {
		return mBitmap;
	}

	// Port of: final int tileStartX = tileX * TILE_SIZE;
	int tileStartX = tileX * TILE_SIZE;

	// Port of: final int tileStartY = tileY * TILE_SIZE;
	int tileStartY = tileY * TILE_SIZE;

	// Port of: final int tileEndX = Math.min(mWidth, (tileX + 1) * TILE_SIZE);
	int tileEndX = std::min(mWidth, (tileX + 1) * TILE_SIZE);

	// Port of: final int tileEndY = Math.min(mHeight, (tileY + 1) * TILE_SIZE);
	int tileEndY = std::min(mHeight, (tileY + 1) * TILE_SIZE);

	// Port of: final int inputTileWidthPx = tileEndX - tileStartX;
	int inputTileWidthPx = tileEndX - tileStartX;

	// Port of: final int inputTileHeightPx = tileEndY - tileStartY;
	int inputTileHeightPx = tileEndY - tileStartY;

	// Port of: if(sampleSize == 1) {
	//             return Bitmap.createBitmap(mBitmap, tileStartX, tileStartY, inputTileWidthPx, inputTileHeightPx);
	//         }
	if (sampleSize == 1) {
		return Bitmap::createSubBitmap(mBitmap, tileStartX, tileStartY, inputTileWidthPx, inputTileHeightPx);
	}

	// Port of: final Matrix scaleMatrix = new Matrix();
	//          scaleMatrix.setScale(1.0f / sampleSize, 1.0f / sampleSize);
	//          return Bitmap.createBitmap(mBitmap, tileStartX, tileStartY, inputTileWidthPx, inputTileHeightPx,
	//              scaleMatrix, true);
	float invScale = 1.0f / static_cast<float>(sampleSize);
	return Bitmap::createSubBitmap(mBitmap, tileStartX, tileStartY, inputTileWidthPx, inputTileHeightPx, invScale, invScale);
}

// Port of: @Override public void dispose()
//   // Nothing to do here
void ImageTileSourceWholeBitmap::dispose() {
	// Nothing to do here
}

} // namespace PinkReader
