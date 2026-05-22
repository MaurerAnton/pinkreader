import QtQuick
import QtQuick.Controls

Item {
    id: asyncImage

    property string imageUrl: ""
    property int sourceWidth: 0
    property int sourceHeight: 0
    property bool showLoadingIndicator: true
    property bool autoLoad: false
    property int fillMode: Image.PreserveAspectFit

    signal loaded()
    signal errorOccurred(string message)

    implicitWidth: img.implicitWidth > 0 ? img.implicitWidth : 64
    implicitHeight: img.implicitHeight > 0 ? img.implicitHeight : 64

    QtObject {
        id: internal
        property string cachedUrl: ""
        property bool isLoading: false

        function resolve() {
            if (!imageUrl || imageUrl === "self" || imageUrl === "default" || imageUrl === "nsfw") {
                return
            }
            cachedUrl = app.imageCache.cacheUrl(imageUrl)
            isLoading = true
        }
    }

    onImageUrlChanged: internal.resolve()
    Component.onCompleted: {
        if (autoLoad && imageUrl) {
            internal.resolve()
        }
    }

    Image {
        id: img
        anchors.fill: parent
        source: internal.cachedUrl
        fillMode: asyncImage.fillMode
        asynchronous: true
        cache: false

        onStatusChanged: {
            if (status === Image.Ready) {
                internal.isLoading = false
                sourceWidth = img.sourceSize.width
                sourceHeight = img.sourceSize.height
                loaded()
            } else if (status === Image.Error) {
                internal.isLoading = false
                errorOccurred("Failed to load image")
            }
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: 32
        height: 32
        color: "#333"
        radius: 16
        visible: showLoadingIndicator && internal.isLoading && img.status !== Image.Ready

        BusyIndicator {
            anchors.centerIn: parent
            running: parent.visible
        }
    }

    Rectangle {
        anchors.centerIn: parent
        width: Math.min(parent.width, parent.height) * 0.6
        height: width
        color: "#222"
        radius: 8
        visible: imageUrl === "nsfw"

        Label {
            anchors.centerIn: parent
            text: "NSFW"
            font.pixelSize: 16
            font.bold: true
            color: "#e94560"
        }
    }
}
