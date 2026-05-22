import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PinkReader

Page {
    id: feedPage
    title: "PinkReader"

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
                    highlighted: app.currentSort.toLowerCase() === modelData.toLowerCase()
                    onClicked: {
                        if (app.currentSubreddit) {
                            app.loadSubreddit(app.currentSubreddit, modelData.toLowerCase())
                        } else {
                            app.loadFrontpage(modelData.toLowerCase())
                        }
                    }
                }
            }
        }

        // Subreddit info header
        Rectangle {
            visible: app.currentSubreddit !== ""
            Layout.fillWidth: true
            Layout.preferredHeight: app.currentSubreddit ? 40 : 0
            color: "#16213e"

            Label {
                anchors.centerIn: parent
                text: "r/" + (app.currentSubreddit || "")
                font.pixelSize: 18
                font.bold: true
                color: "#e94560"
            }
        }

        // Post list
        ListView {
            id: postList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: app.postModel

            delegate: PostCard {
                width: postList.width
                postTitle: title
                postAuthor: author
                postSubreddit: subreddit
                postScore: score
                postComments: commentCount
                postThumbnail: thumbnail
                postDomain: domain
                postCreated: created
                postNsfw: nsfw
                postSpoiler: spoiler
                postFlair: flair
                postStickied: stickied
                postVoteState: voteState
                postGilded: gilded
                postUrl: url
                postHint: postHint || ""
                postIsGallery: isGallery || false
                postIsVideo: isVideo || false
                galleryImages: galleryArray || []

                onPostClicked: {
                    app.loadComments(postId, postSubreddit)
                    stackView.push(postDetailPage, {
                        "postId": postId,
                        "subreddit": postSubreddit,
                        "postTitle": postTitle
                    })
                }
                onThumbnailClicked: {
                    if (galleryArray && galleryArray.length > 0) {
                        mediaViewerPopup.openMedia(url, "gallery", galleryArray, "")
                    } else if (isVideo || isGallery) {
                        mediaViewerPopup.openMedia(url, isVideo ? "video" : "image", [], "")
                    } else if (thumbnail && thumbnail !== "self" && thumbnail !== "default") {
                        mediaViewerPopup.openMedia(url, "image", [], "")
                    }
                }
                onSubredditClicked: {
                    app.loadSubreddit(postSubreddit, "hot")
                }
                onUpvote: app.vote(name, 1)
                onDownvote: app.vote(name, -1)
            }

            footer: Item {
                width: postList.width
                height: 60

                Button {
                    anchors.centerIn: parent
                    text: app.postModel.hasMore ? "Load more..." : ""
                    enabled: app.postModel.hasMore
                    onClicked: app.loadMore()
                    visible: app.postModel.hasMore
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: app.loading
                visible: app.loading
            }
        }
    }

    MediaViewer {
        id: mediaViewerPopup
    }
}
