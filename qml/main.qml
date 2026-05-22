import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PinkReader

ApplicationWindow {
    id: window
    visible: true
    width: 420
    height: 780
    title: app.currentSubreddit ? "r/" + app.currentSubreddit + " - PinkReader" : "PinkReader"

    // Bind palette to theme
    palette.window: app.theme.background
    palette.windowText: app.theme.text
    palette.base: app.theme.surface
    palette.text: app.theme.text
    palette.button: app.theme.accent
    palette.buttonText: app.theme.text
    palette.highlight: app.theme.primary
    palette.highlightedText: "#fff"
    palette.mid: app.theme.divider
    color: app.theme.background

    header: ToolBar {
        id: toolbar
        background: Rectangle { color: app.theme.surface }
        RowLayout {
            anchors.fill: parent
            spacing: 4

            ToolButton {
                text: "☰"
                font.pixelSize: 20
                onClicked: drawer.open()
            }

            TextField {
                id: searchField
                Layout.fillWidth: true
                placeholderText: "Search Reddit..."
                color: app.theme.text
                background: Rectangle {
                    color: app.theme.accent
                    radius: app.theme.radius
                }
                onAccepted: {
                    app.search(text)
                    drawer.close()
                }
            }

            ToolButton {
                text: app.isLoggedIn ? "👤" : "🔑"
                font.pixelSize: 16
                onClicked: stackView.push(loginPage)
            }
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.75
        height: window.height
        background: Rectangle { color: app.theme.surface }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                color: app.theme.background

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 12

                    Label {
                        text: "PinkReader"
                        font.pixelSize: 24
                        font.bold: true
                        color: app.theme.primary
                    }

                    Item { Layout.fillWidth: true }

                    // Theme toggle in drawer
                    Rectangle {
                        width: 44
                        height: 24
                        radius: 12
                        color: app.theme.dark ? app.theme.primary : app.theme.accent

                        Rectangle {
                            width: 20; height: 20; radius: 10
                            color: "#fff"
                            anchors.verticalCenter: parent.verticalCenter
                            x: app.theme.dark ? parent.width - width - 2 : 2
                            Behavior on x { NumberAnimation { duration: 200 } }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: app.theme.toggle()
                        }
                    }
                }
            }

            ListView {
                id: drawerList
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                
                model: ListModel {
                    ListElement { title: "Frontpage"; icon: "🏠"; action: "frontpage" }
                    ListElement { title: "Popular"; icon: "🔥"; action: "popular" }
                    ListElement { title: "All"; icon: "🌐"; action: "all" }
                    ListElement { title: "Settings"; icon: "⚙️"; action: "settings" }
                    ListElement { title: "Accounts"; icon: "🔑"; action: "accounts" }
                }
                
                delegate: ItemDelegate {
                    width: drawerList.width
                    height: 48
                    background: Rectangle { color: "transparent" }
                    
                    contentItem: RowLayout {
                        spacing: 12
                        anchors.margins: 12
                        Label { text: icon; font.pixelSize: 18 }
                        Label {
                            text: title
                            font.pixelSize: 16
                            color: app.theme.text
                            Layout.fillWidth: true
                        }
                    }
                    
                    onClicked: {
                        drawer.close()
                        switch (action) {
                            case "frontpage": app.loadFrontpage("hot"); stackView.pop(null); break
                            case "popular": app.loadSubreddit("popular", "hot"); stackView.pop(null); break
                            case "all": app.loadSubreddit("all", "hot"); stackView.pop(null); break
                            case "settings": stackView.push(settingsPage); break
                            case "accounts": stackView.push(accountsPage); break
                        }
                    }
                }
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: feedPage

        pushEnter: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
                NumberAnimation { property: "x"; from: 100; to: 0; duration: 250; easing.type: Easing.OutCubic }
            }
        }
        pushExit: Transition {
            NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 150 }
        }
        popEnter: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 150 }
        }
        popExit: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 200 }
                NumberAnimation { property: "x"; from: 0; to: 100; duration: 250; easing.type: Easing.InCubic }
            }
        }
    }

    Component { id: feedPage; FeedPage {} }
    Component { id: loginPage; LoginPage {} }
    Component { id: settingsPage; SettingsPage {} }
    Component { id: accountsPage; AccountsPage {} }
    Component { id: postDetailPage; PostDetailPage {} }

    Connections {
        target: app
        function onErrorOccurred(message) { errorBanner.show(message) }
    }

    ErrorBanner {
        id: errorBanner
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
