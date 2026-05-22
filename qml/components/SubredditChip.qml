import QtQuick
import QtQuick.Controls

Rectangle {
    id: chip
    radius: 12
    height: 32
    color: mouseArea.containsMouse ? "#e94560" : "#0f3460"

    property string subredditName: ""

    signal clicked()

    Label {
        anchors.centerIn: parent
        anchors.margins: 8
        text: "r/" + subredditName
        font.pixelSize: 13
        color: "#fff"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: chip.clicked()
    }
}
