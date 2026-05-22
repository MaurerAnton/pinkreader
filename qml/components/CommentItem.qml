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
    property int replyCount: 0
    property string commentId: ""

    signal replyClicked()
    signal copyClicked()
    signal shareClicked()
    signal viewProfileClicked()
    signal reportClicked()

    readonly property int indentPadding: commentDepth * 12

    // Collapse state
    property bool collapsed: false

    Rectangle {
        anchors.left: parent.left
        anchors.leftMargin: indentPadding
        anchors.top: parent.top
        width: 2
        height: parent.height - 8
        color: Qt.rgba(233/255, 69/255, 96/255, Math.max(0.1, (10 - commentDepth) * 0.1))
        visible: commentDepth > 0 && !collapsed
    }

    // Tap to collapse, long-press for menu
    MouseArea {
        anchors.fill: parent
        onClicked: collapsed = !collapsed
        onPressAndHold: commentMenu.popup()
    }

    Menu {
        id: commentMenu
        modal: true

        MenuItem {
            text: "Copy Text"
            onTriggered: copyClicked()
        }

        MenuItem {
            text: "Share"
            onTriggered: shareClicked()
        }

        MenuSeparator { }

        MenuItem {
            text: "View Profile"
            onTriggered: viewProfileClicked()
        }

        MenuSeparator { }

        MenuItem {
            text: "Report"
            onTriggered: reportClicked()
        }
    }

    ColumnLayout {
        id: commentLayout
        anchors.left: parent.left; anchors.leftMargin: indentPadding + 8
        anchors.right: parent.right; anchors.rightMargin: 12
        anchors.top: parent.top; anchors.topMargin: 8
        spacing: 4

        // Collapsed view
        Rectangle {
            visible: collapsed
            Layout.fillWidth: true
            height: 32; radius: 4
            color: app.theme.accent

            RowLayout {
                anchors.fill: parent; anchors.margins: 6
                spacing: 8

                Label {
                    text: commentAuthor
                    font.pixelSize: 13; font.bold: true
                    color: commentIsSubmitter ? app.theme.success :
                           commentDistinguished ? app.theme.primary :
                           app.theme.textSecondary
                }

                Label {
                    text: commentScore + " pts"
                    font.pixelSize: 12; color: app.theme.textSecondary
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: "[+]"
                    font.pixelSize: 14; color: app.theme.primary
                }
            }
        }

        // Expanded view
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: collapsed ? 0 : fullContent.height
            visible: !collapsed
            clip: true

            Behavior on Layout.preferredHeight {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }

            ColumnLayout {
                id: fullContent
                width: parent.width
                spacing: 4

                // Author + badges
                RowLayout {
                    spacing: 6

                    Label {
                        text: commentAuthor
                        font.pixelSize: 13
                        font.bold: commentIsSubmitter
                        color: commentIsSubmitter ? app.theme.success :
                               commentDistinguished === "moderator" ? app.theme.success :
                               commentDistinguished === "admin" ? app.theme.primary :
                               app.theme.textSecondary
                    }

                    Rectangle {
                        visible: commentIsSubmitter
                        color: app.theme.success; radius: 3; height: 16
                        Label { text: "OP"; font.pixelSize: 10; font.bold: true; color: "#fff"; padding: 2 }
                    }
                    Rectangle {
                        visible: commentDistinguished === "moderator"
                        color: app.theme.success; radius: 3; height: 16
                        Label { text: "MOD"; font.pixelSize: 10; font.bold: true; color: "#fff"; padding: 2 }
                    }
                    Rectangle {
                        visible: commentDistinguished === "admin"
                        color: app.theme.primary; radius: 3; height: 16
                        Label { text: "ADMIN"; font.pixelSize: 10; font.bold: true; color: "#fff"; padding: 2 }
                    }
                    Rectangle {
                        visible: commentStickied
                        color: app.theme.success; radius: 3; height: 16
                        Label { text: "📌"; font.pixelSize: 10; padding: 2 }
                    }

                    Label {
                        text: commentScore + " pts"
                        font.pixelSize: 12; color: app.theme.textSecondary
                        Layout.fillWidth: true
                    }

                    Rectangle {
                        visible: commentGilded > 0
                        color: "#ffd700"; radius: 3; height: 16
                        Label { text: "⭐" + commentGilded; font.pixelSize: 10; color: "#000"; padding: 2 }
                    }
                }

                // Body
                Label {
                    text: app.markdown.toHtml(commentBody)
                    font.pixelSize: 14; color: app.theme.text
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                    textFormat: Text.StyledText
                    linkColor: app.theme.primary
                }

                // Actions row
                RowLayout {
                    spacing: 16

                    Label {
                        text: "▲"
                        font.pixelSize: 14; color: app.theme.textSecondary
                    }
                    Label {
                        text: "▼"
                        font.pixelSize: 14; color: app.theme.textSecondary
                    }

                    Label {
                        text: "↩ Reply"
                        font.pixelSize: 12; color: app.theme.primary

                        MouseArea {
                            anchors.fill: parent
                            onClicked: replyClicked()
                        }
                    }

                    Label {
                        text: "[-] collapse"
                        font.pixelSize: 12; color: app.theme.textSecondary
                        MouseArea {
                            anchors.fill: parent
                            onClicked: collapsed = true
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom; width: parent.width - indentPadding; height: 1
        color: app.theme.divider
        anchors.left: parent.left; anchors.leftMargin: indentPadding
    }
}
