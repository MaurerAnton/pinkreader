import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    title: "Settings"
    background: Rectangle { color: app.theme.background }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Appearance
        Label {
            text: "Appearance"
            font.pixelSize: 18; font.bold: true
            color: app.theme.primary
        }

        RowLayout {
            Label {
                text: "Dark theme"
                font.pixelSize: 16; color: app.theme.text
                Layout.fillWidth: true
            }

            Rectangle {
                width: 44; height: 24; radius: 12
                color: app.theme.dark ? app.theme.primary : app.theme.accent

                Rectangle {
                    width: 20; height: 20; radius: 10
                    color: "#fff"
                    anchors.verticalCenter: parent.verticalCenter
                    x: app.theme.dark ? parent.width - width - 2 : 2
                    Behavior on x { NumberAnimation { duration: 200 } }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: app.theme.toggle()
                }
            }
        }

        Rectangle { height: 1; Layout.fillWidth: true; color: app.theme.divider }

        // Font size
        Label {
            text: "Font size"
            font.pixelSize: 18; font.bold: true
            color: app.theme.primary
        }

        RowLayout {
            spacing: 4

            Repeater {
                model: [
                    { label: "S", scale: 0.85 },
                    { label: "M", scale: 1.0 },
                    { label: "L", scale: 1.2 },
                    { label: "XL", scale: 1.4 }
                ]
                delegate: Button {
                    text: modelData.label
                    flat: true
                    highlighted: Math.abs(app.theme.fontScale - modelData.scale) < 0.01
                    contentItem: Label {
                        text: modelData.label
                        color: parent.highlighted ? app.theme.primary : app.theme.textSecondary
                        font.pixelSize: 14; font.bold: parent.highlighted
                        horizontalAlignment: Text.AlignHCenter
                    }
                    onClicked: app.theme.fontScale = modelData.scale
                }
            }
        }

        Rectangle { height: 1; Layout.fillWidth: true; color: app.theme.divider }

        // Content
        Label {
            text: "Content"
            font.pixelSize: 18; font.bold: true
            color: app.theme.primary
        }

        RowLayout {
            Label {
                text: "Show NSFW content"
                font.pixelSize: 14; color: app.theme.text
                Layout.fillWidth: true
            }

            Switch {
                checked: app.showNsfw
                onCheckedChanged: app.showNsfw = checked
            }
        }

        RowLayout {
            Label {
                text: "Auto-hide read posts"
                font.pixelSize: 14; color: app.theme.text
                Layout.fillWidth: true
            }

            Switch {
                checked: app.autoHideRead
                onCheckedChanged: app.autoHideRead = checked
            }
        }

        Rectangle { height: 1; Layout.fillWidth: true; color: app.theme.divider }

        // Cache
        Label {
            text: "Cache"
            font.pixelSize: 18; font.bold: true
            color: app.theme.primary
        }

        RowLayout {
            Label {
                text: "Image cache:"
                font.pixelSize: 14; color: app.theme.text
                Layout.fillWidth: true
            }
            Label {
                text: (app.imageCache.sizeBytes() / 1024 / 1024).toFixed(1) + " MB"
                font.pixelSize: 14; color: app.theme.textSecondary
            }
        }

        RowLayout {
            Label {
                text: "Data cache:"
                font.pixelSize: 14; color: app.theme.text
                Layout.fillWidth: true
            }
            Label {
                text: (app.cacheSize() / 1024 / 1024).toFixed(1) + " MB"
                font.pixelSize: 14; color: app.theme.textSecondary
            }
        }

        Button {
            text: "Clear caches"
            Layout.fillWidth: true
            contentItem: Label {
                text: "Clear caches"; color: app.theme.primary
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                color: app.theme.accent; radius: app.theme.radius
            }
            onClicked: {
                app.clearCache()
                app.imageCache.clear()
            }
        }

        Rectangle { height: 1; Layout.fillWidth: true; color: app.theme.divider }

        // Subscriptions
        Label {
            text: "Subscriptions"
            font.pixelSize: 18; font.bold: true
            color: app.theme.primary
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: app.subscribedSubreddits

            delegate: RowLayout {
                width: parent.width; height: 40

                Label {
                    text: "r/" + modelData
                    font.pixelSize: 16; color: app.theme.text
                    Layout.fillWidth: true
                }

                Button {
                    text: "✕"
                    flat: true
                    contentItem: Label { text: "✕"; color: app.theme.primary }
                    onClicked: app.removeSubscription(modelData)
                }
            }
        }
    }
}
