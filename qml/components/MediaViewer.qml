import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: mediaViewer

    property string mediaUrl: ""
    property string mediaType: "image" // "image", "video", "gif"
    property bool fullScreen: false

    MouseArea {
        anchors.fill: parent
        onClicked: fullScreen = !fullScreen
        
        Image {
            anchors.fill: parent
            source: mediaUrl
            fillMode: Image.PreserveAspectFit
            visible: mediaType === "image"
            asynchronous: true
        }
    }
}
