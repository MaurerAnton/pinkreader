import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Rectangle {
    id: videoPlayer
    color: "#000"

    property string videoUrl: ""
    property string thumbnailUrl: ""
    property bool autoPlay: false
    property bool isLooping: false
    property real volume: 1.0

    signal closed()
    signal playbackStarted()

    MediaPlayer {
        id: mediaPlayer
        source: videoUrl
        audioOutput: AudioOutput {
            id: audio
            volume: videoPlayer.volume
            muted: false
        }
        loops: videoPlayer.isLooping ? MediaPlayer.Infinite : 1

        onPlaybackStateChanged: {
            if (playbackState === MediaPlayer.PlayingState) {
                playbackStarted()
            }
        }

        onErrorOccurred: {
            console.log("MediaPlayer error:", error, errorString)
        }
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectFit

        TapHandler {
            onTapped: controls.visible = !controls.visible
        }
    }

    // Thumbnail overlay (before playback)
    Image {
        anchors.fill: parent
        source: thumbnailUrl
        fillMode: Image.PreserveAspectFit
        visible: mediaPlayer.playbackState === MediaPlayer.StoppedState && thumbnailUrl !== ""
    }

    // Play button overlay
    Rectangle {
        anchors.centerIn: parent
        width: 64; height: 64; radius: 32
        color: Qt.rgba(233/255, 69/255, 96/255, 0.8)
        visible: mediaPlayer.playbackState !== MediaPlayer.PlayingState

        Text {
            anchors.centerIn: parent
            text: mediaPlayer.playbackState === MediaPlayer.PausedState ? "⏸" : "▶"
            font.pixelSize: 28; color: "#fff"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (mediaPlayer.playbackState === MediaPlayer.PlayingState) {
                    mediaPlayer.pause()
                } else if (mediaPlayer.playbackState === MediaPlayer.PausedState) {
                    mediaPlayer.play()
                } else {
                    mediaPlayer.play()
                    autoPlay = true
                }
            }
        }
    }

    // Loading indicator
    BusyIndicator {
        anchors.centerIn: parent
        running: mediaPlayer.playbackState === MediaPlayer.PlayingState
                 && mediaPlayer.bufferProgress < 0.5
        visible: running
    }

    // Controls bar (auto-hide)
    Rectangle {
        id: controls
        anchors.bottom: parent.bottom
        width: parent.width; height: 60
        color: Qt.rgba(0, 0, 0, 0.75)
        visible: mediaPlayer.playbackState === MediaPlayer.PlayingState
        Behavior on visible { NumberAnimation { duration: 300 } }

        Timer {
            interval: 3000
            running: controls.visible
            onTriggered: controls.visible = false
        }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 12

            // Play/Pause
            Button {
                flat: true
                text: mediaPlayer.playbackState === MediaPlayer.PlayingState ? "⏸" : "▶"
                font.pixelSize: 18
                onClicked: {
                    if (mediaPlayer.playbackState === MediaPlayer.PlayingState)
                        mediaPlayer.pause()
                    else
                        mediaPlayer.play()
                }
            }

            // Seek slider
            Slider {
                id: seekSlider
                Layout.fillWidth: true
                from: 0
                to: mediaPlayer.duration > 0 ? mediaPlayer.duration : 1
                value: mediaPlayer.position

                onMoved: {
                    mediaPlayer.position = value
                }

                background: Rectangle {
                    height: 4; radius: 2
                    color: "#444"
                    Rectangle {
                        width: parent.width * (seekSlider.value / (seekSlider.to > 0 ? seekSlider.to : 1))
                        height: parent.height; radius: 2
                        color: app.theme.primary
                    }
                }
            }

            // Time
            Label {
                text: formatTime(mediaPlayer.position) + " / " + formatTime(mediaPlayer.duration)
                font.pixelSize: 12; color: "#aaa"
                Layout.preferredWidth: 100
            }

            // Mute
            Button {
                flat: true
                text: audio.muted ? "🔇" : "🔊"
                font.pixelSize: 16
                onClicked: audio.muted = !audio.muted
            }

            // Close
            Button {
                flat: true
                text: "✕"
                font.pixelSize: 18
                onClicked: {
                    mediaPlayer.stop()
                    closed()
                }
            }
        }
    }

    function formatTime(ms) {
        var secs = Math.floor(ms / 1000)
        var mins = Math.floor(secs / 60)
        var hrs = Math.floor(mins / 60)
        return (hrs > 0 ? hrs + ":" : "")
            + (mins % 60).toString().padStart(2, '0') + ":"
            + (secs % 60).toString().padStart(2, '0')
    }

    Component.onDestruction: {
        mediaPlayer.stop()
    }
}
