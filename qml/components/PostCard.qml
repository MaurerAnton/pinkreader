import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: card
    color: app.theme.surface
    height: Math.max(90, contentColumn.height + 20)

    Behavior on x { NumberAnimation { duration: 200; easing.type: Easing.OutCubic } }
    Behavior on opacity { NumberAnimation { duration: 300 } }

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
    property string postUrl: ""
    property string postHint: ""
    property bool postIsGallery: false
    property bool postIsVideo: false
    property var galleryImages: []
    property var galleryMediaIds: []

    signal postClicked()
    signal subredditClicked()
    signal authorClicked()
    signal upvote()
    signal downvote()
    signal thumbnailClicked()
    signal hideRequested()
    signal saveRequested()
    signal shareRequested()
    signal copyLinkRequested()

    function hasThumbnail() {
        return postThumbnail && postThumbnail !== "self" && postThumbnail !== "default" && postThumbnail !== "nsfw"
    }

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
                color: app.theme.primary

                MouseArea {
                    anchors.fill: parent
                    onClicked: upvote()
                }
            }

            Label {
                text: postScore > 1000 ? (postScore / 1000).toFixed(1) + "k" : postScore
                font.pixelSize: 14
                font.bold: true
                color: app.theme.textSecondary
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
            Layout.preferredWidth: 72
            Layout.preferredHeight: 72
            color: app.theme.accent
            radius: app.theme.radius
            visible: hasThumbnail()

            AsyncImage {
                anchors.fill: parent
                anchors.margins: 2
                imageUrl: postThumbnail
                fillMode: Image.PreserveAspectCrop
                visible: parent.visible
            }

            Rectangle {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 2
                width: 22; height: 22; radius: 4
                color: app.theme.primary
                visible: postIsGallery || postIsVideo || postDomain === "v.redd.it" || postDomain === "youtube.com" || postDomain === "youtu.be"

                Text {
                    anchors.centerIn: parent
                    text: postIsGallery ? "▦" : "▶"
                    font.pixelSize: 12; color: "#fff"
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: 2
                width: countText.width + 8; height: 18; radius: 3
                color: Qt.rgba(0, 0, 0, 0.7)
                visible: postIsGallery

                Text {
                    id: countText
                    anchors.centerIn: parent
                    text: galleryImages ? galleryImages.length + "📷" : "📷"
                    font.pixelSize: 10; color: "#fff"
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: thumbnailClicked()
            }
        }

        // Content
        ColumnLayout {
            id: contentColumn
            Layout.fillWidth: true
            spacing: 4

            Rectangle {
                visible: postFlair !== ""
                color: app.theme.primary
                radius: 3
                height: flairLabel.height + 4
                width: flairLabel.width + 8

                Label {
                    id: flairLabel
                    text: postFlair
                    font.pixelSize: 11; color: "#fff"
                    anchors.centerIn: parent
                }
            }

            Row {
                spacing: 4
                visible: postNsfw || postSpoiler || postStickied

                Rectangle {
                    visible: postNsfw
                    color: app.theme.error; radius: 3
                    Label { text: "NSFW"; font.pixelSize: 10; color: "#fff"; padding: 2 }
                }
                Rectangle {
                    visible: postSpoiler
                    color: app.theme.warning; radius: 3
                    Label { text: "SPOILER"; font.pixelSize: 10; color: "#fff"; padding: 2 }
                }
                Rectangle {
                    visible: postStickied
                    color: app.theme.success; radius: 3
                    Label { text: "📌"; font.pixelSize: 10; color: "#fff"; padding: 2 }
                }
            }

            Label {
                text: postTitle
                font.pixelSize: 15
                font.bold: postStickied
                color: app.theme.text
                wrapMode: Text.Wrap
                maximumLineCount: 4
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            RowLayout {
                spacing: 4

                Label {
                    text: "r/" + postSubreddit
                    font.pixelSize: 12; color: app.theme.primary
                    MouseArea {
                        anchors.fill: parent
                        onClicked: subredditClicked()
                    }
                }

                Label {
                    text: "• u/" + postAuthor
                    font.pixelSize: 12; color: app.theme.textSecondary
                    MouseArea {
                        anchors.fill: parent
                        onClicked: authorClicked()
                    }
                }

                Label {
                    text: "• " + postDomain
                    font.pixelSize: 12; color: app.theme.textSecondary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Rectangle {
                    visible: postGilded > 0
                    color: "#ffd700"; radius: 3
                    Label { text: "⭐" + postGilded; font.pixelSize: 11; color: "#000"; padding: 2 }
                }

                Label {
                    text: "💬 " + postComments
                    font.pixelSize: 12; color: app.theme.textSecondary
                }
            }
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width; height: 1
        color: app.theme.divider
    }

    MouseArea {
        anchors.fill: parent
        preventStealing: true

        property real pressX: 0
        property bool dragging: false

        onPressed: {
            pressX = mouse.x
            dragging = false
        }

        onPositionChanged: {
            if (!dragging && Math.abs(mouse.x - pressX) > 10) {
                dragging = true
            }
            if (dragging) {
                card.x = Math.max(mouse.x - pressX, -130)
            }
        }

        onReleased: {
            if (card.x < -70) {
                hideRequested()
                card.opacity = 0
                card.x = -500
            } else {
                card.x = 0
            }
            dragging = false
        }

        onClicked: {
            if (!dragging) postClicked()
        }

        onPressAndHold: {
            if (!dragging) contextMenu.popup()
        }
    }

    Menu {
        id: contextMenu
        modal: true

        MenuItem {
            text: postVoteState === 1 ? "Remove Upvote" : "Upvote ▲"
            onTriggered: upvote()
        }
        MenuItem {
            text: postVoteState === -1 ? "Remove Downvote" : "Downvote ▼"
            onTriggered: downvote()
        }

        MenuSeparator { }

        MenuItem {
            text: "Save"
            onTriggered: saveRequested()
        }

        MenuItem {
            text: "Hide"
            onTriggered: hideRequested()
        }

        MenuSeparator { }

        MenuItem {
            text: "Copy Link"
            onTriggered: copyLinkRequested()
        }

        MenuItem {
            text: "Share"
            onTriggered: shareRequested()
        }
    }
}
