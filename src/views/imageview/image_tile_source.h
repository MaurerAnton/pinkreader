// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageTileSource.java
#pragma once

namespace PinkReader {

// Forward declaration for Bitmap (placeholder - will be a real type in C++ port)
class Bitmap;

/**
 * Port of org.quantumbadger.redreader.views.imageview.ImageTileSource
 * Interface (pure virtual in C++)
 */
class ImageTileSource {
public:
	virtual ~ImageTileSource() = default;

	// Port of: int getWidth();
	virtual int getWidth() = 0;

	// Port of: int getHeight();
	virtual int getHeight() = 0;

	// Port of: int getHTileCount();
	virtual int getHTileCount() = 0;

	// Port of: int getVTileCount();
	virtual int getVTileCount() = 0;

	// Port of: int getTileSize();
	virtual int getTileSize() = 0;

	// Port of: Bitmap getTile(int sampleSize, int tileX, int tileY);
	virtual Bitmap* getTile(int sampleSize, int tileX, int tileY) = 0;

	// Port of: void dispose();
	virtual void dispose() = 0;
};

} // namespace PinkReader
