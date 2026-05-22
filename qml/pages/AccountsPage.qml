import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    title: "Accounts"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: app.isLoggedIn ? "Logged in as " + app.currentUser : "Not logged in"
            font.pixelSize: 16
            Layout.fillWidth: true
        }

        Button {
            text: app.isLoggedIn ? "Logout" : "Login with Reddit"
            Layout.fillWidth: true
            onClicked: app.isLoggedIn ? app.logout() : app.login()
        }

        Label {
            text: "Logged-in users can:\n• Vote on posts and comments\n• Save posts\n• Comment\n• Manage subscriptions"
            font.pixelSize: 14
            color: "#888"
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }
    }
}
