// Origin: RedReader/src/main/java/org/quantumbadger/redreader/adapters/ShareOrderCallbackListener.java

#pragma once

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.adapters.ShareOrderCallbackListener
 *
 * Callback interface for share order selection.
 */
class ShareOrderCallbackListener {
public:
	virtual ~ShareOrderCallbackListener() = default;

	// Port of: void onSelectedIntent(int position);
	virtual void onSelectedIntent(int position) = 0;
};

} // namespace PinkReader
