import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PinkReader

Page {
    id: postDetailPage
    title: postTitle.length > 30 ? postTitle.substring(0, 30) + "..." : postTitle
    background: Rectangle { color: app.theme.background }

    property string postId: ""
    property string subreddit: ""
    property string postTitle: ""

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: app.theme.accent

            RowLayout {
                anchors.fill: parent; anchors.margins: 12
                Label {
                    text: postTitle || "Loading..."
                    font.pixelSize: 16; font.bold: true; color: app.theme.text
                    Layout.fillWidth: true; wrapMode: Text.Wrap
                    maximumLineCount: 3; elide: Text.ElideRight
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true; Layout.margins: 4
            spacing: 2

            Repeater {
                model: ["Best", "Top", "New", "Controversial", "Old", "Q&A"]
                delegate: Button {
                    text: modelData
                    flat: true
                    font.pixelSize: 12
                    contentItem: Label {
                        text: modelData; color: app.theme.textSecondary; font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                    }
                    onClicked: app.loadComments(postId, subreddit)
                }
            }
        }

        ListView {
            id: commentList
            Layout.fillWidth: true; Layout.fillHeight: true
            clip: true
            model: app.commentModel

            delegate: CommentItem {
                width: commentList.width
                commentAuthor: author; commentBody: body; commentScore: score
                commentDepth: depth; commentCreated: created; commentStickied: stickied
                commentIsSubmitter: isSubmitter; commentDistinguished: distinguished
                commentGilded: gilded
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: app.loading; visible: app.loading
            }
        }
    }
}
