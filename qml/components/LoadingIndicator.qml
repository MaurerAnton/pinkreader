import QtQuick
import QtQuick.Controls

BusyIndicator {
    id: loadingIndicator
    running: false
    property alias message: label.text

    Column {
        anchors.centerIn: parent
        spacing: 8

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
            running: loadingIndicator.running
        }

        Label {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#888"
            font.pixelSize: 14
        }
    }
}
