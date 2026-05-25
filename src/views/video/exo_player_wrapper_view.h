// Origin: org.quantumbadger.redreader.views.video.ExoPlayerWrapperView.java
// Ported from RedReader (GPLv3)
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace PinkReader {

class ExoPlayerWrapperView {
public:
    // --- Listener interface ---
    struct Listener {
        virtual ~Listener() = default;
        virtual void onError() = 0;
    };

    // --- Constructor ---
    ExoPlayerWrapperView(
            void* context,                     // Context*
            void* mediaSource,                 // MediaSource*
            Listener* listener,
            int controlsMarginRightDp);

    // --- Public API ---
    void handleTap();
    void release();
    void updateProgress();
    bool isMuted() const;
    void setMuted(bool mute);
    int isControlViewVisible() const;

    static std::string msToMinutesAndSecondsString(long ms);

private:
    // --- Static helpers ---
    struct ControlState {
        void* controlView = nullptr;           // RelativeLayout*
        void* playButton = nullptr;             // ImageButton*
        void* timeBarView = nullptr;            // DefaultTimeBar*
        void* timeTextView = nullptr;           // TextView*
        void* speedTextView = nullptr;          // TextView*
        void* stepBackButton = nullptr;         // ImageButton*
        void* stepForwardButton = nullptr;      // ImageButton*
    };

    void openSpeedSettingDialog(void* context);

    // --- Fields ---
    Listener* m_listener;
    void* m_videoPlayer = nullptr;             // ExoPlayer*
    ControlState m_controls;
    float m_currentPlaybackSpeed = 1.0f;
    bool m_released = false;
};

} // namespace PinkReader
