import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    title: "Accounts"
    background: Rectangle { color: app.theme.background }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: app.isLoggedIn ? "Logged in as " + app.currentUser : "Not logged in"
            font.pixelSize: 16; color: app.theme.text
            Layout.fillWidth: true
        }

        Button {
            text: app.isLoggedIn ? "Logout" : "Login with Reddit"
            Layout.fillWidth: true
            contentItem: Label {
                text: app.isLoggedIn ? "Logout" : "Login with Reddit"
                color: "#fff"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle { color: app.theme.primary; radius: app.theme.radius }
            onClicked: app.isLoggedIn ? app.logout() : app.login()
        }

        Label {
            text: "Logged-in users can:\n• Vote on posts and comments\n• Save posts\n• Comment\n• Manage subscriptions"
            font.pixelSize: 14; color: app.theme.textSecondary
            Layout.fillWidth: true; wrapMode: Text.Wrap
        }
    }
}
