import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: profilePage
    title: "u/" + username
    background: Rectangle { color: app.theme.background }

    property string username: ""
    property int linkKarma: 0
    property int commentKarma: 0
    property string created: ""

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Profile header
        Rectangle {
            Layout.fillWidth: true
            height: 90
            color: app.theme.accent

            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16

                // Avatar
                Rectangle {
                    Layout.preferredWidth: 60; Layout.preferredHeight: 60
                    radius: 30; color: app.theme.primary

                    Label {
                        anchors.centerIn: parent
                        text: "u/"
                        font.pixelSize: 22; font.bold: true; color: "#fff"
                    }
                }

                ColumnLayout {
                    spacing: 4
                    Layout.fillWidth: true

                    Label {
                        text: "u/" + username
                        font.pixelSize: 20; font.bold: true
                        color: app.theme.text
                    }

                    RowLayout {
                        spacing: 16

                        Label {
                            text: linkKarma + " post karma"
                            font.pixelSize: 13; color: app.theme.primary
                        }
                        Label {
                            text: commentKarma + " comment karma"
                            font.pixelSize: 13; color: app.theme.success
                        }
                    }

                    Label {
                        text: "Joined " + created
                        font.pixelSize: 12; color: app.theme.textSecondary
                    }
                }
            }
        }

        // Sort tabs
        RowLayout {
            Layout.fillWidth: true; Layout.margins: 4
            spacing: 2

            Repeater {
                model: ["New", "Top", "Hot"]
                delegate: Button {
                    text: modelData; flat: true
                    contentItem: Label {
                        text: modelData; color: app.theme.textSecondary; font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                    }
                    onClicked: app.fetchUserPosts(username, modelData.toLowerCase())
                }
            }
        }

        // Posts
        ListView {
            id: postList
            Layout.fillWidth: true; Layout.fillHeight: true
            clip: true
            model: app.postModel

            delegate: PostCard {
                width: postList.width
                postTitle: title; postAuthor: author; postSubreddit: subreddit
                postScore: score; postComments: commentCount; postThumbnail: thumbnail
                postDomain: domain; postNsfw: nsfw; postStickied: stickied; postFlair: flair
                onPostClicked: {
                    app.loadComments(postId, postSubreddit)
                    stackView.push(postDetailPage, {
                        "postId": postId, "subreddit": postSubreddit, "postTitle": title
                    })
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: app.loading; visible: app.loading
            }
        }
    }

    Component.onCompleted: {
        app.fetchUserAbout(username)
        app.fetchUserPosts(username, "new")
    }
}
