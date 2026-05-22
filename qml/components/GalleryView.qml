import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: galleryView

    property var images: []
    property int currentIndex: 0
    readonly property int imageCount: images ? images.length : 0

    signal closed()

    visible: imageCount > 0

    Rectangle {
        anchors.fill: parent
        color: "#000"

        // Image viewer
        SwipeView {
            id: swipe
            anchors.fill: parent
            anchors.bottomMargin: 80
            currentIndex: galleryView.currentIndex
            interactive: imageCount > 1

            Repeater {
                model: images

                Flickable {
                    id: flick
                    contentWidth: innerImage.width
                    contentHeight: innerImage.height
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    property real maxZoom: 3.0
                    property real minZoom: 1.0

                    function resetZoom() {
                        innerImage.scale = 1.0
                        flick.contentX = 0
                        flick.contentY = 0
                    }

                    AsyncImage {
                        id: innerImage
                        width: Math.min(flick.width, implicitWidth)
                        height: Math.min(flick.height, implicitHeight)
                        imageUrl: modelData.url || modelData
                        autoLoad: true
                        fillMode: Image.PreserveAspectFit

                        transform: Scale {
                            origin.x: innerImage.width / 2
                            origin.y: innerImage.height / 2
                            xScale: innerImage.scale
                            yScale: innerImage.scale
                        }

                        property real scale: 1.0

                        MouseArea {
                            anchors.fill: parent
                            onClicked: galleryView.closed()
                            onDoubleClicked: {
                                if (innerImage.scale > 1.0) {
                                    flick.resetZoom()
                                } else {
                                    innerImage.scale = 2.0
                                }
                            }
                        }
                    }
                }
            }
        }

        // Bottom bar
        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 80
            color: Qt.rgba(0, 0, 0, 0.8)

            RowLayout {
                anchors.centerIn: parent
                spacing: 12

                Button {
                    text: "←"
                    font.pixelSize: 18
                    flat: true
                    enabled: galleryView.currentIndex > 0
                    onClicked: {
                        if (galleryView.currentIndex > 0) {
                            galleryView.currentIndex--
                        }
                    }
                }

                Label {
                    text: (galleryView.currentIndex + 1) + " / " + galleryView.imageCount
                    font.pixelSize: 16
                    color: "#fff"
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: "→"
                    font.pixelSize: 18
                    flat: true
                    enabled: galleryView.currentIndex < galleryView.imageCount - 1
                    onClicked: {
                        if (galleryView.currentIndex < galleryView.imageCount - 1) {
                            galleryView.currentIndex++
                        }
                    }
                }

                Button {
                    text: "✕"
                    font.pixelSize: 20
                    flat: true
                    onClicked: galleryView.closed()
                }
            }
        }

        // Close button (top right)
        Button {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 12
            text: "✕"
            font.pixelSize: 22
            flat: true
            z: 10
            onClicked: galleryView.closed()
        }

        // Page indicator
        PageIndicator {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 90
            anchors.horizontalCenter: parent.horizontalCenter
            count: swipe.count
            currentIndex: swipe.currentIndex
            visible: imageCount > 1
        }
    }
}
