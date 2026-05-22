import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    title: "Login"

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width * 0.8

        Label {
            text: "PinkReader"
            font.pixelSize: 32
            font.bold: true
            color: "#e94560"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Sign in to vote, comment, and save posts"
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }

        Button {
            text: "Login with Reddit"
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            font.pixelSize: 16
            onClicked: app.login()
        }

        Button {
            text: "Continue without account"
            flat: true
            Layout.fillWidth: true
            onClicked: stackView.pop()
        }
    }
}
