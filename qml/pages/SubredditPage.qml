import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: subredditPage
    property string subredditName: ""

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: "#16213e"

            ColumnLayout {
                anchors.centerIn: parent

                Label {
                    text: "r/" + subredditName
                    font.pixelSize: 24
                    font.bold: true
                    color: "#e94560"
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 12

                    Button {
                        text: "Subscribe"
                        flat: true
                        onClicked: {
                            app.addSubscription(subredditName)
                        }
                    }
                }
            }
        }

        // Sort buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 4
            spacing: 2

            Repeater {
                model: ["Hot", "New", "Top", "Rising"]
                delegate: Button {
                    text: modelData
                    flat: true
                    onClicked: app.loadSubreddit(subredditName, modelData.toLowerCase())
                }
            }
        }

        // Post feed (reuse same model)
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: app.postModel

            delegate: PostCard {
                width: parent.width
                postTitle: title
                postAuthor: author
                postSubreddit: subreddit
                postScore: score
                postComments: commentCount
                postThumbnail: thumbnail
                postDomain: domain
                postCreated: created
                postNsfw: nsfw
                postStickied: stickied
                postFlair: flair

                onPostClicked: stackView.push(postDetailPage, {
                    "postId": postId,
                    "subreddit": postSubreddit,
                    "postTitle": postTitle
                })
            }
        }
    }
}
