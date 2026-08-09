import QtQuick
import QtQuick.Controls

Button {
    background: Rectangle {
        radius: 8
        color: parent.down ? "#71717a" : parent.hovered ? "#a1a1aa" : "#18181b"
        border.color: "#27272a"
        border.width: 1
    }

    contentItem: Text {
        text: parent.text
        color: "#f4f4f5"
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
