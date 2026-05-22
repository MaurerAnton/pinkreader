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
    property string postSelfText: ""
    property string postAuthor: ""
    property string postUrl: ""

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Post header
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: postSelfText ? undefined : 60
            color: app.theme.accent

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 4

                Label {
                    text: postTitle || "Loading..."
                    font.pixelSize: 16; font.bold: true; color: app.theme.text
                    Layout.fillWidth: true; wrapMode: Text.Wrap
                    maximumLineCount: postSelfText ? 10 : 3
                    elide: Text.ElideRight
                }

                // Self text (post body)
                Label {
                    visible: postSelfText !== ""
                    text: app.markdown.toHtml(postSelfText)
                    font.pixelSize: 14; color: app.theme.text
                    Layout.fillWidth: true; wrapMode: Text.Wrap
                    textFormat: Text.StyledText
                    linkColor: app.theme.primary
                    maximumLineCount: 20
                    elide: Text.ElideRight

                    MouseArea {
                        anchors.fill: parent
                        onClicked: fullText.toggle()
                    }
                }

                // "Show more" indicator
                Label {
                    visible: postSelfText && postSelfText.length > 200
                    text: "[show full text]"
                    font.pixelSize: 12; color: app.theme.primary
                    MouseArea {
                        anchors.fill: parent
                        onClicked: fullText.toggle()
                    }
                }

                QtObject {
                    id: fullText
                    property bool expanded: false
                    function toggle() { expanded = !expanded }
                }
            }
        }

        // Comment sort bar
        RowLayout {
            Layout.fillWidth: true; Layout.margins: 4
            spacing: 2

            Repeater {
                model: ["Best", "Top", "New", "Controversial", "Old", "Q&A"]
                delegate: Button {
                    text: modelData
                    flat: true
                    contentItem: Label {
                        text: modelData; color: app.theme.textSecondary; font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                    }
                    onClicked: app.loadComments(postId, subreddit)
                }
            }
        }

        // Comments
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
