import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: voteButtons
    spacing: 0
    property int score: 0
    property int voteState: 0  // -1, 0, 1

    signal upvoted()
    signal downvoted()

    Label {
        text: "▲"
        font.pixelSize: 18
        color: voteState === 1 ? "#e94560" : "#666"
        Layout.alignment: Qt.AlignHCenter

        MouseArea {
            anchors.fill: parent
            onClicked: upvoted()
        }
    }

    Label {
        text: score > 1000 ? (score / 1000).toFixed(1) + "k" : score
        font.pixelSize: 12
        font.bold: true
        color: voteState === 1 ? "#e94560" : (voteState === -1 ? "#4a90d9" : "#999")
        Layout.alignment: Qt.AlignHCenter
    }

    Label {
        text: "▼"
        font.pixelSize: 18
        color: voteState === -1 ? "#4a90d9" : "#666"
        Layout.alignment: Qt.AlignHCenter

        MouseArea {
            anchors.fill: parent
            onClicked: downvoted()
        }
    }
}
