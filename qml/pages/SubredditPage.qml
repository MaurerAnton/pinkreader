import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: subredditPage
    property string subredditName: ""
    background: Rectangle { color: app.theme.background }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true; Layout.preferredHeight: 80
            color: app.theme.accent

            ColumnLayout {
                anchors.centerIn: parent

                Label {
                    text: "r/" + subredditName
                    font.pixelSize: 24; font.bold: true
                    color: app.theme.primary
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter; spacing: 12

                    Button {
                        text: "Subscribe"
                        flat: true
                        contentItem: Label { text: "Subscribe"; color: app.theme.primary }
                        onClicked: app.addSubscription(subredditName)
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true; Layout.margins: 4; spacing: 2

            Repeater {
                model: ["Hot", "New", "Top", "Rising"]
                delegate: Button {
                    text: modelData; flat: true
                    onClicked: app.loadSubreddit(subredditName, modelData.toLowerCase())
                }
            }
        }

        ListView {
            Layout.fillWidth: true; Layout.fillHeight: true
            clip: true
            model: app.postModel

            delegate: PostCard {
                width: parent.width
                postTitle: title; postAuthor: author; postSubreddit: subreddit
                postScore: score; postComments: commentCount; postThumbnail: thumbnail
                postDomain: domain; postNsfw: nsfw; postStickied: stickied; postFlair: flair
                onPostClicked: stackView.push(postDetailPage, {
                    "postId": postId, "subreddit": postSubreddit, "postTitle": postTitle
                })
            }
        }
    }
}
