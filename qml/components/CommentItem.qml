import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: commentItem
    color: "transparent"
    implicitHeight: commentLayout.height + indentPadding + 16

    property string commentAuthor: ""
    property string commentBody: ""
    property int commentScore: 0
    property int commentDepth: 0
    property string commentCreated: ""
    property bool commentStickied: false
    property bool commentIsSubmitter: false
    property string commentDistinguished: ""
    property int commentGilded: 0

    readonly property int indentPadding: commentDepth * 12

    Rectangle {
        anchors.left: parent.left
        anchors.leftMargin: indentPadding
        anchors.top: parent.top
        width: 2
        height: parent.height - 8
        color: Qt.rgba(233 / 255, 69 / 255, 96 / 255, Math.max(0.1, (10 - commentDepth) * 0.1))
        visible: commentDepth > 0
    }

    ColumnLayout {
        id: commentLayout
        anchors.left: parent.left
        anchors.leftMargin: indentPadding + 8
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.top: parent.top
        anchors.topMargin: 8
        spacing: 4

        // Author row
        RowLayout {
            spacing: 6

            Label {
                text: commentAuthor
                font.pixelSize: 13
                font.bold: commentIsSubmitter
                color: commentIsSubmitter ? "#00b894" :
                       commentDistinguished === "moderator" ? "#00b894" :
                       commentDistinguished === "admin" ? "#e94560" : "#aaa"
            }

            Rectangle {
                visible: commentIsSubmitter
                color: "#00b894"
                radius: 3
                Label {
                    text: "OP"
                    font.pixelSize: 10
                    font.bold: true
                    color: "#fff"
                    padding: 2
                }
            }

            Rectangle {
                visible: commentDistinguished === "moderator"
                color: "#00b894"
                radius: 3
                Label {
                    text: "MOD"
                    font.pixelSize: 10
                    font.bold: true
                    color: "#fff"
                    padding: 2
                }
            }

            Rectangle {
                visible: commentDistinguished === "admin"
                color: "#e94560"
                radius: 3
                Label {
                    text: "ADMIN"
                    font.pixelSize: 10
                    font.bold: true
                    color: "#fff"
                    padding: 2
                }
            }

            Rectangle {
                visible: commentStickied
                color: "#00b894"
                radius: 3
                Label {
                    text: "📌"
                    font.pixelSize: 10
                    padding: 2
                }
            }

            Label {
                text: commentScore + " pts"
                font.pixelSize: 12
                color: "#666"
                Layout.fillWidth: true
            }

            Rectangle {
                visible: commentGilded > 0
                color: "#ffd700"
                radius: 3
                Label {
                    text: "⭐" + commentGilded
                    font.pixelSize: 10
                    color: "#000"
                    padding: 2
                }
            }
        }

        // Body
        Label {
            text: commentBody
            font.pixelSize: 14
            color: "#ddd"
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            textFormat: Text.StyledText
        }

        // Actions
        RowLayout {
            spacing: 12

            Label {
                text: "▲"
                font.pixelSize: 14
                color: "#666"
            }
            Label {
                text: "▼"
                font.pixelSize: 14
                color: "#666"
            }
            Label {
                text: "↩ Reply"
                font.pixelSize: 12
                color: "#666"
            }
        }
    }

    // Separator
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width - indentPadding
        height: 1
        color: "#0f3460"
        anchors.left: parent.left
        anchors.leftMargin: indentPadding
    }
}
