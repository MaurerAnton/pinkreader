import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Drawer {
    id: sidebar
    edge: Qt.RightEdge
    width: parent ? parent.width * 0.8 : 300
    height: parent ? parent.height : 600
    background: Rectangle { color: app.theme.surface }

    property string subredditName: ""
    property string description: ""
    property string title: ""
    property int subscribers: 0
    property int activeUsers: 0
    property string created: ""
    property bool over18: false
    property bool isSubscriber: false

    function load(info) {
        if (!info) return
        subredditName = info.name || ""
        description = info.description || ""
        title = info.title || ""
        subscribers = info.subscribers || 0
        activeUsers = info.activeUsers || 0
        created = info.created || ""
        over18 = info.over18 || false
        open()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            Layout.fillWidth: true; Layout.preferredHeight: 100
            color: app.theme.accent

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 4

                Label {
                    text: "r/" + subredditName
                    font.pixelSize: 24; font.bold: true
                    color: app.theme.primary
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: title
                    font.pixelSize: 14; color: app.theme.textSecondary
                    visible: title !== ""
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        // Stats
        Flickable {
            Layout.fillWidth: true; Layout.fillHeight: true
            contentHeight: contentCol.height
            clip: true

            ColumnLayout {
                id: contentCol
                width: parent.width
                spacing: 0

                // Subscriber stats
                Rectangle {
                    Layout.fillWidth: true; height: 60
                    color: "transparent"

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 32

                        ColumnLayout {
                            Label {
                                text: formatNum(subscribers)
                                font.pixelSize: 20; font.bold: true
                                color: app.theme.text
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Label {
                                text: "subscribers"
                                font.pixelSize: 12; color: app.theme.textSecondary
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }

                        ColumnLayout {
                            Label {
                                text: formatNum(activeUsers)
                                font.pixelSize: 20; font.bold: true
                                color: app.theme.primary
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Label {
                                text: "online"
                                font.pixelSize: 12; color: app.theme.textSecondary
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: app.theme.divider }

                // NSFW badge
                Rectangle {
                    visible: over18
                    Layout.fillWidth: true; Layout.preferredHeight: 36

                    Rectangle {
                        anchors.centerIn: parent; radius: 4
                        color: app.theme.error
                        width: nsfwLabel.width + 12; height: nsfwLabel.height + 6
                        Label {
                            id: nsfwLabel
                            anchors.centerIn: parent
                            text: "NSFW Community"; font.pixelSize: 12
                            font.bold: true; color: "#fff"
                        }
                    }
                }

                // Description
                Rectangle {
                    Layout.fillWidth: true
                    height: descLabel.height + 24
                    visible: description !== ""

                    Label {
                        id: descLabel
                        anchors.left: parent.left; anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: 12
                        text: app.markdown.toHtml(description)
                        font.pixelSize: 14; color: app.theme.text
                        wrapMode: Text.Wrap; textFormat: Text.StyledText
                    }
                }

                Rectangle {
                    visible: description !== ""
                    height: 1; Layout.fillWidth: true; color: app.theme.divider
                }

                // Rules
                Rectangle {
                    Layout.fillWidth: true
                    height: rulesCol.height + 24
                    visible: rulesRepeater.count > 0

                    ColumnLayout {
                        id: rulesCol
                        anchors.left: parent.left; anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: 12
                        spacing: 8

                        Label {
                            text: "Rules"
                            font.pixelSize: 16; font.bold: true
                            color: app.theme.primary
                        }

                        Repeater {
                            id: rulesRepeater
                            model: app.subredditRules

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                Label {
                                    text: (index + 1) + "."
                                    font.pixelSize: 13; font.bold: true
                                    color: app.theme.textSecondary
                                    Layout.alignment: Qt.AlignTop
                                }

                                ColumnLayout {
                                    spacing: 2
                                    Layout.fillWidth: true

                                    Label {
                                        text: modelData.shortName || ""
                                        font.pixelSize: 14; font.bold: true
                                        color: app.theme.text
                                        visible: text !== ""
                                    }
                                    Label {
                                        text: modelData.description || ""
                                        font.pixelSize: 13; color: app.theme.textSecondary
                                        wrapMode: Text.Wrap
                                        visible: text !== ""
                                    }
                                }
                            }
                        }
                    }
                }
                // Actions
                Rectangle { height: 1; Layout.fillWidth: true; color: app.theme.divider }

                Button {
                    Layout.fillWidth: true; Layout.preferredHeight: 44
                    flat: true
                    contentItem: RowLayout {
                        spacing: 8
                        Label {
                            text: "🔔 Subscribe"
                            color: app.theme.primary; font.pixelSize: 15
                        }
                    }
                    onClicked: app.addSubscription(subredditName)
                }

                // Created date
                Label {
                    text: created ? "Created " + created : ""
                    font.pixelSize: 12; color: app.theme.textSecondary
                    Layout.alignment: Qt.AlignHCenter
                    Layout.margins: 8
                    visible: created !== ""
                }
            }
        }
    }

    function formatNum(n) {
        if (n >= 1000000) return (n / 1000000).toFixed(1) + "M"
        if (n >= 1000) return (n / 1000).toFixed(1) + "K"
        return n
    }
}
