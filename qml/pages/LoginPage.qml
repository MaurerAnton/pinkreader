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
            text: app.isLoggedIn
                ? "Logged in as " + app.currentUser
                : "Sign in to vote, comment, and save posts"
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
        }

        Button {
            text: app.isLoggedIn ? "Logout" : "Login with Reddit"
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            font.pixelSize: 16

            onClicked: {
                if (app.isLoggedIn) {
                    app.logout()
                    stackView.pop()
                } else {
                    app.login()
                }
            }
        }

        Button {
            text: "Continue without account"
            flat: true
            Layout.fillWidth: true
            onClicked: stackView.pop()
        }

        // Show auth URL when ready
        Label {
            visible: app.authUrl !== ""
            text: "Opening browser for authorization..."
            font.pixelSize: 13
            color: "#888"
            Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }
    }

    // When OAuth URL is ready, open it
    Connections {
        target: app
        function onAuthUrlReady() {
            Qt.openUrlExternally(app.authUrl)
        }
    }
}
