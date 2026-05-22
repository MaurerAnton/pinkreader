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
    property string thumbnailUrl: ""

    function openMedia(url, type, galleryArr, videoSrc) {
        mediaUrl = url
        mediaType = type || "image"
        galleryImages = galleryArr || []
        videoSource = videoSrc || ""
        thumbnailUrl = ""
        open()
    }

    function closeMedia() {
        if (videoContent.visible) videoContent.item.stop()
        close()
    }

    background: Rectangle { color: "#000" }

    contentItem: Item {
        anchors.fill: parent

        // Video player
        Loader {
            id: videoContent
            anchors.fill: parent
            active: mediaType === "video" || mediaType === "gif"
            visible: active
            sourceComponent: VideoPlayer {
                videoUrl: videoSource || mediaUrl
                isLooping: mediaType === "gif"
                autoPlay: true
                onClosed: closeMedia()
            }
        }

        // Gallery + single image
        GalleryView {
            anchors.fill: parent
            images: mediaType === "gallery" ? galleryImages : (mediaUrl ? [mediaUrl] : [])
            visible: mediaType === "gallery" || mediaType === "image"
            onClosed: closeMedia()
        }

        // Close button
        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 16
            width: 40; height: 40; radius: 20
            color: Qt.rgba(0, 0, 0, 0.5)
            z: 10

            Text {
                anchors.centerIn: parent
                text: "✕"; font.pixelSize: 20; color: "#fff"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: closeMedia()
            }
        }
    }
}
