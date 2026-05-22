import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: submitPage
    title: "Create Post"
    background: Rectangle { color: app.theme.background }

    property string targetSubreddit: app.currentSubreddit || ""

    Flickable {
        anchors.fill: parent
        contentHeight: form.height + 40
        clip: true

        ColumnLayout {
            id: form
            width: parent.width - 24
            x: 12; y: 12
            spacing: 12

            Label {
                text: "New Post"
                font.pixelSize: 22; font.bold: true
                color: app.theme.primary
            }

            // Post type
            RowLayout {
                spacing: 8

                Repeater {
                    model: ["Text", "Link"]
                    delegate: Button {
                        text: modelData
                        highlighted: postTypeSelector.currentIndex === index
                        flat: !highlighted
                        contentItem: Label {
                            text: modelData; color: parent.highlighted ? "#fff" : app.theme.textSecondary
                            font.pixelSize: 14; horizontalAlignment: Text.AlignHCenter
                        }
                        background: Rectangle {
                            color: parent.highlighted ? app.theme.primary : app.theme.accent
                            radius: app.theme.radius
                        }
                        onClicked: postTypeSelector.currentIndex = index
                    }
                }
            }

            // Subreddit
            RowLayout {
                Label { text: "r/"; color: app.theme.primary; font.pixelSize: 16; font.bold: true }

                TextField {
                    id: subredditField
                    text: targetSubreddit
                    color: app.theme.text
                    placeholderText: "subreddit"
                    placeholderTextColor: app.theme.textSecondary
                    Layout.fillWidth: true
                    background: Rectangle { color: app.theme.accent; radius: app.theme.radius }
                }
            }

            // Title
            TextField {
                id: titleField
                Layout.fillWidth: true
                placeholderText: "Title"
                placeholderTextColor: app.theme.textSecondary
                color: app.theme.text
                font.pixelSize: 16; font.bold: true
                background: Rectangle { color: app.theme.accent; radius: app.theme.radius }
            }

            // URL (for link posts)
            TextField {
                id: urlField
                Layout.fillWidth: true
                visible: postTypeSelector.currentIndex === 1
                placeholderText: "URL"
                placeholderTextColor: app.theme.textSecondary
                color: app.theme.text
                background: Rectangle { color: app.theme.accent; radius: app.theme.radius }
            }

            // Body (for text posts)
            TextArea {
                id: bodyField
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                visible: postTypeSelector.currentIndex === 0
                placeholderText: "Text (optional)"
                placeholderTextColor: app.theme.textSecondary
                color: app.theme.text
                font.pixelSize: 14
                wrapMode: TextArea.Wrap
                background: Rectangle { color: app.theme.accent; radius: app.theme.radius }
            }

            // Flair
            TextField {
                id: flairField
                Layout.fillWidth: true
                placeholderText: "Flair (optional)"
                placeholderTextColor: app.theme.textSecondary
                color: app.theme.text
                background: Rectangle { color: app.theme.accent; radius: app.theme.radius }
            }

            // Login warning
            Rectangle {
                visible: !app.isLoggedIn
                Layout.fillWidth: true; Layout.preferredHeight: 36
                color: app.theme.warning; radius: app.theme.radius

                Label {
                    anchors.centerIn: parent
                    text: "Login required to post"
                    font.pixelSize: 13; color: "#fff"
                }
            }

            // Submit
            Button {
                Layout.fillWidth: true; Layout.preferredHeight: 48
                enabled: app.isLoggedIn && titleField.text.trim().length > 0

                contentItem: Label {
                    text: "Submit Post"
                    color: parent.enabled ? "#fff" : app.theme.textSecondary
                    font.pixelSize: 16; font.bold: true
                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.enabled ? app.theme.primary : app.theme.accent
                    radius: app.theme.radius
                }

                onClicked: {
                    var kind = postTypeSelector.currentIndex === 0 ? "self" : "link"
                    var sr = subredditField.text.trim() || "test"
                    var url = postTypeSelector.currentIndex === 1 ? urlField.text.trim() : ""
                    var text = postTypeSelector.currentIndex === 0 ? bodyField.text : ""
                    app.submitPost(kind, sr, titleField.text.trim(), url, text, flairField.text.trim())

                    stackView.pop()
                }
            }
        }
    }

    // Hidden type selector
    Item { ComboBox { id: postTypeSelector } }
}
