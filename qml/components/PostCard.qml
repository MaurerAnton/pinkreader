import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: card
    color: "#1a1a2e"
    height: Math.max(90, contentColumn.height + 20)

    property string postTitle: ""
    property string postAuthor: ""
    property string postSubreddit: ""
    property int postScore: 0
    property int postComments: 0
    property string postThumbnail: ""
    property string postDomain: ""
    property string postCreated: ""
    property bool postNsfw: false
    property bool postSpoiler: false
    property string postFlair: ""
    property bool postStickied: false
    property int postVoteState: 0
    property int postGilded: 0
    property string postId: ""
    property string name: ""

    signal postClicked()
    signal subredditClicked()
    signal upvote()
    signal downvote()

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        // Vote buttons
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            spacing: 2

            Label {
                text: "▲"
                font.pixelSize: 16
                opacity: postVoteState === 1 ? 1.0 : 0.4
                color: "#e94560"

                MouseArea {
                    anchors.fill: parent
                    onClicked: upvote()
                }
            }

            Label {
                text: postScore > 1000 ? (postScore / 1000).toFixed(1) + "k" : postScore
                font.pixelSize: 14
                font.bold: true
                color: "#ccc"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: "▼"
                font.pixelSize: 16
                opacity: postVoteState === -1 ? 1.0 : 0.4
                color: "#4a90d9"

                MouseArea {
                    anchors.fill: parent
                    onClicked: downvote()
                }
            }
        }

        // Thumbnail
        Rectangle {
            Layout.preferredWidth: 64
            Layout.preferredHeight: 64
            color: "#0f3460"
            radius: 4
            visible: postThumbnail && postThumbnail !== "self" && postThumbnail !== "default"

            Image {
                anchors.fill: parent
                anchors.margins: 2
                source: postThumbnail
                fillMode: Image.PreserveAspectCrop
                visible: parent.visible
            }
        }

        // Content
        ColumnLayout {
            id: contentColumn
            Layout.fillWidth: true
            spacing: 4

            // Flair
            Rectangle {
                visible: postFlair !== ""
                color: "#e94560"
                radius: 3
                height: flairLabel.height + 4
                width: flairLabel.width + 8

                Label {
                    id: flairLabel
                    text: postFlair
                    font.pixelSize: 11
                    color: "#fff"
                    anchors.centerIn: parent
                }
            }

            // NSFW/Spoiler/Sticky tags
            Row {
                spacing: 4
                visible: postNsfw || postSpoiler || postStickied

                Rectangle {
                    visible: postNsfw
                    color: "#e94560"
                    radius: 3
                    Label { text: "NSFW"; font.pixelSize: 10; color: "#fff"; padding: 2 }
                }
                Rectangle {
                    visible: postSpoiler
                    color: "#f0a500"
                    radius: 3
                    Label { text: "SPOILER"; font.pixelSize: 10; color: "#fff"; padding: 2 }
                }
                Rectangle {
                    visible: postStickied
                    color: "#00b894"
                    radius: 3
                    Label { text: "📌"; font.pixelSize: 10; color: "#fff"; padding: 2 }
                }
            }

            // Title
            Label {
                text: postTitle
                font.pixelSize: 15
                font.bold: postStickied
                color: "#fff"
                wrapMode: Text.Wrap
                maximumLineCount: 4
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            // Meta row
            RowLayout {
                spacing: 8

                Label {
                    text: "r/" + postSubreddit + " • u/" + postAuthor + " • " + postDomain
                    font.pixelSize: 12
                    color: "#888"
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Rectangle {
                    visible: postGilded > 0
                    color: "#ffd700"
                    radius: 3
                    Label {
                        text: "⭐" + postGilded
                        font.pixelSize: 11
                        color: "#000"
                        padding: 2
                    }
                }

                Label {
                    text: "💬 " + postComments
                    font.pixelSize: 12
                    color: "#888"
                }
            }
        }
    }

    // Separator
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: "#16213e"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: postClicked()
    }
}
