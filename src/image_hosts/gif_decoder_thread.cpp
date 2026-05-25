// Origin: RedReader image/GifDecoderThread.java
#include "gif_decoder_thread.h"
#include <thread>
#include <chrono>
#include <algorithm>

namespace PinkReader {

GifDecoderThread::GifDecoderThread(std::istream& is, OnGifLoadedListener& listener)
	: mPlaying(true)
	, mIs(&is)
	, mView(nullptr)
	, mListener(&listener)
	, mHandler(nullptr) {
}

GifDecoderThread::~GifDecoderThread() {
	StopPlaying();
}

void GifDecoderThread::SetView(void* view) {
	mView = view;
}

void GifDecoderThread::StopPlaying() {
	mPlaying = false;
	// try { mIs->close(); } catch(Throwable) { Log.e(TAG, "Exception while stopping", t); }
	// In C++, we'd close the stream if owned
}

void GifDecoderThread::HandleFrame(void* bitmap) {
	if (mPlaying && mView != nullptr) {
		// Post bitmap to the main thread for display
		// view.setImageBitmap(bitmap) equivalent
		// Deferred: platform-specific image rendering
	}
}

void GifDecoderThread::Start() {
	std::thread t(&GifDecoderThread::Run, this);
	t.detach();
}

void GifDecoderThread::Run() {
	// Atomic flags for decoder state
	std::atomic<bool> loaded(false);
	std::atomic<bool> failed(false);

	// GifDecoder decoder = new GifDecoder()
	// Spawn decoding thread:
	//   new Thread() {
	//     run() {
	//       try { decoder.read(is); loaded.set(true); }
	//       catch(Throwable t) { Log.i(TAG, "Got exception", t); failed.set(true); }
	//     }
	//   }.start();

	if (!mPlaying) {
		return;
	}

	mListener->OnGifLoaded();

	int frame = 0;

	while (mPlaying) {
		// Wait for frames to be decoded:
		// while(decoder.getFrameCount() <= frame + 1 && !loaded.get() && !failed.get()) {
		//   try { sleep(100); } catch(InterruptedException) { return; }
		// }

		// frame = frame % decoder.getFrameCount()
		// Bitmap img = decoder.getFrame(frame)
		// Message msg = Message.obtain(); msg.obj = img; handler.sendMessage(msg)

		// try {
		//   sleep(std::max(32, decoder.getDelay(frame)))
		// } catch(InterruptedException) { return; }

		if (failed) {
			mListener->OnGifInvalid();
			return;
		}

		frame++;
	}

	// catch OutOfMemoryError: listener.onOutOfMemory()
	// catch Throwable: listener.onGifInvalid()
}

} // namespace PinkReader
