// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageTileSourceWholeBitmap.java
#pragma once

#include "views/imageview/image_tile_source.h"

#include <cstdint>

namespace PinkReader {

// Forward declaration for Bitmap (placeholder)
class Bitmap;

/**
 * Port of org.quantumbadger.redreader.views.imageview.ImageTileSourceWholeBitmap
 */
class ImageTileSourceWholeBitmap : public ImageTileSource {
public:
	// Port of: private static final int TILE_SIZE = 512;
	static constexpr int TILE_SIZE = 512;

	// Port of: public ImageTileSourceWholeBitmap(final Bitmap bitmap)
	explicit ImageTileSourceWholeBitmap(Bitmap* bitmap);

	// Port of: @Override public int getWidth()
	int getWidth() override;

	// Port of: @Override public int getHeight()
	int getHeight() override;

	// Port of: @Override public int getTileSize()
	int getTileSize() override;

	// Port of: @Override public int getHTileCount()
	int getHTileCount() override;

	// Port of: @Override public int getVTileCount()
	int getVTileCount() override;

	// Port of: @Override public Bitmap getTile(final int sampleSize, final int tileX, final int tileY)
	Bitmap* getTile(int sampleSize, int tileX, int tileY) override;

	// Port of: @Override public void dispose()
	void dispose() override;

private:
	// Port of: private final Bitmap mBitmap
	// Using uint8_t* as placeholder for raw bitmap data
	Bitmap* mBitmap;

	// Port of: private final int mWidth
	int mWidth;

	// Port of: private final int mHeight
	int mHeight;
};

} // namespace PinkReader
