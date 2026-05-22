import QtQuick
import QtQuick.Controls

Rectangle {
    id: errorBanner
    width: parent.width - 32
    height: errorLabel.height + 20
    radius: 6
    color: "#e94560"
    opacity: 0
    visible: opacity > 0

    property alias message: errorLabel.text
    property alias showDuration: hideTimer.interval

    function show(text) {
        errorLabel.text = text
        opacity = 0.95
        hideTimer.restart()
    }

    Label {
        id: errorLabel
        anchors.centerIn: parent
        width: parent.width - 24
        color: "#fff"
        font.pixelSize: 14
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
    }

    Timer {
        id: hideTimer
        interval: 3000
        onTriggered: errorBanner.opacity = 0
    }

    Behavior on opacity {
        NumberAnimation { duration: 300 }
    }
}
