import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    background: Rectangle { color: app.theme.background }
    title: "Login"

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width * 0.8

        Label {
            text: "PinkReader"
            font.pixelSize: 32; font.bold: true
            color: app.theme.primary
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: app.isLoggedIn
                ? "Logged in as " + app.currentUser
                : "Sign in to vote, comment, and save posts"
            font.pixelSize: 16; color: app.theme.text
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter; wrapMode: Text.Wrap
        }

        Button {
            text: app.isLoggedIn ? "Logout" : "Login with Reddit"
            Layout.fillWidth: true; Layout.preferredHeight: 48
            font.pixelSize: 16
            contentItem: Label {
                text: app.isLoggedIn ? "Logout" : "Login with Reddit"
                color: "#fff"; font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: app.theme.primary; radius: app.theme.radius
            }
            onClicked: {
                if (app.isLoggedIn) { app.logout(); stackView.pop() }
                else app.login()
            }
        }

        Button {
            text: "Continue without account"
            flat: true; Layout.fillWidth: true
            contentItem: Label {
                text: "Continue without account"; color: app.theme.textSecondary
                horizontalAlignment: Text.AlignHCenter
            }
            onClicked: stackView.pop()
        }

        Label {
            visible: app.authUrl !== ""
            text: "Opening browser for authorization..."
            font.pixelSize: 13; color: app.theme.textSecondary
            Layout.alignment: Qt.AlignHCenter
            wrapMode: Text.Wrap; horizontalAlignment: Text.AlignHCenter
        }
    }

    Connections {
        target: app
        function onAuthUrlReady() { Qt.openUrlExternally(app.authUrl) }
    }
}
