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

    header: ToolBar {
        id: toolbar
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
                placeholderText: qsTr("Search Reddit...")
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

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                color: "#1a1a2e"
                
                Label {
                    anchors.centerIn: parent
                    text: "PinkReader"
                    font.pixelSize: 24
                    font.bold: true
                    color: "#e94560"
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
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        
                        Label { text: icon; font.pixelSize: 18 }
                        Label {
                            text: title
                            font.pixelSize: 16
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
    }

    // --- Pages ---

    Component {
        id: feedPage
        FeedPage {}
    }

    Component {
        id: loginPage
        LoginPage {}
    }
    
    Component {
        id: settingsPage
        SettingsPage {}
    }
    
    Component {
        id: accountsPage
        AccountsPage {}
    }
    
    Component {
        id: postDetailPage
        PostDetailPage {}
    }

    // Error popup
    Connections {
        target: app
        function onErrorOccurred(message) {
            errorBanner.show(message)
        }
    }

    ErrorBanner {
        id: errorBanner
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
