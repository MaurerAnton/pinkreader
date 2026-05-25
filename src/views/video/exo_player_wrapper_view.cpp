// Origin: org.quantumbadger.redreader.views.video.ExoPlayerWrapperView.java
// Ported from RedReader (GPLv3)

#include "views/video/exo_player_wrapper_view.h"

#include <cstdio>
#include <string>

namespace PinkReader {

// --- Constructor ---

ExoPlayerWrapperView::ExoPlayerWrapperView(
        void* context,
        void* mediaSource,
        Listener* listener,
        int controlsMarginRightDp)
    : m_listener(listener)
    , m_currentPlaybackSpeed(1.0f)
    , m_released(false) {

    // In original:
    // super(context);
    // mListener = listener;
    //
    // final DefaultTrackSelector trackSelector = new DefaultTrackSelector(context);
    // mVideoPlayer = new ExoPlayer.Builder(context).setTrackSelector(trackSelector).build();
    //
    // final PlayerView videoPlayerView = new PlayerView(context);
    // addView(videoPlayerView);
    // videoPlayerView.setUseController(false);
    // videoPlayerView.setShowBuffering(PlayerView.SHOW_BUFFERING_ALWAYS);
    // videoPlayerView.setPlayer(mVideoPlayer);
    // videoPlayerView.requestFocus();
    //
    // mVideoPlayer.setMediaSource(mediaSource);
    // mVideoPlayer.prepare();
    // mVideoPlayer.setRepeatMode(Player.REPEAT_MODE_ONE);
    // mVideoPlayer.setPlayWhenReady(true);
    //
    // if(PrefsUtility.pref_behaviour_video_zoom_default()) {
    //     videoPlayerView.setResizeMode(AspectRatioFrameLayout.RESIZE_MODE_ZOOM);
    // } else {
    //     videoPlayerView.setResizeMode(AspectRatioFrameLayout.RESIZE_MODE_FIT);
    // }
    //
    // Then creates controls: mControlView, buttons (restart, rewind, play/pause, ffwd, zoom, speed),
    // time bar, time text, speed text, with all layout setup and listeners.
    //
    // The video player error listener calls mListener.onError().
    // Position discontinuity triggers updateProgress().
    // PlayWhenReadyChanged toggles play/pause button icon.
    //
    // Updates progress every 150ms via UI thread handler until mReleased.

    (void)context;
    (void)mediaSource;
    (void)controlsMarginRightDp;
}

// --- Public API ---

void ExoPlayerWrapperView::handleTap() {
    if (m_controls.controlView == nullptr) {
        return;
    }

    // In original:
    // if(mControlView.getVisibility() != VISIBLE) {
    //     mControlView.setVisibility(VISIBLE);
    // } else {
    //     mControlView.setVisibility(GONE);
    // }
}

void ExoPlayerWrapperView::release() {
    if (!m_released) {
        // removeAllViews();
        // mVideoPlayer.release();
        m_released = true;
    }
}

bool ExoPlayerWrapperView::isMuted() const {
    // return mVideoPlayer.getVolume() < 0.01f;
    return false;
}

void ExoPlayerWrapperView::setMuted(bool mute) {
    // mVideoPlayer.setVolume(mute ? 0 : 1);
    (void)mute;
}

int ExoPlayerWrapperView::isControlViewVisible() const {
    // return mControlView != null ? mControlView.getVisibility() : GONE;
    return 8; // GONE = 8
}

// --- updateProgress ---

void ExoPlayerWrapperView::updateProgress() {
    // In original:
    // if(mTimeBarView != null && mTimeTextView != null && !mReleased) {
    //     final long durationMs = mVideoPlayer.getDuration();
    //     if(durationMs > 0) {
    //         final long currentPositionMs = mVideoPlayer.getCurrentPosition();
    //         mTimeBarView.setDuration(durationMs);
    //         mTimeBarView.setPosition(currentPositionMs);
    //         mTimeBarView.setBufferedPosition(mVideoPlayer.getBufferedPosition());
    //         final String newText = msToMinutesAndSecondsString(currentPositionMs)
    //                 + " / " + msToMinutesAndSecondsString(durationMs);
    //         if(!newText.contentEquals(mTimeTextView.getText())) {
    //             mTimeTextView.setText(newText);
    //         }
    //     } else {
    //         mTimeBarView.setDuration(0);
    //         mTimeBarView.setPosition(0);
    //         mTimeBarView.setBufferedPosition(0);
    //     }
    // }
}

// --- msToMinutesAndSecondsString ---

std::string ExoPlayerWrapperView::msToMinutesAndSecondsString(long ms) {
    if (ms < 0) {
        return "<negative time>";
    }

    int secondsTotal = static_cast<int>(ms / 1000);
    int mins = secondsTotal / 60;
    int secs = secondsTotal % 60;

    char buf[16];
    std::snprintf(buf, sizeof(buf), "%d:%02d", mins, secs);
    return std::string(buf);
}

// --- openSpeedSettingDialog ---

void ExoPlayerWrapperView::openSpeedSettingDialog(void* context) {
    // In original:
    // Pauses video: mVideoPlayer.setPlayWhenReady(false);
    // Shows MaterialAlertDialogBuilder with:
    //   - TextView showing current speed
    //   - SeekBar from 0-299 (maps to 0.01x to 3.00x)
    //   - 4 preset speed buttons: 0.5, 1.0, 1.5, 2.0
    //   - Positive button "Go": applies speed, resumes playback
    //   - Negative button "Cancel": resumes playback
    //   - OnCancelListener: resumes playback
    //
    // addPresetSpeedButton helper creates Button, sets click to update SeekBar + TextView
    (void)context;
}

// Note: The original has extensive layout code for createButton(), addButton(),
// and Player.Listener inner class implementations for:
// - onIsPlayingChanged: enables/disables step buttons
// - onPlayerError: logs and calls mListener.onError()
// - onPositionDiscontinuity: calls updateProgress()
// - onPlayWhenReadyChanged: toggles play/pause icon
//
// These are all dependent on Android UI framework classes.

} // namespace PinkReader
