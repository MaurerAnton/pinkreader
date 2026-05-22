import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    title: "Discover Subreddits"
    background: Rectangle { color: app.theme.background }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        // Search bar
        RowLayout {
            spacing: 8

            TextField {
                id: searchField
                Layout.fillWidth: true
                placeholderText: "Search subreddits..."
                placeholderTextColor: app.theme.textSecondary
                color: app.theme.text
                background: Rectangle {
                    color: app.theme.accent; radius: app.theme.radius
                }

                onAccepted: {
                    sortLabel.text = "Sorting by: " + sortSelector.currentText
                    app.searchSubreddits(text, sortSelector.currentValue)
                }
            }

            Button {
                text: "Search"
                contentItem: Label { text: "Search"; color: "#fff" }
                background: Rectangle { color: app.theme.primary; radius: app.theme.radius }
                onClicked: {
                    sortLabel.text = "Sorting by: " + sortSelector.currentText
                    app.searchSubreddits(searchField.text, sortSelector.currentValue)
                }
            }
        }

        // Sort selector
        RowLayout {
            spacing: 8

            Label { text: "Sort:"; color: app.theme.textSecondary; font.pixelSize: 13 }

            ComboBox {
                id: sortSelector
                model: ["Relevance", "Subscribers", "Active Now", "Posts/Day", "Name A-Z"]
                property var values: ["relevance", "subscribers", "active", "posts", "name"]
                property string currentValue: values[currentIndex]

                contentItem: Label {
                    text: parent.displayText; color: app.theme.text
                    font.pixelSize: 13
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: app.theme.accent; radius: app.theme.radius
                }
            }

            Label {
                id: sortLabel
                text: ""
                font.pixelSize: 12; color: app.theme.textSecondary
                Layout.fillWidth: true
            }
        }

        // Results
        ListView {
            id: resultList
            Layout.fillWidth: true; Layout.fillHeight: true
            clip: true
            model: app.subredditModel

            delegate: Rectangle {
                width: resultList.width
                height: 72
                color: app.theme.surface

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 10

                    // Subreddit icon
                    Rectangle {
                        Layout.preferredWidth: 48; Layout.preferredHeight: 48
                        radius: 24; color: app.theme.accent

                        Image {
                            anchors.fill: parent; anchors.margins: 4
                            source: iconUrl; fillMode: Image.PreserveAspectCrop
                            visible: iconUrl !== ""
                        }

                        Label {
                            anchors.centerIn: parent
                            text: "r/"
                            font.pixelSize: 14; font.bold: true
                            color: app.theme.primary
                            visible: iconUrl === ""
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2

                        Label {
                            text: "r/" + name
                            font.pixelSize: 15; font.bold: true
                            color: app.theme.text
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        Label {
                            text: description || ""
                            font.pixelSize: 12; color: app.theme.textSecondary
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                            visible: description !== ""
                        }

                        RowLayout {
                            spacing: 12

                            Label {
                                text: formatNumber(subscribers) + " subscribers"
                                font.pixelSize: 11; color: app.theme.textSecondary
                            }

                            Label {
                                text: formatNumber(activeUsers) + " active"
                                font.pixelSize: 11; color: app.theme.primary
                            }

                            Label {
                                text: postsPerDay.toFixed(1) + " posts/day"
                                font.pixelSize: 11; color: app.theme.success
                            }

                            Rectangle {
                                visible: over18
                                color: app.theme.error; radius: 3; height: 14
                                Label {
                                    text: "NSFW"; font.pixelSize: 9; color: "#fff"; padding: 2
                                }
                            }
                        }
                    }

                    // Subscribe button
                    Button {
                        flat: true
                        contentItem: Label {
                            text: "+"; font.pixelSize: 20; color: app.theme.primary
                        }
                        onClicked: app.addSubscription(name)
                    }
                }

                Rectangle {
                    anchors.bottom: parent.bottom; width: parent.width; height: 1
                    color: app.theme.divider
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        app.loadSubreddit(name, "hot")
                        stackView.pop()
                    }
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: app.loading; visible: app.loading
            }

            Label {
                anchors.centerIn: parent
                text: "Search for subreddits above"
                font.pixelSize: 16; color: app.theme.textSecondary
                visible: app.subredditModel.count === 0 && !app.loading
            }
        }
    }

    function formatNumber(n) {
        if (n >= 1000000) return (n / 1000000).toFixed(1) + "M"
        if (n >= 1000) return (n / 1000).toFixed(1) + "K"
        return n
    }
}
