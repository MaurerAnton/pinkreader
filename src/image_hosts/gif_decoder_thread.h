// Origin: RedReader image/GifDecoderThread.java
#pragma once

#include <string>
#include <atomic>
#include <thread>
#include <memory>
#include <istream>

namespace PinkReader {

// GifDecoderThread: Plays GIF animations by decoding frames in a background thread
// and posting them to an ImageView-like target on the main thread.
class GifDecoderThread {
public:
	// Callback interface for GIF loading events
	struct OnGifLoadedListener {
		virtual ~OnGifLoadedListener() = default;
		virtual void OnGifLoaded() = 0;
		virtual void OnOutOfMemory() = 0;
		virtual void OnGifInvalid() = 0;
	};

private:
	static constexpr const char* TAG = "GifDecoderThread";

	std::atomic<bool> mPlaying;
	std::istream* mIs;
	void* mView; // ImageView* equivalent - platform-specific image display
	OnGifLoadedListener* mListener;

	// Handler that posts frames to the main thread
	// In C++, this would use the platform's equivalent of Android's Handler/Looper
	void* mHandler; // Platform-specific main thread handler

public:
	GifDecoderThread(std::istream& is, OnGifLoadedListener& listener);
	~GifDecoderThread();

	void SetView(void* view);
	void StopPlaying();

	// Thread entry point
	void Run();
	void Start();

private:
	void HandleFrame(void* bitmap);
};

} // namespace PinkReader
