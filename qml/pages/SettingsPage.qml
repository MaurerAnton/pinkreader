import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    title: "Settings"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: "Cache"
            font.pixelSize: 18
            font.bold: true
        }

        RowLayout {
            Label { text: "Cache size:"; Layout.fillWidth: true }
            Label { text: (app.cacheSize() / 1024 / 1024).toFixed(1) + " MB" }
        }

        Button {
            text: "Clear Cache"
            Layout.fillWidth: true
            onClicked: app.clearCache()
        }

        Rectangle { height: 1; Layout.fillWidth: true; color: "#333" }

        Label {
            text: "Subscriptions"
            font.pixelSize: 18
            font.bold: true
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: app.subscribedSubreddits

            delegate: RowLayout {
                width: parent.width
                height: 40

                Label {
                    text: "r/" + modelData
                    Layout.fillWidth: true
                    font.pixelSize: 16
                }

                Button {
                    text: "✕"
                    flat: true
                    onClicked: app.removeSubscription(modelData)
                }
            }
        }
    }
}
