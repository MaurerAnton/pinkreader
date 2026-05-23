import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: videoPlayer
    color: "#000"

    property string videoUrl: ""
    property string thumbnailUrl: ""
    property bool autoPlay: false
    property bool isLooping: false
    property string playbackState: "stopped"
    property real position: 0
    property real duration: 0

    signal closed()

    Rectangle {
        anchors.centerIn: parent
        width: 200; height: 80
        color: Qt.rgba(233/255, 69/255, 96/255, 0.2)
        radius: 8

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 8

            Label {
                text: "Video Player"
                font.pixelSize: 18; font.bold: true
                color: "#fff"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: videoUrl ? "Source: " + videoUrl.substring(0, 50) + "..." : ""
                font.pixelSize: 11; color: "#888"
                Layout.alignment: Qt.AlignHCenter
                visible: videoUrl !== ""
            }
        }
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 12
        text: "Tap to close"
        font.pixelSize: 12; color: "#666"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            playbackState = "stopped"
            closed()
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
}
