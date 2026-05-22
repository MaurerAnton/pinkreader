import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: mediaViewer
    modal: true
    padding: 0
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    property string mediaUrl: ""
    property string mediaType: "image"  // "image", "video", "gif", "gallery"
    property var galleryImages: []
    property string videoSource: ""

    function openMedia(url, type, galleryArr, videoSrc) {
        mediaUrl = url
        mediaType = type || "image"
        galleryImages = galleryArr || []
        videoSource = videoSrc || ""
        open()
    }

    function closeMedia() {
        close()
    }

    background: Rectangle { color: "#000" }

    contentItem: Item {
        anchors.fill: parent

        // Gallery mode
        GalleryView {
            anchors.fill: parent
            images: mediaType === "gallery" ? galleryImages : (mediaUrl ? [mediaUrl] : [])
            visible: mediaType === "gallery" || mediaType === "image"
            onClosed: closeMedia()
        }

        // Video mode
        Rectangle {
            anchors.fill: parent
            color: "#000"
            visible: mediaType === "video" || mediaType === "gif"

            Label {
                anchors.centerIn: parent
                text: mediaType === "gif" ? "🎞️ GIF" : "🎬 Video"
                font.pixelSize: 24
                color: "#aaa"
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.verticalCenter
                anchors.topMargin: 30
                text: mediaUrl
                font.pixelSize: 12
                color: "#666"
                width: parent.width * 0.8
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                elide: Text.ElideMiddle
            }
        }

        // Close button
        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 16
            width: 40
            height: 40
            radius: 20
            color: Qt.rgba(0, 0, 0, 0.5)
            z: 10

            Text {
                anchors.centerIn: parent
                text: "✕"
                font.pixelSize: 20
                color: "#fff"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: closeMedia()
            }
        }
    }
}
