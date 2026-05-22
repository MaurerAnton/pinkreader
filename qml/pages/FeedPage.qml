import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PinkReader

Page {
    id: feedPage
    title: "PinkReader"
    background: Rectangle { color: app.theme.background }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Sort bar
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 8
            spacing: 4

            Repeater {
                model: ["Hot", "New", "Top", "Rising"]
                delegate: Button {
                    text: modelData
                    flat: true
                    contentItem: Label {
                        text: parent.text
                        color: parent.highlighted ? app.theme.primary : app.theme.textSecondary
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: parent.highlighted ? Qt.rgba(233/255, 69/255, 96/255, 0.1) : "transparent"
                        radius: app.theme.radius
                    }
                    highlighted: app.currentSort.toLowerCase() === modelData.toLowerCase()
                    onClicked: {
                        if (app.currentSubreddit)
                            app.loadSubreddit(app.currentSubreddit, modelData.toLowerCase())
                        else
                            app.loadFrontpage(modelData.toLowerCase())
                    }
                }
            }
        }

        // Offline banner
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: app.isOffline ? 36 : 0
            color: app.theme.warning
            visible: app.isOffline
            Behavior on Layout.preferredHeight { NumberAnimation { duration: 200 } }

            RowLayout {
                anchors.fill: parent
                anchors.margins: 4
                spacing: 8

                Label {
                    text: "\uD83D\uDCE1 Offline — cached content"
                    font.pixelSize: 13; color: "#fff"
                    Layout.fillWidth: true
                }

                Rectangle {
                    visible: app.pendingActions > 0
                    color: app.theme.error; radius: 10
                    width: pendingLabel.width + 12; height: pendingLabel.height + 4

                    Label {
                        id: pendingLabel
                        anchors.centerIn: parent
                        text: app.pendingActions
                        font.pixelSize: 11; font.bold: true; color: "#fff"
                    }
                }

                Button {
                    text: "Retry"
                    font.pixelSize: 12
                    flat: true
                    contentItem: Label { text: "Retry"; color: "#fff"; font.pixelSize: 12 }
                    onClicked: app.retryOffline()
                }
            }
        }

        // Subreddit header
        Rectangle {
            visible: app.currentSubreddit !== ""
            Layout.fillWidth: true
            Layout.preferredHeight: app.currentSubreddit ? 40 : 0
            color: app.theme.accent

            Label {
                anchors.centerIn: parent
                text: "r/" + (app.currentSubreddit || "")
                font.pixelSize: 18; font.bold: true
                color: app.theme.primary
            }
        }

        // Post list with pull-to-refresh
        ListView {
            id: postList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: app.postModel

            // Pull to refresh
            header: Item {
                width: postList.width
                height: refreshIndicator.height + 10

                Rectangle {
                    id: refreshIndicator
                    anchors.centerIn: parent
                    width: 30; height: 30; radius: 15
                    color: app.theme.accent
                    opacity: postList.contentY < -60 ? 1.0 : 0.0
                    Behavior on opacity { NumberAnimation { duration: 150 } }

                    BusyIndicator {
                        anchors.centerIn: parent
                        running: postList.contentY < -60
                    }
                }
            }

            onContentYChanged: {
                if (contentY < -100 && !movementEnded) {
                    app.refresh()
                    movementEnded = true
                }
            }

            property bool movementEnded: false

            onMovementEnded: {
                movementEnded = false
            }

            delegate: PostCard {
                width: postList.width
                postTitle: title; postAuthor: author; postSubreddit: subreddit
                postScore: score; postComments: commentCount; postThumbnail: thumbnail
                postDomain: domain; postCreated: created; postNsfw: nsfw
                postSpoiler: spoiler; postFlair: flair; postStickied: stickied
                postVoteState: voteState; postGilded: gilded
                postUrl: url; postHint: postHint || ""
                postIsGallery: isGallery || false; postIsVideo: isVideo || false
                galleryImages: galleryArray || []

                ListView.onAdd: SequentialAnimation {
                    PropertyAction { target: card; property: "opacity"; value: 0 }
                    NumberAnimation { target: card; property: "opacity"; to: 1; duration: 300; easing.type: Easing.OutCubic }
                }

                onPostClicked: {
                    app.loadComments(postId, postSubreddit)
                    stackView.push(postDetailPage, {
                        "postId": postId,
                        "subreddit": postSubreddit,
                        "postTitle": title,
                        "postSelfText": selfText || "",
                        "postAuthor": author,
                        "postUrl": url
                    })
                }
                onThumbnailClicked: {
                    if (galleryArray && galleryArray.length > 0)
                        mediaViewerPopup.openMedia(url, "gallery", galleryArray, "")
                    else if (isVideo)
                        mediaViewerPopup.openMedia(url, "video", [], "")
                    else if (thumbnail && thumbnail !== "self" && thumbnail !== "default")
                        mediaViewerPopup.openMedia(url, "image", [], "")
                }
                onSubredditClicked: app.loadSubreddit(postSubreddit, "hot")
                onUpvote: app.vote("t3_" + postId, 1)
                onDownvote: app.vote("t3_" + postId, -1)
            }

            footer: Item {
                width: postList.width; height: 60

                Button {
                    anchors.centerIn: parent
                    text: app.postModel.hasMore ? "Load more..." : ""
                    enabled: app.postModel.hasMore
                    flat: true
                    contentItem: Label {
                        text: "Load more..."
                        color: app.theme.primary
                        font.pixelSize: 14
                        visible: app.postModel.hasMore
                    }
                    onClicked: app.loadMore()
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: app.loading
                visible: app.loading
            }
        }
    }

    MediaViewer { id: mediaViewerPopup }
}
